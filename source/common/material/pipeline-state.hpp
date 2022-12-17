#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 4) Write this function
            if(faceCulling.enabled){
                /// now I am going to enable the face culling
                //faceculling means that the object from acertain face will not be drawn if it is not seen by the camera for optimaiztion
                glEnable(GL_CULL_FACE);
                // choosing which face that will be culled grom the struct the culled face is the back face
                glCullFace(faceCulling.culledFace);
                // choosing which direction that will maintain the culled face CCW means that i will cull the face if the points doesn't follow ccw direstions 
                // this also means that rotation around z axis won't cull neither front or back f=face
                glFrontFace(faceCulling.frontFace);
            }
            else{
                // if the face culling is not enabled then i will disable it what does this mean this means that i will have the back face even that i am not seeing it
                glDisable(GL_CULL_FACE);
            }
            if(depthTesting.enabled){
                // now i am going to enable the depth testing 
                glEnable(GL_DEPTH_TEST);
                // now i am going to choose the depth function what does it mean it means that i will draw the pixels if it is less or equal the already drawn one
                glDepthFunc(depthTesting.function);
            }
            else{
                // if the depth testing is not enabled then i will disable it it will affect highly the opaque objects
                glDisable(GL_DEPTH_TEST);
            }
            if(blending.enabled){
                // now i am going to enable the blending
                glEnable(GL_BLEND);
                // now i am going to choose the blending equation what does it mean it means that i will add the source and destination colors
                // alpha (source )*souce  + (1 - alpha (source)) * destination
                //source is the color of the pixel that i am going to draw
                //destination is the color of the pixel that is already drawn
                //this is the meaning of adding 
                glBlendEquation(blending.equation);
                //here i am choosing that the equation will be the source color * the source alpha + the destination color * (1 - source alpha)
                glBlendFunc(blending.sourceFactor,blending.destinationFactor);
                //the struct has constant color so if you are going to use any factor that blend with constant color it is set by the blend color function
                //this will only affect if you will blend with constant color
                glBlendColor(blending.constantColor.r,blending.constantColor.g,blending.constantColor.b,blending.constantColor.a);
            }
            else{
                // if the blending is not enabled then i will disable it it will affect highly the transparent objects
                glDisable(GL_BLEND);
            }
            // it is used to see if the colors can be written on the frame buffer or no do here we have all colors can be written on the frame buffer(all have true values)
            glColorMask(colorMask.r,colorMask.g,colorMask.b,colorMask.a);
            // this enables that you could write on the depth buffer 
            glDepthMask(depthMask);

        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}