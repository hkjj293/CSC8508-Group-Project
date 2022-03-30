/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "OGLTexture.h"
#include "OGLRenderer.h"

#include "../DebugMode/DebugMode.h"
#include "../Common/TextureLoader.h"

using namespace NCL;
using namespace NCL::Rendering;

OGLTexture::OGLTexture(bool withFBO)
{
	FBO = 0;
	glGenTextures(1, &texID);
	if (withFBO) {
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
		//glDrawBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

OGLTexture::OGLTexture(GLuint texToOwn, bool withFBO ) {
	FBO = 0;
	texID = texToOwn;
	if (withFBO) {
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

OGLTexture::~OGLTexture()
{
	if(texID > 0)
		glDeleteTextures(1, &texID);

	if (FBO > 0) {
		glDeleteFramebuffers(1, &FBO);
	}
	std::cout << "Deleted Tex" << std::endl;
}


TextureBase* OGLTexture::RGBATextureFromData(char* data, int width, int height, int channels) {
	OGLTexture* tex = new(Ty< OGLTexture>()) OGLTexture();
	if (tex->texID <= 0) return nullptr;
	tex->SetHeight(height);
	tex->SetWidth(width);
	int dataSize = width * height * channels; //This always assumes data is 1 byte per channel
	MemoryInformations Info;
	std::stringstream ss;
	ss << "Memory Size for OGLTexture : " << dataSize << " Byte.\nMemory Location for OGLTexture : " << tex << "\n";
	Info.name = "OGLTexture";
	Info.info = ss.str();
	Info.size = dataSize;
	DebugMode::AddMemoryInfo(Info);
	int sourceType = GL_RGB;

	switch (channels) {
		case 1: sourceType = GL_RED	; break;

		case 2: sourceType = GL_RG	; break;
		case 3: sourceType = GL_RGB	; break;
		case 4: sourceType = GL_RGBA; break;
		//default:
	}

	glBindTexture(GL_TEXTURE_2D, tex->texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, sourceType, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

TextureBase* OGLTexture::RGBATextureFromFilename(const std::string&name) {
	char* texData	= nullptr;
	int width		= 0;
	int height		= 0;
	int channels	= 0;
	int flags		= 0;
	TextureLoader::LoadTexture(name, texData, width, height, channels, flags);  

	TextureBase* glTex = RGBATextureFromData(texData, width, height, channels);

	free(texData);

	return glTex;
}

void OGLTexture::GenFrameBuffer() {
	if(FBO != 0) glDeleteFramebuffers(1, &FBO);
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}