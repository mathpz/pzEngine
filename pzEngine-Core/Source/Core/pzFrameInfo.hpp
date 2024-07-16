#pragma once

#include "Core/pzCamera.hpp"
#include "Core/pzGameObject.hpp"

#include <vulkan/vulkan.h>

namespace pz
{

	#define MAX_LIGHTS 10

	struct PointLight
	{
		glm::vec4 position{}; // ingore w
		glm::vec4 color{}; // w is intensity
	};

	struct GlobalUbo
	{
		glm::mat4 projection{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 inverseView{ 1.0f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
		PointLight pointLights[MAX_LIGHTS];

		int numPointLights = 0;

	};

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		PzCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		PzGameObject::Map& gameObjects;
	};
} // namespace pz