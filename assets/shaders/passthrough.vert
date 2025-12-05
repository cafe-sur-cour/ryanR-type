#version 150

in vec2 position;
in vec2 texCoord;

out vec2 vertexTexCoord;

uniform mat4 projection;
uniform mat4 transform;

void main() {
    vertexTexCoord = texCoord;
    gl_Position = projection * transform * vec4(position, 0.0, 1.0);
}
