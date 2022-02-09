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
uniform mat4 ModelingMatrix;

uniform sampler2D TexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;

uniform float heightFactor;
uniform float imageWidth;
uniform float imageHeight;

out Data{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;

out vec3 LightVector;// Vector from Vertex to Light;
out vec3 CameraVector;// Vector from Vertex to Camera;

void main(){
    // get texture value, compute height
    // compute normal vector
    float height = texture(TexGrey, VertexTex).x * heightFactor;

    vec4 tmp = vec4(VertexPosition + VertexNormal * height, 1.0f);
    vec4 vertex = MVP * tmp;
    
    data.Position = (ModelingMatrix * tmp).xyz;
    data.Normal = normalize( (transpose(inverse(ModelingMatrix)) * vec4(VertexNormal, 0.0f)).xyz );
    data.TexCoord = VertexTex;

    LightVector = normalize(lightPosition - data.Position);
    CameraVector = normalize(cameraPosition - data.Position);

    gl_Position = vertex;
}