#pragma once
#include "../ecs/component.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace our {

    //we made an enum called light type that defines the type of light
    //it can be directional, point or spot
    //directional light is like the sun, it is a light that is infinitely far away
    //and it shines in all directions
    //point light is a light that is at a specific point in space and it shines in all directions
    //spot light is a light that is at a specific point in space and it shines in a specific direction and has a specific angle 
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };


    // This component adds lighting ability to the owing entity.
    class LightComponent : public Component {
    public:
        LightType type;             // The current type of light, Possible values are Directional, Point and Spot.
        glm::vec3 color;            // The light color
        glm::vec3 direction;        //for directional light
        glm::vec2 cone_angles;     //define the angle of a spot light’s cone (Spot light only).//wide and narrow angles
        glm::vec3 attenuation;    //intensity of the light at different distances (Point light and Spot light only).
        static std::string getID() { return "Light"; }

        // Reads light properties by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}