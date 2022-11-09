#include "Menu.h"

Menu::Menu(DrawScreen* ds, FileManage* fm)
{
	if (ds == (DrawScreen*)nullptr) {
		ds_p = new DrawScreen;
		assert(ds_p);
	}

	else ds_p = ds;

	fm_p = fm;
}

void Menu::StartMenu(const int BladeCount) {
	ds_p->screen_clear();

	ds_p->center_text(MenuStrs, 2);

	int key;
	cin >> key;
	if (key == 1) //학습 모드
	{
		GeneticAlgorithm* ga = new GeneticAlgorithm(ds_p, BladeCount);
		ga->SetFileManage_Pointer(fm_p);
		assert(ga);
		ga->LetsLearn();
		delete ga;
	}
	else if (key == 2) //경쟁 모드
	{

		PingPong* game_version = new PingPong(ds_p, false);
		game_version->SetFileManage_Pointer(fm_p);


		assert(game_version);
		game_version->lets_ping_pong_compete_mode();
		delete game_version;
	}


}
