#pragma once
#include "GameEntity.h"
#include "../common/Vector3.h"

using namespace NCL;
using namespace Rendering;
using namespace Maths;

typedef std::map<GameEntity*, Vector3> PainterMap;
typedef std::pair<GameEntity*, Vector3> PainterPair;

class Painter {
public:
	~Painter() {}
	static void Paint(GameEntity* entity, Vector3 uv);
	static PainterMap GetPaintInfos();
	static void ClearPaint();
private:
	Painter() {}
	static PainterMap paintInfos;
};