#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color

    // from the texture at the received texture coordinates
    // you are getting the the texture of the object and the texture coordinates of the object from the varying
    //  I pass the coodrinates of the texture it self with respect to the texture normalized from 0-1 on the s,t axes 
    
    // by multiplying the tint with the vertex color and with the texture color
    frag_color = texture(tex, fs_in.tex_coord) * fs_in.color * tint;
}