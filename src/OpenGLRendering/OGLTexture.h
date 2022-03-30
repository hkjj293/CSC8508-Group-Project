/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "../Common/TextureBase.h"
#include "glad\glad.h"

#include <string>

namespace NCL {
	namespace Rendering {
		class OGLTexture : public TextureBase
		{
		public:
			//friend class OGLRenderer;
			 OGLTexture(bool withFBO = false);
			 OGLTexture(GLuint texToOwn, bool withFBO = false);
			~OGLTexture();
			void Init(std::vector<std::string> args) override { 
				if (args.size() > 0 && args[0] == "FBO") {
					GenFrameBuffer(); 
				} 
			}
			void SetHeight(int height) { this->height = height; }
			void SetWidth(int width) { this->width = width; }
			int GetHeight() { return height; }
			int GetWidth() { return width; }

			static TextureBase* RGBATextureFromData(char* data, int width, int height, int channels);

			static TextureBase* RGBATextureFromFilename(const std::string&name);

			GLuint GetObjectID() const	{
				return texID;
			}

			GLuint GetFBO() const {
				return FBO;
			}

			void GenFrameBuffer();
		protected:						
			GLuint texID;
			int height;
			int width;
			GLuint FBO;
		};
	}
}

