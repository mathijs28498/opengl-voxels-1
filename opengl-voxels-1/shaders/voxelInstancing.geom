#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec4 gColor[];
in float gSizeMult[];
in uint gEnabledFaces[];


out vec4 fColor;
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
    float voxSizeReal = voxSizeMult;

    vec4 luf = vec4(0, voxSizeReal, voxSizeReal, 0);
    vec4 ruf = vec4(voxSizeReal, voxSizeReal, voxSizeReal, 0);
    vec4 ldf = vec4(0, 0, voxSizeReal, 0);
    vec4 rdf = vec4(voxSizeReal, 0, voxSizeReal, 0);
    vec4 lub = vec4(0, voxSizeReal, 0, 0);
    vec4 rub = vec4(voxSizeReal, voxSizeReal, 0, 0);
    vec4 ldb = vec4(0, 0, 0, 0);
    vec4 rdb = vec4(voxSizeReal, 0, 0, 0);
    
//    vec4 dx = mvp[0] * center / 2.0 * voxSize;
//    vec4 dy = mvp[1] * center / 2.0 * voxSize;
//    vec4 dz = mvp[2] * center / 2.0 * voxSize;

    // FRONT
    if ((gEnabledFaces[0] & 0x01u) != 0u) {
        normal = vec3(0, 0, 1);
        addQuad(mvp, center, ruf, rdf, luf, ldf);
    }

    // BACK
    if ((gEnabledFaces[0] & 0x02u) != 0u) {
        normal = vec3(0, 0, -1);
        addQuad(mvp, center, lub, ldb, rub, rdb);
    }
    
    // LEFT
    if ((gEnabledFaces[0] & 0x04u) != 0u) {
        normal = vec3(1, 0, 0 );
    //    testDot = dot(-viewDir, normal);
        addQuad(mvp, center, luf, ldf, lub, ldb);
    }

    // RIGHT
    if ((gEnabledFaces[0] & 0x08u) != 0u) {
        normal = vec3(-1, 0, 0);
        addQuad(mvp, center, rub, rdb, ruf, rdf);
    }
    
    // TOP
    if ((gEnabledFaces[0] & 0x10u) != 0u) {
        normal = vec3(0, 1, 0);
    //    testDot = dot(-viewDir, normal);
        addQuad(mvp, center, rub, ruf, lub, luf);
    }

    // BOT
    if ((gEnabledFaces[0] & 0x20u) != 0u) {
        normal = vec3(0, -1, 0);
    //    if ((test & 0x20) != 0) 
        addQuad(mvp, center, ldb, ldf, rdb, rdf);
    }
}