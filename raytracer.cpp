#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "vec3f.h"
#include "vec3i.h"
#include <cmath>
#include<pthread.h>

#define THREAD_NUM 4

typedef unsigned char RGB[3];

using namespace std;
using namespace parser;

Scene scene;
unsigned char* image;
Camera camera;

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

    for (int j=start_row; j <= end_row; j++){                            // rows [start, end]
        for (int i=0; i < image_width; i++){                            // columns
            
            float s_u = (i + .5) * pixel_width;
            float s_v = (j + .5) * pixel_height;

            Vec3f s = q + (u * s_u) - (camera.up * s_v);            // s = q + u * s_u - v * s_v
            
            Ray primaryRay(camera.position, s - camera.position);   // d = s - e

            for (auto sphere: scene.spheres){

                Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];

                Vec3f o_minus_c = camera.position - center;                                     // o - c
                float d_dot_o_minus_c = primaryRay.getDirection().dot(o_minus_c);               // d . (o - c)
                float d_dot_d = primaryRay.getDirection().dot(primaryRay.getDirection());       // d . d

                float sqrt_discr = sqrt( pow(d_dot_o_minus_c, 2) - d_dot_d * (o_minus_c.dot(o_minus_c) - pow(sphere.radius, 2)) );

                if (!isnan(sqrt_discr)){  // ray and sphere intersect
                    float t_1 = (-d_dot_o_minus_c - sqrt_discr) / d_dot_d;
                    float t_2 = (-d_dot_o_minus_c + sqrt_discr) / d_dot_d;

                    //cout << "t_1: " << t_1 << ", t_2: " << t_2 << endl;

                    image[start_index++] = 255;
                    image[start_index++] = 255;
                    image[start_index++] = 255;
                }
                else{
                    image[start_index++] = 0;
                    image[start_index++] = 0;
                    image[start_index++] = 0;
                }
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
        
        write_ppm("deneme.ppm", image, camera.image_width, camera.image_height);

        delete [] image;
    }
}
