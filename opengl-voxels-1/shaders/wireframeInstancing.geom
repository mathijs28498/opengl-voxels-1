#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 16) out;

in vec3 gColor[];
in float gSizeMult[];
in uint gEnabledFaces[];


out vec3 fColor;
out vec3 fragPos;
out vec3 fViewPos;
out vec3 normal;

uniform float voxSize;

uniform mat4 model = mat4(1.0);
uniform mat4 view;
uniform mat4 proj;
uniform vec3 viewPos;
uniform vec3 viewDir;

void addQuad(mat4 mvp, vec4 center, vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
    gl_Position = mvp * (center + v1);
    fragPos = gl_Position.xyz;
    EmitVertex();
    gl_Position = mvp * (center + v2);
    fragPos = gl_Position.xyz;
    EmitVertex();
    gl_Position = mvp * (center + v3);
    fragPos = gl_Position.xyz;
    EmitVertex();
    gl_Position = mvp * (center + v4);
    fragPos = gl_Position.xyz;
    EmitVertex();
    EndPrimitive();
}


// TODO: Only draw visible faces with normal towards you (max_vertices could be 12)
void main() {
    if (gEnabledFaces[0] == 0x00u) {
        return;
    }
    float voxSizeMult = voxSize * pow(2, gSizeMult[0] - 1);

    vec4 position = gl_in[0].gl_Position;
    vec4 center = vec4(position.xyz * voxSize, 1);
//    mat4 mvp = proj * view;
    mat4 mvp = proj * view * model;
//    vec4 relativeCenter = mvp * center;
//    if (relativeCenter.w <= 0) return;
//    vec4 relativeCenterTop = relativeCenter + voxSizeMult;
//    float relativeVoxSize = voxSizeMult / relativeCenter.w;
//    relativeCenter /= relativeCenter.w;
//    // TODO: Improve this
//    if (relativeCenter.x + relativeVoxSize < -1.5 || relativeCenter.x + relativeVoxSize > 1.5 || relativeCenter.y + relativeVoxSize < -1.5 || relativeCenter.y > 1.5) {
//        return;
//    }

    fColor = gColor[0];
    float voxSizeReal = voxSizeMult + 0.005;
    float voxMin = -0.005;

    vec4 luf = vec4(voxMin, voxSizeReal, voxSizeReal, 0);
    vec4 ruf = vec4(voxSizeReal, voxSizeReal, voxSizeReal, 0);
    vec4 ldf = vec4(voxMin, voxMin, voxSizeReal, 0);
    vec4 rdf = vec4(voxSizeReal, voxMin, voxSizeReal, 0);
    vec4 lub = vec4(voxMin, voxSizeReal, voxMin, 0);
    vec4 rub = vec4(voxSizeReal, voxSizeReal, voxMin, 0);
    vec4 ldb = vec4(voxMin, voxMin, voxMin, 0);
    vec4 rdb = vec4(voxSizeReal, voxMin, voxMin, 0);
    
    gl_Position = mvp * (center + lub);
    EmitVertex();
    gl_Position = mvp * (center + luf);
    EmitVertex();
    gl_Position = mvp * (center + ruf);
    EmitVertex();
    gl_Position = mvp * (center + rdf);
    EmitVertex();
    gl_Position = mvp * (center + ldf);
    EmitVertex();
    gl_Position = mvp * (center + luf);
    EmitVertex();
    EndPrimitive();

    
    gl_Position = mvp * (center + ruf);
    EmitVertex();
    gl_Position = mvp * (center + rub);
    EmitVertex();
    gl_Position = mvp * (center + rdb);
    EmitVertex();
    gl_Position = mvp * (center + ldb);
    EmitVertex();
    gl_Position = mvp * (center + lub);
    EmitVertex();
    gl_Position = mvp * (center + rub);
    EmitVertex();
    EndPrimitive();

    gl_Position = mvp * (center + rdb);
    EmitVertex();
    gl_Position = mvp * (center + rdf);
    EmitVertex();
    EndPrimitive();

    gl_Position = mvp * (center + ldb);
    EmitVertex();
    gl_Position = mvp * (center + ldf);
    EmitVertex();
    EndPrimitive();

    
}