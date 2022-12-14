#pragma once
#include "Common_Define.h"
#include "NeuralNetwork.h"
#include "PingPong.h"
#include "FileManage.h"
// init->play->choice->crossover->mutation 이 한 세대
class GeneticAlgorithm
{
public:
	enum WEIGHT_RESET_ENUM { UNIFORM_RANDOM, NORMAL_ZERO, ZERO };
	const vector<size_t> NeuralShape = { 9, 10, 10, 10, 3 };
	//2를 사용하면 한쪽에 몰리고 3을 하면 한 곳에만 가만히 있는다.
	// 유전 알고리즘을 사용하는 것은 큰 신경망에 적합하지 않은 것 같다.
	// 랜덤 값이 각 값을 크게 바꿔버린다.
	// 계속 값에 1보다 작은 값을 곱해버리니 뒤로갈 수록 신경망이 약해진다. 소수 정확도가 높거나
	// 1보다 큰 값을 곱해야 한다.

	//
	const size_t InputNodeCount = NeuralShape[0];
	const size_t OutputNodeCount = NeuralShape[NeuralShape.size() - 1];
	const size_t NeuralLayerCount = NeuralShape.size();
	// 이전의 신경망 입력 : 공의 위치 x, y, 탁구채의 위치 x, y, 공의 방향
	// 이후에 다음 시도
	// 공의 x위치
	// 공의 y위치
	// 아래는 OneHotEncoding
	// 공의 방향1
	// 공의 방향2
	// 공의 방향3
	// 공의 방향4
	// 공의 방향5
	// 공의 방향6(다꺼지면 stop)
	// 탁구채의 y 위치
	//

	// 두번째 시도
	// 위의 값을 모두 int 그대로 입력하고자 한다. 이렇게 하면 sigmoid 는 사용이 불가능 하고 relu만 가능할 것이다.
	// 실패 했다. 한 곳에 몰리기만 하고 제대로 작동 하지 않는다 아마 각자 최대값이 달라서 그런 것 같다.

	//세번째 시도 공과 탁구채와의 거리를 얻기 대각선 거리
	// 실패 이상하게 한쪽으로 몰린다. 이 값만 커서 그런지

	// 화면의 비율을 적절하게 조정하니 잘 작동한다.

	// 정지 키가 없으면 개체수가 적어도 계속 움직이는 개체가 살아 남는다.
	// 정치 키가 있으면 보통 가만히 있는 개체가 살아 남게 된다. 움직이면서 탁구공을 치는 것보다 가만히 있는게 더 탁구공을 잘 치는 것 같다.
	// 현재 탁구채의 위치를 피해서 탁구공을 던져줘야 하나

	// 점수가 같으면 distance가 더 작은 개체를 우선순위로 정렬
	const size_t WeightMatrixCount = NeuralLayerCount - 1;

	//ZERO : 0으로 모든 가중치를 초기화
	//NORMAL_ZERO : 0을 중심으로 모든 가중치 정규분포 초기화
	//UNIFROM_RANDOM : -1과 1 사이로 모든 가중치 균일분포 초기화
	static constexpr WEIGHT_RESET_ENUM ResetWeightsMode = NORMAL_ZERO;
	//랜덤으로 설정하지 않는 것이 학습이 더 잘되는 것 같다.
	static constexpr double MutationMeanValue = 0;
	static constexpr double MutationSigmaValue = 0.01;
	static constexpr double MutationSigmaValue_BeforeGetScore = 0.05;
	static constexpr size_t MinimumScore_BeforeChangeSigma = 2;
	//변이 값을 너무 크게 하면 안된다. 그럼 신경망이 고장난다.
	static constexpr double Elite_Percentage = 0.3;
	static constexpr int PerGenerationGameTries = 100;


	static constexpr size_t CleanUpStartScore = 20;
	static constexpr size_t CleanUpMinimumScore = 5;
	//신경망 입력에 배수
	static constexpr int MultipleNumberForNNInput = 1;


	//학습의 속도를 빠르게 하기 위해 점수 허들을 넣을지 여부 
	//0이면 허들 없고 그대로 점수 상승 
	//0이상의 값이면 그 값이 도달했을때 다음세대로 진행, 리스폰이 0이고 해당 점수를 받으면 점수 허들을 두배씩 올린다.
	static constexpr size_t FirstScoreHurdle = 100;
	//GoalScore
	static constexpr size_t GoalScore = 100000;
	GeneticAlgorithm(DrawScreen* ds, size_t blades_count);
	~GeneticAlgorithm();

	void init();
	void play();
	void choice();
	void crossover();
	void mutation();
	void apply();
	void LetsLearn();

	void SetFileManage_Pointer(FileManage* fm);
	void SetDrawScreen_Pointer(DrawScreen* ds);
private:


	//신경망
	NeuralNetwork* nn;
	PingPong* ppg;
	DrawScreen* ds_p;
	FileManage* fm_p;

	size_t AllAIBladesCount;
	size_t Generation;
	size_t LoopRepeat;

	vector <Blade_Info> AllBladeScoreVector;
	vector <Coor> AllBladeCoorVector;
	vector <OneDNNWeights> EliteNeuralWeightMatrixVector;

	size_t* Output_CMD_Array;
	bool* KeyUp_Binary_CMD;
	bool* KeyDown_Binary_CMD;

	size_t CurrentScoreHuddle;


	OneDNNWeights DNN_Copy(OneDNNWeights source, size_t size);
	void CleanUpBladesForVisability(size_t GameTries, int blade_id);
	void SetBladeDirection(NNOUT_DIRECTION direction, int blade_id);
	OneDNNWeights AddNormalDistribution(OneDNNWeights standard);
	void ChangeRandomDirectionForPerfectLearn(int ball_x, int RepeatLoop);
	void SaveAllDNNWeightsIntoFile();
	void SaveStatistics();
	void CurrentHurdleUpdate();
};

