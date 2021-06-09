#version 330 core
layout (location = 0) in vec3 vVertPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in int vEnabledFaces;

out vec3 gColor;
out int gEnabledFaces;

void main() {
	gl_Position =  vec4(vVertPos, 1.0);
	gColor = vColor;
	gEnabledFaces = vEnabledFaces;
}