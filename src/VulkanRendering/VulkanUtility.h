#pragma once

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#endif

#include <iostream>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace VulkanUtil {

	static void WaitForInput() {
		char a;
		std::cin >> a;
	}
	namespace Instance {
		static vk::Result getAvailableExtensions(
			const char* pLayerName,
			uint32_t& PropertyCount,
			std::vector<vk::ExtensionProperties>& AvailableExtProp) {
			AvailableExtProp.clear();
			vk::enumerateInstanceExtensionProperties(pLayerName, &PropertyCount, NULL);
			AvailableExtProp.resize(PropertyCount);
			return vk::enumerateInstanceExtensionProperties(pLayerName, &PropertyCount,
				AvailableExtProp.data());
		}

		static bool checkValidationExtensionSupport(
			const char* layer,
			std::vector<const char*> extensions) {
			uint32_t propertyCount;
			std::vector<vk::ExtensionProperties> availableExtProp;
			getAvailableExtensions(layer, propertyCount, availableExtProp);
			for (const char* extensionName : extensions) {
				bool extensionFound = false;
				for (const auto& extensionProperties : availableExtProp) {
					if (strcmp(extensionName, extensionProperties.extensionName) == 0) {
						extensionFound = true;
						break;
					}
				}
				if (!extensionFound) {
					return false;
				}
			}
			return true;
		}

		static bool checkValidationLayerExtensionSupport(const char* layer) {
			uint32_t AvailPropertyCount;
			std::vector<vk::ExtensionProperties> AvailableExtProp;
			getAvailableExtensions(layer, AvailPropertyCount, AvailableExtProp);
			std::vector<const char*> extensions;
			for (auto& ExtProp : AvailableExtProp) {
				extensions.push_back(ExtProp.extensionName);
			}
			return checkValidationExtensionSupport(NULL, extensions);
		}

		static vk::Result getAvailableLayers(
			uint32_t& layerCount,
			std::vector<vk::LayerProperties>& availableLayers) {
			availableLayers.clear();
			vk::enumerateInstanceLayerProperties(&layerCount, NULL);
			availableLayers.resize(layerCount);
			return vk::enumerateInstanceLayerProperties(&layerCount,
				availableLayers.data());
		}

		static bool checkValidationLayerSupport(std::vector<const char*> layers,
			bool fullSupportRequired = false) {
			uint32_t layerCount;
			std::vector<vk::LayerProperties> availableLayers;
			getAvailableLayers(layerCount, availableLayers);
			for (const char* layerName : layers) {
				bool layerFound = false;
				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						if (fullSupportRequired &&
							!checkValidationLayerExtensionSupport(layerName)) {
							continue;
						}
						layerFound = true;
						break;
					}
				}
				if (!layerFound) {
					return false;
				}
			}
			return true;
		}

		static void listAvailableExtNLayer() {
			uint32_t extCount;
			std::vector<vk::ExtensionProperties> availableExtProp;
			getAvailableExtensions(nullptr, extCount, availableExtProp);
			std::cout << std::endl << "Avaliable extensions for instance:" << std::endl;
			for (auto& ExtProp : availableExtProp) {
				std::cout << ExtProp.extensionName << "  Version: " << ExtProp.specVersion << std::endl;
			}
			uint32_t layCount;
			std::vector<vk::LayerProperties> availableLayers;
			getAvailableLayers(layCount, availableLayers);
			std::cout << std::endl << "Avaliable layers for instance:" << std::endl;
			for (auto& LayProp : availableLayers) {
				std::cout << LayProp.layerName << "  Version: " << LayProp.specVersion << std::endl;
			}
			std::cout << "===========================================================" << std::endl;
		}
	}

	namespace Device {
		static vk::Result getAvailableExtensions(
			vk::PhysicalDevice device,
			const char* pLayerName,
			uint32_t& PropertyCount,
			std::vector<vk::ExtensionProperties>& AvailableExtProp) {
			AvailableExtProp.clear();
			device.enumerateDeviceExtensionProperties(pLayerName, &PropertyCount, NULL);
			AvailableExtProp.resize(PropertyCount);
			return device.enumerateDeviceExtensionProperties(pLayerName, &PropertyCount,
				AvailableExtProp.data());
		}

		static bool checkValidationExtensionSupport(
			vk::PhysicalDevice device,
			const char* layer,
			std::vector<const char*> extensions) {
			uint32_t propertyCount;
			std::vector<vk::ExtensionProperties> availableExtProp;
			getAvailableExtensions(device, layer, propertyCount, availableExtProp);
			for (const char* extensionName : extensions) {
				bool extensionFound = false;
				for (const auto& extensionProperties : availableExtProp) {
					if (strcmp(extensionName, extensionProperties.extensionName) == 0) {
						extensionFound = true;
						break;
					}
				}
				if (!extensionFound) {
					return false;
				}
			}
			return true;
		}

		static bool checkValidationLayerExtensionSupport(vk::PhysicalDevice device, const char* layer) {
			uint32_t AvailPropertyCount;
			std::vector<vk::ExtensionProperties> AvailableExtProp;
			getAvailableExtensions(device, layer, AvailPropertyCount, AvailableExtProp);
			std::vector<const char*> extensions;
			for (auto& ExtProp : AvailableExtProp) {
				extensions.push_back(ExtProp.extensionName);
			}
			return checkValidationExtensionSupport(device, NULL, extensions);
		}

		static vk::Result getAvailableLayers(
			vk::PhysicalDevice device,
			uint32_t& layerCount,
			std::vector<vk::LayerProperties>& availableLayers) {
			availableLayers.clear();
			device.enumerateDeviceLayerProperties(&layerCount, NULL);
			availableLayers.resize(layerCount);
			return device.enumerateDeviceLayerProperties(&layerCount,
				availableLayers.data());
		}

		static bool checkValidationLayerSupport(
			vk::PhysicalDevice device,
			std::vector<const char*> layers,
			bool fullSupportRequired = false) {
			uint32_t layerCount;
			std::vector<vk::LayerProperties> availableLayers;
			getAvailableLayers(device, layerCount, availableLayers);
			for (const char* layerName : layers) {
				bool layerFound = false;
				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						if (fullSupportRequired &&
							!checkValidationLayerExtensionSupport(device, layerName)) {
							continue;
						}
						layerFound = true;
						break;
					}
				}
				if (!layerFound) {
					return false;
				}
			}
			return true;
		}

		static void listAvailableExtNLayer(vk::PhysicalDevice device) {
			uint32_t extCount;
			std::vector<vk::ExtensionProperties> availableExtProp;
			getAvailableExtensions(device, nullptr, extCount, availableExtProp);
			std::cout << std::endl << "Avaliable extensions for device:" << std::endl;
			for (auto& ExtProp : availableExtProp) {
				std::cout << ExtProp.extensionName << "  Version: " << ExtProp.specVersion << std::endl;
			}
			uint32_t layCount;
			std::vector<vk::LayerProperties> availableLayers;
			getAvailableLayers(device, layCount, availableLayers);
			std::cout << std::endl << "Avaliable layers for device:" << std::endl;
			for (auto& LayProp : availableLayers) {
				std::cout << LayProp.layerName << "  Version: " << LayProp.specVersion << std::endl;
			}
			std::cout << "===========================================================" << std::endl;
		}

		static std::string MapPhysicalDeviceType(vk::PhysicalDeviceType type) {
			switch (type) {
			case vk::PhysicalDeviceType::eIntegratedGpu: return "Integrated GPU";
			case vk::PhysicalDeviceType::eDiscreteGpu: return "Discrete GPU";
			case vk::PhysicalDeviceType::eVirtualGpu: return "Virtual GPU";
			case vk::PhysicalDeviceType::eCpu: return "CPU";
			case vk::PhysicalDeviceType::eOther: return "Other";
			}
		}
	}


}  // namespace VulkanUtil
