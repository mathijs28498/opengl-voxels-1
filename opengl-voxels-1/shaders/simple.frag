#version 330 core
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 color;

void main() {
	fragColor = vec4(1.0, 0, 0, 1.0);
}