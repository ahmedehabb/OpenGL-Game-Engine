#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){
    our::Texture2D* texture = new our::Texture2D();
    //TODO: (Req 11) Finish this function to create an empty texture with the given size and format
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);   
                 //target, mipmap level, internal format, width, height, border, format, type, data
    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();
    //Bind the texture such that we upload the image data to its storage
   
    //TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"
    //here I binded the texture by creating a texture with the constuctor of the texture class
    //then I  binded the texture to the GL_TEXTURE_2D using bind function in the utils class
    texture->bind();
    //here we could use 1 because there's no padding used in the image
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //here we set the texture parameters
    //first parameter is the target which is GL_TEXTURE_2D
    //second is the mipmap level which is 0
    //third is the internal format of how pixels will be saved in vram
    //fourth is the width of the image
    //fifth is the height of the image
    //sixth is the border which is 0
    //seventh is the format of the pixels (data of the texture itself) which is GL_RGBA
    //eighth is the type of the pixels which is GL_UNSIGNED_BYTE 0-255
    //ninth is the pointer to the pixels data 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    //here i am asking to build a mip map so that it could be used for magnification and minification from the different levels
    if(generate_mipmap) 
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }    

    stbi_image_free(pixels); //Free image data after uploading to GPU
    return texture;
}