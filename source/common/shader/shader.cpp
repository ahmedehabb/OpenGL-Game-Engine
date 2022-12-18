#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();

    //TODO: Complete this function
    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.
    //Note: The function "glCreateShader" creates a shader object and returns
    // a GLuint which is the shader object name. You should use it to create
    // a shader object.
    //Note: The function "glCreateShader" takes a GLenum which is the type of
    // shader to be created.
    //type can be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
    GLuint shader = glCreateShader(type);
    //Note: The function "glShaderSource" sets the source code in the shader
    // object. You should use it to set the source code of the shader object.
    //The function "glShaderSource" takes 
    // 1. A GLuint which is the shader object name
    // 2. count A GLsizei which is the number of strings in the array of strings
    // 3. string (const GLchar**) A pointer to an array of strings which is the array of strings containing the source code of the shader
    // 4. length (const GLint*) A pointer to an array of string lengths. If length is NULL, each string is assumed to be null terminated.
    //an array of string lengths for each string in the third parameter. if null is passed,
    //then the function will deduce the lengths automatically by searching for '\0'. the termination of a character string
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    //Note: The function "glCompileShader" compiles the source code strings
    // that have been stored in the shader object. You should use it to compile
    // the shader object.
    glCompileShader(shader);
    //check for errors
    if(std::string error = checkForShaderCompilationErrors(shader); error.size() != 0){
         std::cerr << "ERROR IN " << filename << std::endl;
         std::cerr << error << std::endl;
         glDeleteShader(shader);
         return false;
     }
    //Note: The function "glAttachShader" attaches a shader object to a program
    // object. You should use it to attach the shader object to the program
    // object. Attach this shader to the program if no errors found in shader.
    glAttachShader(this->program, shader);
    //Delete the shader as it is already attached to the program.
    //If a shader object to be deleted is attached to a program object, it will be flagged for deletion, 
    // but it will not be deleted until it is no longer attached to any program object, 
    // for any rendering context (i.e., it must be detached from wherever it was attached before it will be deleted).
    // A value of 0 for shader will be silently ignored.
    glDeleteShader(shader);

    //We return true if the compilation succeeded
    return true;
}



bool our::ShaderProgram::link() const {
    //TODO: Complete this function
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.
    //Note: The function "glLinkProgram" links the program object specified
    // by program. You should use it to link the program object.
    // Link the vertex and fragment shader together.
    // links the program object specified by program. 
    // If any shader objects of type GL_VERTEX_SHADER are attached to program, 
    // they will be used to create an executable that will run on the programmable vertex processor. 
    // If any shader objects of type GL_FRAGMENT_SHADER are attached to program, 
    // they will be used to create an executable that will run on the programmable fragment processor.
    glLinkProgram(this->program);
    // Check if there is any link errors between the fragment shader and vertex shader.
    std::string error = checkForLinkingErrors(program);
    if(auto error = checkForLinkingErrors(program); error.size() != 0){
         std::cerr << "LINKING ERROR" << std::endl;
         std::cerr << error << std::endl;
         return false;
     }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}