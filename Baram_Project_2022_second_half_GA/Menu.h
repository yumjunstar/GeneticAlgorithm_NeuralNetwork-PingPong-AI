#pragma once
#include "Common_Define.h"
#include "DrawScreen.h"
#include "PingPong.h"
#include "GeneticAlgorithm.h"
class Menu
{
	DrawScreen* ds_p;
	const string MenuStrs[2] = { "AI 학습 시키기", "학습한 내용을 바탕으로 플레이하기" };
public:
	Menu(DrawScreen* ds);
	void StartMenu();
};

//학습에 맞게 세팅,
// BladeBothSide = true
// AmountBladeEach = 1
// AmountBall = 1
// 
// 
// 동시에 조종할 수 있도록 함수를 따로 만들어야 한다.
// NN에서 PPG가 통신할때는 배열 binary로 전송
// PPG에서 필요한 함수
// keyup(bool object [], int );
// keydown(bool object[]);
// 
// PPG에서 NN으로 여러 정보를 보낼때도 binary 사용, blade의 개수는 다른 함수로 가져오기
// 
// PPG에 필요한 함수
// int current_blade_count(); 살아 있는 탁구채 개수 반환
// int ball_count(); 있으면 좋지만 없어도 상관 없음 선언한뒤에 안변할 테니
// int* get_blade_coor(); //탁구채의 상대적인 위치 반환
// pair<int, int>* get_ball_coor(); //현재 공의 위치 반환
// 
// GA에서 NN으로 처음 시작할때
// 
// GA에 필요한 함수
// 가중치 배열의 묶음을 여기서 만들어야 한다.
// init() 초기화 하고 초기 집단 형성
// play() -> 게임 시작하고 여러 신경망을
// choice()
// crossover()
// mutation()
// 
// NN에서 필요한 함수
// 
// void set_weight(matrix bundle[], int count)//가중치 적용 함수
// bool query(struct Ball_Info, struct Blade_Info); 
// 
// PPG로 부터 입력을 받으면 함수를 호출하여 결과를 출력하는 함수 true면 위로 false면 아래로 key를 내린다.
// 
// NN에서는 GA로 부터 가중치 값을 받는 함수를 만들어야 함
// GA에서는 PPG로부터 점수값을 받는 함수를 만들어야 한다.
//
// 학습 중이면 블레이드를 오른쪽에만 염색체 갯수만큼, 
// 