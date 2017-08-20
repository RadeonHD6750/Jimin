/*
	LSTM Cell 정의
	
	Cell State를 시간별로 저장해야 한다.
	
	그리고 다시 되불러야한다.
	
	각 게이트별로 편미분 함수 구현
	오차신호를 이용한 가중치 수정
	
	Hebb 학습규칙
	
	작성자 서지민
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

class LSTM_Cell
{
	
	private:
	
	double Learning_Rate;
	double Beta_Rate;
	double Cell_State;
	double Cell_State_Save;
	
	double *Forgot_Weight;
	double *Input_Weight;
	double *Cell_Delta_Weight;
	double *Output_Weight;
	
	double Forgot_Bias;
	double Input_Bias;
	double Cell_Delta_Bias;
	double Output_Bias;

	public:

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
		delete []Forgot_Weight;
		delete []Input_Weight;
		delete []Cell_Delta_Weight;
		delete []Output_Weight;
	}
	
	void Build(int Input_Length,double Learning_Rate,double Beta_Rate)
	{
		
		this->Input_Length = Input_Length;
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
		
		Forgot_Weight = new double[Input_Length+1];
		Input_Weight = new double[Input_Length+1];
		Cell_Delta_Weight = new double[Input_Length+1];
		Output_Weight = new double[Input_Length+1];
		
		Init();
	}
	
	void Set_Value(double Learning_Rate,double Beta_Rate)
	{
		this->Learning_Rate = Learning_Rate;
		this->Beta_Rate = Beta_Rate;
	}
	
	void Init()
	{
		Init(Forgot_Weight);
		Init(Input_Weight);
		Init(Cell_Delta_Weight);
		Init(Output_Weight);
		
		Cell_State= 0;
		Active = 0;
	}
	
	void Init(double Weight[])
	{
		for(int i=0;i<Input_Length+1;i++)
		{
			Weight[i] = RandomRange(-1.0,1.0);
		}
		
		Forgot_Bias = RandomRange(-1.0,1.0);
		Input_Bias = RandomRange(-1.0,1.0);
		Cell_Delta_Bias = RandomRange(-1.0,1.0);
		Output_Bias = RandomRange(-1.0,1.0);
	}
	
	void Propagate_Test()
	{
		double *Test_String = new double[Input_Length];
		
		cout << " LSTM Propagate Test \n Test String: ";
		for(int i=0;i<Input_Length;i++)
		{
			Test_String[i] = RandomRange(-1.0,1.0);
			cout << Test_String[i] << "  ";
		}
		cout << endl;
		
		double Result = Propagate(Test_String);
		
		cout << "Result : " << Result << endl<< endl<< endl<< endl;
		
	}
	
	/**********************************************
				신경망 각종 게이트 제어
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
		
		double X = 0;
		
		for(int i=0;i<Input_Length+1;i++)
		{
			X = X + (Forgot_Weight[i] * Signal_Ex[i]);
		}
		
		X = X + (Forgot_Bias * 1.0);
		
		X = Sigmoid(X);
		
		delete []Signal_Ex;
		
		return X;
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
		
		double X = 0;
		
		for(int i=0;i<Input_Length+1;i++)
		{
			X = X + (Input_Weight[i] * Signal_Ex[i]);
		}
		
		X = X + (Input_Bias * 1.0);
		
		X = Sigmoid(X);
		
		delete []Signal_Ex;
		
		return X;
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
		
		double X = 0;
		
		for(int i=0;i<Input_Length+1;i++)
		{
			X = X + (Cell_Delta_Weight[i] * Signal_Ex[i]);
		}
		
		X = X + (Cell_Delta_Bias * 1.0);
		
		X = Tanh(X);
		
		delete []Signal_Ex;
		
		return X;
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
		
		double X = 0;
		
		for(int i=0;i<Input_Length+1;i++)
		{
			X = X + (Output_Weight[i] * Signal_Ex[i]);
		}
		
		X = X + (Output_Bias * 1.0);
		
		X = Sigmoid(X);
		
		delete []Signal_Ex;
		
		return X;
	}
	
	//세포상태
	double Cell_State_Function(double Signal[])
	{
		double Forgot_Value = Forgot_Function(Signal);
		double Input_Value = Input_Function(Signal);
		double Cell_Delta = Cell_Delta_Function(Signal);
		
		//아직 갱신이 안되었음으로 Cell_State는 과거 입력이다.
		double Cell_State = (Forgot_Value * this->Cell_State) + (Input_Value * Cell_Delta);
		
		this->Cell_State = Cell_State; //이제 여기서 갱신한다.
			
		return Cell_State;
	}
	
	/**********************************************
					신경망 활성함수
	**********************************************/
	
	
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
					신경망 출력
	**********************************************/
	
	//최종출력
	double Propagate(double Signal[])
	{
		double Out = Output_Function(Signal);
		
		double Cell_State = Cell_State_Function(Signal);
		
		double X = Out * Tanh(Cell_State);
		
		Active = X; 
		
		return X;
	}
	
	/**********************************************
				신경망 오차 미분
	**********************************************/
	
	//오차
	double Result_Error(double Target, double Result)
	{
		return Target - Result;
	}
	
	//최초 오차계산
	double Result_Error_Delta(double Error,double Result_Delta)
	{
		return Error + Result_Delta;
	}
	
	//상태 오차 함수
	double State_Error_Delta(double Result_Error_Delta, double Result, double Cell_State, double Next_Cell_State,double Forgot_Value)
	{
		//Cell_State SE= RE * 출력값 * {(1 - tanh(상태값)^2) + t+1오차신호} * t+1Forgot_Value;
		return Result_Error_Delta * Result * (  (1 - Tanh(Cell_State)) + Next_Cell_State ) * Forgot_Value; 
	}
	
	//입력활성화 오차함수
	double Cell_Delta_Error_Delta(double State_Error_Delta,double Cell_Delta)
	{
		//Cell_Delta CDE= CSE * Cell_Delta * (1 - pow(Cell_Delta,2));
		return State_Error_Delta * Cell_Delta * (1 - pow(Cell_Delta,2));
	}
	
	//입력게이트 오차함수
	double Input_Gate_Delta(double State_Error_Delta, double Cell_Delta, double Input_Value)
	{
		//Input IE = CSE * Input_Value * (1 - Input_Value);
		return State_Error_Delta * Cell_Delta * Input_Value * (1 - Input_Value);
	}
	
	//망각게이트 오차함수
	double Forgot_Gate_Delta(double State_Error_Delta,double Was_Cell_State,double Forgot_Value)
	{
		//Forgot FE = CSE * t-1Cell_State * Forgot_Value * (1 - Forgot_Valu
		return State_Error_Delta * Was_Cell_State * Forgot_Value * (1 - Forgot_Value);
	}
	
	//출력게이트 오차함수
	double Output_Gate_Delta(double Result_Delta_Error, double Cell_State,double Output_Value)
	{
		//Output OP = RE * tanh(Cell_State) * Output_Value * (1 - Output_Value);
		return Result_Delta_Error * Tanh(Cell_State) * Output_Value * (1  - Output_Value);
	}
	
	
	/**********************************************
					신경망 학습함수
	**********************************************/
	
	//오류 역전파 이제 이것으로 사용하면 됨
	void BP_Update(double Signal[],double Error)
	{
		//각종 게이트
		double Forgot_Value = Forgot_Function(Signal);
		double Input_Value = Input_Function(Signal);
		double Output_Value = Output_Function(Signal);
		
		//입력활성값
		double Cell_Delta = Cell_Delta_Function(Signal);
		
		double Cell_State = Cell_State_Function(Signal);
		
		double Result = Propagate(Signal);
		
		//각종 오차신호
		double State_Error = State_Error_Delta(Error, Result, Cell_State, Cell_State_Save,Forgot_Value);
		double Cell_Delta_Error = Cell_Delta_Error_Delta(State_Error,Cell_Delta);
		double Forgot_Error = Forgot_Gate_Delta(State_Error,this->Cell_State,Forgot_Value);
		double Input_Error = Input_Gate_Delta(State_Error, Cell_Delta, Input_Value);
		double Output_Error = Output_Gate_Delta(Error, Cell_State,Output_Value);
		
		
		//실제로 갱신
		BP_Update(Signal,Cell_Delta_Weight,Cell_Delta_Error);
		BP_Update(Signal,Forgot_Weight,Forgot_Error);
		BP_Update(Signal,Input_Weight,Input_Error);
		BP_Update(Signal,Output_Weight,Output_Error);
	}
	
	//한 게이트 역전파 갱신
	void BP_Update(double Signal[],double Weight[],double Error)
	{
		double *Signal_Ex = new double[Input_Length+1];
		
		for(int i=0;i<Input_Length;i++)
		{
			Signal_Ex[i] = Signal[i];
		}
		Signal_Ex[Input_Length] = Active;
		
		for(int i=0;i<Input_Length+1;i++)
		{
			Weight[i] = Weight[i] + Learning_Rate * (Error * Signal_Ex[i]);
		}
		
		delete []Signal_Ex;
		
	}
	
	//가중치 갱신하기
	void Hebb_Update(double Signal[])
	{
		double Forgot = Forgot_Function(Signal);
		double Input = Input_Function(Signal);
		double Cell_Delta = Cell_Delta_Function(Signal);
		double Out = Output_Function(Signal);
		
		Hebb_Update(Signal,Forgot,Forgot_Weight,Forgot_Bias);
		Hebb_Update(Signal,Input,Input_Weight,Input_Bias);
		Hebb_Update(Signal,Cell_Delta,Cell_Delta_Weight,Cell_Delta_Bias);
		Hebb_Update(Signal,Out,Output_Weight,Output_Bias);
		
	}
	
	//Hebb 학습규칙
	void Hebb_Update(double Signal[],double Result,double Weight[],double Bias_Weight)
	{
		double Gradient = (1 - pow(Result,2)) * 0.5;
		
		for(int i=0;i<Input_Length;i++)
		{
			Weight[i] = Weight[i] + ( Learning_Rate * Result) * ( Beta_Rate * Signal[i] - Weight[i])  * Gradient;
		}
		
		Bias_Weight = Bias_Weight +  ( Learning_Rate * Result) * ( Beta_Rate * 1.0 - Bias_Weight)  * Gradient;
	}
	
	/**********************************************
					기타 지원함수
	**********************************************/
	
	double RandomRange(double min,double MAX)
	{
		double f = (double)rand() / RAND_MAX;
	
    	return min + f * (MAX - min);
	}

	int RandomRange(int n1, int n2) 
	{ 
   		return (rand() % (n2 - n1 + 1)) + n1; 
	} 

	double Round( double value, int pos )
	{
		 double temp;
		 temp = value * pow( 10, pos );  // 원하는 소수점 자리수만큼 10의 누승을 함
		 temp = floor( temp + 0.5 );          // 0.5를 더한후 버림하면 반올림이 됨
		 temp *= pow( 10, -pos );           // 다시 원래 소수점 자리수로
		 return temp;
	}
};