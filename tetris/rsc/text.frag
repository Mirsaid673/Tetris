#version 330 core

out vec4 out_color;

in vec2 o_tex_coord;
in vec3 o_color;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
    vec4 col = texture(tex, o_tex_coord);
    if(col.a < 1.f)
        discard;
    out_color = vec4(color, 1);
}