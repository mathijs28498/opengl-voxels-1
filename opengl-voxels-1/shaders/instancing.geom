#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 gColor[];

out vec3 fColor;

uniform float voxSize;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void addQuad(mat4 mvp, vec4 center, vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
    gl_Position = mvp * (center + v1);
    EmitVertex();
    gl_Position = mvp * (center + v2);
    EmitVertex();
    gl_Position = mvp * (center + v3);
    EmitVertex();
    gl_Position = mvp * (center + v4);
    EmitVertex();
    EndPrimitive();
}

bool IsCulled(vec4 normal) {
    return normal.z > 0;
}

// TODO: Make own addQuad function
// TODO: Only draw visible faces with normal towards you (max_vertices could be 12)

// TODO ERROR: Maybe this doesn't work (voxSize is now / 2)
void main() {
    float realSize = voxSize / 2;
    vec4 position = gl_in[0].gl_Position;
    vec4 center = vec4(position.xyz * voxSize, 1);
    mat4 mvp = proj * view * model;
//    fColor = vec3(1 - cos(length(position) * realSize) / 2 + 0.5, sin(length(position) * realSize) / 2 + 0.5, cos(length(position) * realSize) / 2 + 0.5);
    fColor = gColor[0];
    
    vec4 luf = vec4(-realSize, realSize, realSize, 0);
    vec4 ruf = vec4(realSize, realSize, realSize, 0);
    vec4 ldf = vec4(-realSize, -realSize, realSize, 0);
    vec4 rdf = vec4(realSize, -realSize, realSize, 0);
    vec4 lub = vec4(-realSize, realSize, -realSize, 0);
    vec4 rub = vec4(realSize, realSize, -realSize, 0);
    vec4 ldb = vec4(-realSize, -realSize, -realSize, 0);
    vec4 rdb = vec4(realSize, -realSize, -realSize, 0);

    // FRONT
    addQuad(mvp, center, ruf, rdf, luf, ldf);
    // BACK
    addQuad(mvp, center, lub, ldb, rub, rdb);

    // LEFT
    addQuad(mvp, center, luf, ldf, lub, ldb);
    // RIGHT
    addQuad(mvp, center, rub, rdb, ruf, rdf);

    // TOP
    addQuad(mvp, center, rub, ruf, lub, luf);
    // BOT
    addQuad(mvp, center, ldb, ldf, rdb, rdf);
}