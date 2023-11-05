#include <iostream>
#include <stack>
#include "suffix.h"
#include "NFA.h"
#include "DFA.h"
#include "charset.h"
#include "Simplify.h"

using namespace std;

int main() {
	//����������ʽ
	string op;
	cin >> op;
	string s;
	getChar(op);
	cout << "�����ַ���Ϊ��" << charNum << endl;
	//ת��Ϊ��׺���ʽ
	s=Suffix(op);
	cout << "��׺���ʽ��" << s << endl;
	//��׺���ʽתNFA
	NFA r;
	r=buildNFA(s);
	testprint(r);
	//NFAתDFA
	getTransition(r);
	cout << endl << "״̬ת����" << endl;
	printDtran();
	cout << endl << "״̬ת������" << endl;
	printMu();
	//DFA����
	initPai();
	dividePai();
	getResult();
	printSimplify();
	return 0;
}