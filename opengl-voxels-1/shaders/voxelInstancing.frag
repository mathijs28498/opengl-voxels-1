#version 330 core
in vec3 fColor;
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform float lightIntensity;

vec3 calcDirLight(vec3 viewDir);

void main() { 
    FragColor = vec4(calcDirLight(viewPos - fragPos), 1.0);   
//    FragColor = vec4(fColor, 1.0);   
}  

vec3 calcDirLight(vec3 viewDir) {
    vec3 lightDirNorm = normalize(-lightDir);
    float diff = max(dot(normal, lightDirNorm), 0.0);

    vec3 reflectDir = reflect(-lightDirNorm, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    
    float r = lightIntensity * 3;
    if (r > 1) r = 1;
    float gb = lightIntensity * 1;
    if (gb > 1) gb = 1;

    vec3 ambient = fColor * 0.1 * lightIntensity + 0.01;
    vec3 diffuse = diff * fColor * 0.8 * vec3(r, gb, gb);
    vec3 specular = spec * fColor * vec3(r, gb, gb);


    return ambient + diffuse;
}