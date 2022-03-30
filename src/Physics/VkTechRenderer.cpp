#include "VkTechRenderer.h"
#include "../common/TextureLoader.h"
#include "../game/GameEntity.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer(GameWorld& world) : VulkanRenderer(*Window::GetWindow()), gameWorld(world){
	InitVulkan();
}

VkTechRenderer::~VkTechRenderer() {
	device.destroy(sampler);
	device.destroy(desSetLayout);
	device.destroy(pipeline.pipeline);
	device.destroy(pipeline.layout);
}

void VkTechRenderer::InitVulkan() {
	// Get resources from AssetManager
	// Initialize the resources
	// Init Pipeline
	/*skyboxTex = (VulkanTexture*)TextureLoader::LoadAPITexture("checkerboard.png");

	skyboxMesh = new VulkanMesh("Quad.msh");
	skyboxMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	skyboxMesh->UploadToGPU(this);

	builder = VulkanShaderBuilder()
		.WithVertexBinary("SimpleVert.spv")
		.WithFragmentBinary("SimpleFrag.spv");
	skyboxShader = builder.Build(*this);*/

	matrix = Matrix4();
	matrix.SetPositionVector(Vector3(0.2f, 0.2f, 0.2f));
	InitUniformBuffer(matrixDataObject, matrix.array, sizeof(matrix.array));

	//BuildPipeline();
}

void VkTechRenderer::RenderFrame() {
	BuildObjectList();
	/*SortObjectList();
	RenderShadowMap();
	RenderSkybox();
	RenderCamera();*/
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 VPMat = gameWorld.GetMainCamera(0)->BuildProjectionMatrix(screenAspect) * gameWorld.GetMainCamera(0)->BuildViewMatrix();

	UpdateUniformBuffer(matrixDataObject, VPMat.array, sizeof(VPMat.array));

	frameCmdBuffer.beginRenderPass(defaultBeginInfo, vk::SubpassContents::eInline);

	for (int i = 0; i < activeObjects.size(); i++) {
		Matrix4 MMat = activeObjects[i]->GetTransform().GetMatrix();
		if (activeObjects[i]->GetRenderObject()->GetMesh()) {
			frameCmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, (*ObjectPipelineMap[activeObjects[i]]).pipeline);
			frameCmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, (*ObjectPipelineMap[activeObjects[i]]).layout, 0, 1, set.data(), 0, nullptr);
			VulkanMesh* mesh = dynamic_cast<VulkanMesh*>(activeObjects[i]->GetRenderObject()->GetMesh());
			frameCmdBuffer.pushConstants((*ObjectPipelineMap[activeObjects[i]]).layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(MMat.array), MMat.array);
			if (mesh) SubmitDrawCall(mesh, frameCmdBuffer);
		}
	}
	frameCmdBuffer.endRenderPass();
}

void VkTechRenderer::UpdatePaints() {
	
}

