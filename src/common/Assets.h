/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace NCL {
	class Assets {
	public:
		static std::string SHADERDIR;
		static std::string MESHDIR;
		static std::string TEXTUREDIR;
		static std::string SOUNDSDIR;
		static std::string FONTSSDIR;
		static std::string DATADIR;
		static bool ReadTextFile(const std::string& filepath, std::string& result);
		static bool ReadBinaryFile(const std::string& filepath, char** into, size_t& size);

		static std::string RetrieveAssetDirectory(std::string config = "dir.config") {
			std::string line;
			std::ifstream dirFile(config);
			if (dirFile.is_open())
			{
				while (std::getline(dirFile, line))
				{
					
				}
				dirFile.close();
				return line;
			}
			return "##";
		}

		static void FetchDirConfig(std::string config = "dir.config") {
			std::string dir = "";
			dir = RetrieveAssetDirectory(config);
			if (dir == "##") return;
			SHADERDIR = dir + "Shaders/";
			MESHDIR = dir + "Meshes/";
			TEXTUREDIR = dir + "Textures/";
			SOUNDSDIR = dir + "Sounds/";
			FONTSSDIR = dir + "Fonts/";
			DATADIR = dir + "Data/";
		}
	private:
		Assets() {

		};
		~Assets() {};
	};
}