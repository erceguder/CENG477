#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "vec3f.h"
#include "vec3i.h"

typedef unsigned char RGB[3];

using namespace std;

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.

    const RGB BAR_COLOR[8] =
    {
        { 255, 255, 255 },  // 100% White
        { 255, 255,   0 },  // Yellow
        {   0, 255, 255 },  // Cyan
        {   0, 255,   0 },  // Green
        { 255,   0, 255 },  // Magenta
        { 255,   0,   0 },  // Red
        {   0,   0, 255 },  // Blue
        {   0,   0,   0 },  // Black
    };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char* image = new unsigned char [width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int colIdx = x / columnWidth;
            image[i++] = BAR_COLOR[colIdx][0];
            image[i++] = BAR_COLOR[colIdx][1];
            image[i++] = BAR_COLOR[colIdx][2];
        }
    }

    write_ppm("test.ppm", image, width, height);

    Vec3i vec1(3,7,1);
    Vec3i vec2(3, 0, 1);
    // cout << (vec1.dotProduct(vec2)).getX() << " " << vec1.dotProduct(vec2).getY() << " " << vec1.dotProduct(vec2).getZ() << endl;
    // cout << vec1.dotProduct(vec2) << endl;
    // cout << (vec1 * vec2).getX() << " " << (vec1 * vec2).getY() << " " << (vec1 * vec2).getZ() << endl;
    

    // Ray ray(vec1, vec2);
    // ray.setO(vec1);
    // ray.setD(vec2);

    // cout << ray.getD().getX() << " " << ray.getD().getY() << " " << ray.getD().getZ() << endl;

}
