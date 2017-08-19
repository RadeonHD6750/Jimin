/*
	뉴런들을 모아 하나의 층을 만들어 동작한다.
	
	LSTM 추가
	
	작성자 서지민
*/

#include <iostream>

#include "LSTM_Cell.h"

using namespace std;

class LSTM_Layer
{
	
	private:
	
	/**********************************************
					신경망 학습요소
	**********************************************/
	
	double Learning_Rate;
	double Beta_Rate;
	
	public:
	
	/**********************************************
					신경망 구조
	**********************************************/
	
	LSTM_Cell *lstm_layer;
	
	int Layer_Length;
	int Input_Length;
	
	LSTM_Layer()
	{
		
	}
	
	~LSTM_Layer()
	{
		delete[] lstm_layer;
	}
	
	//신경망 구축하기
	void Build(int Layer_Length, int Input_Length,double Learning_Rate, double Beta_Rate)
	{
		this->Layer_Length = Layer_Length;
		this->Input_Length = Input_Length;
		
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		//lSTM Build
		lstm_layer = new LSTM_Cell[Layer_Length];
		
		for(int i=0;i<Layer_Length;i++)
		{
			lstm_layer[i].Build(Input_Length,Learning_Rate,Beta_Rate);
		}
		
		
	}
	
	void Set_Value(double Learning_Rate,double Beta_Rate)
	{
		
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		for(int i=0;i<Layer_Length;i++)
		{	
			
			lstm_layer[i].Set_Value(Learning_Rate,Beta_Rate);
		}
	}
	
	//초기화
	void Init()
	{
		for(int i=0;i<Layer_Length;i++)
		{
			lstm_layer[i].Init();
		}
	}
	
	/**********************************************
					신경망 전파함수
	**********************************************/
	
	//LSTM 전파
	double* Propagate(double Signal[])
	{
		double *Result = new double[Layer_Length];
		
		
		for(int i=0;i<Layer_Length;i++)
		{
			Result[i] = lstm_layer[i].Propagate(Signal);
		}
		
		
		return Result;
	}
	
	/**********************************************
					신경망 학습식
	**********************************************/
	
	//LSTM Update
	/*
	void LSTM_Update(double Signal[],double Error_Array[])
	{
		for(int i=0;i<Layer_Length;i++)
		{
			lstm_layer->Cell_Update(Signal,Error_Array[i]);
		}
	}
	
	//LSTM Update
	void LSTM_Update(double Signal[])
	{
		for(int i=0;i<Layer_Length;i++)
		{
			lstm_layer->Cell_Update(Signal);
		}
	}*/
	
	//Hebb 학습규칙
	void Hebb_Update(double Signal[])
	{
		for(int i=0;i<Layer_Length;i++)
		{
			lstm_layer->Hebb_Update(Signal);
		}
	}

};