#version 300 es
precision mediump float;

in vec3 FragPos; // From vertex shader
in vec3 Normal;  // From vertex shader

out vec4 FragColor;

void main() {
    vec3 scaledPos = FragPos / vec3(0.666); // Scale values to a smaller range
    vec3 color = scaledPos * 0.5 + 0.5;     // Map to [0,1] range
    FragColor = vec4(color, 1.0);
}