#pragma once
#pragma once
#include "../OpenGLRendering/OGLRenderer.h"
#include "../OpenGLRendering/OGLShader.h"
#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLMesh.h"

#include "../game/AssetsManager.h"

#include "GameWorld.h"
#include "..//common/Matrix4.h"
namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;
		class GameLoadingRenderer : public OGLRenderer {
		public:
			GameLoadingRenderer();
			~GameLoadingRenderer();

		protected:
			void RenderFrame()	override;

			Matrix4 SetupDebugLineMatrix()	const override { return Matrix4(); };
			Matrix4 SetupDebugStringMatrix()const override { return Matrix4(); };

			OGLMesh* painterMesh;
			OGLShader* painterShader;
			TexturePtr painterTex;
		};
	}
}