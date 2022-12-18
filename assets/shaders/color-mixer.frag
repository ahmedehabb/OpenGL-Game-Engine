#version 330 core

// This shader is designed to work with "triangle.vert" and it receives an
// interpolated varying which represents the vertex color.

in Varyings {
    vec3 color;
} fs_in;

out vec4 frag_color;

//TODO: (Req 1) Declare 3 uniforms: red, green, blue. Each of these will be a vec4.
//The red uniform will be used to mix the red channel, the green uniform will be used
//to mix the green channel, and the blue uniform will be used to mix the blue channel.
//The red, green, and blue uniforms will be used to apply a color matrix to the vertex
uniform vec4 red = vec4(1.0, 0.0, 0.0, 0.0);
uniform vec4 green = vec4(0.0, 1.0, 0.0, 0.0);
uniform vec4 blue = vec4(0.0, 0.0, 1.0, 0.0);

// currently the shader just returns the interpalated color varying.
// However, we want to mix the color channels around. We can do this using a 
// color matrix which we will send to the shader as 3 uniforms: red, green, blue.
// Each of these 3 variables will be a vec4. To apply the channel mixing for a
// certain channel (e.g. red), we apply this linear transformation:
// frag_color.r = red.r * fs_in.color.r + red.g + fs_in.color.g + red.b + fs_in.color.b + red.a;
// However, this line is too long to write, so we can simplify it using a dot product
// (which is defined in the "dot" function).

//TODO: (Req 1) Finish this shader and apply the channel mixing using the "dot" function.
//You will need to declare 3 uniforms: red, green, blue.

void main(){
    //this is the line that we want to simplify using the dot function
    //red.rgb will return vec3 with red.r , red.g , red.b and then apply dot product
    frag_color.r = dot(red.rgb, fs_in.color) + red.a;
    frag_color.g = dot(green.rgb, fs_in.color) + green.a;
    frag_color.b = dot(blue.rgb, fs_in.color) + blue.a;
}