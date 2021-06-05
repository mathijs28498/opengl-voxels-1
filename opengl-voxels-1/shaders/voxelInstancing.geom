#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 gColor[];

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
    vec4 position = gl_in[0].gl_Position;
    vec4 center = vec4(position.xyz * voxSize, 1);
    mat4 mvp = proj * view * model;
    vec4 relativeCenter = mvp * center;
    if (relativeCenter.w <= 0) return;
    vec4 relativeCenterTop = relativeCenter + voxSize;
    float relativeVoxSize = voxSize / relativeCenter.w;
    relativeCenter /= relativeCenter.w;
    if (relativeCenter.x + relativeVoxSize < -1.1 || relativeCenter.x + relativeVoxSize > 1 || relativeCenter.y + relativeVoxSize < -1.1 || relativeCenter.y > 1) {
        return;
    }

    fColor = gColor[0];

    vec4 luf = vec4(0, voxSize, voxSize, 0);
    vec4 ruf = vec4(voxSize, voxSize, voxSize, 0);
    vec4 ldf = vec4(0, 0, voxSize, 0);
    vec4 rdf = vec4(voxSize, 0, voxSize, 0);
    vec4 lub = vec4(0, voxSize, 0, 0);
    vec4 rub = vec4(voxSize, voxSize, 0, 0);
    vec4 ldb = vec4(0, 0, 0, 0);
    vec4 rdb = vec4(voxSize, 0, 0, 0);
    
    vec4 dx = mvp[0] * center / 2.0 * voxSize;
    vec4 dy = mvp[1] * center / 2.0 * voxSize;
    vec4 dz = mvp[2] * center / 2.0 * voxSize;

    // FRONT
    normal = vec3(0, 0, 1);
    // TODO: Take into account the position of the blocks, now culling happens too fast sometimes
    float testDot = dot(-viewDir, normal);
    if (testDot > 0.01 || true) 
        addQuad(mvp, center, ruf, rdf, luf, ldf);

    // BACK
    normal = vec3(0, 0, -1);
    if (testDot < -0.01 || true) 
        addQuad(mvp, center, lub, ldb, rub, rdb);
    
    // LEFT
    normal = vec3(1, 0, 0 );
    testDot = dot(-viewDir, normal);
    if (testDot < -0.01 || true) 
        addQuad(mvp, center, luf, ldf, lub, ldb);

    // RIGHT
    normal = vec3(-1, 0, 0);
    if (testDot > 0.01 || true) 
        addQuad(mvp, center, rub, rdb, ruf, rdf);
    
    // TOP
    normal = vec3(0, 1, 0);
    testDot = dot(-viewDir, normal);
    if (testDot > 0.01 || true) 
        addQuad(mvp, center, rub, ruf, lub, luf);

    // BOT
    normal = vec3(0, -1, 0);
    if (testDot < -0.01 || true) 
        addQuad(mvp, center, ldb, ldf, rdb, rdf);
}