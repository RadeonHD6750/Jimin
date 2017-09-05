/*
	필터부분을 구현한 파일이다.
	이름은 Kernal으로 한다.
	
	완전연결신경망으로 치자면 최소단위인 Neuron과 동일하다.
	작성자 서지민
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "Dynamic Matrix.h"

using namespace std;

class Kernal
{
	private:
	
	int Width,Height;
	int Window_Size;
	
		
	int Padding_w; 
	int Padding_h; 
		
	int Delta_w; 
	int Delta_h;
	
	double Alpha,Beta;
	double Bias_Weight;
	
	Dynamic_Matrix Filter;
	
	public:
	
	int Result_Width;
	int Result_Height;
	/*************************************************
					Kenal 생성하기
	************************************************/
	
	Kernal()
	{
		Alpha = 0.25;
		Beta = 0.15;
		Bias_Weight = RandomRange(-1.0, 1.0);
	}
	
	~Kernal()
	{
		
	}
	
	void Build(int Width,int Height,int Window_Size)
	{
		this->Width = Width;
		this->Height = Height;
		this->Window_Size = Window_Size;
		
		Result_Width = (Width - Window_Size) + 1;
		Result_Height = (Height - Window_Size) + 1;
		
		Padding_w = Width + Window_Size - 1;
		Padding_h = Height + Window_Size - 1;
		
		Delta_w = Padding_w - Width;
		Delta_h = Padding_h - Height;
		
		this->Filter.Build(Window_Size);
		Init();
	}
	
	void Init()
	{
		for(int i=0;i<Height;i++)
		{
			for(int j=0;j<Width;j++)
			{
				Filter.Set_Value(i,j,  RandomRange(-1.0,1.0)  ); 
			}
		}
	}
	
	void Set_Filter(int i, int j, double Data)
	{
		Filter.Set_Value(i,j, Data  ); 
	}
	
	/*************************************************
					Convolute
	************************************************/
	
	//다차원 배열 함수인자
	double Convolute_Function(Dynamic_Matrix Raw_Data, int r, int c)
	{
		
		double X = 0;
		double Temp = 0;
		
		for(int i=0;i<Window_Size;i++)
		{
			for(int j=0;j<Window_Size;j++)
			{
				Temp = Raw_Data.Get_Value(r + i,c + j);
				X = X + (Temp * Filter.Get_Value(i,j));
			}
		}
		
		X = X + ( 1.0 * Bias_Weight);
		
		return X;
	}
	
	Dynamic_Matrix Feature_Map_Function(Dynamic_Matrix Raw_Data)
	{
		
		
		/*
		 원본손실을 방지하기 위한 채우기 연산
		*/
		Dynamic_Matrix Padding_Data;
		
		int Padding_w = Width + Window_Size - 1;
		int Padding_h = Height + Window_Size - 1;
		
		int Delta_w = Padding_w - Width;
		int Delta_h = Padding_h - Height;
		
		Padding_Data.Build(Padding_w, Padding_h);
		
		
		for(int i=0;i<Height;i++)
		{
			for(int j=0;j<Width;j++)
			{
				Padding_Data.Set_Value(i + Delta_h, j + Delta_w, Raw_Data.Get_Value(i,j));
			}
		}
		
		///////////////////////////////////////////////////////
		
		
		//특징지도 만들기
		Dynamic_Matrix Feature_Map;
		
		Padding_Data.Build(Width, Height);
		
		//이제 만들어진 채우기 데이터로 진짜로 합성곱 연산하기
		
		int X = 0;
		
		for(int i=Delta_h;i<Height;i++)
		{
			for(int j=Delta_w;j<Width;j++)
			{
				X = Convolute_Function(Raw_Data, i, j);
				Padding_Data.Set_Value(i, j, X);
			}
		}
		
		return Feature_Map;
	}
	
	/*************************************************
					Pooling
	************************************************/
	
	//여기는 굳이 원본을 지킬 필요가 없다. 당초 원본을 줄일려고 하는 것이니까 말이다.
	
	//MAX Pooling
	double Pooling_Function(Dynamic_Matrix Raw_Data, int r, int c)
	{
		vector<double> Data_List;
		
		for(int i=0;i<Window_Size;i++)
		{
			for(int j=0;j<Window_Size;j++)
			{
				Data_List.push_back(Raw_Data.Get_Value(r + i,c + j));
			}
		}
		
		double MAX = abs(Data_List[0]);
		int Next =0;
		
		for(int i=0;i<Data_List.size();i++)
		{
			Next = abs(Data_List[i]);
			
			if( MAX < Next)
			{
				MAX = Next;
			}
		}
		
		return MAX;
	}
	
	Dynamic_Matrix Sampling_Function(Dynamic_Matrix Raw_Data)
	{
		Dynamic_Matrix Sampled_Data;
		
		Sampled_Data.Build(Result_Width,Result_Height);
		
		int X = 0;
		
		for(int i=0;i<Result_Height;i++)
		{
			for(int j=0; j<Result_Width; j++)
			{
				X = Pooling_Function(Raw_Data, i, j);
				
				Sampled_Data.Set_Value(i,j,X);
			}
		}
		
		return Sampled_Data;
	}
	
	
	/*************************************************
					최종 전파함수
	************************************************/
	Dynamic_Matrix Propagate(Dynamic_Matrix Raw_Data)
	{
		Dynamic_Matrix Feature_Map = Feature_Map_Function(Raw_Data);
		
		Dynamic_Matrix Sampled_Data = Sampling_Function(Feature_Map);
		
		return Sampled_Data;
	}
	
	//단일값 출력 1*1 필터
	double Single_Propagate(Dynamic_Matrix Raw_Data)
	{
		Dynamic_Matrix Feature_Map = Feature_Map_Function(Raw_Data);
		
		Dynamic_Matrix Sampled_Data = Sampling_Function(Feature_Map);
		
		vector<double> Data_List;
		
		for(int i=0;i<Result_Height;i++)
		{
			for(int j=0;j<Result_Width;j++)
			{
				Data_List.push_back(Sampled_Data.Get_Value(i,j));
			}
		}
		
		double MAX = abs(Data_List[0]);
		int Next =0;
		
		for(int i=0;i<Data_List.size();i++)
		{
			Next = abs(Data_List[i]);
			
			if( MAX < Next)
			{
				MAX = Next;
			}
		}
		
		return MAX;
	}
	
	/*************************************************
					여과기 학습함수
	************************************************/
	
	//일단 Hebb 학습규칙을 사용해 본다.
	void Hebb_Update(Dynamic_Matrix Signal)
	{
		double Gradient =0;
		double Result = 0;
		double Delta =0;

		for(int i=0;i<Window_Size;i++)
		{
	
			
			for(int j=0;j<Window_Size;j++)
			{
				Delta = Filter.Get_Value(i,j);
				Result = Convolute_Function(Signal,i,j);
				Gradient = (1 - pow(Result,2)) * 0.5;
				
				Delta = Delta + (Alpha * Result) * (Beta *  (Signal.Get_Value(i,j) - Filter.Get_Value(i,j)) ) *Gradient;
				Filter.Set_Value(i,j, Delta);
			}
			
		}

		Bias_Weight = Bias_Weight +  (Alpha * Result) * (Beta *  (1 - Bias_Weight) ) *Gradient;
		
	}
	
	
	
	/*************************************************
					각종 지원함수들
	************************************************/
	
	double RandomRange(double min,double MAX)
	{
		double float_Value = (double)rand() / RAND_MAX;

		return min + float_Value * (MAX - min);
	}

	int RandomRange(int min, int MAX) 
	{ 
			 return (rand() % (MAX - min + 1)) + min; 
	} 

	double Round( double value, int pos )
	{
		double temp;

		temp = value * pow( 10, pos );  
		temp = floor( temp + 0.5 );          
		temp = temp * pow( 10, -pos );   

		return temp;
	}
};




