#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "vec3f.h"
#include "vec3i.h"
#include <cmath>
#include <pthread.h>
#include <limits>
#include <chrono>


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

        int faces_count = scene.meshes[i].faces.size();

        for (int j=0; j<faces_count; j++){
            
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

Vec3f diffuse_shading(Vec3f diffuse_coeff, Vec3f w_i, Vec3f normal, Vec3f light_intensity){//, double distance){
    double distance = w_i.length();
    
    Vec3f wi = w_i.normalize();
    Vec3f n = normal.normalize();                   // two lines can be omitted if parameters are given normalized

    double cos_theta = MAX(0, wi.dot(n));
    Vec3f i_over_r_squared = light_intensity * (1/(distance * distance));           // I/(r^2)
    
    return (diffuse_coeff * cos_theta).elementwiseMultiplication(i_over_r_squared);
}

Vec3f ambient_shading(Vec3f ambient_coeff, Vec3f radiance){
    return ambient_coeff.elementwiseMultiplication(radiance);
}

Vec3f specular_shading(Material material, Vec3f normal, Vec3f w_i, Vec3f w_o, Vec3f light_intensity){//, double distance){
    Vec3f res;
    double distance = w_i.length();
    
    Vec3f wi = w_i.normalize();
    Vec3f wo = w_o.normalize();
    //Vec3f n = normal.normalize();

    double cos_theta = wi.dot(normal);// * (1/(wi.length() * normal.length()));       // if the light is coming from behind the surface
    double theta = acos(cos_theta) * 180.0 / PI;
    if (theta >= 90)
        return res;
    
    Vec3f wi_plus_wo = wi + wo;
    Vec3f half = wi_plus_wo * (1/wi_plus_wo.length());

    double cos_alpha = pow(MAX(0, normal.dot(half)), material.phong_exponent);
    Vec3f i_over_rsqured = light_intensity * (1/(distance * distance));
    
    res = (material.specular * cos_alpha).elementwiseMultiplication(i_over_rsqured);

    return res;
}

bool triangle_intersects(bool shadow, Ray ray, Face face, double* min_t, Vec3f* normal_p){
    Vec3f a = scene.vertex_data[face.v0_id - 1];
    Vec3f b = scene.vertex_data[face.v1_id - 1];
    Vec3f c = scene.vertex_data[face.v2_id - 1];

    Vec3f normal = face.normal;

    if (!shadow && (normal.dot(ray.getDirection()) > 0)) return false;   // Back-face culling

    if (ray.getDirection().dot(normal) == 0) return false;  // doubleing point precision loss ?

    double t = ((a - ray.getOrigin()).dot(normal)) / (ray.getDirection().dot(normal));

    Vec3f p = ray.getPoint(t);

    Vec3f v_p = (p - b) * (a - b);
    Vec3f v_c = (c - b) * (a - b);
    if (v_p.dot(v_c) < (-1e-12)) return false;

    v_p = (p - a) * (c - a);
    v_c = (b - a) * (c - a);
    if (v_p.dot(v_c) < (-1e-12)) return false;

    v_p = (p - c) * (b - c);
    v_c = (a - c) * (b - c);
    if (v_p.dot(v_c) < (-1e-12)) return false;

    if (t < *min_t){
        *min_t = t;
        *normal_p = normal;
        return true;
    }
    return false;
}

/*double determinant(double matrix[3][3]){
    double result = matrix[0][0] * (matrix[1][1]*matrix[2][2] - matrix[2][1]*matrix[1][2]);
    result += matrix[0][1] * (matrix[2][0]*matrix[1][2] - matrix[1][0]*matrix[2][2]);
    result += matrix[0][2] * (matrix[1][0]*matrix[2][1] - matrix[2][0]*matrix[1][1]);

    return result;
}

bool triangle_intersects(Ray ray, Triangle triangle, double* min_distance, Vec3f* normal_p){
    //Returns true if the triangle is the closest object to the camera.
    
    Vec3f a = scene.vertex_data[triangle.indices.v0_id - 1];
    Vec3f b = scene.vertex_data[triangle.indices.v1_id - 1];
    Vec3f c = scene.vertex_data[triangle.indices.v2_id - 1];

    Vec3f direction = ray.getDirection();

    double A[3][3] = {
        {(a-b).x, (a-c).x, direction.x},
        {(a-b).y, (a-c).y, direction.y},
        {(a-b).z, (a-c).z, direction.z}
    };

    double A_1[3][3] = {
        {(a-camera.position).x, (a-c).x, direction.x},
        {(a-camera.position).y, (a-c).y, direction.y},
        {(a-camera.position).z, (a-c).z, direction.z}
    };

    double A_2[3][3] = {
        {(a-b).x, (a-camera.position).x, direction.x},
        {(a-b).y, (a-camera.position).y, direction.y},
        {(a-b).z, (a-camera.position).z, direction.z}
    };

    double A_3[3][3] = {
        {(a-b).x, (a-c).x, (a-camera.position).x},
        {(a-b).y, (a-c).y, (a-camera.position).y},
        {(a-b).z, (a-c).z, (a-camera.position).z}
    };

    double det_A = determinant(A);
    
    double beta = determinant(A_1) / det_A;
    double gamma = determinant(A_2) / det_A;
    double t = determinant(A_3) / det_A;

    if (beta >= 0 && gamma >= 0 && (beta + gamma) <= 1)
        if (t < *min_distance){
            *min_distance = t;
            *normal_p = triangle.indices.normal;

            return true;
        }

    return false;
}*/

bool sphere_intersects(Ray ray, Sphere sphere, double* min_t, Vec3f* normal_p){
    /*
        Returns true if the sphere is the closest object to the camera.
    */
    Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];

    Vec3f o_minus_c = ray.getOrigin() - center;                               // o - c
    double d_dot_o_minus_c = ray.getDirection().dot(o_minus_c);               // d . (o - c)
    double d_dot_d = ray.getDirection().dot(ray.getDirection());              // d . d

    double sqrt_discr = sqrt( pow(d_dot_o_minus_c, 2) - d_dot_d * (o_minus_c.dot(o_minus_c) - pow(sphere.radius, 2)) );

    if (!isnan(sqrt_discr)){  // ray and sphere intersect
        double t_1 = (-d_dot_o_minus_c - sqrt_discr) / d_dot_d;
        double t_2 = (-d_dot_o_minus_c + sqrt_discr) / d_dot_d;
        double t = MIN(t_1, t_2);                // does it apply all the time??

        if (t < *min_t){
            *min_t = t;
            *normal_p = (ray.getPoint(t) - center).normalize();
            
            return true;
        }
    }
    return false;
}

