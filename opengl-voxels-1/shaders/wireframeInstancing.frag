#version 330 core
in vec3 fViewPos;
in vec3 fColor;
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

void main() { 
    FragColor = vec4(fColor, 1.0);   
}  