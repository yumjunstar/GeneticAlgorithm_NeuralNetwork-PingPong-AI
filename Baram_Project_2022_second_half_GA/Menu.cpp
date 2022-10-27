#include "Menu.h"

Menu::Menu(DrawScreen* ds)
{
	if (ds == (DrawScreen*)nullptr) ds_p = new DrawScreen;
	else ds_p = ds;
}

void Menu::StartMenu(const int BladeCount) {
	ds_p->screen_clear();

	ds_p->center_text(MenuStrs, 2);

	int key;
	scanf_s("%d", &key);
	if (key == 1) //학습 모드
	{
		GeneticAlgorithm* ga = new GeneticAlgorithm(ds_p, BladeCount);
		ga->LetsLearn();
		delete ga;
	}
	else if (key == 2) //경쟁 모드
	{

		PingPong* game_version = new PingPong(ds_p, false);
		game_version->lets_ping_pong();
		delete game_version;
	}


}
