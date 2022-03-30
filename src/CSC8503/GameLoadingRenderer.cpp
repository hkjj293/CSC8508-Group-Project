#include "GameLoadingRenderer.h"
#include "../Common/TextureLoader.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

GameLoadingRenderer::GameLoadingRenderer() : OGLRenderer(*Window::GetWindow()){
	painterShader = dynamic_cast<OGLShader*>(LoadShader("LoadingShader.set"));
	painterMesh = new OGLMesh();
	painterMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	painterMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	painterMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(0,1),Vector2(1,1), Vector2(1,0) });
	painterMesh->UploadToGPU();
	AssetsManager::LoadTextureFromFile("LoadingTex", "Loading.png");
	painterTex = AssetsManager::FetchTexture("LoadingTex");
}

GameLoadingRenderer::~GameLoadingRenderer() {

}

void GameLoadingRenderer::RenderFrame() {
	BindShader(painterShader);
	BindTextureToShader(painterTex.get(), "mainTex", 0); 
	GLuint timeLocation = glGetUniformLocation(painterShader->GetProgramID(), "dt");
	glUniform1f(timeLocation, Window::GetTimer()->GetTotalTimeSeconds());
	BindMesh(painterMesh);
	DrawBoundMesh();
}