void VkTechRenderer::CreateNewEntityPipeline(GameEntity* entity) {
	VulkanShader* shader = dynamic_cast<VulkanShader*>(entity->GetRenderObject()->GetShader());
	VulkanMesh* mesh = dynamic_cast<VulkanMesh*>(entity->GetRenderObject()->GetMesh());
	VulkanTexture* texture = dynamic_cast<VulkanTexture*>(entity->GetRenderObject()->GetDefaultTexture());

	VulkanDescriptorSetLayoutBuilder desSetLayoutBuilder;
	desSetLayoutBuilder
		.WithDebugName("desSetLayoutBuilder")
		.WithUniformBuffers(1, vk::ShaderStageFlagBits::eVertex)
		.WithSamplers(1, vk::ShaderStageFlagBits::eFragment);
	desSetLayout = desSetLayoutBuilder.Build(*this);

	//Global 
	set.push_back(BuildDescriptorSet(desSetLayout));

	vk::PushConstantRange pushConstant;
	pushConstant
		.setOffset(0)
		.setSize(sizeof(matrix.array))
		.setStageFlags(vk::ShaderStageFlagBits::eVertex);

	VulkanPipelineBuilder pipelineBuilder;
	pipelineBuilder
		.WithDebugName("Pipeline")
		.WithDepthState(vk::CompareOp::eLess, true, true)
		.WithPass(defaultRenderPass)
		.WithShaderState(shader)
		.WithVertexSpecification(mesh->GetVertexSpecification(), vk::PrimitiveTopology::eTriangleList)
		.WithDescriptorSetLayout(desSetLayout)
		.WithPushConstant(pushConstant);

	VulkanPipeline* pipeline = new VulkanPipeline(pipelineBuilder.Build(*this));
	pipelinePool.push_back(pipeline);
	ObjectPipelineMap[entity] = pipeline;

	// Descriptor sets
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo
		.setMagFilter(vk::Filter::eLinear)
		.setMinFilter(vk::Filter::eLinear)
		.setAddressModeU(vk::SamplerAddressMode::eRepeat)
		.setAddressModeV(vk::SamplerAddressMode::eRepeat)
		.setAddressModeW(vk::SamplerAddressMode::eRepeat)
		.setAnisotropyEnable(true)
		.setMaxAnisotropy(gpu.getProperties().limits.maxSamplerAnisotropy)
		.setBorderColor(vk::BorderColor::eIntOpaqueBlack)
		.setUnnormalizedCoordinates(false)
		.setCompareEnable(false)
		.setCompareOp(vk::CompareOp::eAlways)
		.setMipmapMode(vk::SamplerMipmapMode::eLinear)
		.setMipLodBias(0.0f)
		.setMinLod(0.0f)
		.setMaxLod(0.0f);

	vk::Sampler sampler = device.createSampler(samplerInfo);
	vk::DescriptorBufferInfo bufferInfo;
	vk::DescriptorImageInfo imageInfo;
	bufferInfo
		.setBuffer(matrixDataObject.buffer)
		.setOffset(0)
		.setRange(sizeof(matrix.array));

	imageInfo
		.setImageLayout(texture->GetLayout())
		.setImageView(texture->GetDefaultView())
		.setSampler(sampler);

	vk::WriteDescriptorSet desWrite[2];
	desWrite[0]
		.setDstSet(set[0])
		.setDstBinding(0)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDescriptorCount(1)
		.setPBufferInfo(&bufferInfo)
		.setPImageInfo(nullptr)
		.setPTexelBufferView(nullptr);

	desWrite[1]
		.setDstSet(set[0])
		.setDstBinding(1)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		.setDescriptorCount(1)
		.setPBufferInfo(nullptr)
		.setPImageInfo(&imageInfo)
		.setPTexelBufferView(nullptr);

	device.updateDescriptorSets(2, desWrite, 0, nullptr);
}

void VkTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld.OperateOnContents(
		[&](GameEntity* o) {
			if (o->IsActive()) {
				activeObjects.emplace_back(o);
				if (ObjectPipelineMap[o] == nullptr) {
					CreateNewEntityPipeline(o);
				}
			}
		}
	);
}

void VkTechRenderer::SortObjectList() {
	
}

void VkTechRenderer::RenderShadowMap() {
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	//glCullFace(GL_FRONT);

	//BindShader(shadowShader);
	//int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	//Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0), Vector3(0, 1, 0));
	//Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	//Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	//shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	//for (const auto& i : activeObjects) {
	//	Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
	//	Matrix4 mvpMatrix = mvMatrix * modelMatrix;
	//	glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
	//	BindMesh((*i).GetMesh());
	//	int layerCount = (*i).GetMesh()->GetSubMeshCount();
	//	for (int i = 0; i < layerCount; ++i) {
	//		DrawBoundMesh(i);
	//	}
	//}

	//glViewport(0, 0, currentWidth, currentHeight);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glCullFace(GL_BACK);
}

void VkTechRenderer::RenderSkybox() {
	/*glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	BindShader(skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int texLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	BindMesh(skyboxMesh);
	DrawBoundMesh();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);*/
}

