#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uView;
uniform mat4 uProjection;

out float FragDepth; // Pass depth to fragment shader

void main() {
    vec4 pos = uProjection * uView * vec4(aPos, 1.0);
    FragDepth = pos.z / pos.w; // Perspective division for depth
    gl_Position = pos;
}
