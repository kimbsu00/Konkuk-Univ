#pragma once
#include <iostream>
#include <iomanip>
#include <string>

#include "Register.h"

#define REGISTER_SIZE 32

using namespace std;

enum { FLAG_C = 0, FLAG_V, FLAG_S, FLAG_Z };

class Register
{
public:
	bool bit[REGISTER_SIZE];
	bool flag[4]; //C, V, S, Z순
public:
	Register();
	~Register();

	void showBit(bool flag=false) const;	 		 //현재 레지스터에 저장된 값을 출력한다. flag가 true인 경우 flag도 출력한다.
	void setNum(int num);					//십진수의 값을 이진수변환하여 레지스터에 저장한다.
	int getNum(void) const;					//레지스터에 저장된 값을 반환한다.

	bool shiftLeft(void);					//Register에서   왼쪽으로 쉬프트연산을 수행한다.
	bool shiftRight(void);					//Register에서 오른쪽으로 쉬프트연산을 수행한다.

	static Register complement(Register a);			//a의 2의보수를 저장한 레지스터를 반환한다.
	static Register Adder_Subtracter(const Register & a, const Register & b, const bool & isSub);
	// ^ isSub가 true일 때, a-b, false일 때 a+b를 한 결과를 반환한다.
	static long long multiplication(const Register & m, Register q);		//곱셈을 수행한다.
	static void division(Register q, const Register & m);				//나눗셉을 수행한다.
};
