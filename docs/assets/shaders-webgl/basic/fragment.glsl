#version 100
precision mediump float;

varying vec3 FragPos; // From vertex shader
varying vec3 Normal;  // From vertex shader

void main() {
    vec3 scaledPos = FragPos / vec3(0.666); // Scale values to a smaller range
    vec3 color = scaledPos * 0.5 + 0.5;     // Map to [0,1] range
    gl_FragColor = vec4(color, 1.0);        // Output color
}
