#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt
        
        // point or vector transformed by matrix M = M * (vector/point) 
        
        // since eye and center are points so we add 1 to them as homogenous component
        glm::vec4 eyeTransformed = M * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 centerTransformed = M * glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f);
        
        // since up is a vector so we add 0 to them as homogenous component
        glm::vec4 upTransformed = M * glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f);
        
        // cast them to vec3 by removing homogenous part (1 in point), (0 in vector)
        return glm::lookAt(glm::vec3(eyeTransformed), glm::vec3(centerTransformed), glm::vec3(upTransformed));
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective

        // aspect ratio is x/y
        float aspectRatio = (float)viewportSize[0] / viewportSize[1];

        if (this->cameraType == our::CameraType::ORTHOGRAPHIC)
        {
            // will get the orthowidth from multiplying orthoHeight with the aspect ratio 
            // (width/height) * height = width  
            // will get half of it to assign them to left and right , will only change sign
            float leftRightMagnitude = this->orthoHeight * aspectRatio/2;
            // the left will be -ve * this magnitude, and right will be +ve * this magnitude
            return glm::ortho(-leftRightMagnitude, leftRightMagnitude, -(this->orthoHeight/2), (this->orthoHeight/2));
        } else
        {
            return glm::perspective(this->fovY,aspectRatio,this->near,this->far);
        }
    }
}