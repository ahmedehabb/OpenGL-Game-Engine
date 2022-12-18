#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
       
        //we will use glm::translate to translate the matrix
        //parameters are the matrix which is a 4x4 identity matrix and the position
        glm::mat4 MT = glm::translate(glm::mat4(1.0f), position);
        //we will use glm::scale to scale the matrix
        glm::mat4 MS = glm::scale(glm::mat4(1.0f), scale);
        //we will use glm::yawPitchRoll to convert euler angles to a rotation matrix where
        //yaw is the rotation around the y axis
        //pitch is the rotation around the x axis
        //roll is the rotation around the z axis
        //and is applied in that order (yaw, pitch, roll)
        glm::mat4 MR = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        //we will return the matrix by multiplying the matrices in the order of scaling, rotation and translation
        //thats why we multiply the matrices in the order of MS, MR and MT
        //which is from right to left
        //and take care that order of the matrices is important and matters a lot
        return  MT * MR * MS; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}