#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

out Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 3) Change the next line to apply the transformation matrix
    // to the vertex position
    //so gl_position contains the transformed vertex position
    //we add 1.0 to the position vector to make it a vec4 with w=1.0 as required by the transform matrix and it is a point
    //if it was a vector we would add 0.0 to the w component
    gl_Position = transform * vec4(position, 1.0);
    // No need to change any of the following lines
    vs_out.position = position;
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    vs_out.normal = normal;
}