int clamp(double val){
    if (val < 0)
        return 0;
    if (val>255)
        return 255;
    return val;
}

bool in_shadow(Ray shadow_ray, PointLight light){
    
    double t_to_light = 1;//(light.position - shadow_ray.getOrigin()).length() / shadow_ray.getDirection().length();
    Vec3f dummy_normal;

    for (auto sphere: scene.spheres)
        if (sphere_intersects(shadow_ray, sphere, &t_to_light, &dummy_normal))
            return true;
    
    for (auto triangle: scene.triangles)
        if (triangle_intersects(true, shadow_ray, triangle.indices, &t_to_light, &dummy_normal))
            return true;
    
    for (auto mesh: scene.meshes)
        for (auto face: mesh.faces){
            if (triangle_intersects(true, shadow_ray, face, &t_to_light, &dummy_normal))
                return true;
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

    double left = camera.near_plane.x;
    double right = camera.near_plane.y;
    double bottom = camera.near_plane.z;
    double top = camera.near_plane.w;

    double pixel_width = (right - left) / image_width;
    double pixel_height = (top - bottom) / image_height;

    Vec3f m = camera.position + (camera.gaze * camera.near_distance);   // m = e + gaze*distance
    Vec3f u = camera.gaze * camera.up;                                  // u = (-w) x v
    Vec3f q = m + (u * left) + (camera.up * top);                       // q = m + u*l + v*t

    for (int j=start_row; j <= end_row; j++){                           // rows [start, end]
        for (int i=0; i < image_width; i++){                            // columns
            
            double min_t = numeric_limits<double>::max();
            Vec3f normal;                                               // normal at intersection point

            int material_id = 0;

            bool intersects = false;
            
            double s_u = (i + .5) * pixel_width;
            double s_v = (j + .5) * pixel_height;

            Vec3f s = q + (u * s_u) - (camera.up * s_v);                // s = q + u * s_u - v * s_v
            Ray primaryRay(camera.position, s - camera.position);       // d = s - e

            for (auto sphere: scene.spheres)
                if(sphere_intersects(primaryRay, sphere, &min_t, &normal)){ 
                    // this sphere is the closest object so far
                    material_id = sphere.material_id;
                    intersects = true;
                }

            for (auto triangle: scene.triangles)
                if (triangle_intersects(false, primaryRay, triangle.indices, &min_t, &normal)){
                    // this triangle is the closest object so far
                    material_id = triangle.material_id;
                    intersects = true;
                }

            for (auto mesh: scene.meshes)
                for (auto face: mesh.faces){
                    if (triangle_intersects(false, primaryRay, face, &min_t, &normal)){
                        // this triangle is the closest object so far
                        material_id = mesh.material_id;
                        intersects = true;
                    }
                }

            if (!intersects){
                image[start_index++] = scene.background_color.x;
                image[start_index++] = scene.background_color.y;
                image[start_index++] = scene.background_color.z;
            }
            else{
                Vec3f intersection_pt = primaryRay.getPoint(min_t);

                Vec3f ambient = ambient_shading(scene.materials[material_id-1].ambient, scene.ambient_light);
                Vec3f diffuse;
                Vec3f specular;

                for (auto point_light: scene.point_lights){
                    Vec3f w_i = point_light.position - intersection_pt;
                    Vec3f w_o = camera.position - intersection_pt;

                    // Ray shadow_ray(intersection_pt + (normal*scene.shadow_ray_epsilon), w_i);
                    
                    // if (in_shadow(shadow_ray, point_light)) continue;

                    diffuse = diffuse + diffuse_shading(scene.materials[material_id-1].diffuse, w_i, 
                            normal, point_light.intensity);

                    specular = specular + specular_shading(scene.materials[material_id-1], normal, w_i, 
                            w_o, point_light.intensity);
                }

                /* Clamp values under 0 and over 255 */                
                image[start_index++] = clamp(diffuse.x + ambient.x + specular.x);//clamp(ambient.x);
                image[start_index++] = clamp(diffuse.y+ ambient.y + specular.y);//clamp(ambient.y);
                image[start_index++] = clamp(diffuse.z + ambient.z + specular.z);//clamp(ambient.z);
            }
            //cout << "i:" << i << ", j: " << j << endl;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]){

    auto t_start = chrono::high_resolution_clock::now();

    scene.loadFromXml(argv[1]);
    pthread_t threads[THREAD_NUM];

    //pthread_create(&threads[0], NULL, computeNormals, NULL);
    computeNormals();

    for (int i=0; i < scene.cameras.size(); i++){

        camera = scene.cameras[i];
        image = new unsigned char [camera.image_width * camera.image_height * 3];

        if (image == NULL){
            cout << "image cannot be allocated\n";
            exit(1);
        }

        /* This assumes image height is a factor of 4 */
        int row_borders[THREAD_NUM][2] = {
            {0, (camera.image_height/THREAD_NUM) - 1},
            {camera.image_height/THREAD_NUM, 2*(camera.image_height/THREAD_NUM) - 1},
            {2*(camera.image_height/THREAD_NUM), 3*(camera.image_height/THREAD_NUM) - 1},
            {3*(camera.image_height/THREAD_NUM), 4*(camera.image_height/THREAD_NUM) - 1},
            // {4*(camera.image_height/THREAD_NUM), 5*(camera.image_height/THREAD_NUM) - 1},
            // {5*(camera.image_height/THREAD_NUM), 6*(camera.image_height/THREAD_NUM) - 1},
            // {6*(camera.image_height/THREAD_NUM), 7*(camera.image_height/THREAD_NUM) - 1},
            // {7*(camera.image_height/THREAD_NUM), 8*(camera.image_height/THREAD_NUM) - 1}
        };

        /*int single_thread_borders[2] = {0, camera.image_height-1};
        pthread_create(threads, NULL, trace_routine, single_thread_borders);*/

        for (int i=0; i < THREAD_NUM; i++){
            pthread_create(&threads[i], NULL, trace_routine, row_borders[i]);
        }

        for (int i=0; i < THREAD_NUM; i++){
            pthread_join(threads[i], NULL);
        }
        
        write_ppm(camera.image_name.c_str(), image, camera.image_width, camera.image_height);

        delete [] image;

    }
    auto t_end = chrono::high_resolution_clock::now();
    cout << chrono::duration<double, milli>(t_end - t_start).count() / 1000 << endl;
}
