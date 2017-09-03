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

using namespace std;

class Kernal
{
	private:
	
	int Width,Height;
	int Window_Size;
	
	Dynamic_Matrix Filter;
	
	public:
	
	/*************************************************
					Kenal 생성하기
	************************************************/
	
	Kernal()
	{
		
	}
	
	~Kernal()
	{
		
	}
	
	void Build(int Width,int Height,int Window_Size)
	{
		this->Width = Width;
		this->Height = Height;
		this->Window_Size = Window_Size;
		
		Filter.Build(Window_Size);
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
	
	/*************************************************
					Convolute
	************************************************/
	double Convolute_Function(double Raw_Data[][], int r, int c)
	{
		double X = 0;
		for(int i=0;i<Window_Size;i++)
		{
			for(int j=0;j<Window_Size;j++)
			{
				X = X + (Raw_Data[r + i][c + j] * Filter[i][j]);
			}
		}
		
		return X;
	}
	
	double** Feature_Map_Function(double Raw_Data[][])
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
				Padding_Data.Set_Value(i + Delta_h, j + Delta_w, Raw_Data[i][j]);
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
				X = Convolute_Function(Raw_Data, i, j)
				Padding_Data.Set_Value(i, j, X);
			}
		}
		
		return Feature_Map.Get_Matrix();
	}
	
	/*************************************************
					Pooling
	************************************************/
	
	double Pooling_Function(double Raw_Data[][], int r, int c)
	{
		
	}
	
	double** Sampling_Function(double Raw_Data[][], int i, int j)
	{
		
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

/*
	동적 2차원 배열
	
	작성자 서지민
*/
class Dynamic_Matrix
{
	public:
	
	int Width,Height;
	int Window_Size;
	
	double **Mat;
	
	Dynamic_Matrix()
	{
		
	}
	
	~Dynamic_Matrix()
	{
		for(int i=0;i<Height;i++)
		{
			delete [] Mat[i];
		}
		delete []Mat;
	}
	
	void Build(Window_Size)
	{
		this->Width = Window_Size;
		this->Height = Window_Size;
		this->Window_Size = Window_Size;
		
		Mat = new double *[Window_Size];
		
		for(int i=0;i<Window_Size;i++)
		{
			Mat[i] = new double[Window_Size];
		}
		
		for(int i=0;i<Window_Size;i++)
		{
			for(int j=0;j<Window_Size;j++)
			{
				Mat[i][j] = 0;
			}
		}
	}
	
	void Build(int Width,int Height)
	{
		this->Width = Window_Size;
		this->Height = Window_Size;
		
		Mat = new double *[Height];
		
		for(int i=0;i<Height;i++)
		{
			Mat[i] = new double[Width];
		}
		
		for(int i=0;i<Height;i++)
		{
			for(int j=0;j<Width;j++)
			{
				Mat[i][j] = 0;
			}
		}
	}
	
	void Set_Value(int i,int j, double Data)
	{
		Mat[i][j] = Data;
	}
	
	double Get_Value(int i,int j)
	{
		return Mat[i][j];
	}
	
	double** Get_Matrix()
	{
		return Mat;
	}
};