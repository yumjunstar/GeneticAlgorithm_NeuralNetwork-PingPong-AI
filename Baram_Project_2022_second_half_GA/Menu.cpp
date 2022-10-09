#include "Menu.h"

Menu::Menu(DrawScreen* ds)
{
	if (ds == (DrawScreen*)nullptr) ds_p = new DrawScreen;
	else ds_p = ds;
}

void Menu::StartMenu() {
	system("cls");
	string menu_str[] = { "AI �н� ��Ű��", "�н��� ������ �������� �÷����ϱ�" };
	ds_p->center_text(menu_str, 2);

	int BladeCount = 10;
	PingPong* game_version = nullptr;
	GeneticAlgorithm* ga = new GeneticAlgorithm(ds_p, BladeCount);
	int key;
	scanf_s("%d", &key);
	switch (key) {
	case 1:
		//�н� ���
		ga->LetsLearn();
		break;
	case 2:
		//���� ���
		game_version = new PingPong(ds_p, false);
		game_version->lets_ping_pong();
		break;
	}

	delete game_version;
}
