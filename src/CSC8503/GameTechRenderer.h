#pragma once
#include "../OpenGLRendering/OGLRenderer.h"
#include "../OpenGLRendering/OGLShader.h"
#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLMesh.h"

#include "GameWorld.h"
#include "..//common/Matrix4.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		struct Dimension {
			int x;
			int y;
		};
		class RenderObject;
		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();  
			void SetTextureInit(bool hasInit) { initTexture = hasInit; };
			void init();

		protected:
			void RenderFrame()	override;

			Matrix4 SetupDebugLineMatrix()	const override;
			Matrix4 SetupDebugStringMatrix()const override;

			OGLShader*	defaultShader;

			GameWorld&	gameWorld;

			void initTextures();
			void UpdatePaints();
			void BuildObjectList(bool isCameraBased, int cameraNum);
			void SortObjectList(bool isCameraBased, int cameraNum);
			void RenderShadowMap();
			void RenderCamera(int cameraNum);
			void RenderSkybox(int cameraNum);

			void LoadSkybox();

			vector<GameEntity*> activeObjects;
			vector<GameEntity*> activeTransparentObjects;

			OGLMesh*	painterMesh;
			OGLShader*	painterShader;
			OGLShader*	initShader;
			GLuint		PainterFBO;

			OGLShader*  skyboxShader;
			OGLMesh*	skyboxMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowColourTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;

			Dimension		viewportDimension;

			bool painted;
			bool initTexture;
		};
	}
}

