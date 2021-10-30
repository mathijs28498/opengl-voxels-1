#version 330 core
struct Material {
  vec3 color;
};

layout (location = 0) in uint positionInt;
layout (location = 1) in uint materialAndEnabledInt;

out vec3 gColor;
out float gSizeMult;
out uint gEnabledFaces;

Material materials[7] = Material[7] (
	Material(vec3(0.2, 0.23, 0.92)),
	Material(vec3(0.2, 0.56, 0.92)),
	Material(vec3(0.81, 0.75, 0.27)),
	Material(vec3(0.29, 0.81, 0.27)),
	Material(vec3(0.44, 0.48, 0.47)),
	Material(vec3(0.23, 0.26, 0.25)),
	Material(vec3(0.9, 0.9, 0.9))
);

vec3 intToVec3(uint intArg) {
	float i0 = int( intArg		  & 0xFFu);
	float i1 = int((intArg >> 8u)  & 0xFFu);
	float i2 = int((intArg >> 16u) & 0xFFu);
	return vec3(i0, i1, i2);  
}

uint int32ToInt16(uint intArg) {
	return 0x00000000u | (intArg & 0xFFFFu);
}

vec4 intToVec4(uint intArg) {
	float i0 = int( intArg		  & 0xFFu);
	float i1 = int((intArg >> 8u)  & 0xFFu);
	float i2 = int((intArg >> 16u) & 0xFFu);
	float i3 = int((intArg >> 24u) & 0xFFu);
	return vec4(i0, i1, i2, i3);  
}

void main() {
	vec4 position = intToVec4(positionInt);

	gl_Position = vec4(position.xyz, 1.0);
	uint matID = int32ToInt16(materialAndEnabledInt);
	gColor = materials[matID].color;
	gSizeMult = position.w;
	gEnabledFaces = (materialAndEnabledInt >> 24u) & 0xFFu;
}