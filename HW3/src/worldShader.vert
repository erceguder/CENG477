#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTex;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 NormalMatrix;
uniform mat4 MVP;

uniform sampler2D TexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;

uniform float heightFactor;
uniform float imageWidth;
uniform float imageHeight;

out Data
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;


out vec3 LightVector;// Vector from Vertex to Light;
out vec3 CameraVector;// Vector from Vertex to Camera;

void main(){
    // get texture value, compute height
    // compute normal vector
    vec4 tmp = MVP * vec4(VertexPosition, 1.0f);
    
    data.Position = vec3(tmp.x/tmp.w, tmp.y/tmp.w, tmp.z/tmp.w);
    data.Normal = VertexNormal;
    data.TexCoord = VertexTex;

    gl_Position = tmp;
}