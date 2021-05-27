#version 330 core
layout (location = 0) in vec3 vVertPos;
layout (location = 1) in vec3 vColor;

out vec3 gColor;

void main() {
	gl_Position =  vec4(vVertPos, 1.0);
	gColor = vColor;
}