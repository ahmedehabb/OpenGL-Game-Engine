#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //TODO: (Req 5) Complete this function
            // this function will create a texture and save its name in the name variable
            //first parameter is the number of textures that i want to create
            // second parameter is the pointer to the variable that will hold the name of the texture
            glGenTextures(1, &name);

        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 

            //TODO: (Req 5) Complete this function
            // this function will delete the texture
            // first parameter is the number of textures that i want to delete
            // second parameter is the pointer to the variable that will hold the name of the texture
            glDeleteTextures(1, &name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //TODO: (Req 5) Complete this function
            // this function will bind the texture to the GL_TEXTURE_2D
            // first parameter is the target that i want to bind the texture to which is GL_TEXTURE_2D creating a 2D texture
            // second parameter is the name of the texture that i want to bind

            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //TODO: (Req 5) Complete this function
            // this function will unbind the texture from the GL_TEXTURE_2D
            // first parameter is the target that i want to unbind the texture from which is GL_TEXTURE_2D creating a 2D texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}