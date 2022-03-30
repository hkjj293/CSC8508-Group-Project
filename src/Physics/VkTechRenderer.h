#pragma once

#include "../VulkanRendering/VulkanRenderer.h"
#include "../VulkanRendering/VulkanShader.h"
#include "../VulkanRendering/VulkanTexture.h"
#include "../VulkanRendering/VulkanMesh.h"
#include "../VulkanRendering/VulkanPipelineBuilder.h"
#include "../VulkanRendering/VulkanDescriptorSetLayoutBuilder.h"
#include "../VulkanRendering/VulkanRenderPassBuilder.h"
#include "../VulkanRendering/VulkanShaderBuilder.h"
#include "../Common/TextureLoader.h"

#include "../common/Matrix4.h"
#include "../CSC8503/GameWorld.h"
#include "../game/Painter.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;

		class VkTechRenderer : public VulkanRenderer {
		public:
			VkTechRenderer(GameWorld& world);
			~VkTechRenderer();

		protected:
			void	InitVulkan();
			void	RenderFrame()	override;

			void	InitDefaultRenderPass() override { VulkanRenderer::InitDefaultRenderPass(); };
			void	InitDefaultDescriptorPool() override { VulkanRenderer::InitDefaultDescriptorPool(); };

			void	CreateNewEntityPipeline(GameEntity* entity);
			//Matrix4 SetupDebugLineMatrix()	const;
			//Matrix4 SetupDebugStringMatrix()const;

			//VulkanShader* defaultShader;
			void UpdatePaints();
			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera();
			void RenderSkybox();

			//void LoadSkybox();

			void BuildPipeline();


			GameWorld& gameWorld;

			vector<GameEntity*> activeObjects;
			map<GameEntity*, VulkanPipeline*> ObjectPipelineMap;
			vector<VulkanPipeline*> pipelinePool;

			VulkanTexture* basicTex;
			VulkanShader* skyboxShader;
			VulkanMesh* skyboxMesh;
			UniformData matrixDataObject;
			Matrix4 matrix;
			VulkanTexture* skyboxTex;

			////shadow mapping things
			//VulkanShader* shadowShader;
			//VulkanTexture* shadowTex;
			//VulkanTexture* shadowFBO;
			//Matrix4     shadowMatrix;

			//Vector4		lightColour;
			//float		lightRadius;
			//Vector3		lightPosition;

			vk::DescriptorBufferInfo bufferInfo;
			vk::DescriptorImageInfo imageInfo;
			vk::Sampler sampler;
			vk::WriteDescriptorSet desWrite[2];
			VulkanDescriptorSetLayoutBuilder desSetLayoutBuilder;
			vk::DescriptorSetLayout desSetLayout;
			std::vector<vk::DescriptorSet> set;

			VulkanPipelineBuilder pipelineBuilder;
			VulkanPipeline pipeline;
			
			VulkanShaderBuilder builder;
		};
	}
}