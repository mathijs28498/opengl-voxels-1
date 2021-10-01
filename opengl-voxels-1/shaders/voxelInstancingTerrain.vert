#version 330 core
//precision highp int;

layout (location = 0) in unsigned int positionInt;
layout (location = 1) in unsigned int colorAndEnabledInt;
layout (location = 2) in unsigned int vEnabledFaces;

out vec3 gColor;
out float gSizeMult;
out unsigned int gEnabledFaces;

vec3 intToVec3(unsigned int intArg) {
	float i0 = int( intArg		  & 0xFFu);
	float i1 = int((intArg >> 8u)  & 0xFFu);
	float i2 = int((intArg >> 16u) & 0xFFu);
	return vec3(i0, i1, i2);  
}

vec4 intToVec4(unsigned int intArg) {
	float i0 = int( intArg		  & 0xFFu);
	float i1 = int((intArg >> 8u)  & 0xFFu);
	float i2 = int((intArg >> 16u) & 0xFFu);
	float i3 = int((intArg >> 24u) & 0xFFu);
	return vec4(i0, i1, i2, i3);  
}

void main() {
	vec4 position = intToVec4(positionInt);

	gl_Position = vec4(position.xyz, 1.0);
	gColor = intToVec3(colorAndEnabledInt) / 255.0;
	gSizeMult = position.w;
	gEnabledFaces = (colorAndEnabledInt >> 24u) & 0xFFu;
}