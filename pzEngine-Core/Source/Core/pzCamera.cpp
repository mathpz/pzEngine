// pre compiled headers
#include "pzpch.hpp"

#include "pzCamera.hpp"

namespace pz
{
	void PzCamera::setOrthograpichProjection(
		float left, float right, float top, float bottom, float nearPlane, float farPlane) {
		m_projectionMatrix = glm::mat4{ 1.0f };
		m_projectionMatrix[0][0] = 2.f / (right - left);
		m_projectionMatrix[1][1] = 2.f / (bottom - top);
		m_projectionMatrix[2][2] = 1.f / (farPlane - nearPlane);
		m_projectionMatrix[3][0] = -(right + left) / (right - left);
		m_projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		m_projectionMatrix[3][2] = -nearPlane / (farPlane - nearPlane);
	}

	void PzCamera::setPerspectiveProjection(float fovY, float aspectRatio, float nearPlane, float farPlane) {
		assert(glm::abs(aspectRatio - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovY / 2.f);
		m_projectionMatrix = glm::mat4{ 0.0f };
		m_projectionMatrix[0][0] = 1.f / (aspectRatio * tanHalfFovy);
		m_projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		m_projectionMatrix[2][2] = farPlane / (farPlane - nearPlane);
		m_projectionMatrix[2][3] = 1.f;
		m_projectionMatrix[3][2] = -(farPlane * nearPlane) / (farPlane - nearPlane);
	}
} // namespace pz