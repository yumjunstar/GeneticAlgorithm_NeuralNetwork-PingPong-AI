#include "DrawScreen.h"
#include "Ball.h"
#include "Blade.h"
#include "PingPong.h"
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "Common_Define.h"
#include "Menu.h"
void init_setting();

int main(void) {
	init_setting();
	DrawScreen *dds = new DrawScreen;
	Menu m(dds);
	m.StartMenu();
	delete dds;
}
void init_setting()
{
	srand(time(NULL));
	char temp[100];
	sprintf_s(temp, 100, "mode con cols=%d lines=%d | title %s", SIZE_OF_COL_SCREEN + 50, SIZE_OF_ROW_SCREEN + 10, "Ping Pong AI");
	system(temp);
}