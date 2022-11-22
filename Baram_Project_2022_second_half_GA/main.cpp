#include "DrawScreen.h"
#include "Ball.h"
#include "Blade.h"
#include "PingPong.h"
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "Common_Define.h"
#include "Menu.h"
#include "FileManage.h"
void init_setting();

int main(void) {
	init_setting();
	DrawScreen *dds = new DrawScreen;
	FileManage* fm = new FileManage;
	Menu m(dds, fm);
	m.StartMenu(100);
	delete dds;
	delete fm;
}
void init_setting()
{

	//10%������ �״�� ����� �׷��� ������ �ڲ� ���ο� �������̰� ���ܼ�
	//����� ��ġ�� �� ����.
	//�׳� ���� ������ �ִ� �͵鵵 ���´�.
	//��Ƽ ���������� ȭ�� ����ϱ�
	srand(time(NULL));
	DrawScreen::set_title("PingPong AI");
	DrawScreen::set_console_size(SIZE_OF_ROW_SCREEN, SIZE_OF_COL_SCREEN);
	DrawScreen::hide_scrollbar();
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	//system("color 42");
}