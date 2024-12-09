#version 330 core

out vec4 FragColor;
in float FragDepth; // Depth passed from vertex shader

void main() {
    FragColor = vec4(FragDepth, FragDepth, FragDepth, 1.0); // Grayscale by depth
}