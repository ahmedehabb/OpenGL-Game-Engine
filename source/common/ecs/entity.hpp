#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <list>
#include <iterator>
#include <string>
#include <glm/glm.hpp>

namespace our {

    class World; // A forward declaration of the World Class

    class Entity{
        World *world; // This defines what world own this entity
        std::list<Component*> components; // A list of components that are owned by this entity

        friend World; // The world is a friend since it is the only class that is allowed to instantiate an entity
        Entity() = default; // The entity constructor is private since only the world is allowed to instantiate an entity

    public:
        std::string name; // The name of the entity. It could be useful to refer to an entity by its name
        Entity* parent;   // The parent of the entity. The transform of the entity is relative to its parent.
                          // If parent is null, the entity is a root entity (has no parent).
        Transform localTransform; // The transform of this entity relative to its parent.
        std::string tag; // The tag of the entity. It could be useful to refer to an entity by its tag
        World* getWorld() const { return world; } // Returns the world to which this entity belongs

        glm::mat4 getLocalToWorldMatrix() const; // Computes and returns the transformation from the entities local space to the world space

        void deserialize(const nlohmann::json&); // Deserializes the entity data and components from a json object

        
        // This template method create a component of type T,
        // adds it to the components map and returns a pointer to it 
        template<typename T>
        T* addComponent(){
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            //TODO: (Req 8) Create an component of type T, set its "owner" to be this entity, then push it into the component's list
            // Create an component of type T
            T* newComponent = new T();
            // Set its "owner" to be this entity
            newComponent->owner = this;
            // then push it into the component's list
            this->components.push_back(newComponent);
            // Don't forget to return a pointer to the new component
            return newComponent;
        }

        // This template method searhes for a component of type T and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(){
            //TODO: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // Return the component you found, or return null of nothing was found.
            auto it = components.begin();
            while(it != components.end()){
                // used dynamic_cast<T*>(*it) != NULL as the bad cast check 
                // could have used try and catch for the bad cast but this check satisfy our need 
                if (dynamic_cast<T*>(*it) != NULL)
                {
                    return dynamic_cast<T*>(*it);
                }
                it++;
            }
            // return null of nothing was found.
            return nullptr;
        }

        // This template method dynami and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(size_t index){
            auto it = components.begin();
            std::advance(it, index);
            if(it != components.end())
                return dynamic_cast<T*>(*it);
            return nullptr;
        }

        // This template method searhes for a component of type T and deletes it
        template<typename T>
        void deleteComponent(){
            //TODO: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // If found, delete the found component and remove it from the components list
            
            // could have done it in one loop only but this is better for code reusability
            
            // find the first component that can be dynamically cast to "T*".
            T* firstFound = getComponent<T*>();
            // if its not null , which is successfull dynamic cast or found
            if(firstFound){
                // reuse function deleteComponent which take T* and delete it
                deleteComponent<T*>(firstFound);
            }
        }

        // This template method searhes for a component of type T and deletes it
        void deleteComponent(size_t index){
            auto it = components.begin();
            std::advance(it, index);
            if(it != components.end()) {
                delete *it;
                components.erase(it);
            }
        }

        // This template method searhes for the given component and deletes it
        template<typename T>
        void deleteComponent(T const* component){
            //TODO: (Req 8) Go through the components list and find the given component "component".
            // If found, delete the found component and remove it from the components list
            auto it = components.begin();
            while(it != components.end()){
                if (dynamic_cast<T*>(*it) != NULL && (*it) == component)
                {
                    // first deallocate the component
                    delete *it;
                    // Erase :: Removes from the vector container and calls its destructor 
                    // but If the contained object is a pointer it doesnt take ownership of destroying it.
                    // so we have to explicitly call delete on each contained pointer to delete the content
                    components.erase(it);
                    return;
                }
                it++;
            }
        }

        // Since the entity owns its components, they should be deleted alongside the entity
        ~Entity(){
            //TODO: (Req 8) Delete all the components in "components".
            
            // looping over every one and delete it manually then clear
            for (Component* component : components){
                delete component;
            }
            // clear at the end of components 
            components.clear();
        }

        // Entities should not be copyable
        Entity(const Entity&) = delete;
        Entity &operator=(Entity const &) = delete;
    };

}