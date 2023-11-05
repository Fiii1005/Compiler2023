#include "charset.h"
#include "NFA.h"
#include <vector>
#include <set>
#include <algorithm>
#include <stack>
using namespace std;

vector<NFALine*> NFAstate;

//存储状态数
int StateNum = 0;
//存储状态和标志
vector<pair<bool, set<int>>> Dstates;
//存储状态转移矩阵
vector<int*> Dtran;
//NFAnum
int NFAend = 0;

//求转移后的状态集合
void traState(int s,char c,set<int>& state) {
	NFALine* temp = NFAstate[s];
	int num = temp->link;
	for (int i = 0; i < num; i++) {
		if (temp->next[i].first == c) {
			state.insert((temp->next[i].second)->ID);
		}
	}
}
//求空串转移状态集合
void e_closure(set<int>& state) {
	stack<int> wait;
	set<int>::iterator it;
	for (it = state.begin(); it != state.end(); it++) {
		wait.push(*it);
	}
	while (!wait.empty()) {
		int temp = wait.top();
		wait.pop();
		NFALine* tempLine = NFAstate[temp];
		int num = tempLine->link;
		for (int i = 0; i < num; i++) {
			if (tempLine->next[i].first == '#') {
				state.insert(tempLine->next[i].second->ID);
				wait.push((tempLine->next[i].second)->ID);
			}
		}
	}
}

//求转移后状态闭包
void move(set<int>& s, char a,set<int>& change) {
	set<int>::iterator it;
	//存储状态转移后的包
	for (it = s.begin(); it != s.end(); it++) {
		traState(*it, a, change);
	}
}

//判断集合是否在Dstates中
int findState(set<int> a) {
	for (int i = 0; i < StateNum; i++) {
		if (a == Dstates[i].second) {
			return i;
		}
	}
	return -1;
}

void getTransition(NFA nfar) {
	NFAstate = nfar.State;
	NFAend = nfar.num - 1;
	//已标记状态位移
	int offset = 0;
	//载入初始状态
	set<int> start;
	start.insert(0);
	e_closure(start);
	Dstates.emplace_back(false, start);
	int* tran = new int[charNum];
	Dtran.emplace_back(tran);
	StateNum++;
	while (offset < StateNum) {
		//加上标记
		set<int> T = Dstates[offset].second;
		Dstates[offset].first = true;
		for (int i = 0; i < charNum; i++) {
			set<int> change;
			move(T, charSET[i], change);
			if (change.size() == 0) {
				Dtran[offset][i] = -1;
				continue;
			}
			e_closure(change);
			set<int> U = change;
			//U的状态号
			int index = findState(U);
			if (index==-1) {
				Dstates.emplace_back(false, U);
				int* tran = new int[charNum];
				Dtran.emplace_back(tran);
				index = StateNum;
				StateNum++;
			}
			Dtran[offset][i] = index;
		}
		offset++;
	}
}

//打印集合里的内容
void printSet(set<int> s) {
	set<int>::iterator it;
	cout << "{";
	for (it = s.begin(); it != s.end(); it++) {
		cout << *it << ',';
	}
	cout << "}";
	if (s.size() <= 2) {
		cout << "\t\t\t";
	}
	else {
		cout << "\t\t";
	}
}

void printDtran() {
	cout << "State\t\t\t";
	for (int i = 0; i < charNum; i++) {
		cout << charSET[i] << "->\t\t\t";
	}
	cout << endl;
	for (int i = 0; i < StateNum; i++) {
		printSet(Dstates[i].second);
		for (int j = 0; j < charNum; j++) {
			if (Dtran[i][j] == -1) {
				cout << "空\t\t\t";
				continue;
			}
			printSet(Dstates[Dtran[i][j]].second);
		}
		cout << endl;
	}
}

//打印状态转换矩阵
void printMu() {
	cout << "S\t\t";
	for (int i = 0; i < charNum; i++) {
		cout << charSET[i] << "\t\t";
	}
	cout << endl;
	for (int i = 0; i < StateNum; i++) {
		cout << i << "\t\t";
		for (int j = 0; j < charNum; j++) {
			if (Dtran[i][j] == -1) {
				cout << "空\t\t";
				continue;
			}
			cout << Dtran[i][j] << "\t\t";
		}
		cout << endl;
	}
}