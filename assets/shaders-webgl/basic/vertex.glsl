#version 100
attribute vec3 aPos;    // Vertex position
attribute vec3 aNormal; // Vertex normal

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

varying vec3 FragPos;   // Fragment position in world space
varying vec3 Normal;    // Normal vector in world space

void main() {
    FragPos = vec3(uModel * vec4(aPos, 1.0)); // Transform to world space
    Normal = normalize(mat3(uModel) * aNormal); // Transform normal (simplified for WebGL 1.0)

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0); // Transform to clip space
}
