#include "RendererBase.h"
using namespace NCL;
using namespace Rendering;

RendererBase::RendererBase(Window& window) : hostWindow(window)	{

}



RendererBase::~RendererBase()
{
}

ShaderMap RendererBase::LoadShaderSet(const std::string& name, const std::string& platformName) {
	std::ifstream metaFile(Assets::SHADERDIR + name);
	std::string line;
	ShaderMap map;
	if (metaFile.is_open())
	{
		while (!metaFile.eof()) {
			std::string input;
			metaFile >> input;
			if (input == platformName) {
				metaFile >> input;
				if (input != "{") break;
				std::string shaderType = "";
				std::string shaderName = "";
				while (shaderType != "}") {
					metaFile >> shaderType;
					metaFile >> shaderName;
					map[shaderType] = shaderName;
				}
			}
		}
		metaFile.close();
	}
	return map;
}
