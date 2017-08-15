/*
	뉴런 하나를 정의하고 동작하는 클래스
	
	작성자 서지민
*/

#include <iostream>
#include <cmath>

#include "Util.h"

using namespace std;

class Neuron
{
	
	private:
	
	/**********************************************
					신경망 학습요소
	**********************************************/
	
	int Active_Function;
	double Learning_Rate;
	double Beta_Rate;
	
	public:
	
	/**********************************************
					신경망 구조
	**********************************************/
	
	int Input_Length;
	double *Input_Signal;
	double *Weight;
	double Bias_Weight;
	double Result;
	double Error;
	
	/**********************************************
					신경망 구축
	**********************************************/
	
	Neuron()
	{
		Learning_Rate = 0.25;
		Beta_Rate = 0.15;
	}
	
	~Neuron()
	{
		
		delete [] Input_Signal;
		delete [] Weight;
	}
	
	void Build(int Active_Function,int Input_Length,double Learning_Rate,double Beta_Rate)
	{
		this->Active_Function =Active_Function;
		this->Input_Length = Input_Length;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		Weight = new double[Input_Length];
		Input_Signal = new double[Input_Length];
		
		Init();
	}
	
	void Set_Value(int Active_Function,double Learning_Rate,double Beta_Rate)
	{
	
		this->Active_Function = Active_Function;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
	}
	
	void Init()
	{
		for(int i=0;i<Input_Length;i++)
		{
			Weight[i] = RandomRange(-1.0,1.0);
		}
		Bias_Weight = RandomRange(-1.0,1.0);
		
	}


	/**********************************************
					신경망 전파함수
	**********************************************/
	
	//내적함수
	double Dot(double Signal[])
	{
		double X=0;
		
		for(int i=0;i<Input_Length;i++)
		{
			X = X + (Weight[i] * Signal[i]);
			this->Input_Signal[i] = Signal[i];
		}
	
		X = X + (Bias_Weight + 1.0);
		
		return X;
	}

	double Propagate(double Signal[])
	{
		
		double X=Dot(Signal);
		
		switch(Active_Function)
		{
			case 0:
				X = Tanh(X);
				break;
				
			case 1:
				X = Sigmoid(X);
				break;
				
			default:
				X = Tanh(X);
				break;
		}
		
		Result = X;
		
		return X;
	}
	
	//양극 탄젠트
	double Tanh(double X)
	{
		return X = tanh(X);
	}

	//시그모이드
	double Sigmoid(double X)
	{
		return X = 1 / (1 + exp(-X));
	}
	
	
	/**********************************************
					신경망 학습함수
	**********************************************/
	
	//오류 역전파 함수
	void BP_Update(double Error_Delta)
	{
		
		double Gradient = (1 - pow(Result,2)) * 0.5;
		
		for(int i=0;i<Input_Length;i++)
		{
			Weight[i] = Weight[i] + Learning_Rate * (Input_Signal[i] * Error_Delta) * Gradient;
		}
		
		Bias_Weight = Bias_Weight + Learning_Rate * (1.0 * Error_Delta) * Gradient;
		
		Error = Error_Delta;
	}
	
	//자기 조직 입력벡터 거리
	double SOM_Distance(double Signal[])
	{
		double Distance = 0;
		
		for(int i=0;i<Input_Length;i++)
		{
			Distance = Distance + (Signal[i] - Weight[i]);
		}
		Distance = Distance + (1 - Bias_Weight);
		
		return Distance;
	}
	
	//자기 조직 사상
	void SOM_Update(double Signal[])
	{
		double Gradient = (1 - pow(Result,2)) * 0.5;
		
		for(int i=0;i<Input_Length;i++)
		{
			Weight[i] = Weight[i] + Learning_Rate * (Signal[i] - Weight[i]) * Gradient;
		}
		
		Bias_Weight = Bias_Weight + Learning_Rate * (1.0 * Bias_Weight) * Gradient;
		
		
	}
	
	//Hebb Rule
	void Hebb_Update(double Signal[])
	{
		double Gradient = (1 - pow(Result,2)) * 0.5;
		
		for(int i=0;i<Input_Length;i++)
		{
			Weight[i] = Weight[i] + ( Learning_Rate * Result) * ( Beta_Rate * Signal[i] - Weight[i])  * Gradient;
		}
		
		Bias_Weight = Bias_Weight +  ( Learning_Rate * Result) * ( Beta_Rate * 1.0 - Bias_Weight)  * Gradient;

	}

	
};