#version 330 core
layout (location = 0) in vec3 vVertPos;

void main() {
	gl_Position =  vec4(vVertPos, 1.0);
}