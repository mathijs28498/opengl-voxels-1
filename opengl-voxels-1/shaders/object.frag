#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vertColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform Material material;
//uniform Light light;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform vec3 viewPos;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main() {

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir) * 0.5;

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], norm, viewDir, fragPos) * 0.5;
    }
    int i = 0;
//    result = calcPointLight(pointLights[2], norm, viewDir, fragPos);

    fragColor = vec4(result, 1.0);

//    vec3 lightDir;
//    float attenuation;
//    if (light.data[3] == 1) {
//        lightDir = normalize(light.data.xyz - fragPos);
//        // TODO: Maybe change to length
//        float dist = distance(fragPos, light.data.xyz);
////        float dist = length(light.data.xyz - fragPos);
//        attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
//    } else {
//        lightDir = normalize(-light.data.xyz);
//    }
//
//    // calc ambient light
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
//
//    // calc diffuse light
//    vec3 norm = normalize(normal);
//    float lightAngle = max(dot(norm, lightDir), 0);
//    vec3 diffuse = lightAngle * vec3(texture(material.diffuse, texCoord)) * light.diffuse;
//
//    // calc specular light
//    vec3 viewDir = normalize(viewPos - fragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
//    vec3 specular = spec * vec3(texture(material.specular, texCoord)) * light.specular;
//
//    fragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, texCoord));

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.diffuse, texCoord));

    float dist = distance(fragPos, fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    return (ambient + diffuse + specular) * attenuation;
}