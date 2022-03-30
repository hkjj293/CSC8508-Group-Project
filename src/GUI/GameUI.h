#pragma once
#include <vector>
#include "GameMenu.h"

struct ExInputResult;

class GameUI
{
public:
	GameUI();
	~GameUI();

	virtual void Init();
	void PushMenu(const GameMenuPtr& menu);
	GameMenuPtr PopMenu();
	void RemoveMenu(const GameMenuPtr& menu);
	void ClearMenus();

	virtual void UpdateUI() const;
	virtual void DrawUI() const;

private:
	bool IsValid = false;
	std::vector<GameMenuPtr> menus;
};