~v
#version 300 es
layout (location = 0) in mediump vec4 vertex; // <vec2 pos, vec2 tex>

out mediump vec2 TexCoords;

uniform mediump mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}
~f
#version 300 es
in mediump vec2 TexCoords;
out mediump vec4 color;

uniform mediump sampler2D text;
uniform mediump vec3 textColor;

void main()
{
    mediump vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}