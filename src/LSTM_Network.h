/*
	LSTM 네트워크 정의
	
	작성자 서지민
*/
	
#include <iostream>

#include "LSTM_Layer.h"

using namespace std;

class LSTM_Network
{
	
	private:
	
	/**********************************************
					신경망 학습요소
	**********************************************/

	double Learning_Rate;
	double Beta_Rate;
	int BP_Start;
	
	public:
	
	/**********************************************
					신경망 구조
	**********************************************/
	
	LSTM_Layer *Layer;
	
	int Class_Length;
	int *Layer_Length;
	int Input_Length;
	int Result_Length;
	
	/**********************************************
					신경망 구축하기
	**********************************************/
	
	LSTM_Network()
	{
		
	}
	
	~LSTM_Network()
	{
		
	}
	
	void Build(int Class_Length,int Layer_Length[],double Learning_Rate,double Beta_Rate)
	{
		this->Class_Length = Class_Length;
		this->Layer_Length = Layer_Length;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		Input_Length = Layer_Length[0];
		Result_Length = Layer_Length[Class_Length];
		
		Layer = new LSTM_Layer[Class_Length];
	
		int Length;
		int Input_Length;
		
		for(int i=0;i<Class_Length;i++)
		{
			Input_Length = Layer_Length[i];
			Length = Layer_Length[i+1];
			
			Layer[i].Build(Length,Input_Length,Learning_Rate,Beta_Rate);
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
	
	void Set_Value(double Learning_Rate,double Beta_Rate)
	{
		
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		for(int i=0;i<Class_Length;i++)
		{	
			Layer[i].Set_Value(Learning_Rate,Beta_Rate);
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
	
	/**********************************************
					신경망 학습식
	**********************************************/
	
	//기존 오류역전파
	void BP_Update(double Signal[],double Error_Array[])
	{
		//하위 호환을 위하여 만들어둔 함수
		Hebb_BP_Update(Signal,Error_Array);
		
		/*
			출력층만 오류역전파를 적용하고 나머지 깊은층에 대하여는
		Hebb 학습규칙을 사용한다. 어차피 깊은층까지 오차가 전파되기
		힘들다.
		*/
	}

	
	//LSTM Hebb Rule
	void Hebb_BP_Update(double Signal[],double Error_Array[])
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
		
		Layer[Class_Length-1].BP_Update(Temp,Error_Array); //차후에 오류역전파 코딩하기 -> 코딩완료

	}
	
	/**********************************************
					신경망 가중치 I/O 
	**********************************************/

	/*
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
	*/
	
};