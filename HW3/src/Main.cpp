#include <iostream>
#include <string>
#include <vector>
#include "EclipseMap.h"

using namespace std;

int main(int argc, char* argv[]){
    EclipseMap *openGL = new EclipseMap();
	openGL->Render(argv[2],argv[1],argv[3]);
    // openGL->Render("earth_texture_map.jpg", "height_map.jpg", "moon_texture_map.jpg");
}