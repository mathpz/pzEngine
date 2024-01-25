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
    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();
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