#include "charset.h"
#include "NFA.h"
#include <vector>
#include <set>
#include <algorithm>
#include <stack>
using namespace std;

vector<NFALine*> NFAstate;

//�洢״̬��
int StateNum = 0;
//�洢״̬�ͱ�־
vector<pair<bool, set<int>>> Dstates;
//�洢״̬ת�ƾ���
vector<int*> Dtran;
//NFAnum
int NFAend = 0;

//��ת�ƺ��״̬����
void traState(int s,char c,set<int>& state) {
	NFALine* temp = NFAstate[s];
	int num = temp->link;
	for (int i = 0; i < num; i++) {
		if (temp->next[i].first == c) {
			state.insert((temp->next[i].second)->ID);
		}
	}
}
//��մ�ת��״̬����
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

//��ת�ƺ�״̬�հ�
void move(set<int>& s, char a,set<int>& change) {
	set<int>::iterator it;
	//�洢״̬ת�ƺ�İ�
	for (it = s.begin(); it != s.end(); it++) {
		traState(*it, a, change);
	}
}

//�жϼ����Ƿ���Dstates��
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
	//�ѱ��״̬λ��
	int offset = 0;
	//�����ʼ״̬
	set<int> start;
	start.insert(0);
	e_closure(start);
	Dstates.emplace_back(false, start);
	int* tran = new int[charNum];
	Dtran.emplace_back(tran);
	StateNum++;
	while (offset < StateNum) {
		//���ϱ��
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
			//U��״̬��
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

//��ӡ�����������
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
				cout << "��\t\t\t";
				continue;
			}
			printSet(Dstates[Dtran[i][j]].second);
		}
		cout << endl;
	}
}

//��ӡ״̬ת������
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
				cout << "��\t\t";
				continue;
			}
			cout << Dtran[i][j] << "\t\t";
		}
		cout << endl;
	}
}