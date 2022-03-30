#include "GameTechRenderer.h"
#include "../game/GameEntity.h"
#include "../Common/Camera.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"
#include "../Common/TextureLoader.h"
#include "Transform.h"
#include "../game/Painter.h"
#include "../game/Bullet.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#define SHADOWSIZE 2048

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	{
	viewportDimension = { currentWidth,currentHeight };
	initTexture = false;
	painted = false;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	shadowShader = new OGLShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");

	// Depth Attachment
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			     SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	//FBO
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT ,GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glClearColor(1, 1, 1, 1);

	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(-200.0f, 60.0f, 200.0f);

	//painter
	painterShader = dynamic_cast<OGLShader*>(LoadShader("PaintEffectShader.set"));
	initShader = dynamic_cast<OGLShader*>(LoadShader("InitEffectShader.set"));
	painterMesh = new OGLMesh();
	painterMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	painterMesh->SetVertexIndices({ 2,1,0,0,3,2 });
	painterMesh->SetVertexTextureCoords({ Vector2(0,1), Vector2(0,0),Vector2(1,0), Vector2(1,1) });
	painterMesh->UploadToGPU();

	//Skybox!
	skyboxShader = new OGLShader("skyboxVertex.glsl", "skyboxFragment.glsl");
	skyboxMesh = new OGLMesh();
	skyboxMesh->SetVertexPositions({Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	skyboxMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	skyboxMesh->UploadToGPU();

	LoadSkybox();
}

void GameTechRenderer::init() {
	//viewportDimension = { currentWidth,currentHeight };
	//initTexture = false;
	//painted = false;
	//glDeleteFramebuffers(1, &shadowFBO);
	//// Depth Attachment
	//glGenTextures(1, &shadowTex);
	//glBindTexture(GL_TEXTURE_2D, shadowTex);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	//	SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glBindTexture(GL_TEXTURE_2D, 0);

	////FBO
	//glGenFramebuffers(1, &shadowFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	//glDrawBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////painter
}

GameTechRenderer::~GameTechRenderer()	{
	delete painterShader;
	delete shadowShader;
	delete painterMesh;
	delete skyboxMesh;
	delete skyboxShader;
	glDeleteTextures(1, &shadowTex);
	glDeleteTextures(1, &skyboxTex);
	glDeleteFramebuffers(1, &shadowFBO);
}

void GameTechRenderer::LoadSkybox() {
	string filenames[6] = {
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
	};

	int width[6]	= { 0 };
	int height[6]	= { 0 };
	int channels[6] = { 0 };
	int flags[6]	= { 0 };

	vector<char*> texData(6, nullptr);

	for (int i = 0; i < 6; ++i) {
		TextureLoader::LoadTexture(filenames[i], texData[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[0] != height[0])) {
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			return;
		}
	}
	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	GLenum type = channels[0] == 4 ? GL_RGBA : GL_RGB;

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width[i], height[i], 0, type, GL_UNSIGNED_BYTE, texData[i]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GameTechRenderer::RenderFrame() {
	glEnable(GL_CULL_FACE);
	//glClearColor(1, 1, 1, 1);
	BuildObjectList(false,0);
	if (!initTexture) {
		initTextures();
		initTexture = true;
	}
	UpdatePaints();

	SortObjectList(false, 0);
	RenderShadowMap();
	glClearColor(0.2, 0.2, 0.2, 1);
	viewportDimension = { currentWidth / min(gameWorld.GetLocalPlayerCount(), 2) , currentHeight / ((gameWorld.GetLocalPlayerCount() - 1) / 2 + 1) };
	for (int i = 0; i < gameWorld.GetLocalPlayerCount(); i++) {
		BuildObjectList(true, i); 
		glViewport(
			(i % 2) * viewportDimension.x,
			(((gameWorld.GetLocalPlayerCount()-1) / 2) - (i / 2)) * viewportDimension.y,
			viewportDimension.x,
			viewportDimension.y);
		RenderSkybox(i);
		RenderCamera(i);
	}
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
	viewportDimension = { currentWidth, currentHeight };

}

void GameTechRenderer::initTextures() {
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < (activeObjects).size(); i++) {
		GameTimer t;
		if ((activeObjects)[i]->GetName() != "Wall") continue;
		(activeObjects)[i]->GetRenderObject()->SetColour(Vector4(Vector3((activeObjects)[i]->GetRenderObject()->GetColour()), 0.0f));
		//OGLTexture* objTex = dynamic_cast<OGLTexture*>(activeObjects[i]->GetRenderObject()->GetDefaultTexture());
		OGLTexture* renderTex = dynamic_cast<OGLTexture*>((activeObjects)[i]->GetRenderObject()->GetDefaultTexture());

		if (renderTex->GetFBO() == 0) continue;

		glBindFramebuffer(GL_FRAMEBUFFER, renderTex->GetFBO());
		GLuint tex = renderTex->GetObjectID();
		glViewport(0, 0, renderTex->GetWidth(), renderTex->GetHeight());

		BindShader(initShader);

		BindTextureToShader(renderTex, "hitTex", 0);

		int modelLocation = glGetUniformLocation(initShader->GetProgramID(), "modelMatrix");
		Matrix4 modelMatrix = Matrix4();// (activeObjects)[i]->GetTransform().GetMatrix();

		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		BindMesh(painterMesh);
		//int layerCount = it->first->GetRenderObject()->GetMesh()->GetSubMeshCount();
		//for (int i = 0; i < layerCount; ++i) {
		DrawBoundMesh();
		//}
		glViewport(0, 0, currentWidth, currentHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		t.Tick();
		//std::cout << "InitTextures: " << t.GetTimeDeltaMSec() << std::endl;
	}
	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::UpdatePaints() {
	glDisable(GL_CULL_FACE);
	PainterMap map = Painter::GetPaintInfos();
	for (auto& it = map.begin(); it != map.end(); it++) {
		if ((it->second - it->first->GetRenderObject()->GetTransform()->GetPosition()).Length() > (1 + it->first->GetRenderObject()->GetTransform()->GetScale().Length() * 0.5f)) continue;
		it->first->GetRenderObject()->SetColour(Vector4(Vector3(it->first->GetRenderObject()->GetColour()), 1.0f));

		OGLTexture* renderTex = dynamic_cast<OGLTexture*>(it->first->GetRenderObject()->GetDefaultTexture());
		if (renderTex->GetFBO() == 0) continue;

		glBindFramebuffer(GL_FRAMEBUFFER, renderTex->GetFBO());
		GLuint tex = renderTex->GetObjectID();
		glViewport(0, 0, renderTex->GetWidth(), renderTex->GetHeight());

		BindShader(painterShader);

		BindTextureToShader(renderTex, "hitTex", 0);

		int hitUV = glGetUniformLocation(painterShader->GetProgramID(), "hitPos");
		glUniform3fv(hitUV, 1, it->second.array);

		int initID = glGetUniformLocation(painterShader->GetProgramID(), "isInit");
		glUniform1i(initID, 0);

		int modelLocation = glGetUniformLocation(painterShader->GetProgramID(), "modelMatrix");
		Matrix4 modelMatrix = it->first->GetRenderObject()->GetTransform()->GetMatrix();

		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		BindMesh(it->first->GetRenderObject()->GetMesh());
		//int layerCount = it->first->GetRenderObject()->GetMesh()->GetSubMeshCount();
		//for (int i = 0; i < layerCount; ++i) {
		DrawBoundMesh();
		//}
		glViewport(0, 0, currentWidth, currentHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Painter::ClearPaint();
	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::BuildObjectList(bool isCameraBased, int cameraNum) {
	activeObjects.clear();
	activeTransparentObjects.clear();

	gameWorld.OperateOnContents(
		[&](GameEntity* o) {
			if (o->IsActive()) {
				if (isCameraBased) {
					if ((o->GetName() == "Wall" && (o->GetRenderObject()->GetTransform()->GetPosition() - gameWorld.GetMainCamera(cameraNum)->GetPosition()).Length() < (gameWorld.GetPlayer(cameraNum)->GetRenderObject()->GetTransform()->GetPosition() - gameWorld.GetMainCamera(cameraNum)->GetPosition()).Length()) 
						|| (o->GetName() == "Ground" && gameWorld.GetMainCamera(cameraNum)->GetPosition().y < (o->GetRenderObject()->GetTransform()->GetPosition().y + (o->GetRenderObject()->GetTransform()->GetScale().y * 0.5f)))) {
						activeTransparentObjects.emplace_back(o);
					}
					else {
						activeObjects.emplace_back(o);
					}
				}
				else {
					activeObjects.emplace_back(o);
				}
			}
		}
	);
}

void GameTechRenderer::SortObjectList(bool isCameraBased, int cameraNum) {
	//Who cares!

}

void GameTechRenderer::RenderShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);
	glDepthMask(GL_TRUE);
	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0), Vector3(0,1,0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (int j = 0; j < 2; j++) {
		for (const auto& i : activeObjects) {
			if (i->GetRenderObject()->GetColour().w == 0.0f) continue;
			Matrix4 modelMatrix = (*i).GetRenderObject()->GetTransform()->GetMatrix();
			Matrix4 mvpMatrix = mvMatrix * modelMatrix;
			glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
			OGLTexture* renderTex = dynamic_cast<OGLTexture*>(i->GetRenderObject()->GetDefaultTexture());
			BindTextureToShader(renderTex, "mainTex", 0);

			BindMesh((*i).GetRenderObject()->GetMesh());
			int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i);
			}
		}
	}

	glViewport(0, 0, currentWidth, currentHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox(int cameraNum) {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	float screenAspect = (float)viewportDimension.x / (float)viewportDimension.y;
	Matrix4 viewMatrix = gameWorld.GetMainCamera(cameraNum)->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera(cameraNum)->BuildProjectionMatrix(screenAspect);

	BindShader(skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int texLocation  = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	BindMesh(skyboxMesh);
	DrawBoundMesh();

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GameTechRenderer::RenderCamera(int cameraNum) {
	float screenAspect = (float)viewportDimension.x / (float)viewportDimension.y;
	Matrix4 viewMatrix = gameWorld.GetMainCamera(cameraNum)->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera(cameraNum)->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int projLocation	= 0;
	int viewLocation	= 0;
	int modelLocation	= 0;
	int colourLocation  = 0;
	int hasVColLocation = 0;
	int hasTexLocation  = 0;
	int isTransparent	= 0;
	int shadowLocation  = 0;

	int lightPosLocation	= 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	int count = 0;

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDepthFunc(GL_LESS);
	for (int j = 0; j < 2; j++) {
		vector<GameEntity*>* list;

		if (j == 1) {
			list = &activeTransparentObjects;
			glDepthMask(GL_FALSE);
		}
		else {
			list = &activeObjects;
			glDepthMask(GL_TRUE);
		}

		for (const auto& i : *list) {
			if (i->GetRenderObject()->GetColour().w == 0.0f) continue;
			//Draw to Texture
			OGLShader* shader = (OGLShader*)(*i).GetRenderObject()->GetShader();
			//std::cout << shader->GetProgramID() << std::endl;
			BindShader(shader);
			BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (activeShader != shader) {
				projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
				viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
				modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
				shadowLocation = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
				colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
				hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
				hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
				isTransparent = glGetUniformLocation(shader->GetProgramID(), "isTransparent");

				lightPosLocation = glGetUniformLocation(shader->GetProgramID(), "lightPos");
				lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
				lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");
				GLuint timeLocation = glGetUniformLocation(shader->GetProgramID(), "dt");
				cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
				GLuint playerLocation = glGetUniformLocation(shader->GetProgramID(), "playerPos");
				glUniform3fv(cameraLocation, 1, (float*)&gameWorld.GetMainCamera(cameraNum)->GetPosition());
				glUniform3fv(playerLocation, 1, (float*)&gameWorld.GetPlayer(cameraNum)->GetTransform().GetPosition());

				glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
				glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

				glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
				glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
				glUniform1f(lightRadiusLocation, lightRadius);
				glUniform1f(timeLocation, Window::GetTimer()->GetTotalTimeSeconds());

				int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
				glUniform1i(shadowTexLocation, 1);

				activeShader = shader;
			}

			Matrix4 modelMatrix = (*i).GetRenderObject()->GetTransform()->GetMatrix();
			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

			Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
			glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

			glUniform4fv(colourLocation, 1, (float*)&i->GetRenderObject()->GetColour());

			glUniform1i(hasVColLocation, !(*i).GetRenderObject()->GetMesh()->GetColourData().empty());

			glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture() ? 1 : 0);
			glUniform1i(isTransparent, j==1 ? 1 : 0);

			BindMesh((*i).GetRenderObject()->GetMesh());
			int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i);
				count++;
			}
			//std::cout << this << std::endl;
		}
	}
	std::cout << count << std::endl;
	glDepthMask(GL_TRUE);
}

Matrix4 GameTechRenderer::SetupDebugLineMatrix()	const  {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera(0)->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera(0)->BuildProjectionMatrix(screenAspect);

	return projMatrix * viewMatrix;
}

Matrix4 GameTechRenderer::SetupDebugStringMatrix()	const {
	return Matrix4::Orthographic(-1, 1.0f, 100, 0, 0, 100);
}
