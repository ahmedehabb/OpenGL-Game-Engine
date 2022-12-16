#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    //get the position of the current pixel
    ivec2 pos = ivec2(gl_FragCoord.xy);
    //get the color of the current pixel
    int color = (pos.x/size + pos.y/size) % 2;
    frag_color = vec4(colors[color], 1.0);
}