#pragma once
#include <vector>
#include <string>

namespace NCL {
	namespace Rendering {
		class TextureBase
		{
		public:
			virtual ~TextureBase();
			virtual void Init(std::vector<std::string> args) = 0;
		protected:
			TextureBase();
		};
	}
}

