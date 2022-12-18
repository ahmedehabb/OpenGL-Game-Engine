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
    //gl_FragCoord is a vec4 that contains the position of the current pixel in the window
    //the first 2 components of gl_FragCoord are the x and y position of the current pixel
    //ivec2 is a 2D integer vector that is used to store the position of the current pixel so that we can use it to calculate the color of the current pixel
    //so that when we divide the x and y position of the current pixel by the size of tile, we get an integer value
    ivec2 pos = ivec2(gl_FragCoord.xy);
    //get the color of the current pixel
    //the color of the current pixel is determined by the position of the pixel
    //if the sum of the x and y position of the pixel each divided by the size of tile is even, then the color is colors[0]
    //otherwise, the color is colors[1]
    int color = (pos.x/size + pos.y/size) % 2;
    frag_color = vec4(colors[color], 1.0);
}