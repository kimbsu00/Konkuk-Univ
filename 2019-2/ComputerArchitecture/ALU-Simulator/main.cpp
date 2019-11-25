#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>

#include "Register.h"

using namespace std;

int main(void)
{
	while (true)
	{
		int a, b;

		cout << "첫번째 숫자 입력 : ";
		cin >> a;

		cout << "두번째 숫자 입력 : ";
		cin >> b;

		Register ra, rb;

		ra.setNum(a);
		rb.setNum(b);

		//이진수 출력

		cout << "---> 입력된 값 <---" << endl;
		cout << endl;

		cout << "첫번째 숫자 : " << ra.getNum() << endl;
		ra.showBit();
		cout << endl << endl;

		cout << "두번째 숫자 : " << rb.getNum() << endl;
		rb.showBit();
		cout << endl << endl << endl;

		_getch();

		//덧셈

		Register result;

		result = Register::Adder_Subtracter(ra, rb, false);

		cout << "-- > 덧셈 < --" << endl;
		cout << endl;

		cout << "  ";
		ra.showBit();
		cout << endl;

		cout << "+ ";
		rb.showBit();
		cout << " CVSZ" << endl;

		for (int i = 0; i < 39; i++)
			cout << "-";
		cout << endl;

		cout << "  ";
		result.showBit(true);
		cout << endl << endl;

		cout << "답 : ";
		result.showBit();
		cout << "  /  " << result.getNum() << endl;
		cout << endl;

		cout << "결과 : " << ra.getNum() << " + " << rb.getNum() << " = " << result.getNum() << endl;
		cout << endl << endl;

		_getch();

		//뺄셈

		result = Register::Adder_Subtracter(ra, rb, true);

		cout << "-- > 뺄셈 < --" << endl;
		cout << endl;

		cout << "  ";
		ra.showBit();
		cout << endl;

		cout << "- ";
		rb.showBit();
		cout << " CVSZ" << endl;

		for (int i = 0; i < 39; i++)
			cout << "-";
		cout << endl;

		cout << "  ";
		result.showBit(true);
		cout << endl << endl;

		cout << "답 : ";
		result.showBit();
		cout << "  /  " << result.getNum() << endl;
		cout << endl;

		cout << "결과 : " << ra.getNum() << " - " << rb.getNum() << " = " << result.getNum() << endl;
		cout << endl << endl;

		_getch();

		//곱셈

		cout <<"-- > 곱셈 < --"<< endl;
		cout << endl;

		long long mul_result = Register::multiplication(ra, rb);
		
		cout << "결과 : " << ra.getNum() << " * " << rb.getNum() << " = " << mul_result << endl;
		cout << endl << endl;

		_getch();

		//나눗셈

		cout << "-- > 나눗셈 < --" << endl;
		cout << endl;

		Register::division(ra, rb);

		cout << endl << endl;

		//종료

		char ch;

		cout << "다시 하시겠습니까? (y/n) : ";
		cin >> ch;

		if (ch != 'y')
		{
			cout << endl << "프로그램을 종료 합니다." << endl;
			break;
		}

		cout << endl;
	}

	return 0;
}
