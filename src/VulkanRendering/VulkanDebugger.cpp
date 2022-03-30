#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include "VulkanDebugger.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VulkanDebugger::VulkanDebugger(vk::Instance* instance) {
	pInstance = instance;
	SetupDebugMessenger();
}

VulkanDebugger::~VulkanDebugger() {
	vk::DispatchLoaderDynamic dl(*pInstance, vkGetInstanceProcAddr);
	pInstance->destroyDebugUtilsMessengerEXT(messenger,nullptr,dl);
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugger::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	}

	return VK_FALSE;
}

void VulkanDebugger::SetupDebugMessenger() {
	const vk::DebugUtilsMessengerCreateInfoEXT createInfo =
		vk::DebugUtilsMessengerCreateInfoEXT()
		.setMessageSeverity(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
		.setMessageType(
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
		.setPfnUserCallback(debugCallback)
		.setPUserData(nullptr);

	vk::DispatchLoaderDynamic dl(*pInstance, vkGetInstanceProcAddr);
	messenger = pInstance->createDebugUtilsMessengerEXT(createInfo, nullptr, dl);
}

