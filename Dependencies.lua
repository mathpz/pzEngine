-- pzEngine dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/pzEngine-Core/Third_Party/glfw/include"
IncludeDir["ImGui"] = "%{wks.location}/pzEngine-Core/Third_Party/imgui"
IncludeDir["glm"] = "%{wks.location}/pzEngine-Core/Third_Party/glm/glm"
IncludeDir["spdlog"] = "%{wks.location}/pzEngine-Core/Third_Party/spdlog/include"
IncludeDir["tinyobjloader"] = "%{wks.location}/pzEngine-Core/Third_Party/tinyobjloader"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "vulkan-1"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"