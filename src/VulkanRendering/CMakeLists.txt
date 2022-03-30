add_library(VulkanRendering STATIC
    VulkanTexture.cpp
    VulkanShaderBuilder.cpp
    VulkanShader.cpp
    VulkanRenderPassBuilder.cpp
    VulkanRenderer.cpp
    VulkanPipelineBuilder.cpp
    VulkanMesh.cpp
    VulkanDescriptorSetLayoutBuilder.cpp
    #VulkanDebugger.cpp
    
    #VulkanUtility.h
    VulkanTexture.h
    VulkanShaderBuilder.h
    VulkanShader.h
    VulkanRenderPassBuilder.h
    VulkanRenderer.h
    VulkanPipelineBuilder.h
    VulkanMesh.h
    VulkanDescriptorSetLayoutBuilder.h
    #VulkanDebugger.h
)