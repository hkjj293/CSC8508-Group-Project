#pragma once

#include "VulkanUtility.h"

class VulkanDebugger {
public:
	VulkanDebugger(vk::Instance* instance);
	~VulkanDebugger();

protected:
	void SetupDebugMessenger();
	static VKAPI_ATTR VkBool32 VKAPI_CALL
		debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

	vk::Instance* pInstance;
	vk::DebugUtilsMessengerEXT messenger;
};
