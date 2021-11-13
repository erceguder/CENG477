#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "vec3f.h"
#include "vec3i.h"
#include <cmath>
#include <pthread.h>
#include <limits>

#define THREAD_NUM 4
#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef unsigned char RGB[3];

using namespace std;
using namespace parser;

Scene scene;
unsigned char* image;
Camera camera;

float determinant(float matrix[3][3]){
    float result = matrix[0][0] * (matrix[1][1]*matrix[2][2] - matrix[2][1]*matrix[1][2]);
    result += matrix[0][1] * (matrix[2][0]*matrix[1][2] - matrix[1][0]*matrix[2][2]);
    result += matrix[0][2] * (matrix[1][0]*matrix[2][1] - matrix[2][0]*matrix[1][1]);

    return result;
}

bool triangle_intersects(Vec3f direction, Triangle triangle, float min_distance){

    Vec3f a = scene.vertex_data[triangle.indices.v0_id - 1];
    Vec3f b = scene.vertex_data[triangle.indices.v1_id - 1];
    Vec3f c = scene.vertex_data[triangle.indices.v2_id - 1];

    float A[3][3] = {
        {(a-b).x, (a-c).x, direction.x},
        {(a-b).y, (a-c).y, direction.y},
        {(a-b).z, (a-c).z, direction.z}
    };

    float A_1[3][3] = {
        {(a-camera.position).x, (a-c).x, direction.x},
        {(a-camera.position).y, (a-c).y, direction.y},
        {(a-camera.position).z, (a-c).z, direction.z}
    };

    float A_2[3][3] = {
        {(a-b).x, (a-camera.position).x, direction.x},
        {(a-b).y, (a-camera.position).y, direction.y},
        {(a-b).z, (a-camera.position).z, direction.z}
    };

    float A_3[3][3] = {
        {(a-b).x, (a-c).x, (a-camera.position).x},
        {(a-b).y, (a-c).y, (a-camera.position).y},
        {(a-b).z, (a-c).z, (a-camera.position).z}
    };

    float det_A = determinant(A);
    
    float beta = determinant(A_1) / det_A;
    float gamma = determinant(A_2) / det_A;
    float t = determinant(A_3) / det_A;

    if (beta >= 0 && gamma >= 0 && (beta + gamma) <= 1)     /* What if another object is in front of this triangle */ 
        if (t < min_distance)
            return (min_distance = t);
        
    return false;
}

bool sphere_intersects(Vec3f direction, Sphere sphere, float min_distance){

    Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];

    Vec3f o_minus_c = camera.position - center;                     // o - c
    float d_dot_o_minus_c = direction.dot(o_minus_c);               // d . (o - c)
    float d_dot_d = direction.dot(direction);                       // d . d

    float sqrt_discr = sqrt( pow(d_dot_o_minus_c, 2) - d_dot_d * (o_minus_c.dot(o_minus_c) - pow(sphere.radius, 2)) );

    if (!isnan(sqrt_discr)){  // ray and sphere intersect
        float t_1 = (-d_dot_o_minus_c - sqrt_discr) / d_dot_d;
        float t_2 = (-d_dot_o_minus_c + sqrt_discr) / d_dot_d;
        float t = MIN(t_1, t_2);

        if (t < min_distance)
            return (min_distance = t);
        
    }
    return false;
}

void* trace_routine(void* row_borders){
    /*
        row_borders:    first and the last rows of the image for the thread to work on
    */
    int start_row = ((int*) row_borders)[0];
    int end_row = ((int*) row_borders)[1];

    int image_width = camera.image_width, image_height = camera.image_height;

    int start_index = start_row * image_width * 3;

    float left = camera.near_plane.x;
    float right = camera.near_plane.y;
    float bottom = camera.near_plane.z;
    float top = camera.near_plane.w;

    float pixel_width = (right - left) / image_width;
    float pixel_height = (top - bottom) / image_height;

    Vec3f m = camera.position + (camera.gaze * camera.near_distance);   // m = e + gaze*distance
    Vec3f u = camera.gaze * camera.up;                                  // u = (-w) x v
    Vec3f q = m + (u * left) + (camera.up * top);                       // q = m + u*l + v*t

    for (int j=start_row; j <= end_row; j++){                           // rows [start, end]
        for (int i=0; i < image_width; i++){                            // columns
            
            float min_distance = numeric_limits<float>::max();

            bool intersects = false;
            
            float s_u = (i + .5) * pixel_width;
            float s_v = (j + .5) * pixel_height;

            Vec3f s = q + (u * s_u) - (camera.up * s_v);            // s = q + u * s_u - v * s_v
            
            Vec3f direction = s - camera.position;                  // d = s - e

            Ray primaryRay(camera.position, direction);             // NOT USED?

            for (auto sphere: scene.spheres){

                if(sphere_intersects(direction, sphere, min_distance)){
                    image[start_index++] = 255;
                    image[start_index++] = 255;
                    image[start_index++] = 255;

                    intersects = true;
                }

            }

            for (auto triangle: scene.triangles){
                
                if (triangle_intersects(direction, triangle, min_distance)){
                    image[start_index++] = 255;
                    image[start_index++] = 255;
                    image[start_index++] = 255;
             
                    intersects = true;
                }
            }

            for (auto mesh: scene.meshes){
                for (auto face: mesh.faces){
                    
                    Triangle triangle = {mesh.material_id, face};

                    if (triangle_intersects(direction, triangle, min_distance)){
                        image[start_index++] = 255;
                        image[start_index++] = 255;
                        image[start_index++] = 255;
                        
                        intersects = true;
                        break;
                    }
                }
            }
            
            if (!intersects){
                image[start_index++] = scene.background_color.x;
                image[start_index++] = scene.background_color.y;
                image[start_index++] = scene.background_color.z;
            }
            
        }
    }
}

int main(int argc, char* argv[])
{
    scene.loadFromXml(argv[1]);

    pthread_t threads[THREAD_NUM];

    for (int i=0; i < scene.cameras.size(); i++){

        camera = scene.cameras[i];
        image = new unsigned char [camera.image_width * camera.image_height * 3];

        /* This assumes image height is a factor of 4 */
        int row_borders[4][2] = {
            {0, (camera.image_height/4) - 1},
            {camera.image_height/4, 2*(camera.image_height/4) - 1},
            {2*(camera.image_height/4), 3*(camera.image_height/4) - 1},
            {3*(camera.image_height/4), 4*(camera.image_height/4) - 1}
        };

        pthread_create(&threads[0], NULL, trace_routine, row_borders[0]);
        pthread_create(&threads[1], NULL, trace_routine, row_borders[1]);
        pthread_create(&threads[2], NULL, trace_routine, row_borders[2]);
        pthread_create(&threads[3], NULL, trace_routine, row_borders[3]);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);
        
        write_ppm(camera.image_name.c_str(), image, camera.image_width, camera.image_height);

        delete [] image;

    }
}
