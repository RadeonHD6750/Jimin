/*
	CNN 라이브러리이다.
	
	작성자 서지민
*/

#include <iostream>
#include <vector>

#include "Kernal Pipeling.h"

/*
	CNN에서도 완전연결이 필요함으로 여기서부터 포함시킨다.
	
	그리고 하위 계층에서는 이 파일만 포함시켜도 자동으로 완전연결도 포함된다.
*/
#include "Full_Connected.h"

class Convolutional_NN
{
	private:
	
	int Width,Height;
	int Result_Width,Result_Height;
	int Result_Length;
	
	int Window_Size;
	int Class_Size;
	int Kernal_Size;
	
	int CNN_Result_Length;
	int Kernal_Result_Length;
	
	public:
	
	Kernal_Pipeling *kp;
	
	Full_Connected full_connected; //완전연결 신경망
	
	/*********************************************************************
					CNN 생성하기
	********************************************************************/
	
	Convolutional_NN()
	{
		
	}
	
	~Convolutional_NN()
	{
		delete []kp;
	}
	
	//완전연결신경망 종단연결
	void Build(int Width,int Height,int Window_Size,int Class_Size,int Kernal_Size,int Result_Length)
	{
		this->Width = Width;
		this->Height = Height;
		this->Window_Size = Window_Size;
		this->Class_Size = Class_Size;
		this->Kernal_Size = Kernal_Size;
		this->Result_Length = Result_Length;
		
		kp = new Kernal_Pipeling[Kernal_Size];
		
		for(int i=0;i<Kernal_Size;i++)
		{
			kp[i].Build(Width,Height,Window_Size,Class_Size);
		}
		
		//커널 마지막 길이와 커널 총 개수를 곱한 결과 
		int CNN_Result_Length = kp[Kernal_Size-1].Result_Length * Kernal_Size; //이것이 완전연결 신경망 입력길이
		int Kernal_Result_Length = kp[Kernal_Size-1].Result_Length;
		
		this->CNN_Result_Length = CNN_Result_Length;
		
		int Layer_Length[6]; 
		Layer_Length[0] = CNN_Result_Length;
		
		int Length =0;
		
		for(int i=1;i<6;i++)
		{
			Layer_Length[i] = (Layer_Length[i-1] / 2);
		}
		
		Layer_Length[5] = Result_Length;
		
		full_connected.Build(0, 5, Layer_Length,0.25, 0.15);
	}
	
	//무연결
	void Build(int Width,int Height,int Window_Size,int Class_Size,int Kernal_Size,int Result_Width, int Result_Height)
	{
		this->Width = Width;
		this->Height = Height;
		this->Window_Size = Window_Size;
		this->Class_Size = Class_Size;
		this->Kernal_Size = Kernal_Size;
		this->Result_Width = Result_Width;
		this->Result_Height = Result_Height;
		this->Result_Length = Result_Width * Result_Height;
		
		
		kp = new Kernal_Pipeling[Kernal_Size];
		
		for(int i=0;i<Kernal_Size;i++)
		{
			kp[i].Build(Width,Height,Window_Size,Class_Size);
		}
	}
	
	void Init()
	{
		for(int i=0;i<Kernal_Size;i++)
		{
			kp[i].Init();
		}
		
		full_connected.Init();
	}
	
	/*********************************************************************
					CNN 전파함수
	********************************************************************/
	
	
	//커널부분 전파함수
	
	double* Kernal_Pipeling_Propagate(Dynamic_Matrix Signal)
	{
		
		vector<double*> Kernal_Result_List;
		
		for(int i=0;i<Kernal_Size;i++)
		{
			Kernal_Result_List.push_back(kp[i].Propagate(Signal));
		}
		
		double *Result = new double[CNN_Result_Length];
		
		int Count =0;
		
		for(int i=0;i<Kernal_Size;i++)
		{
			for(int j=0;j<Kernal_Result_Length;j++)
			{
				Result[Count] = Kernal_Result_List[i][j];
				Count++;
			}
		}
		
		return Result;
	}
	/*
	//무연결 커널 전파함수
	Dynamic_Matrix Kernal_Pipeling_Propagate(Dynamic_Matrix Signal)
	{
		
	}*/
	
};