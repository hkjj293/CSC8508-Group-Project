#pragma once
#include <string>
#include "../CSC8503/GameWorld.h"
#include "../common/TextureBase.h"
#include <stack>

typedef int TexID;
typedef int ShaderID;
typedef int MeshID;

typedef std::shared_ptr<TextureBase> TexturePtr;
typedef std::shared_ptr<ShaderBase> ShaderPtr;
typedef std::shared_ptr<MeshGeometry> MeshPtr;
typedef std::shared_ptr<RendererBase> RendererPtr;

class AssetsManager {
public:
	~AssetsManager();
	static void LoadAssets(std::string label, std::string jsonFile);

	static TexID LoadTextureFromFile(std::string name, std::string fileName, bool isShared = true);
	static ShaderID LoadShaderFromFile(std::string name, std::string fileName, bool isShared = true);
	static MeshID LoadMeshFromFile(std::string name, std::string fileName, bool isShared = true);

	static TexID LoadTexture(std::string name, TexturePtr texture, bool isShared = true);
	static ShaderID LoadShader(std::string name, ShaderPtr shader, bool isShared = true);
	static MeshID LoadMesh(std::string name, MeshPtr mesh, bool isShared = true);

	static TexturePtr FetchTexture(std::string name, TexID id = 0);
	static ShaderPtr FetchShader(std::string name, ShaderID id = 0);
	static MeshPtr FetchMesh(std::string name, MeshID id = 0);

	static void UnloadTexture(std::string name, TexID id);
	static void UnloadShader(std::string name, TexID id);
	static void UnloadMesh(std::string name, TexID id);

	static void SetRenderer(RendererPtr renderer);
	static RendererPtr GetRenderer();

	static void Reset();
private:
	AssetsManager() {};

	static std::map<std::string, std::vector<TexturePtr>> texturePoolMap;
	static std::map<std::string, std::vector<ShaderPtr>> shaderPoolMap;
	static std::map<std::string, std::vector<MeshPtr>> meshPoolMap;
	static std::map<std::string, std::stack<TexID>> TexStackMap;
	static std::map<std::string, std::stack<ShaderID>> ShaderStackMap;
	static std::map<std::string, std::stack<MeshID>> MeshStackMap;
	static RendererBase* renderer;
};