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

		const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }

	private:
		glm::mat4 m_projectionMatrix{1.f};
	};
} // namespace pz