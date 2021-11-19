#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include "vec3f.h"

namespace parser
{
    struct Vec4f{
        double x, y, z, w;   // Left, right, bottom, top
    };
    
    struct Camera{
        Vec3f position;
        Vec3f gaze;
        Vec3f up;
        Vec4f near_plane;
        double near_distance;
        int image_width, image_height;
        std::string image_name;
    };

    struct PointLight
    {
        Vec3f position;
        Vec3f intensity;
    };

    struct Material
    {
        bool is_mirror;
        Vec3f ambient;
        Vec3f diffuse;
        Vec3f specular;
        Vec3f mirror;
        double phong_exponent;
    };

    struct Face
    {
        int v0_id;
        int v1_id;
        int v2_id;
        Vec3f normal;
    };

    struct Mesh
    {
        int material_id;
        std::vector<Face> faces;
        // std::vector<Vec3f> normals;
    };

    struct Triangle
    {
        int material_id;
        Face indices;
        // Vec3f normal;
    };

    struct Sphere
    {
        int material_id;
        int center_vertex_id;
        double radius;
    };

    struct Scene
    {
        //Data
        Vec3f background_color;
        double shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        Vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<Vec3f> vertex_data;
        std::vector<Mesh> meshes;
        std::vector<Triangle> triangles;
        std::vector<Sphere> spheres;

        //Functions
        void loadFromXml(const std::string &filepath);
    };
}

#endif
