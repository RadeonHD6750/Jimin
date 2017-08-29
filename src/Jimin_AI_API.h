/*
	라이브러리는 이미 Jimin_ANN.h이나 당장에
	동작할 API를 만들었음
	
	어딜가나 여기서처럼 코딩하면 됨
	
	아무튼 라이브러리는 Jimin_ANN.h으로 한다.
	
	#include "Jimin_ANN.h"를 하여 사용하면 된다.
	
	작성자 서지민

*/

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

#include "Jimin_ANN.h"

using namespace std;

class Jimin_AI_API
{
	
	private:
	
	int Active_Function;
	double Learning_Rate;
	double Beta_Rate;
	
	public:
	
	Jimin_ANN Network;
	
	int Class_Length;
	int Input_Length;
	int Result_Length;
	
	/**********************************************
					신경망 구축하기
	**********************************************/
	
	Jimin_AI_API()
	{
		Learning_Rate = 0.25;
		Beta_Rate = 0.15;
	}
	
	~Jimin_AI_API()
	{
		
	}
	
	//완전연결
	void Build(int Active_Function,int Class_Length,int Layer_Length[],double Learning_Rate, double Beta_Rate)
	{
		this->Active_Function = Active_Function;
		this->Class_Length = Class_Length;
		this->Input_Length = Layer_Length[0];
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		this->Result_Length = Layer_Length[Class_Length];
		
		Network.Build(Active_Function,Class_Length,Layer_Length,Learning_Rate,Beta_Rate,0.5,0.01);
		
		Propagate_Test();
		
		cout << "신경망 정상 동작 \n\n\n\n";
	}
	
	//Convolute
	void Build(int Width,int Height,int Class_Length,int Result_Length,double Learning_Rate, double Beta_Rate)
	{
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
	}
	
	void Init()
	{
		Network.Init();
	}
	
	/**********************************************
					신경망 전파함수
	**********************************************/
	
	double* Propagate(double Signal[])
	{
		double *Result = Network.Propagate(Signal);
		
		return Result;
	}
	
	/**********************************************
					행동 정책
	**********************************************/
	int Action_Value_Function(double Signal[])
	{
		return Network.Action_Value_Function(Signal);
	}
	
	double Get_Action_Value(double Signal[],int Neuron)
	{
		return Network.Get_Action_Value(Signal,Neuron);
	}
	
	/**********************************************
					신경망 학습함수
	**********************************************/
	
	void Hebb_Update(double Signal[])
	{
		Network.Hebb_Update(Signal);
	}
	
	void Network_Update(double Signal[],double Error_Delta[])
	{
		Network.Network_Update(Signal,Error_Delta);
	}
	
	/**********************************************
					신경망 가중치 I/O
	**********************************************/
	
	/*
	double* Get_Neuron_Weight()
	{
		
	}
	
	void Set_Neuron_Weight(double Weight[])
	{
		
	}*/
	
	
	/**********************************************
					신경망 각종 동작테스트
	******************************************/
	
	void Propagate_Test()
	{
		cout << "신경망 전파 동작 테스트 \n\n";
		double *Test_Vector = new double[this->Input_Length];
		
		cout << "입력패턴 \n";
		for(int i=0;i<Input_Length;i++)
		{
			Test_Vector[i] = RandomRange(-1.0,1.0);
			cout << Round(Test_Vector[i],3) << "  ";
		}
		cout << "\n\n\n";
		
		double Start = CPU_Clock_Count();
		
		double *Result = Propagate(Test_Vector);
		
		double End = CPU_Clock_Count();
		
		cout << "신경망 출력패턴 \n";
		for(int i=0;i<Result_Length;i++)
		{
			cout << Round(Result[i],3) << "  ";
		}
		cout << "\n소요시간: " << (End-Start) << "Clock  \n\n\n";
		
		delete []Test_Vector;
		delete []Result;
	}
	
	//지도학습
	void Supervised_Learning_Test(int Data_Size)
	{
		cout << "Supervised_Learning_Test Start \n\n";
		
		vector<double*> Signal_List;
		vector<double*> Target_List;
		
		Pattern *Data = new Pattern[Data_Size];
		
		for(int set=0;set<Data_Size;set++)
		{
			
			Data[set].Build(Input_Length,Result_Length);
			
			
			Signal_List.push_back(Data[set].GetSignal());
			Target_List.push_back(Data[set].GetTarget());
		}
		
		
		Network.Supervised_Learning(Data_Size,Signal_List,Target_List,200,0.01,2);
		
		for(int i=0;i<Data_Size;i++)
		{
			Data[i].Delete();
		}
		
		delete []Data;
		
		cout << "Supervised_Learning_Test Finish \n\n\n\n";
	}
	
	//강화학습
	void Reinforcement_Learning_Test(int Data_Size)
	{
		cout << "Reinforcement_Learning_Test Start \n\n";
		
		vector<double*> Signal_List;
		vector<int> Action_List;
		vector<double> Reward_List;
		
		int Action_Value;
		double Reward;
		
		Pattern *Data = new Pattern[Data_Size];
		
		for(int set=0;set<Data_Size;set++)
		{
			Data[set].Build(Input_Length,Result_Length);
			
			Action_Value = RandomRange(0,Result_Length);
			Reward = RandomRange(-1.0,1.0);
			
			Signal_List.push_back(Data[set].GetSignal());
			Action_List.push_back(Action_Value);
			Reward_List.push_back(Reward);
		}
		
		Network.Reinforcement_Learning(Data_Size,Signal_List,Action_List,Reward_List,200,0.01,5);
		
		for(int i=0;i<Data_Size;i++)
		{
			Data[i].Delete();
		}
		
		delete []Data;
		
		cout << "Reinforcement_Learning_Test Finish \n\n\n\n";
	}
	
	void AI_Learning_Testing()
	{
		cout << "Jimin AI Running Test\n";
	
		srand(time(NULL));	
	
		int Length[7] = {256,32,12,10,8,6,4}; //신경망 구조 정의

		Build(0,6, Length,0.25,0.15); //신경망 구축하기

		int Data_Size = 0;
		int Learning_Case = 1;

		cout << "테스트 크기: ";
		cin >> Data_Size;
		cout <<"\n\n";

		cout << "학습유형 1 2: ";
		cin >> Learning_Case;
		cout << "\n\n";

		switch(Learning_Case)
		{
			case 1:
				Supervised_Learning_Test(Data_Size); //학습동작 테스트
				break;


			case 2:
				Reinforcement_Learning_Test(Data_Size);
				break;

			default:
				break;
		}

	}
	
	class Pattern
	{
		public:
		double *Signal;
		double *Target;
		
		void Build(int Input_Length,int Result_Length)
		{
			Signal = new double[Input_Length];
			Target = new double[Result_Length];
			
			for(int i=0;i<Input_Length;i++)
			{
				Signal[i] = RandomRange(-1.0,1.0);
			}

			for(int i=0;i<Result_Length;i++)
			{
				Target[i] = RandomRange(-1.0,1.0);
			}
		}
		
		void Delete()
		{
			delete []Signal;
			delete []Target;
		}
		
		double* GetSignal()
		{
			return this->Signal;
		}
		
		double* GetTarget()
		{
			return this->Target;
		}
		
	};
};