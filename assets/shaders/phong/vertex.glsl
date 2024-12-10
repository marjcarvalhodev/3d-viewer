#version 330 core

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

out vec3 FragPos;   // Fragment position in world space
out vec3 Normal;    // Normal vector in world space

void main() {
    // Hardcoded transformation matrices
    mat4 model = mat4(1.0); // Identity matrix
    mat4 view = lookAt(vec3(0.0, 0.0, 3.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 projection = perspective(radians(45.0), 800.0 / 600.0, 0.1, 100.0);

    // Transform vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normal to world space
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Transform vertex to clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

