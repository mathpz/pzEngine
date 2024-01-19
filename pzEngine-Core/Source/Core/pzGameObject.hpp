#pragma once

// pre compiled header
#include "pzpch.hpp"

// pzEngine
#include "pzModel.hpp"

// glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace pz
{
    struct TransformComponent
    {
        glm::vec3 translation{};  // position offset
        glm::vec3 scale{ 1.f, 1.f, 1.f };    // scale
        glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };  // Quaternion for rotation

        glm::mat4 mat4()
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);

            // Apply rotation using quaternion
            transform *= glm::mat4_cast(rotation);

            transform = glm::scale(transform, scale);
            return transform;
        }
    
    };


    class PzGameObject
    {
        public:

            using id_t = unsigned int;
            static PzGameObject createGameObject()
            {
                static id_t currentId = 0;
                return PzGameObject{currentId++};
            }

            PzGameObject(const PzGameObject&) = delete;
            PzGameObject &operator=(const PzGameObject&) = delete;
            PzGameObject(PzGameObject&&) = default;
            PzGameObject &operator=(PzGameObject&&) = default;

            id_t getId() const { return id; }

            std::shared_ptr<PzModel> model{};
            glm::vec3 color{};
            TransformComponent transform{};

        private:
            PzGameObject(id_t objId) : id{objId} {}
            id_t id;
    };

} // namespace pz