#include "Register.h"



Register::Register()
{
	for (int i = 0; i < REGISTER_SIZE; i++)
		bit[i] = false;

	for (int i = 0; i < 4; i++)
		flag[i] = false; // false로 초기화
}


Register::~Register()
{
}

void Register::showBit(bool flag) const
{
	for (int i = 0; i < REGISTER_SIZE; i++)
		cout << bit[i] ? 1 : 0;

	if (!flag)
		return;

	cout << " ";
	for (int i = 0; i < 4; i++)
		cout << this->flag[i];
}

void Register::setNum(int num)
{
	bool isMinus = false;
	num < 0 ? num = -num, isMinus = true : isMinus = false;

	for (int i = 0; num; i++)
	{
		if (i == REGISTER_SIZE - 1) break;
		bit[REGISTER_SIZE - 1 - i] = num % 2;
		num /= 2;
	}

	if (isMinus)
		*this = complement(*this);
}

int Register::getNum(void) const
{
	int num = 0;

	for (int i = 0; i < REGISTER_SIZE; i++)
	{
		num <<= 1;
		num |= (int)bit[i];
	}

	return num;
}

bool Register::shiftLeft(void)
{
	bool temp = bit[0];

	for (int i = 0; i < REGISTER_SIZE - 1; i++)
		bit[i] = bit[i + 1];

	bit[REGISTER_SIZE - 1] = 0;

	return temp;
}

bool Register::shiftRight(void)
{
	bool temp = bit[REGISTER_SIZE - 1];

	for (int i = REGISTER_SIZE - 2; i >= 0; i--)
		bit[i + 1] = bit[i];

	return temp;
}

Register Register::complement(Register a)
{
	for (int i = 0; i < REGISTER_SIZE; i++)
		a.bit[i] = a.bit[i] ^ 1;

	int carry = 1;

	for (int i = REGISTER_SIZE - 1; i >= 0; i--)
	{
		int temp = a.bit[i] + carry;
		(temp == 2) ? (a.bit[i] = 0, carry = 1) : (a.bit[i] = temp, carry = 0);
		if (!carry) break;
	}

	return a;
}

Register Register::Adder_Subtracter(const Register & a, const Register & b, const bool &isSub)
{
	Register minuend = a;				// 피감수 및 연산의 결과가 저장되는 벡터
	Register subtrahend = b;			// 감수

	// 덧셈
	if (!isSub)
	{
		int carry = 0;
		bool carry1;		//오버플로 체크를 위한 31번째 루프의 캐리값 저장

		for (int i = REGISTER_SIZE - 1; i >= 0; i--)
		{
			int temp = (int)minuend.bit[i] + subtrahend.bit[i] + carry;
			temp == 3 ? (minuend.bit[i] = 1, carry = 1) : (temp == 2 ? (minuend.bit[i] = 0, carry = 1) : (minuend.bit[i] = temp, carry = 0));

			if (i == 1)
				carry1 = carry;
		}

		// 각 플래그 설정
		if (carry)
			minuend.flag[FLAG_C] = true;
		if ((bool)carry != carry1)
			minuend.flag[FLAG_V] = true;
		if (minuend.bit[0])
			minuend.flag[FLAG_S] = true;
		if (minuend.getNum() == 0)
			minuend.flag[FLAG_Z] = true;

		return minuend;
	}
	// 뺄셈
	else
	{
		// 뺄셈의 경우 감수가 양수이면 보수화한 후에 덧셈을 수행하면 되기 때문에 보수화 후에 재귀호출함
		subtrahend = complement(subtrahend);
		return Adder_Subtracter(minuend, subtrahend, false);
		/*
		보수화 확인용 출력 코드
		for (int i = 0; i < REGISTER_SIZE; i++)
			cout << subtrahend[i] << " ";
		cout << endl;
		*/
	}
}

