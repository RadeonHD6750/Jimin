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
	
	void Build(int Width,int Height)
	{
		this->Width = Width;
		this->Height = Height;
		
		Filter.Build(Width,Height);
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
	double Convolute_Function(double Raw_Data[][], int i, int j)
	{
		
	}
	
	/*************************************************
					Pooling
	************************************************/
	
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
	
	void Build(int Width,int Height)
	{
		this->Width = Width;
		this->Height = Height;
		
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
};