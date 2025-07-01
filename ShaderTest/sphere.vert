#version 110

uniform float radius;
uniform float rotX;
uniform float rotY;
uniform vec2 screenCenter;
uniform float zoom;

varying vec3 vNormal;
varying vec3 vPosition;
varying vec4 vColor;

vec3 rotateX(vec3 v, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return vec3(v.x, c * v.y - s * v.z, s * v.y + c * v.z);
}
vec3 rotateY(vec3 v, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return vec3(c * v.x + s * v.z, v.y, -s * v.x + c * v.z);
}

vec3 getCorner(int face, float x, float y) {
    if (face == 0) return vec3(x, y, 1.0);
    if (face == 1) return vec3(-x, y, -1.0);
    if (face == 2) return vec3(-1.0, y, x);
    if (face == 3) return vec3(1.0, y, -x);
    if (face == 4) return vec3(x, 1.0, -y);
    if (face == 5) return vec3(x, -1.0, y);
    return vec3(0.0);
}

void main() {
    int face = int(gl_MultiTexCoord0.x);
    vec2 pos = gl_Vertex.xy;

    vec3 cubePos = getCorner(face, pos.x, pos.y);
    vec3 spherePos = normalize(cubePos) * radius;
    vec3 rotatedPos = rotateY(rotateX(spherePos, rotX), rotY);

    vPosition = rotatedPos;
    vNormal = normalize(rotatedPos);

    float distance = 256.0;
    float scale = distance / (distance + rotatedPos.z);
    vec2 projPos = screenCenter + vec2(rotatedPos.x, -rotatedPos.y) * scale;
    projPos = screenCenter + (projPos - screenCenter) * zoom;
    vec2 ndc = (projPos / vec2(400.0, 400.0)) - vec2(1.0, 1.0);
    ndc.y = -ndc.y;

    gl_Position = vec4(ndc, 0.0, 1.0);
    vColor = gl_Color;
       // Face colors: define array
    vec4 faceColors[6];
    faceColors[0] = vec4(1, 0, 0, 1);   // red front
    faceColors[1] = vec4(0, 1, 0, 1);   // green back
    faceColors[2] = vec4(0, 0, 1, 1);   // blue left
    faceColors[3] = vec4(1, 1, 0, 1);   // yellow right
    faceColors[4] = vec4(0, 1, 1, 1);   // cyan top
    faceColors[5] = vec4(1, 0, 1, 1);   // magenta bottom

   // vColor = faceColors[face];
}