long long Register::multiplication(const Register &m, Register q)
{
	/*

	Booth 알고리즘 사용 (q-1비트 레지스터는 변수 q1에 저장함)

	*/

	Register a;
	bool q1 = false;

	// 출력 구문
	cout << " m : ";
	m.showBit();
	cout << endl << endl;

	cout << "ind  A                              A CVSZ  Q                              Q  q-1" << endl;
	for (int i = 0; i < 95; i++)
		cout << "-";
	cout << endl;

	cout << " 0 : ";
	a.showBit();
	cout << "       ";
	q.showBit();
	cout << "  ";
	cout << (q1 ? 1 : 0) << endl;

	for (int i = 0; i < REGISTER_SIZE; i++)			//레지스터의 길이만큼 반복
	{
		if (!q.bit[REGISTER_SIZE - 1] && q1)		//Q의 최하위 비트가 0이고 q-1이 1일 때, 덧셈 수행
		{
			a = Adder_Subtracter(a, m, false);

			cout << setw(2) << (i + 1) << " : ";
			a.showBit(true);
			cout << "  ";
			q.showBit();
			cout << "  ";
			cout << (q1 ? 1 : 0);
			cout << "  => A = A + M" << endl;
		}

		else if (q.bit[REGISTER_SIZE - 1] && !q1)	//Q의 최하위 비트가 1이고 q-1이 0일 때, 뺄셈 수행
		{
			a = Adder_Subtracter(a, m, true);

			cout << setw(2) << (i + 1) << " : ";
			a.showBit(true);
			cout << "  ";
			q.showBit();
			cout << "  ";
			cout << (q1 ? 1 : 0);
			cout << "  => A = A - M" << endl;
		}

		q1 = q.shiftRight();		//오른쪽으로 쉬프트(최하위 비트는 q1에 저장)
		q.bit[0] = a.shiftRight();	//오른쪽으로 쉬프트(최하위 비트는 q의 최상위 비트에 저장)

		cout << setw(2) << (i + 1) << " : ";
		a.showBit();
		cout << "       ";
		q.showBit();
		cout << "  ";
		cout << (q1 ? 1 : 0);
		cout << "  => Shift Right" << endl;
	}

	// 곱셈 결과 (A*(2^32) + Q)
	long long result = (((long long)a.getNum()) << REGISTER_SIZE) | (long long)q.getNum();

	cout << endl;
	cout << "S flag : " << ((result < 0) ? 1 : 0) << endl;
	cout << "Z flag : " << ((result == 0) ? 1 : 0) << endl;
	cout << "답 : ";
	a.showBit();
	cout << " ";
	q.showBit();
	cout << "  /  " << result << endl;

	cout << endl;

	return result;
}

void Register::division(Register q, const Register & m)
{
	if (m.getNum() == 0)
	{
		cout << "0으로 나눌 수 없습니다." << endl;
		return;
	}

	Register a;
	int num = q.getNum();

	a.setNum(q.bit[0] ? 0xffffffff : 0);	//q가 음수이면 1111 ... 1111로 설정함

	bool add = m.bit[0] ? !a.bit[0] : a.bit[0];	//m을 더해야 될지, 빼야할지 저장함(부호가 같으면 빼고, 다르면 더함)

	cout << " m : ";
	m.showBit();
	cout << endl << endl;

	cout << "ind  A                              A CVSZ  Q                              Q" << endl;
	for (int i = 0; i < 91; i++)
		cout << "-";
	cout << endl;

	cout << " 0 : ";
	a.showBit();
	cout << "       ";
	q.showBit();
	cout << endl;

	bool preSign = a.bit[0];
	//나눗셈하는 도중 덧셈 혹은 뺄셈을 시도했을 때, 부호가 바뀌는지 확인을 위한 이전 부호 저장

	for (int i = 0; i < REGISTER_SIZE; i++)
	{
		a.shiftLeft();								//Shift Left
		a.bit[REGISTER_SIZE - 1] = q.shiftLeft();	//Shift Left(q의 최상위 비트는 a의 최하위 비트가 됨)

		cout << setw(2) << (i + 1) << " : ";
		a.showBit();
		cout << "       ";
		q.showBit();
		cout << "  => Shift Left" << endl;

		a = Register::Adder_Subtracter(a, m, !add);	//연산을 시도

		cout << setw(2) << (i + 1) << " : ";
		a.showBit(true);
		cout << "  ";
		q.showBit();
		cout << "  => " << (add ? "A = A + M" : "A = A - M") << endl;

		bool checkZero = true;						//피제수가 0인지 확인을 위한 변수

		for (int j = 0; j < REGISTER_SIZE; j++)
			if (a.bit[j])
				checkZero = false;

		for (int j = 0; j < REGISTER_SIZE - i - 1; j++)
			if (q.bit[j])
				checkZero = false;

		//위의 두 for문으로 checkZero에 피제수가 0이면 true값이 저장됨

		if ((a.bit[0] == preSign) || checkZero == true)	//부호가 같거나 또는 부호가 다르더라도 피제수가 0일 경우
		{
			q.bit[REGISTER_SIZE - 1] = true;			//q의 최하위 비트를 1로 설정

			cout << setw(2) << (i + 1) << " : ";
			a.showBit();
			cout << "       ";
			q.showBit();
			cout << "  => Set Q0 = 1" << endl;
		}

		else		//아닐 경우, 다시 되돌림
		{
			a = Register::Adder_Subtracter(a, m, add);

			cout << setw(2) << (i + 1) << " : ";
			a.showBit(true);
			cout << "  ";
			q.showBit();
			cout << "  => " << (add ? "A = A - M" : "A = A + M") << endl;
		}
	}

	if (add)
		q = complement(q);	//q에 보수를 취한 값이 몫임

	cout << endl;

	cout << "몫     : ";
	q.showBit();
	cout << "  /  " << q.getNum() << endl;

	cout << "나머지 : ";
	a.showBit();
	cout << "  /  " << a.getNum() << endl;

	cout << endl;
	cout << "결과 : " << num << " / " << m.getNum() << " = " <<
		q.getNum() << " ... " << a.getNum() << "" << endl;
	cout << endl << endl;
}
