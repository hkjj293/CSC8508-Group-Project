#include "Painter.h"

PainterMap Painter::paintInfos;

void Painter::Paint(GameEntity* entity, Vector3 uv) {
	paintInfos.insert(PainterPair(entity,uv));
}

PainterMap Painter::GetPaintInfos() {
	return paintInfos;
}

void Painter::ClearPaint() {
	paintInfos.clear();
}