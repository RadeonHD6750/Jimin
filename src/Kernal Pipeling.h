/*
	어느 필터의 파이프라인이다.
	CNN은 필터를 여러개 두고 연산을 하는데 이를 모듈화하기 위한 헤더파일이다.
	
	작성자 서지민
*/

#include <iostream>

#include "Kernal.h"

using namespace std;

class Kernal_Pipeling
{
	private:
	
	int Width,Height;
	int Window_Size;
	int Class_Size;
	int Result_Length;
	
	Kernal *kernal;
	
	public:
	
	/*****************************************************
					Kernal 파이프라이닝 생성하기
	****************************************************/
	Kernal_Pipeling()
	{
		
	}
	
	~Kernal_Pipeling()
	{
		delete []kernal;
	}
	
	void Build(int Width,int Height,int Window_Size,int Class_Size)
	{
		this->Width = Width;
		this->Height = Height;
		this->Window_Size = Window_Size;
		this->Class_Size = Class_Size;
		
		kernal = new Kernal[Class_Size];
		
		kernal[0].Build(Width,Height,Window_Size);
		
		int Result_w = kernal[0].Result_Width;
		int Result_h = kernal[0].Result_Height;
		
		for(int i = 1; i<Class_Size; i++)
		{
			kernal[i].Build(Result_w,Result_h,Window_Size);
			
			Result_w = kernal[i].Result_Width;
			Result_h = kernal[i].Result_Height;
		}
	}
	
	void Init()
	{
		for(int i=0;i<Class_Size;i++)
		{
			kernal[i].Init();
		}
	}
	
	/*****************************************************
					Kernal 파이프라이닝 동작
	****************************************************/
	
	Dynamic_Matrix Propagate(Dynamic_Matrix Signal)
	{
		
		Dynamic_Matrix Temp = Signal;
	
		Dynamic_Matrix Temp2;
			
		for(int i = 0; i<Class_Size;i++)
		{
			
			Temp2 = kernal[i].Propagate(Temp);
			
			Temp = Temp2;
		}
		
		return Temp2;
	}
	
	/*****************************************************
					Kernal 파이프라이닝 학습하기
	****************************************************/
	
	void Hebb_Update(Dynamic_Matrix Signal)
	{
		
		Dynamic_Matrix Temp = Signal;
	
		Dynamic_Matrix Temp2;
		
		for(int i = 0; i<Class_Size;i++)
		{
			Kernal[i].Hebb_Update(Temp);
			
			Temp2 = kernal[i].Propagate(Temp);
			
			Temp = Temp2;
		}
	}
	
};