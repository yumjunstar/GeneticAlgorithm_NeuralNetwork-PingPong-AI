#include "Menu.h"

Menu::Menu(DrawScreen* ds)
{
	if (ds == (DrawScreen*)nullptr) ds_p = new DrawScreen;
	else ds_p = ds;
}

void Menu::StartMenu() {
	ds_p->screen_clear();

	ds_p->center_text(MenuStrs, 2);

	const int BladeCount = 10;
	PingPong* game_version = nullptr;
	GeneticAlgorithm* ga = new GeneticAlgorithm(ds_p, BladeCount);


	int key;
	scanf_s("%d", &key);
	switch (key) {
	case 1:
		//학습 모드
		ga->LetsLearn();
		break;
	case 2:
		//경쟁 모드
		game_version = new PingPong(ds_p, false);
		game_version->lets_ping_pong();
		break;
	}

	delete game_version;
}
