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
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define PI 3.14159265

typedef unsigned char RGB[3];

using namespace std;
using namespace parser;

Scene scene;
unsigned char* image;
Camera camera;

void computeNormals(){

    int triangle_count = scene.triangles.size();
    
    for (int i=0; i<triangle_count; i++){

        Face indices = scene.triangles[i].indices;

        Vec3f v0 = scene.vertex_data[indices.v0_id-1];
        Vec3f v1 = scene.vertex_data[indices.v1_id-1];
        Vec3f v2 = scene.vertex_data[indices.v2_id-1];                  // vertices of the triangle

        // cout << v0 << ", " << v1 << ", " << v2 << endl;
        // cout << (v2-v1) * (v0-v1) << endl;

        Vec3f normal = ((v2-v1) * (v0-v1)).normalize();
        scene.triangles[i].indices.normal = normal;
    }

    int mesh_count = scene.meshes.size();

    for (int i=0; i<mesh_count; i++){

        Mesh mesh = scene.meshes[i];

        int faces_size = scene.meshes[i].faces.size();
        for (int j=0; j<faces_size; j++){
            
            Face indices = mesh.faces[j];

            Vec3f v0 = scene.vertex_data[indices.v0_id-1];
            Vec3f v1 = scene.vertex_data[indices.v1_id-1];
            Vec3f v2 = scene.vertex_data[indices.v2_id-1];

            // cout << v0 << ", " << v1 << ", " << v2 << endl;
            // cout << (v2-v1) * (v0-v1) << endl;

            Vec3f normal = ((v2-v1) * (v0-v1)).normalize();
            scene.meshes[i].faces[j].normal = normal;

        }
    }

}

Vec3f diffuse_shading(Vec3f diffuse_coeff, Vec3f w_i, Vec3f normal, Vec3f light_intensity, float distance){
    Vec3f wi = w_i.normalize();
    Vec3f n = normal.normalize();                   // two lines can be omitted if parameters are given normalized

    float cos_theta = MAX(0, wi.dot(n));
    Vec3f i_over_r_squared = light_intensity * (1/(distance * distance));           // I/(r^2)
    
    return (diffuse_coeff * cos_theta).elementwiseMultiplication(i_over_r_squared);
}

Vec3f ambient_shading(Vec3f ambient_coeff, Vec3f radiance){
    Vec3f res = ambient_coeff.elementwiseMultiplication(radiance);
    
    return res;
}

Vec3f specular_shading(Vec3f specular_coeff, int phong_exponent, Vec3f normal, Vec3f w_i, Vec3f w_o, Vec3f light_intensity, float distance){
    Vec3f res;
    
    Vec3f wi = w_i.normalize();
    Vec3f wo = w_o.normalize();
    Vec3f n = normal.normalize();

    float cos_theta = wi.dot(n) * (1/(wi.length() * n.length()));       // if the light is coming from behind the surface
    float theta = acos(cos_theta) * 180.0 / PI;
    if (theta >= 90)
        return res;
    
    Vec3f wi_plus_wo = wi + wo;
    Vec3f half = wi_plus_wo * (1/wi_plus_wo.length());

    float cos_alpha = pow(MAX(0, n.dot(half)), phong_exponent);
    Vec3f i_over_rsqured = light_intensity * (1/(distance * distance));
    
    res = (specular_coeff * cos_alpha).elementwiseMultiplication(i_over_rsqured);

    return res;
}

bool triangle_intersects(Ray ray, Triangle triangle, float* min_t, Vec3f* normal_p){
    Vec3f a = scene.vertex_data[triangle.indices.v0_id - 1];
    Vec3f b = scene.vertex_data[triangle.indices.v1_id - 1];
    Vec3f c = scene.vertex_data[triangle.indices.v2_id - 1];

    Vec3f normal = triangle.indices.normal;

    if (ray.getDirection().dot(normal) == 0) return false;  /* floating point precision loss ? */

    float t = ((a - ray.getOrigin()).dot(normal)) / (ray.getDirection().dot(normal));

    Vec3f p = ray.getPoint(t);

    Vec3f v_p = (p - b) * (a - b);
    Vec3f v_c = (c - b) * (a - b);
    if (v_p.dot(v_c) <= 0) return false;

    v_p = (p - a) * (c - a);
    v_c = (b - a) * (c - a);
    if (v_p.dot(v_c) <= 0) return false;

    v_p = (p - c) * (b - c);
    v_c = (a - c) * (b - c);
    if (v_p.dot(v_c) <= 0) return false;

    if (t < *min_t){
        *min_t = t;
        *normal_p = normal;
        return true;
    }
    return false;
}

