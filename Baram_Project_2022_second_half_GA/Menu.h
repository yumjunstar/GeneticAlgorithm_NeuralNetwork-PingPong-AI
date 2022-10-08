#pragma once
#include "Common_Define.h"
#include "DrawScreen.h"
#include "PingPong.h"
class Menu
{
	DrawScreen* ds_p;
public:
	Menu(DrawScreen* ds)
	{
		if (ds == (DrawScreen*)nullptr) ds_p = new DrawScreen;
		else ds_p = ds;
	}
	void StartMenu() {
		system("cls");
		string menu_str[] = { "AI �н� ��Ű��", "�н��� ������ �������� �÷����ϱ�" };
		ds_p->center_text(menu_str, 2);

		PingPong* game_version = nullptr;
		int key;
		scanf_s("%d", &key);
		switch (key) {
		case 1:
			//�н� ���
			game_version = new PingPong(ds_p, true);
			game_version->lets_ping_pong();

			break;
		case 2:
			//���� ���
			game_version = new PingPong(ds_p, false);
			game_version->lets_ping_pong();
			break;
		}

		delete game_version;
	}
};

//�н��� �°� ����,
// BladeBothSide = true
// AmountBladeEach = 1
// AmountBall = 1
// 
// 
// ���ÿ� ������ �� �ֵ��� �Լ��� ���� ������ �Ѵ�.
// NN���� PPG�� ����Ҷ��� �迭 binary�� ����
// PPG���� �ʿ��� �Լ�
// keyup(bool object [], int );
// keydown(bool object[]);
// 
// PPG���� NN���� ���� ������ �������� binary ���, blade�� ������ �ٸ� �Լ��� ��������
// 
// PPG�� �ʿ��� �Լ�
// int current_blade_count(); ��� �ִ� Ź��ä ���� ��ȯ
// int ball_count(); ������ ������ ��� ��� ���� �����ѵڿ� �Ⱥ��� �״�
// int* get_blade_coor(); //Ź��ä�� ������� ��ġ ��ȯ
// pair<int, int>* get_ball_coor(); //���� ���� ��ġ ��ȯ
// 
// GA���� NN���� ó�� �����Ҷ�
// 
// GA�� �ʿ��� �Լ�
// ����ġ �迭�� ������ ���⼭ ������ �Ѵ�.
// init() �ʱ�ȭ �ϰ� �ʱ� ���� ����
// play() -> ���� �����ϰ� ���� �Ű����
// choice()
// crossover()
// mutation()
// 
// NN���� �ʿ��� �Լ�
// 
// void set_weight(matrix bundle[], int count)//����ġ ���� �Լ�
// bool query(struct Ball_Info, struct Blade_Info); 
// 
// PPG�� ���� �Է��� ������ �Լ��� ȣ���Ͽ� ����� ����ϴ� �Լ� true�� ���� false�� �Ʒ��� key�� ������.
// 
// NN������ GA�� ���� ����ġ ���� �޴� �Լ��� ������ ��
// GA������ PPG�κ��� �������� �޴� �Լ��� ������ �Ѵ�.
//
// �н� ���̸� ���̵带 �����ʿ��� ����ü ������ŭ, 
// 