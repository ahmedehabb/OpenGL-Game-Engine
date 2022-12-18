#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            //size of the each component was given in the vertex.hpp
            // remember to store the number of elements in "elementCount" since you will need it for drawing
           //First we will get the size of the elements vector and store it in elementCount
            elementCount = elements.size();
        
            //Generate a vertex array object and store it in VAO
            //we will use this vertex array object to define how to read the vertex & element buffer during rendering
            //we will bind the vertex array object to the vertex array
            //we need the vertex array as it defines how to read from buffers and how to send the data to the shaders and attributes
            // vertex array object stores all the "configurations" of the vertex buffer
            // stores things like how to read data inside the vertex buffer
            glGenVertexArrays(1, &VAO);

            // VAOs are bounded at setup time and when drawing the geometry
            // we create and bind the vao first, as any buffer or element array
            // created will be bound to this buffer which is what we want
            glBindVertexArray(VAO);

            //Generate a vertex buffer and store it in VBO
            //we will use this vertex buffer to store the actual vertex data on the VRAM
            //we will bind the vertex buffer to the array buffer          
            // vertex buffer is essentially just an array
            // its not that smart, it only has data with no clue what that data is
            glGenBuffers(1, &VBO);
            //Vertex and Element buffers are bound only when you need to update them
            //we will bind the vertex buffer to the array buffer as openGl can have many buffers and it operates on States
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            //vector.data() returns a pointer to the first element in the array which is used internally by the vector.
            //the parameters are the target, the size of the vertices vector in Bytes, the vertices vector pointer to the vertices that I want to send
            //and the usage of the buffer which can be GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
            //where GL_STATIC_DRAW means that the data will be sent once and used many times,The data store contents will be modified once and used many times.
            //GL_DYNAMIC_DRAW means that the data will be sent many times and used many times,The data store contents will be modified repeatedly and used many times.
            //GL_STREAM_DRAW means that the data will be sent once and used few times,the data store contents will be modified once and used at most a few times.
            //Also usage is a hint to the GL implementation as to how a buffer object's data store will be accessed. 
            //This enables the GL implementation to make more intelligent decisions that may significantly impact buffer object performance. 
            //It does not, however, constrain the actual usage of the data store.
            //vector.data() returns a pointer to the first element in the array which is used internally by the vector.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            
            //we will define how to read the vertex & element buffer during rendering as we have bound the vertex array object to the vertex array
            //we will define the attribute location of the position, color, tex_coord and normal and enable them as to be able to read them
            //this helps the vertexAttribPointer to know where to read the data from the buffer
            //the parameters are the attribute location, the number of components, the type of the data, 
            //whether the data is normalized which means that the data is from 0 to 1 and divided by max value of the type
            //the stride is the size of the window to read the data from the buffer and it is in bytes, and the size that it takes to jump to the next element
            //0 means that let openGl figure it out and calculate it for you
            //the offset which is the offset from the beginning of the buffer to the first element of the attribute and it is in bytes
            //attribute location is the location of the attribute in the shader and we can define it in the shader so that multiple attributes can be used
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex),  (void *)offsetof(Vertex, position));

            //we will enable the attribute location of the position, so the vertexAttribPointer will be able to read the data from the buffer
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);

            //we will do the same for the color but the component of the color is 4 as it is a vec4, the type is unsigned byte as it is a byte whuch is from 0 to 255
            //so we will have to normalize it as to be from 0 to 1 , to be able to use it in the shader
            //the offset is the offset from the beginning of the buffer to the first element of the attribute and it is in bytes
            //attribute location is the location of the attribute in the shader and we can define it in the shader so that multiple attributes can be used
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));

            //we will enable the attribute location of the Color, so the vertexAttribPointer will be able to read the data from the buffer
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);

            //we will do the same for the tex_coord but the component of the tex_coord is 2 as it is a vec2, the type is float
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

            //we will enable the attribute location of the Tex_coord, so the vertexAttribPointer will be able to read the data from the buffer
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            //we will do the same for the normal but the component of the normal is 3 as it is a vec3, the type is float
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            //we will enable the attribute location of the normal, so the vertexAttribPointer will be able to read the data from the buffer
            //The surface normal at the vertex (This will be used for lighting in the final phase)
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);

            //Generate an element buffer and store it in EBO
            //element buffer contains data that tells openGl how to draw the vertices
            //we will generate an element buffer and store it in EBO with the size of elements vector
            //we use elements vector to store the indices of the vertices out of which each rectangle will be constructed
            //so that we can use same vertices for different rectangles
            //glGenBuffers takes the number of buffers to be generated and the address of the buffer to be generated
            //the address of the buffer is GLuint
            glGenBuffers(1, &EBO);
            //Bind the element buffer to the element array buffer as openGl can have many buffers and it operates on States
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            //Copy the elements vector to the element array buffer
            //where the parameters are the target, the size of the elements vector in Bytes, the elements vector pointer to the elements that I want to send
            //and the usage of the buffer which can be GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

            //since we are not using global VOAs
            //we don't need to call disableVertexAttribArray
            //as they are contained inside the voa
            //unbinding the buffers
            //we unbind the vertex array object because if we don't unbind it, any subsequent vertex array object calls will modify the currently bound vertex array object
            //so we unbind it to prevent any modification to the currently bound vertex array object
            //we unbind the vertex buffer as we don't want to modify it
            //we unbind the element buffer as we don't want to modify it
            //and mainly for safety reasons
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            // Hint: Use the function glBindVertexArray to bind the vertex array object
            // Hint: Use the function glDrawElements to draw the elements
            // Bind the VAO because the VAO is responsible for remembering which buffers (VBO & EBO) are bound
            // and it remembers the state of the buffers, and the configuration of the vertex attributes
            glBindVertexArray(VAO);
            //since we are using the element buffer, we will use glDrawElements
            //the parameters are the type of the primitive, the number of elements, 
            //the type of the elements in the element Buffer which is unsigned int 
            //and the offset we givee it 0 and let openGl calculate it
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);
            //unbind the VAO after drawing
            glBindVertexArray(0);

        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            // Hint: Use the function glDeleteBuffers to delete the buffers
            // Hint: Use the function glDeleteVertexArrays to delete the vertex array object
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);

        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}