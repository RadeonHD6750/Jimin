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
	double f = (double)rand() / RAND_MAX;
	
    return min + f * (MAX - min);
}

static int RandomRange(int n1, int n2) 
{ 
   return (rand() % (n2 - n1 + 1)) + n1; 
} 

static double Round( double value, int pos )
{
      double temp;
      temp = value * pow( 10, pos );  // 원하는 소수점 자리수만큼 10의 누승을 함
      temp = floor( temp + 0.5 );          // 0.5를 더한후 버림하면 반올림이 됨
      temp *= pow( 10, -pos );           // 다시 원래 소수점 자리수로
      return temp;
}

