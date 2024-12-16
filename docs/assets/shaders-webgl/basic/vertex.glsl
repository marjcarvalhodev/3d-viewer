#version 300 es
precision mediump float;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

in vec3 aPos;    // Vertex position
in vec3 aNormal; // Vertex normal

out vec3 FragPos;   // Fragment position in world space
out vec3 Normal;    // Normal vector in world space

void main() {
    FragPos = vec3(uModel * vec4(aPos, 1.0)); // Transform to world space
    Normal = normalize(mat3(transpose(inverse(uModel))) * aNormal); // Transform normal

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0); // Transform to clip space
}
