/*
	완전연결 신경망 구현
	
	오류역전파
	헤브 학습규칙
	
	오류역전파는 출력층만 적용하고 나머지 깊이의 계층에서는
	
	헤브 학습규칙을 사용한다.
	
	작성자 서지민
*/
	
#include <iostream>
#include <cmath>

#include "Neuron_Layer.h"

using namespace std;

class Full_Connected
{
	
	private:
	
	/**********************************************
					신경망 학습요소
	**********************************************/
	
	int Active_Function;
	double Learning_Rate;
	double Beta_Rate;
	int BP_Start;
	
	public:
	
	/**********************************************
					신경망 구조
	**********************************************/
	
	Neuron_Layer *Layer;
	
	int Class_Length;
	int *Layer_Length;
	int Input_Length;
	int Result_Length;
	
	/**********************************************
					신경망 구축하기
	**********************************************/
	
	Full_Connected()
	{
		
	}
	
	~Full_Connected()
	{
	
	}
	
	void Build(int Active_Function,int Class_Length,int Layer_Length[],double Learning_Rate,double Beta_Rate)
	{
		this->Active_Function = Active_Function;
		this->Class_Length = Class_Length;
		this->Layer_Length = Layer_Length;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		Input_Length = Layer_Length[0];
		Result_Length = Layer_Length[Class_Length];
		
		Layer = new Neuron_Layer[Class_Length];
	
		int Length;
		int Input_Length;
		
		for(int i=0;i<Class_Length;i++)
		{
			Input_Length = Layer_Length[i];
			Length = Layer_Length[i+1];
			
			Layer[i].Build(Active_Function,Length,Input_Length,Learning_Rate,Beta_Rate);
		}
		
		if(Class_Length - 1 > 0)
		{
			BP_Start = Class_Length - 1 ;
		}
		else
		{
			BP_Start = 0;
		}
		
	}
	
	void Set_Value(int Active_Function,double Learning_Rate,double Beta_Rate)
	{
		
		this->Active_Function = Active_Function;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		for(int i=0;i<Class_Length;i++)
		{	
			Layer[i].Set_Value(Active_Function,Learning_Rate,Beta_Rate);
		}
	}
	
	void Init()
	{
		for(int i=0;i<Class_Length;i++)
		{
			
			Layer[i].Init();
		}
	}
	
	
	/**********************************************
					신경망 전파함수
	**********************************************/
	double* Propagate(double Signal[])
	{
		
		double *Temp = Signal;
		double *Result = new double[Result_Length];
		
		for(int i=0;i<Class_Length;i++)
		{
			Result = new double[Layer_Length[i+1]];
			Result = Layer[i].Propagate(Temp);
			
			Temp = Result;
		}
		
		return Result;
	}
	
	double* Propagate_LSTM(double Signal[])
	{
		
		double *Temp = Signal;
		double *Result = new double[Result_Length];
		
		for(int i=0;i<Class_Length;i++)
		{
			Result = new double[Layer_Length[i+1]];
			Result = Layer[i].Propagate_LSTM(Temp);
			
			Temp = Result;
		}
		
		return Result;
	}
	
	/**********************************************
					신경망 학습식
	**********************************************/

	void Hebb_BP_Update(double Signal[],double Error_Delta[])
	{
		double *Temp = Layer[0].Propagate(Signal);
		double *Temp2 = NULL;
		
		for(int i=1;i<BP_Start;i++)
		{
			Layer[i].Hebb_Update(Temp);
			
			Temp2 = Layer[i].Propagate(Temp);
			
			Temp = Temp2;
			
		}
		
		Layer[Class_Length-1].BP_Update(Error_Delta);
	}
	
	
	//Hebb Rule
	void Hebb_Update(double Signal[])
	{
		
		Layer[0].Hebb_Update(Signal);
		
		double *Temp = Layer[0].Propagate(Signal);
		double *Temp2 = NULL;
		
		for(int i=1;i<BP_Start;i++)
		{
			Layer[i].Hebb_Update(Temp);
			
			Temp2 = Layer[i].Propagate(Temp);
			
			Temp = Temp2;
			
		}

	}
	
	//LSTM Hebb Rule
	void LSTM_Update(double Signal[],double Error_Array[])
	{
		
		Layer[0].Hebb_Update(Signal);
		
		double *Temp = Layer[0].Propagate_LSTM(Signal);
		double *Temp2 = NULL;
		
		for(int i=1;i<BP_Start;i++)
		{
			Layer[i].LSTM_Update(Temp);
			
			Temp2 = Layer[i].Propagate_LSTM(Temp);
			
			Temp = Temp2;
			
		}
		
		Layer[Class_Length-1].LSTM_Update(Temp,Error_Array);

	}
	
	/**********************************************
					신경망 가중치 I/O 
	**********************************************/

	
	double* Get_Weight(int Class,int Neuron)
	{
		int Length = Layer[Class].neuron_layer[Neuron].Input_Length + 1;
		
		double *Weight = new double[Length];
		
		double *Weight_Array = Layer[Class].neuron_layer[Neuron].Weight;
		double Bias_Weight = Layer[Class].neuron_layer[Neuron].Bias_Weight;
		
		for(int i=0;i<Length-1;i++)
		{
			Weight[i] = Weight_Array[i];
		}
		
		Weight[Length-1] = Bias_Weight;
		
		return Weight;
	}
	
	void Set_Weight(int Class,int Neuron,int Length,double Weight[])
	{
		double *Weight_Array = new double[Length];
		
		for(int i=0;i<Length;i++)
		{
			Weight_Array[i] = Weight[i];
		}
		double Bias_Weight = Weight[Length];
		
		Layer[Class].neuron_layer[Neuron].Weight = Weight_Array;
		Layer[Class].neuron_layer[Neuron].Bias_Weight = Bias_Weight;
	}
	
	
};