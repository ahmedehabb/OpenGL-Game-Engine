#pragma once

#include <unordered_set>
#include "entity.hpp"

namespace our {

    // This class holds a set of entities
    class World {
        std::unordered_set<Entity*> entities; // These are the entities held by this world
        std::unordered_set<Entity*> markedForRemoval; // These are the entities that are awaiting to be deleted
                                                      // when deleteMarkedEntities is called
    public:

        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json& data, Entity* parent = nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity* add() {
            //TODO: (Req 8) Create a new entity, set its world member variable to this,
            // and don't forget to insert it in the suitable container.
            // Create a new entity
            Entity* entity = new Entity();
            // set its world member variable to this
            entity->world = this;
            // insert it in the suitable container
            entities.insert(entity);
            // returns a pointer to that entity
            return entity;
        }

        // This returns and immutable reference to the set of all entites in the world.
        const std::unordered_set<Entity*>& getEntities() {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity* entity){
            //TODO: (Req 8) If the entity is in this world, add it to the "markedForRemoval" set.
            if(entities.find(entity) != entities.end()){
                markedForRemoval.insert(entity);
            }
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities(){
            //TODO: (Req 8) Remove and delete all the entities that have been marked for removal
            for (Entity* entity : markedForRemoval){
                entities.erase(entity);
                // Erase :: Removes from the vector container and calls its destructor 
                // but If the contained object is a pointer it doesnt take ownership of destroying it.
                // so we have to explicitly call delete on each contained pointer to delete the content
                delete entity;
            }
            // at the end marked for removal contains all entities that are erased so we must clear it
            markedForRemoval.clear();
        }

        //This deletes all entities in the world
        void clear(){
            //TODO: (Req 8) Delete all the entites and make sure that the containers are empty
            for (Entity* entity : entities){
                delete entity;
            }
            entities.clear();
            // at the end marked for removal could contain entities that are erased so we must clear it
            markedForRemoval.clear();
        }

        //Since the world owns all of its entities, they should be deleted alongside it.
        ~World(){
            clear();
        }

        // The world should not be copyable
        World(const World&) = delete;
        World &operator=(World const &) = delete;
    };

}