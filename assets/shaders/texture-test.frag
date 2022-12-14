#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    //TODO: (Req 5) Change the following line to read the fragment color
    // from the texture at the received texture coordinates
    //here you are getting the the texture of the object and the texture coordinates of the object
    //here I pass the coodrinates of the texture it self(with respect to the texture normalized from 0-1 on the s,t axes)
    frag_color = texture(tex, fs_in.tex_coord);


}