#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            //Hint: Use the function glCreateProgram to create a shader program
            //Hint: The function glCreateProgram returns a GLuint which is the shader program
            //the fuction glCreateProgram returns a GLuint which is the shader program and it is not equal to 0
            program = glCreateProgram();         
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            //Hint: Use the function glDeleteProgram to delete the shader program
            //Hint: The function glDeleteProgram takes a GLuint which is the shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            //name is the name of the uniform
            //Hint: Use the function glGetUniformLocation which takes the program shader and the name of the uniform as parameters
            //Hint: Use the function c_str() to convert the string to a c string
            //Hint: The function glGetUniformLocation returns a GLuint which is the location of the uniform
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            //uniform is the name of the uniform
            //Glfloat is a float
            //Hint: Use the function glUniform1f because we will send one float only if there were 2 we would use glUniform2f and so on.
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //glUniform1f(the location of the uniform, value to be sent);
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            //uniform is the name of the uniform
            //Gluint is an unsigned integer
            //Hint: Use the function glUniform1ui because we will send one unsigned integer only if there were 2 we would use glUniform2ui and so on.
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //glUniform1ui(the location of the uniform, value to be sent);
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            //Glint is a signed integer
            //Hint: Use the function glUniform1i because we will send one integer only if there were 2 we would use glUniform2i and so on.
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //glUniform1i(the location of the uniform, value to be sent);
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            //glm::vec2 is a 2D vector with two floats (x and y)
            //Hint: Use the function glUniform2f because we will send two floats only if there were 3 we would use glUniform3f and so on.
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //glUniform2f(the location of the uniform, value to be sent);
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            //glm::vec3 is a 3D vector with three floats (x, y and z)
            //Hint: Use the function glUniform3f because we will send three floats only if there were 4 we would use glUniform4f and so on.
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //glUniform3f(the location of the uniform, value to be sent);
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            //glm::vec4 is a 4D vector with four floats (x, y, z and w)
            //Hint: Use the function glUniform4f because we will send four floats only if there were 5 we would use glUniform5f and so on.
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //glUniform4f(the location of the uniform, value to be sent);
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            //glm::mat4 is a 4x4 matrix with 16 floats
            //Hint: Use the function glUniformMatrix4fv because we will send a 4x4 matrix
            //Hint: Use the function getUniformLocation to get the location of the uniform
            //Hint: Use the function glm::value_ptr to get the pointer to the first element of the matrix
            //glUniformMatrix4fv(the location of the uniform, number of matrices, transpose Specifies whether to transpose the matrix as the values are loaded into the uniform variable.,
            //pointer to the first element of the matrix);
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        //Hint: Use the keyword delete
        //Question: Why do we delete the copy constructor and assignment operator?
        //Answer: Because we don't want to copy the shader program because it is a resource that we want to be unique
        //Resource Acquisition Is Initialization (RAII)
        //and also if we copy the shader program we will have two shader programs with the same id which is not good
        //as when we delete the shader program, the one that is copied will cause an error because that id is already deleted
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
    };

}

#endif