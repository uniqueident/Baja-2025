~v
#version 330 core
// <vec2 position, vec2 texCoords>
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;

    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
~f
#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    FragColor = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}