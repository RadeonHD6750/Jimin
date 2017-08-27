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

static double RandomRange(double min,double MAX)
{
	double float_Value = (double)rand() / RAND_MAX;

	return min + float_Value * (MAX - min);
}

static int RandomRange(int min, int MAX) 
{ 
	  return (rand() % (MAX - min + 1)) + min; 
} 

static double Round( double value, int pos )
{
	double temp;
		
	temp = value * pow( 10, pos );  
	temp = floor( temp + 0.5 );          
	temp = temp * pow( 10, -pos );   
		
	return temp;
}

