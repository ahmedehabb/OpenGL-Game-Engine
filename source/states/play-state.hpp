#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/collision.hpp>
#include <systems/movement.hpp>
#include <systems/player-movement.hpp>
#include <ecs/entity.hpp>


// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    our::PlayerMovementSystem playerMovementSystem;
    our::GameMananger gameManager;


    void onInitialize() override {
        std::cout<<"playstate"<<std::endl;
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        gameManager.enter(getApp(),gameManager.play_state);
        playerMovementSystem.enter(getApp());
        renderer.initialize(size, config["renderer"]);
    }

    

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        std::cout<<"collision should be called now"<<std::endl;
        collisionSystem.update(&world);
        gameManager.update(deltaTime);
        playerMovementSystem.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();
        

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }
    void onImmediateGui() {

        //ImGui::ShowDemoWindow();
        ImGui::Begin("Game Menu", false);

        ImGui::Text("Crazy TAXI");
        ImGui::Text("Score: %d", our::GameMananger::gm.score);
        if (our::GameMananger::gameOver)
        {
            if (our::GameMananger::win)
            {
                ImGui::Text("WINNER!!");
            }
            else 
                ImGui::Text("GAME OVER!!");

            if (ImGui::Button("Restart Game")) {
                createLevel(std::rand() % 5 + 1);
                
                cameraController.resetPosition(&world);
                our::GameMananger::gameOver = false;
                our::GameMananger::win = false;
                our::GameMananger::score = 0;
            }
        }
        ImGui::End();

    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
    void createLevel(int diffuclity)
    {
        for(auto entity : world.getEntities())
        {
            if(entity->tag == "obstacle")
                world.markForRemoval(entity);
        }
        
        world.deleteMarkedEntities();
        for (int i = 0; i < diffuclity; i++)
        {
            float z = 1 + (std::rand() % 45) * -1;
            float x = -8 + (std::rand() % 16);
            instantiateCars(glm::vec3(x,-1,z));
        }
    }
    void instantiateCars(glm::vec3 position)
    {
        //instantiate coin
        our::Entity* obstacle = world.add();
        obstacle->tag = "obstacle";
        obstacle->name = "obstacle";
        obstacle->parent = nullptr;
        obstacle->localTransform.position = position;
        obstacle->localTransform.scale = glm::vec3(0.05, 0.05, 0.05);
        obstacle->localTransform.rotation = glm::vec3(glm::radians(-90.0f), 0, 0);
        //add mesh renderer
        obstacle->addComponent<our::MeshRendererComponent>();
        obstacle->getComponent<our::MeshRendererComponent>()->mesh = our::AssetLoader<our::Mesh>::get("car");
        obstacle->getComponent<our::MeshRendererComponent>()->material = our::AssetLoader<our::Material>::get("car");
        
        //add collision 
        obstacle->addComponent<our::CollisionComponent>();
        obstacle->getComponent<our::CollisionComponent>()->center = glm::vec3(0, 0, 0);
        obstacle->getComponent<our::CollisionComponent>()->radius = 3;
        
    }
};