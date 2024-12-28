~v
#version 330 core
attribute vec4 vertexIn;
attribute vec2 textureIn;

varying vec2 textureOut;

void main() {
    gl_Position = vertexIn;

    textureOut = textureIn;
}
~f
#version 330 core
in vec2 TexCoords;

out vec4 color;
