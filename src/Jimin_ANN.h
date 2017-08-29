/*
	여러 종류의 신경망들이 여기서 결합이 된다.

	지도학습
	강화학습
	LSTM 추가
	
	그외 파일 입출력을 위한 I/O 함수들
	
	Neuron.h과 LSTM_Cell.h을 완벽히 분리하는데 성공하여 해결했음
	
	CNN도 결합할까 생각중인데 CNN을 만들기 귀찮고 힘들다.
	
	작성자 서지민

*/

#include <iostream> 
#include <vector>

#include "Full_Connected.h"
#include "LSTM_Network.h"

#include "Util.h"

using namespace std;

class Jimin_ANN
{
	private:
	
	/**********************************************
					신경망 학습요소
	**********************************************/
	
	int Active_Function;
	
	double Epsilon;
	double Epsilon_Loss;
	
	double Init_Epsilon;
	double Init_Epsilon_Loss;
	
	double Learning_Rate;
	double Beta_Rate;
	
	
	public:
	
	Full_Connected full_connected; //완전연결 신경망
	
	LSTM_Network lstm_network; //순환신경망
	
	/**********************************************
					신경망 구조
	**********************************************/
	
	int Class_Length;
	int Input_Length;
	int Result_Length;

	/**********************************************
					신경망 구축하기
	**********************************************/
	Jimin_ANN()
	{
		Init_Epsilon = 0.5;
		Init_Epsilon_Loss = 0.01;
		
		srand(CPU_Clock_Count());
	}
	
	~Jimin_ANN()
	{

	}
	
	//완전연결
	void Build(int Active_Function,int Class_Length,int Layer_Length[],double Learning_Rate,double Beta_Rate,double Epsilon,double Epsilon_Loss)
	{
		this->Active_Function = Active_Function;
		this->Class_Length = Class_Length;
		this->Input_Length = Layer_Length[0];
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		this->Epsilon = Epsilon;
		this->Epsilon_Loss = Epsilon_Loss;
		
		this->Init_Epsilon = Epsilon;
		this->Init_Epsilon_Loss = Epsilon_Loss;

		this->Result_Length = Layer_Length[Class_Length];


		cout << "완전연결 신경망 구축 시작 \n\n";
		full_connected.Build(Active_Function,Class_Length,Layer_Length,Learning_Rate,Beta_Rate);
		cout << "완전연결 신경망 구축 완료 \n\n";
		
		cout << "LSTM 신경망 구축 시작 \n\n";
		lstm_network.Build(Class_Length,Layer_Length,Learning_Rate,Beta_Rate);
		cout << "LSTM 신경망 구축 시작 \n\n";
	}
	
	//Convolute
	void Build(int Width,int Height,int Class_Length,int Result_Length)
	{
		cout << "합성곱 신경망 구축 시작 \n\n";
		
	
		cout << "합성곱 신경망 구축 완료 \n\n";
	}
	
	void Set_Value(int Active_Function,double Learning_Rate,double Beta_Rate,double Epsilon,double Epsilon_Loss)
	{
		this->Active_Function = Active_Function;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		this->Epsilon = Epsilon;
		this->Epsilon_Loss = Epsilon_Loss;
		
		full_connected.Set_Value(Active_Function,Learning_Rate,Beta_Rate);
		lstm_network.Set_Value(Learning_Rate,Beta_Rate);
	}
	
	void Init()
	{
		Epsilon = Init_Epsilon;
		Epsilon_Loss = Init_Epsilon_Loss;
		full_connected.Init();
		lstm_network.Init();
	}
	
	
	/**********************************************
					신경망 전파함수
	**********************************************/
	
	double* Propagate(double Signal[])
	{
		
		double *Result =  full_connected.Propagate(Signal);
		//double *Result =  lstm_network.Propagate(Signal); //잠시 LSTM 동작테스트하기
		
		return Result;
	}
	
	/**********************************************
					강화학습 함수
	**********************************************/
	
	//행동도축
	int Action_Value_Function(double Signal[])
	{
		double *Value_Array = Propagate(Signal);
		
		double MAX=Value_Array[0];
		int Best=0;
		
		for(int i=0;i<Result_Length;i++)
		{
			if(MAX < Value_Array[i])
			{
				MAX = Value_Array[i];
				
				Best = i;
			}
		}
		
		int Action_Value;
		
		if(Epsilon_Function(Epsilon))
		{
			Action_Value = RandomRange(0,Result_Length);
		}
		else
		{
			Action_Value = Best;
		}
		
		return Action_Value;
	}
	
