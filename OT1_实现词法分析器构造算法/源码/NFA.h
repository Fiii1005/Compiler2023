#pragma once
#include <string>
#include <vector>
using namespace std;
struct NFALine {
	int ID;
	int link; //后续转移的状态数,-1表示该状态是结束状态
	//下一个转移的节点
	//转移边的字符，'#'表示空
	vector<pair<char, NFALine*>> next;
	NFALine();
	NFALine(int l);
};
//NFA有关操作
struct NFA {
	int num; //当前NFA中的状态数
	vector<NFALine*> State; //为当前所有的状态编号
	NFALine* start; //当前子NFA的开始状态
	NFALine* end;  //当前子NFA的结束状态
	NFA();
	//加一个字符a的情况
	void addLineA(char c);
	//并的情况
	void orLineA(NFA* linka);
	//连接的情况
	void linkLineA(NFA* linka);
	//闭包
	void eLinkA();
	//设置状态号
	void SetID();
};
NFA buildNFA(string s);
//检验
void testprint(NFA r);