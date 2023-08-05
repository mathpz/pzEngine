#pragma once

#include "pzModel.hpp"

// std
#include <memory>

namespace pz
{
    struct Transform2dComponent
    {
        glm::vec2 translation{};  // position offset
        glm::vec2 scale{1.f, 1.f};    // scale
        float rotation;

        glm::mat2 mat2() 
        {
            const float sin = std::sin(rotation);
            const float cos = std::cos(rotation);
            glm::mat2 rotationMat{{cos, sin}, {-sin, cos}};

            glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
            return rotationMat * scaleMat; 
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
            Transform2dComponent transform2d{};

        private:
            PzGameObject(id_t objId) : id{objId} {}
            id_t id;
    };

} // namespace pz