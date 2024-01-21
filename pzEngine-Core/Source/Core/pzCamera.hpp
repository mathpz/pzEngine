#pragma once

// pre compiled headers
#include "pzpch.hpp"

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace pz
{
	class PzCamera
	{
	public:
		void setOrthograpichProjection(float leftPlane, float rightPlane, float topPlane, float bottomPlane, float nearPlane, float farPlane);

		void setPerspectiveProjection(float fovY, float aspectRatio, float nearPlane, float farPlane);

		void setViewDirection(const glm::vec3 position, const glm::vec3 direction, const glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
		void setViewTarget(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

		const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& getView() const { return m_viewMatrix; }

	private:
		glm::mat4 m_projectionMatrix{1.f};
		glm::mat4 m_viewMatrix{1.f};

	};
} // namespace pz