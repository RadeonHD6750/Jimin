/*
	여기서는 각종 지원함수가 있다.
	
	난수를 뽑는 것부터 반올림 함수까지
	기술되어 있다.
	
	작성자 서지민

*/

#include <iostream>
#include <ctime>
#include <cstdlib>


using namespace std;

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

//Milli Second 반환
double NowTime()
{
	clock_t nowclock = clock();
	
	double NowSec = (double)(nowclock / 1000);
	double NowMilli = (NowSec / 1000);
	
	return NowMilli;
}

double CPU_Clock_Count()
{
	clock_t nowclock = clock();
	
	return nowclock;
}