void VkTechRenderer::RenderCamera() {
	/*float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;
	int colourLocation = 0;
	int hasVColLocation = 0;
	int hasTexLocation = 0;
	int shadowLocation = 0;

	int lightPosLocation = 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto& i : activeObjects) {
		OGLShader* shader = (OGLShader*)(*i).GetShader();
		BindShader(shader);
		BindTextureToShader((OGLTexture*)(*i).GetDefaultTexture(), "mainTex", 0);

		if (activeShader != shader) {
			projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
			viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
			modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
			shadowLocation = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
			colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
			hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
			hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");

			lightPosLocation = glGetUniformLocation(shader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");

			cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
			glUniform3fv(cameraLocation, 1, (float*)&gameWorld.GetMainCamera()->GetPosition());

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
			glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
			glUniform1f(lightRadiusLocation, lightRadius);

			int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
			glUniform1i(shadowTexLocation, 1);

			activeShader = shader;
		}

		Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
		glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

		glUniform4fv(colourLocation, 1, (float*)&i->GetColour());

		glUniform1i(hasVColLocation, !(*i).GetMesh()->GetColourData().empty());

		glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetDefaultTexture() ? 1 : 0);

		BindMesh((*i).GetMesh());
		int layerCount = (*i).GetMesh()->GetSubMeshCount();
		for (int i = 0; i < layerCount; ++i) {
			DrawBoundMesh(i);
		}
	}*/
}

void VkTechRenderer::BuildPipeline() {
	//Per Shader
	desSetLayoutBuilder
		.WithDebugName("desSetLayoutBuilder")
		.WithUniformBuffers(1, vk::ShaderStageFlagBits::eVertex)
		.WithSamplers(1, vk::ShaderStageFlagBits::eFragment);
	desSetLayout = desSetLayoutBuilder.Build(*this);

	//Global 
	set.push_back(BuildDescriptorSet(desSetLayout));

	vk::SamplerCreateInfo samplerInfo;
	samplerInfo
		.setMagFilter(vk::Filter::eLinear)
		.setMinFilter(vk::Filter::eLinear)
		.setAddressModeU(vk::SamplerAddressMode::eRepeat)
		.setAddressModeV(vk::SamplerAddressMode::eRepeat)
		.setAddressModeW(vk::SamplerAddressMode::eRepeat)
		.setAnisotropyEnable(true)
		.setMaxAnisotropy(gpu.getProperties().limits.maxSamplerAnisotropy)
		.setBorderColor(vk::BorderColor::eIntOpaqueBlack)
		.setUnnormalizedCoordinates(false)
		.setCompareEnable(false)
		.setCompareOp(vk::CompareOp::eAlways)
		.setMipmapMode(vk::SamplerMipmapMode::eLinear)
		.setMipLodBias(0.0f)
		.setMinLod(0.0f)
		.setMaxLod(0.0f);
	sampler = device.createSampler(samplerInfo);

	bufferInfo
		.setBuffer(matrixDataObject.buffer)
		.setOffset(0)
		.setRange(sizeof(matrix.array));

	imageInfo
		.setImageLayout(skyboxTex->GetLayout())
		.setImageView(skyboxTex->GetDefaultView())
		.setSampler(sampler);


	desWrite[0]
		.setDstSet(set[0])
		.setDstBinding(0)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDescriptorCount(1)
		.setPBufferInfo(&bufferInfo)
		.setPImageInfo(nullptr)
		.setPTexelBufferView(nullptr);

	desWrite[1]
		.setDstSet(set[0])
		.setDstBinding(1)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		.setDescriptorCount(1)
		.setPBufferInfo(nullptr)
		.setPImageInfo(&imageInfo)
		.setPTexelBufferView(nullptr);

	device.updateDescriptorSets(2, desWrite, 0, nullptr);

	vk::PushConstantRange pushConstant;
	pushConstant
		.setOffset(0)
		.setSize(sizeof(matrix.array))
		.setStageFlags(vk::ShaderStageFlagBits::eVertex);

	pipelineBuilder
		.WithDebugName("Pipeline")
		.WithDepthState(vk::CompareOp::eLess, true, true)
		.WithPass(defaultRenderPass)
		.WithShaderState(skyboxShader)
		.WithVertexSpecification(skyboxMesh->GetVertexSpecification(), vk::PrimitiveTopology::eTriangleList)
		.WithDescriptorSetLayout(desSetLayout)
		.WithPushConstant(pushConstant);
	pipeline = pipelineBuilder.Build(*this);
}

