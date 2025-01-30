~v
#version 300 es
layout (location = 0) in mediump vec4 vertex;

out mediump vec2 TexCoords;

uniform mediump mat4 model;
uniform mediump mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
~f
#version 300 es
in mediump vec2 TexCoords;
out mediump vec4 color;

uniform mediump sampler2D image;
uniform mediump vec3 spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}