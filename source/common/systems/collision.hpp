#pragma once
#define  OBSTACLE_SCORE  5

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include <systems/game-manager.hpp>

#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>
namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CollisionSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world)
        {std::cout<<"collision"<<std::endl;

            glm::vec3 carPosition = glm::vec3(0, 0, 0);
            float carRadius = 0;

            Entity *carMesh = nullptr;

            for (auto entity : world->getEntities())
            {  std::cout<<entity->name<<std::endl; 
                if (entity->name == "car")
                {
                    carMesh = entity;
                    break;
                }

            }
            std::cout<<"collision_2"<<std::endl;

            if (!carMesh)
                return;
            std::cout<<"collision_3"<<std::endl;    

            // get the current player entity and get its position and radius

            CollisionComponent *collision = carMesh->getComponent<CollisionComponent>();
            std::cout<<(collision!=nullptr)<<std::endl;

            carPosition = collision->center + glm::vec3(carMesh->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1));
            carRadius = collision->radius * glm::length(carMesh->localTransform.scale) ;

            for (auto entity : world->getEntities())
            {

                // get all entities that have collision component
                CollisionComponent *collision = entity->getComponent<CollisionComponent>();
                std::cout<<entity->name<<std::endl;
                if (collision && entity->name != "car")
                {  
                    

                    // get the new radius and position of the entity
                    glm::vec3 newPosition = collision->center + glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1));
                    float newRadius = collision->radius ;
                    // compare with player position to check if it collides or not
                    std::cout <<glm::length(newPosition-carPosition)<< std::endl;
                    if (glm::length(newPosition - carPosition) < carRadius + newRadius)
                    {  
                        // collision occurred
                        if (entity->tag == "finish")
                        {
                            our::GameMananger::gameOver = true;
                        }
                        if(entity->tag == "obstacle")
                        {
                            our::GameMananger::gm.downScore(OBSTACLE_SCORE);
                            std::cout<<our::GameMananger::gm.score<<std::endl;

                            // ImGui::Text("Score: %d", our::GameMananger::gm.score);
                            // ImGui::End();
                        

                        }
                        
                    }

                }
            }
        }
    };

}
