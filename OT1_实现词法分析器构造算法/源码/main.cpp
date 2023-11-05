#include <iostream>
#include <stack>
#include "suffix.h"
#include "NFA.h"
#include "DFA.h"
#include "charset.h"
#include "Simplify.h"

using namespace std;

int main() {
	//输入正则表达式
	string op;
	cin >> op;
	string s;
	getChar(op);
	cout << "输入字符数为：" << charNum << endl;
	//转换为后缀表达式
	s=Suffix(op);
	cout << "后缀表达式：" << s << endl;
	//后缀表达式转NFA
	NFA r;
	r=buildNFA(s);
	testprint(r);
	//NFA转DFA
	getTransition(r);
	cout << endl << "状态转换表：" << endl;
	printDtran();
	cout << endl << "状态转换矩阵：" << endl;
	printMu();
	//DFA化简
	initPai();
	dividePai();
	getResult();
	printSimplify();
	return 0;
}