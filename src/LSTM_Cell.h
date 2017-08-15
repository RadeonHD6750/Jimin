/*
	LSTM Cell 정의
	
	기존의 Neuron.h 파일을 받는다.
	
	작성자 서지민
*/

#include <iostream>

#include "Neuron.h"

using namespace std;

class LSTM_Cell
{
	
	private:
	
	double Learning_Rate;
	double Beta_Rate;
	double Cell_State;
	

	public:
	
	Neuron Forgot;
	Neuron Input;
	Neuron Cell_Delta;
	Neuron Output;
	
	int Input_Length;
	
	double Active;
	
	/**********************************************
					신경망 생성하기
	**********************************************/
	
	LSTM_Cell()
	{
		
	}
	
	~LSTM_Cell()
	{
		
	}
	
	void Build(int Input_Length,double Learning_Rate,double Beta_Rate)
	{
		Forgot.Build(1,Input_Length,Learning_Rate,Beta_Rate);
		Input.Build(1,Input_Length,Learning_Rate,Beta_Rate);
		Cell_Delta.Build(0,Input_Length,Learning_Rate,Beta_Rate);
		Output.Build(1,Input_Length,Learning_Rate,Beta_Rate);
	}
	
	/**********************************************
					신경망 입출력 제어함수
	**********************************************/
	
	//망각함수
	double Forgot_Function(double Signal[])
	{
		double *Signal_Ex = new double[Input_Length+1];
		
		for(int i=0;i<Input_Length;i++)
		{
			Signal_Ex[i] = Signal[i];
		}
		Signal_Ex[Input_Length] = Active;
		
		
		return Forgot.Propagate(Signal_Ex);
	}
	
	//입력제어 함수
	double Input_Function(double Signal[])
	{
		double *Signal_Ex = new double[Input_Length+1];
		
		for(int i=0;i<Input_Length;i++)
		{
			Signal_Ex[i] = Signal[i];
		}
		Signal_Ex[Input_Length] = Active;
		
		return Input.Propagate(Signal_Ex);
	}
	
	//상태변화 함수
	double Cell_Delta_Function(double Signal[])
	{
		double *Signal_Ex = new double[Input_Length+1];
		
		for(int i=0;i<Input_Length;i++)
		{
			Signal_Ex[i] = Signal[i];
		}
		Signal_Ex[Input_Length] = Active;
		
		return Cell_Delta.Propagate(Signal_Ex);
	}
	
	//출력제어 함수
	double Output_Function(double Signal[])
	{
		double *Signal_Ex = new double[Input_Length+1];
		
		for(int i=0;i<Input_Length;i++)
		{
			Signal_Ex[i] = Signal[i];
		}
		Signal_Ex[Input_Length] = Active;
		
		return Output.Propagate(Signal_Ex);
	}
	
	//세포상태
	double Cell_State_Function(double Signal[])
	{
		double Forgot = Forgot_Function(Signal);
		double Input = Input_Function(Signal);
		double Delta = Cell_Delta_Function(Signal);
		
		//아직 갱신이 안되었음으로 Cell_State는 과거 입력이다.
		double Cell_State = (Forgot * this->Cell_State) + (Input * Delta);
		
		this->Cell_Delta = Cell_Delta; //이제 여기서 갱신한다.
			
		return Cell_State;
	}
	
	/**********************************************
					신경망 출력
	**********************************************/
	
	//최종출력
	double Propagate(double Signal[])
	{
		double Out = Output_Function(Signal);
		
		double Cell_State = Cell_State_Function(Signal);
		
		double X = Out * tanh(Cell_State);
		
		Active = X; 
		
		return X;
	}
	
	/**********************************************
					신경망 학습함수
	**********************************************/
	
	
	//가중치 갱신하기
	void Cell_Update(double Signal[])
	{
		Forgot.Hebb_Update(Signal);
		Input.Hebb_Update(Signal);
		Cell_Delta.Hebb_Update(Signal);
		Output.Hebb_Update(Signal);
	}
};