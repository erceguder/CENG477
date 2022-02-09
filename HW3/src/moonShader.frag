#version 410

in Data{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;

in vec3 LightVector;
in vec3 CameraVector;

uniform vec3 lightPosition;
uniform sampler2D TexColor;
uniform sampler2D MoonTexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;

out vec4 FragColor;

vec3 ambientReflectenceCoefficient = vec3(0.5f);
vec3 ambientLightColor = vec3(0.6f);

vec3 specularReflectenceCoefficient= vec3(1.0f);
vec3 specularLightColor = vec3(1.0f);
float SpecularExponent = 10;

vec3 diffuseReflectenceCoefficient= vec3(1.0f);
vec3 diffuseLightColor = vec3(1.0f);

void main(){
    vec4 texColor = texture(MoonTexColor, data.TexCoord);
    float cos_theta = dot(data.Normal, LightVector);

    if (cos_theta < 0){
        cos_theta = 0;
    }

    vec3 ambient = ambientReflectenceCoefficient * ambientLightColor;
    vec3 diffuse = diffuseReflectenceCoefficient * cos_theta * diffuseLightColor;

    vec3 h = normalize(LightVector + CameraVector);

    float cos_alpha = dot(data.Normal, h);

    if (cos_alpha < 0){
        cos_alpha = 0;
    }

    cos_alpha = pow(cos_alpha, SpecularExponent);

    vec3 spec = specularReflectenceCoefficient * cos_alpha * specularLightColor;

    vec3 colour = clamp(ambient + diffuse + spec, 0.0f , 255.0f);

    FragColor = texColor * vec4(colour, 1.0f);
}
