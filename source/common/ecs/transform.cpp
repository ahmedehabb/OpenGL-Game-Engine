#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        //we will use glm::yawPitchRoll to convert euler angles to a rotation matrix
        //we will use glm::scale to scale the matrix
        //we will use glm::translate to translate the matrix
        //we will use glm::yawPitchRoll to rotate the matrix
        glm::mat4 MT = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 MS = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 MR = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        return  MT * MR * MS; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}