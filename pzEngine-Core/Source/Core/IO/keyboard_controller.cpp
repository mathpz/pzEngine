#include "pzpch.hpp"

#include "keyboard_controller.hpp"
#include "Core/pzInput.hpp"

namespace pz
{
    void KeyboardMovementController::moveInPlaneXZ(
        GLFWwindow* window, float dt, PzGameObject& gameObject) {
        glm::vec3 rotate{ 0 };
        if (Input::isKeyPressed(keys.lookRight)) rotate.y += 1.f;
        if (Input::isKeyPressed(keys.lookLeft)) rotate.y -= 1.f;
        if (Input::isKeyPressed(keys.lookUp)) rotate.x += 1.f;
        if (Input::isKeyPressed(keys.lookDown)) rotate.x -= 1.f;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        const glm::vec3 upDir{ 0.f, -1.f, 0.f };

        glm::vec3 moveDir{ 0.f };
        if (Input::isKeyPressed(keys.moveForward)) moveDir += forwardDir;
        if (Input::isKeyPressed(keys.moveBackward)) moveDir -= forwardDir;
        if (Input::isKeyPressed(keys.moveRight)) moveDir += rightDir;
        if (Input::isKeyPressed(keys.moveLeft)) moveDir -= rightDir;
        if (Input::isKeyPressed(keys.moveUp)) moveDir += upDir;
        if (Input::isKeyPressed(keys.moveDown)) moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }
    }


} // namespace pz

