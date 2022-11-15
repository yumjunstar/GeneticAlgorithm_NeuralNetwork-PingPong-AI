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
	char temp[100];
	sprintf_s(temp, 100, "mode con cols=%d lines=%d | title %s", SIZE_OF_COL_SCREEN + 50, SIZE_OF_ROW_SCREEN + 10, "Ping Pong AI");
	system(temp);
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	//system("color 42");
}