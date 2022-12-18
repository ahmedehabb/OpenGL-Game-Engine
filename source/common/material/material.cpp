#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        // setup the pipeline state which will set faceCulling, depthTesting, blending of the pipeline state
        this->pipelineState.setup();
        // set the shader to be used 
        this->shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        
        //call the setup of its parent which is Material
        Material::setup();
        
        // set the "tint" uniform to the value in the member variable tint 
        // which is the uniform vec4 tint in the shader;
        this->shader->set("tint", this->tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        
        //call the setup of its parent which is TintedMaterial
        TintedMaterial::setup();
        
        // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
        this->shader->set("alphaThreshold", this->alphaThreshold);

        // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
        // we can use the first one since the shader has only this one transform and we dont need to add to GL_TEXTURE0
        
        
        // First, we set the active texture unit.
        // Here, we pick unit 0 which is actually the active unit by default but we still wrote this line for demonstration.
        // made textureUnitIndex to keep it dynamic if we needed to change the texture unit after some time instead of passing 0
        int textureUnitIndex = 0;
        glActiveTexture(GL_TEXTURE0 + textureUnitIndex);

        // added this check for nullptrs of both the texture and sampler since they could crash game where they arent still initialized
        if (texture == nullptr || sampler == nullptr)
        {
            return ;
        }
        
        // binding the texture, we also bind it to the active unit. So this texture is now bound to unit 0.
        this->texture->bind();
        // bind the sampler to the same texture unit index where the texture is binded
        this->sampler->bind(textureUnitIndex);
        // send the unit number to the uniform variable "tex" which is
        // uniform sampler2D tex;
        this->shader->set("tex", textureUnitIndex);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}