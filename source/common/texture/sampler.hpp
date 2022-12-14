#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our {

    // This class defined an OpenGL sampler
    class Sampler {
        // The OpenGL object name of this sampler 
        GLuint name;
    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name" 
        Sampler() {
            //TODO: (Req 6) Complete this function
            // this function will create a sampler and save its name in the name variable
            //first parameter is the number of samplers that i want to create
            // second parameter is the pointer to the variable that will hold the name of the sampler
            glGenSamplers(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler() { 
            //TODO: (Req 6) Complete this function
            // this function will delete the sampler
            // first parameter is the number of samplers that i want to delete
            // second parameter is the pointer to the variable that will hold the name of the sampler
            glDeleteSamplers(1, &name);
         }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const {
            //TODO: (Req 6) Complete this function
            // this function will bind the sampler to the texture unit
            // first parameter is the texture unit that i want to bind the sampler to
            // second parameter is the name of the sampler that i want to bind
            glBindSampler(textureUnit, name);
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit){
            //TODO: (Req 6) Complete this function
            // this function will unbind the sampler from the texture unit
            // first parameter is the texture unit that i want to unbind the sampler from
            
            glBindSampler(textureUnit, 0);
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const {
            //TODO: (Req 6) Complete this function
            // this function will set the sampler parameter
            // first parameter is the name of the sampler that i want to set the parameter for
            // second parameter is the parameter that i want to set
            // third parameter is the value of the parameter that i want to set
            glSamplerParameteri(name, parameter, value);
            //parameter is filter or wrapping
            //value could be repeeat or clamp to edge or constant
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const {
            //TODO: (Req 6) Complete this function
            // this function will set the sampler parameter
            // first parameter is the name of the sampler that i want to set the parameter for
            // second parameter is the parameter that i want to set
            // third parameter is the value of the parameter that max anistropy
            glSamplerParameterf(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const {
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json& data);

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

}