#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    // Reads light properties by the names given in the json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if (!data.is_object()) return;
        std::string typeLight = data.value("lightType", "point");
        if (typeLight == "directional") {
            type = LightType::DIRECTIONAL;
        }
        else if (typeLight == "point") {
            type = LightType::POINT;
        }
        else if (typeLight == "spot") {
            type = LightType::SPOT;
        }
        
        
      
        color = data.value("color", glm::vec3(1.0f, 1.0f, 1.0f));
        cone_angles = data.value("cone-angels", glm::vec2(glm::radians(15.0f), glm::radians(30.0f)));
        direction = data.value("direction", glm::vec3(-1.0f,0.0f, 0.0f));
        attenuation = data.value("attenuation", glm::vec3(0.0f,0.0f, 1.0f));
    }
}