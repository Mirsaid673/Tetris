#version 330 core

layout (location = 5) in vec2 a_pos;
layout (location = 3) in vec2 a_tex_coord;

out vec2 o_tex_coord;

uniform mat3 PVM;
uniform vec2 tex_coord_scale;
uniform vec2 tex_coord_offset;

void main()
{
    gl_Position = vec4(PVM * vec3(a_pos, 1), 1);
    o_tex_coord = (a_tex_coord * tex_coord_scale) + tex_coord_offset;
}