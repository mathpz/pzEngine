#include "pzpch.hpp"

#include "ImGuiLayer.hpp"

#include "Core/Platform/Vulkan/imgui_impl_glfw.h"
#include "Core/Platform/Vulkan/imgui_impl_vulkan.h"

#include "Application.hpp"
#include "Core/pzGameObject.hpp"

namespace pz
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::onAttach()
	{
		// https://vkguide.dev/docs/extra-chapter/implementing_imgui/
		//1: create descriptor pool for IMGUI
		// the size of the pool is very oversize, but it's copied from imgui demo itself.
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
		pool_info.pPoolSizes = pool_sizes;

		VkDescriptorPool imguiPool;
		Application& app = Application::Get();
		vkCreateDescriptorPool(app.GetDevice().device(), &pool_info, nullptr, &imguiPool);

		// 2: initialize imgui library
		//this initializes the core structures of imgui
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForVulkan(app.GetWindow().getGLFWwindow(), true);
		//this initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = app.GetDevice().getInstance();
		init_info.PhysicalDevice = app.GetDevice().getPhysicalDevice();
		init_info.Device = app.GetDevice().device();
		init_info.Queue = app.GetDevice().getQueue();
		init_info.DescriptorPool = imguiPool;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&init_info, app.GetRenderer().getSwapChainRenderPass());


	}

	void ImGuiLayer::onDetach()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		// Rendering
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), app.GetRenderer().getCurrentCommandBuffer());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::onImGuiRender()
	{
		Application& app = Application::Get();

		static float f0 = 0.0f;
		static float f1 = 0.0f;
		static float f2 = 0.0f;

		pz::PzGameObject::Map& gameObjects = app.GetGameObjects();

		ImGui::Text("Translation");
		for (auto& [id, obj] : gameObjects)
		{
			ImGui::Text("Object Id: %d", id);

			ImGui::PushID(id);
			ImGui::SliderFloat("X:", &obj.transform.translation.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Y:", &obj.transform.translation.y, -5.0f, 5.0f);
			ImGui::SliderFloat("Z:", &obj.transform.translation.z, -5.0f, 5.0f);
			ImGui::PopID();
		}


	}

}  // namespace pz