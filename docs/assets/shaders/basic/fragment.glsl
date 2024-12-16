#version 330 core

in vec3 FragPos; // From vertex shader
in vec3 Normal;  // From vertex shader

out vec4 FragColor;

// Hardcoded lighting
void main() {
    // Map FragPos coordinates to color
    // vec3 color = (FragPos + vec3(1.0)) * 0.5; // Normalize to range [0,1]
    vec3 scaledPos = FragPos / vec3(0.666); // Scale values to a smaller range
    vec3 color = scaledPos * 0.5 + 0.5;   // Map to [0,1] range
    FragColor = vec4(color, 1.0);
}