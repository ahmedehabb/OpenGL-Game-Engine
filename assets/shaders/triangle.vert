#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

//varying means that the value of the variable is different from one run to another
//varying means that the value of the variable is different for each vertex 
//the value of the variable is passed to the fragment shader
//the value of the variable is interpolated between the vertices
out Varyings {
    vec3 color;
} vs_out;


//define the translation and scale uniform variables
//the default value for translation is (0.0, 0.0) and for scale is (1.0, 1.0) from the given code
//uniform means that the value of the variable is the same for all vertices
uniform vec2 translation = vec2(0.0, 0.0);
uniform vec2 scale = vec2(1.0, 1.0);
// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

//TODO: (Req 1) Finish this shader

void main(){
    //define the vertices of the triangle as vec2 because the 3rd coordinate is 0.0 in the normalized device coordinates
    vec2 vertices[3] = vec2[3](vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.0, 0.5));
    //define the colors 
    vec3 colors[3] = vec3[3](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
    //the gl_VertexID is the index of the vertex in the vertices array
    //the gl_Position is the position of the vertex in the normalized device coordinates
    //the position of the vertex is the scale of the vertex in the vertices array plus the translation
    //the 3rd coordinate is 0.0 because it is zero in the normalized device coordinates
    gl_Position = vec4(scale * vertices[gl_VertexID] + translation, 0.0, 1.0);
    //the color of the vertex is the color of the vertex in the colors array
    vs_out.color = colors[gl_VertexID];
}