	//가치값 추출
	double Get_Action_Value(double Signal[],int Neuron)
	{
		double *Value_Array = Propagate(Signal);
		
		return Value_Array[Neuron];
	
	}
	
	//입실론 연산
	bool Epsilon_Function(double Probability)
	{
		double rn = RandomRange(0.0,1.0);
		
		if(rn <= Probability)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//입실론 감쇄
	void Epsilon_Loss_Function()
	{
		if(Epsilon - Epsilon_Loss > 0)
		{
			Epsilon = Epsilon - Epsilon_Loss;
		}
	}
	
	//보상회로 동작
	void Reward_Function(double Error_Array[],double Value_Array[],int Length,int Neuron,double Reward_Value)
	{
		double Gradient = 0;
		
		for(int i=0;i<Length;i++)
		{
			Gradient = (1 - pow(Value_Array[i],2)) * 0.5;
			
			Error_Array[i] = - Reward_Value  * Gradient;
		}
		
		Gradient = (1 - pow(Value_Array[Neuron],2)) * 0.5;
		
		Error_Array[Neuron] = Reward_Value  * Gradient;
	}
	
	/**********************************************
					신경망 학습식
	**********************************************/

	//Hebb Rule
	
	void Hebb_Update(double Signal[])
	{
		full_connected.Hebb_Update(Signal);
	}
	
	//부분 오류역전파
	void Network_Update(double Signal[],double Error_Delta[])
	{
		full_connected.Hebb_BP_Update(Signal,Error_Delta);
		//lstm_network.Hebb_BP_Update(Signal,Error_Delta);
		
		Epsilon_Loss_Function();
	}
	
	
	/**********************************************
					오차 계산식
	**********************************************/
	
	//패턴에 대한 오차
	double* Error_Array_Function(double Target[],double Signal[])
	{
		double *Result = Propagate(Signal);
		double *Error_Array = new double[Result_Length];
		
		for(int i=0;i<Result_Length;i++)
		{
			Error_Array[i] = Target[i] - Result[i];
		}
		
		delete []Result;
		
		return Error_Array;
	}
	
	//오차 비용 계산
	double Cost_Function(double Target[],double Signal[])
	{
		
		double *Error_Delta_Array = Error_Array_Function(Target,Signal);
	
		double Error;
		
		for(int i=0;i<Result_Length;i++)
		{
			Error = Error + pow(Error_Delta_Array[i],2);
		}
		
		Error = Error / Result_Length;
		
		delete []Error_Delta_Array;
		
		return Error;
	}
	
	/**********************************************
					신경망 학습식 함수
	******************************************/
	
	//강화학습
	void Reinforcement_Learning(int Data_Size,vector<double*> &Signal_List,vector<int> Action_List,vector<double> Reward_List,int Episode,double MAX_Cost, int View)
	{
		cout << "Learning Start \n\n\n";
		
		double *Error_Array = new double[Result_Length];
		double *Result = new double[Result_Length];
		double Error=0;
		
		int i=0;
		int set = 0;
		int Action_Value=0;
		double Value=0;
		double Reward_Value=0;
		
		
		cout << "Hebb Error Back Propagate Start \n\n\n";
		
		Init();
		
		for(i=0;i<Episode;i++)
		{
			for(set=0;set<Data_Size;set++)
			{
				Result = Propagate(Signal_List[set]);
				
				Action_Value = Action_List[set];
				
				Reward_Value = Reward_List[set];
				
				Reward_Function(Error_Array,Result,Result_Length,Action_Value,Reward_Value);
				
				Network_Update(Signal_List[set],Error_Array);
			}
	
		}
		cout << "Hebb Error Back Propagate Finish \n\n\n";
		
		
		cout << "학습결과 \n\n";
		
		for(set=0;set<Data_Size;set++)
		{
			cout << "학습패턴  " << set << "\n\n";
			
			Result = Propagate(Signal_List[set]);
			
			cout << "Propagate \n";
			Show_Array(Result,Result_Length);
			
			Action_Value = Action_Value_Function(Signal_List[set]);
			Value = Get_Action_Value(Signal_List[set],Action_Value);
			Reward_Value = Reward_List[set];
			
			cout << "Action " << Action_Value << "  ";
			cout << "Action Value " << Value << "  ";
			cout << "Reward  " << Reward_Value << "\n\n\n";
		}
		cout << "\n\n";
		
		delete []Error_Array;
		delete []Result;
		
		cout << "Reinforcement Finish \n\n\n\n";
	}
	
	//지도학습
	void Supervised_Learning(int Data_Size,vector<double*> &Signal_List,vector<double*> &Target_List,int Episode,double MAX_Cost, int View)
	{
		cout << "Learning Start \n\n\n";
		
		double *Error_Array = new double[Result_Length];
		double *Result = new double[Result_Length];
		double Error=0;
		double Total_Error = 0;
		double B_Error = 10000;
		double Error_Delta = 10000;
		double Hit_Rate=0;
		
		int i=0;
		int set = 0;
		
		/*
				선행학습을 하여 미리 깊은층들 가중치를 학습하는 것도 좋으나 반복문 사용 때문에
			오버헤드가 증가한다. 고로 그냥 동시에 한다.
		*/
		
		cout << "Hebb Error Back Propagate Start \n\n\n";
		
		Init();
		
		double Start = NowTime();
		for(i=0;i<Episode;i++)
		{
			
			Error_Delta = 0;
			Error = 0;
			Total_Error = 0;
			
			for(set=0;set<Data_Size;set++)
			{
				Error_Array = Error_Array_Function(Target_List[set],Signal_List[set]);
				
				Network_Update(Signal_List[set],Error_Array);
				
				Error = Cost_Function(Target_List[set],Signal_List[set]);
				
				Total_Error = Total_Error + Error;
			}
			
			Total_Error = Total_Error / Data_Size;
			
			Hit_Rate = (1 - Total_Error) * 100;
			
			if(i % View == 0)
			{
				cout << "Episode  " << i << "\n";
				cout << "Error  " << Total_Error << "\n";
				cout << "정확도  " << Hit_Rate << "%\n\n";
			}
			
			Error_Delta = 0;
			Error_Delta = B_Error - Total_Error;
			Error_Delta = abs(Error_Delta) * 10;
			
			if(  
				(Total_Error < MAX_Cost)  //기존 학습종료식
			   ||  ( (Hit_Rate > 0.8) &&  (Error_Delta < ((Learning_Rate - Beta_Rate) / Data_Size)) ) //변화률이 학습률/데이터 크기 보다 작으면 종료
			  )
				
			{
				cout << "\n\n\n\nLearning Finish Result \n";
				cout << "Episode  " << i << "\n";
				cout << "Error  " << Total_Error << "\n";
				cout << "정확도  " << Hit_Rate << "%\n\n";
				break;
			}
			
			B_Error = Total_Error;
			
		}
		double End = NowTime();
		
		Error_Delta = 0;
		Error = 0;
		Total_Error = 0;
		Hit_Rate = 0;
		
		cout << "학습데이터 검증 \n\n\n";
		
		for(set=0;set<Data_Size;set++)
		{
			Result = Propagate(Signal_List[set]);
			
			cout << "==================================\n";
			cout <<"학습패턴 " << set << "\n\n";
			
			cout << "목표벡터 :";
			Show_Array(Target_List[set],Result_Length); cout << endl;
			
			cout << "신경망  :";
			Show_Array(Result,Result_Length); cout << endl;
			
			Error_Array = Error_Array_Function(Target_List[set],Signal_List[set]);
				
			Error = Cost_Function(Target_List[set],Signal_List[set]);
			
			cout << "오차: " << Error << "\n==================================\n\n\n\n\n";
				
			Total_Error = Total_Error + Error;
		}
		cout << "학습완료 소요시간: " << (End - Start) << "ms\n\n";
		cout << "Hebb Error Back Propagate Finish \n\n\n";
		
		delete []Error_Array;
		delete []Result;
		
		cout << "Supervised_Learning Finish \n\n\n\n";
	
	}
	
	void Show_Array(double Arr[], int Length)
	{
		for(int i=0;i<Length;i++)
		{
			cout << Round(Arr[i],3) << "  ";
		}
		cout << "\n\n";
	}
	
	/**********************************************
					신경망 가중치 I/O 
	******************************************/
	
	//가중치 불러오기
	double* Get_Weight(int Class,int Neuron)
	{
		return full_connected.Get_Weight(Class,Neuron);
	}
	
	//가중치 설정
	void Set_Weight(int Class,int Neuron,int Length, double Weight[])
	{
		return full_connected.Set_Weight(Class,Neuron,Length,Weight);
	}
};