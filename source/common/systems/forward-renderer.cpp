#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
namespace our {

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
          
          
          // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //TODO: (Req 10) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            PipelineState skyPipelineState{};
            skyPipelineState.depthTesting.enabled = true;
            //IF WE changed depth function to GL_GEQUAL then the sky will be drawn above ground
            skyPipelineState.depthTesting.function = GL_LEQUAL; 
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            // we want to draw the sky from the inside, so we need to cull the front faces
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            //TODO: (Req 11) Create a framebuffer
            //we need to create a framebuffer to render the scene to a texture
            glGenFramebuffers(1, &postprocessFrameBuffer);
            //bind that frame buffer to be able to draw on it
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
            
            //TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits).

            //create a color texture to render the scene to it
            colorTarget = new Texture2D();
            //bind the texture to be able to draw on it
            colorTarget->bind();
            //color texture needs to calculate the number of mipmaps using the formula: floor(log2(max(width, height))) + 1
            GLuint mip_levels = glm::floor(glm::log2(glm::max<float>(this->windowSize.x, this->windowSize.y))) + 1;
            glTexStorage2D(GL_TEXTURE_2D, mip_levels, GL_RGBA8, this->windowSize.x, this->windowSize.y);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            
            //create a depth texture to render the scene to it
            depthTarget = new Texture2D();
            //bind the texture to be able to draw on it
            depthTarget->bind();
            // The color texture will need mipmaps while the depth texture will only need 1 mip level.
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, this->windowSize.x, this->windowSize.y);
            // Depth only needs 1 mip level
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);
            
            //TODO: (Req 11) Unbind the framebuffer just to be safe
            //check if frame buffer is complete
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                fprintf(stderr, "Framebuffer is not complete!");
            //unbind the frame buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);
            
            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler* postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram* postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world){
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        glm::vec3 cameraForward = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
            //TODO: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second".
            // get the projection of both points on the cameraForward vector but without normalization of the cameraForward
            // since dividing both by the magnitude wont affect the result
            
            // first.center and second.center are wrt to world so dont need furthur processing
            float projectionOfFirst =  glm::dot(first.center, cameraForward); 
            float projectionOfSecond =  glm::dot(second.center, cameraForward);
            return projectionOfFirst  > projectionOfSecond  ;
        });

        
        //TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        
        glm::mat4 VP = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        

        //TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        //glViewport takes 4 parameters: x, y, width and height
        glViewport(0.0f, 0.0f,windowSize.x,windowSize.y);
        //TODO: (Req 9) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        //TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        // glColorMask takes 4 parameters: red, green, blue and alpha and specifies whether the color buffer is enabled for writing or not
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        //we need to enable depth mask since we are using depth buffer
        glDepthMask(GL_TRUE);

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //TODO: (Req 11) bind the framebuffer
            //bind the frame buffer to be able to use it
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
        }

        //TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //TODO: (Req 9) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
    
        for (auto opaque : opaqueCommands) {
            
            opaque.material->setup();
            //multiply the VP matrix with the localToWorld matrix to get the model-view-projection matrix
            opaque.material->shader->set("transform",  opaque.localToWorld);
            //calculate the light for opaque objects
            opaque.material->shader->set("object_to_world", opaque.localToWorld);
            opaque.material->shader->set("object_to_world_inv_transpose", glm::transpose(glm::inverse(opaque.localToWorld)));
            opaque.material->shader->set("view_projection", VP);
            opaque.material->shader->set("camera_position", camera->getOwner()->getLocalToWorldMatrix()* glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f));

            setLightShaders(opaque.material->shader);


            opaque.mesh->draw();
            
        }
        // If there is a sky material, draw the sky 
        if(this->skyMaterial){
            //TODO: (Req 10) setup the sky material
            this->skyMaterial->setup();
            //TODO: (Req 10) Get the camera position
            glm::mat4 cameraTransform = camera->getOwner()->getLocalToWorldMatrix();
            //TODO: (Req 10) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)

            // then we will need the VP matrix to be converted into ndc space
            glm::mat4 skyModelMatrix =glm::translate(VP ,glm::vec3(cameraTransform[3]));
            
            //TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            // the third row should be (0,0,0,1) so the z component = 1 but here opengl on transposed version of our convention 
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f
            );
            //TODO: (Req 10) set the "transform" uniform
            this->skyMaterial->shader->set("transform", alwaysBehindTransform * skyModelMatrix);
            //TODO: (Req 10) draw the sky sphere
            this->skySphere->draw();
        }
        //TODO: (Req 9) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (auto transparent : transparentCommands) {
            transparent.material->setup();
            transparent.material->shader->set("object_to_world", transparent.localToWorld);
            transparent.material->shader->set("object_to_world_inv_transpose", glm::transpose(glm::inverse(transparent.localToWorld)));
            transparent.material->shader->set("view_projection", VP);
            transparent.material->shader->set("camera_position", camera->getOwner()->getLocalToWorldMatrix()* glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f));
            setLightShaders(transparent.material->shader);
            transparent.mesh->draw();

        }

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //TODO: (Req 11) Return to the default framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            //TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            //bind vertex array to be able to draw
            glBindVertexArray(postProcessVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            
        }
    }
    void ForwardRenderer::setLightShaders(ShaderProgram *shader)
    {
        shader->set("light_count", (int)lights.size());
        int light_index = 0;
        const int MAX_LIGHT_COUNT = 8;
        shader->set("sky_light.sky", glm::vec3(0.2, 0.6, 0.8));
	    shader->set("sky_light.horizon", glm::vec3(0.5, 0.5, 0.5));
	    shader->set("sky_light.ground", glm::vec3(0.2, 0.7, 0.4));
        for (const auto &light : lights)
        {
            std::string prefix = "lights[" + std::to_string(light_index) + "].";
            shader->set(prefix + "type", static_cast<int>(light->type));

            shader->set(prefix + "color", glm::normalize(light->color));

            switch (light->type)
            {
            case LightType::DIRECTIONAL:
                shader->set(prefix + "direction", glm::normalize(light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->direction, 1)));
                break;
            case LightType::POINT:
                glm::vec4 light4 = light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->getOwner()->localTransform.position, 1);
                shader->set(prefix + "position", glm::vec3(light4));
                shader->set(prefix + "attenuation", light->attenuation);
                break;
            case LightType::SPOT:
                glm::vec4 Slight = light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->getOwner()->localTransform.position, 1);
                shader->set(prefix + "position", glm::vec3(Slight));
                shader->set(prefix + "direction", glm::normalize(light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->direction, 1)));
                shader->set(prefix + "attenuation", light->attenuation);
                shader->set(prefix + "cone_angles", glm::vec2(glm::radians(light->cone_angles[0]), glm::radians(light->cone_angles[1])));
                break;
            }
            light_index++;
            if (light_index >= MAX_LIGHT_COUNT)
                break;
        }
    }

}