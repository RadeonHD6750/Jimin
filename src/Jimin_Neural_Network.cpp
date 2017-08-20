/*
	서지민의 인공신경망 라이브러리
	
	Jimin_ANN.h를 포함
	
	여긴 그냥 메인함수일뿐이다.
	
	작성자 서지민

*/

#include <iostream>

#include "Jimin_AI_API.h"

using namespace std;

int main()
{
	cout << "Start to main \n\n";
	
	Jimin_AI_API AI;
	
	AI.AI_Learning_Testing();
	
	cout << "return to main \n\n";
	
	return 0;
}