/*
float determinant(float matrix[3][3]){
    float result = matrix[0][0] * (matrix[1][1]*matrix[2][2] - matrix[2][1]*matrix[1][2]);
    result += matrix[0][1] * (matrix[2][0]*matrix[1][2] - matrix[1][0]*matrix[2][2]);
    result += matrix[0][2] * (matrix[1][0]*matrix[2][1] - matrix[2][0]*matrix[1][1]);

    return result;
}

bool triangle_intersects(Vec3f direction, Triangle triangle, float* min_distance, Vec3f* normal){
    //Returns true if the triangle is the closest object to the camera.
    
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

    if (beta >= 0 && gamma >= 0 && (beta + gamma) <= 1)
        if (t < *min_distance){
            *min_distance = t;
            *normal = triangle.indices.normal;

            return true;
        }

    return false;
}*/

bool sphere_intersects(Vec3f direction, Sphere sphere, float* min_t){
    /*
        Returns true if the sphere is the closest object to the camera.
    */
    Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];

    Vec3f o_minus_c = camera.position - center;                     // o - c
    float d_dot_o_minus_c = direction.dot(o_minus_c);               // d . (o - c)
    float d_dot_d = direction.dot(direction);                       // d . d

    float sqrt_discr = sqrt( pow(d_dot_o_minus_c, 2) - d_dot_d * (o_minus_c.dot(o_minus_c) - pow(sphere.radius, 2)) );

    if (!isnan(sqrt_discr)){  // ray and sphere intersect
        float t_1 = (-d_dot_o_minus_c - sqrt_discr) / d_dot_d;
        float t_2 = (-d_dot_o_minus_c + sqrt_discr) / d_dot_d;
        float t = MIN(t_1, t_2);                // does it apply all the time??

        if (t < *min_t)
            return (*min_t = t);
        
    }
    return false;
}

float clamp(float val){
    if (val < 0)
        return 0;
    if (val>255)
        return 255;
    return val;
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
            
            float min_t = numeric_limits<float>::max();
            Vec3f normal;                                               // normal at intersection point

            int material_id = 0;

            bool intersects = false;
            
            float s_u = (i + .5) * pixel_width;
            float s_v = (j + .5) * pixel_height;

            Vec3f s = q + (u * s_u) - (camera.up * s_v);                // s = q + u * s_u - v * s_v
            
            Vec3f direction = s - camera.position;                      // d = s - e

            Ray primaryRay(camera.position, direction);

            for (auto sphere: scene.spheres){

                if(sphere_intersects(direction, sphere, &min_t)){ 
                    // this sphere is the closest object so far

                    material_id = sphere.material_id;
                    intersects = true;
                }
            }

            for (auto triangle: scene.triangles){
                
                if (triangle_intersects(primaryRay, triangle, &min_t, &normal)){
                    // this triangle is the closest object so far

                    material_id = triangle.material_id;
                    intersects = true;
                }
            }

            for (auto mesh: scene.meshes){
                for (auto face: mesh.faces){
                    
                    Triangle triangle = {mesh.material_id, face};

                    if (triangle_intersects(primaryRay, triangle, &min_t, &normal)){
                        // this triangle is the closest object so far
                        material_id = mesh.material_id;
                        intersects = true;
                    }
                }
            }

            if (!intersects){
                image[start_index++] = scene.background_color.x;
                image[start_index++] = scene.background_color.y;
                image[start_index++] = scene.background_color.z;
            }
            else{
                Vec3f ambient = ambient_shading(scene.materials[material_id-1].ambient, scene.ambient_light);
                Vec3f diffuse;
                Vec3f specular;

                for (auto point_light: scene.point_lights){
                    Vec3f intersection_pt = primaryRay.getPoint(min_t);
                    Vec3f w_i = point_light.position - intersection_pt;

                    float distance = (point_light.position - intersection_pt).length();

                    diffuse = diffuse + diffuse_shading(scene.materials[material_id-1].diffuse, w_i, 
                            normal, point_light.intensity, distance);
                }

                /* Clamp values under 0 and over 255 */                
                image[start_index++] = int(clamp(diffuse.x + ambient.x));
                image[start_index++] = int(clamp(diffuse.y+ ambient.y));
                image[start_index++] = int(clamp(diffuse.z + ambient.z));
            }
        }
    }
}

int main(int argc, char* argv[])
{   
    scene.loadFromXml(argv[1]);
    computeNormals();

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

        /*int single_thread_borders[2] = {0, camera.image_height-1};
        pthread_create(threads, NULL, trace_routine, single_thread_borders);*/

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
