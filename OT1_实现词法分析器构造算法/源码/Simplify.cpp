#include "DFA.h"
#include <set>
#include <vector>
#include "charset.h"
using namespace std;
//�洢��ɻ��ֵ�λ��
int offset = 0;
//�洢����pai
vector<pair<set<int>,int>> pai;
int paiNum = 0;
//��󻯼��״̬-��ת����״̬
vector<int*> result;
//״̬�Ƿ�����̬����
vector<bool> endState;

//�жϷ���̬�����е�ĳ��״̬�Ƿ�����״̬
void removeDied() {
	set<int>::iterator it;
	for (it = pai[1].first.begin(); it != pai[1].first.end(); it++) {
		bool flag = true;
		for (int j = 0; j < charNum; j++) {
			if (Dtran[*it][j] != *it) {
				flag = false;
				break;
			}
		}
		if (flag) {
			pai[1].first.erase(*it);
		}
	}
}
//�洢������̬��״̬��
set<int> DFAend;

//��ʼ����
void initPai() {
	set<int> endyes;
	set<int> endno;
	for (int i = 0; i < StateNum; i++) {
		set<int> temp = Dstates[i].second;
		if (temp.find(NFAend) == temp.end()) {
			endno.insert(i);
		}
		else {
			endyes.insert(i);
			DFAend.insert(i);
		}
	}
	pai.emplace_back(endyes, 0);
	paiNum++;
	if (endno.size() != 0) {
		pai.emplace_back(endno, 0);
		paiNum++;
		removeDied();
	}
}

//����ĳ��״̬���ĸ�״̬������
int getPaiIndex(int s) {
	for (int i = 0; i < paiNum; i++) {
		if (pai[i].first.find(s) != pai[i].first.end()) {
			return i;
		}
	}
	return -1;
}

//״̬����
//offset��ʾ�Ѿ���ɷ����״̬
//paiNum��ʾ�������һ��״̬
//��ȫ����ɼ���ʱ��С�����
void dividePai() {
	if (paiNum == 1) {
		cout << "ֻ����̬" << endl;
		return;
	}
	while (offset < paiNum) {
		set<int> now = pai[paiNum - 1].first;
		//index��ʾ�ü���֮ǰȷ�Ϲ������ַ�
		int index = pai[paiNum - 1].second;
		//�����洢�м����µ�״̬<״̬���ϣ�ת�Ƶ�״̬�����ڼ���>
		vector<pair<set<int>, int>> temp;
		set<int>::iterator it;
		for (it = now.begin(); it != now.end(); it++) {
			int toS = Dtran[*it][index];
			int toSindex = getPaiIndex(toS);
			int tempnum = temp.size();
			bool exit = false;
			for (int j = 0; j < tempnum; j++) {
				if (toSindex == temp[j].second) {
					exit = true;
					temp[j].first.insert(*it);
					break;
				}
			}
			if (!exit) {
				set<int> newset;
				newset.insert(*it);
				temp.emplace_back(newset, toSindex);
			}
		}
		pai.pop_back();
		paiNum--;
		int tempnum = temp.size();
		//���ת�ƺ��п�״̬������ֻ������״̬�Ļ�����������
		//���򱨴��������о�����ô����
		bool havEmpty = false;
		for (int j = 0; j < tempnum; j++) {
			if (temp[j].second == -1) {
				havEmpty = true;
				break;
			}
		}
		if (havEmpty) {
			if (tempnum == 2) {
				temp.clear();
				temp.emplace_back(now, -1);
				tempnum = 1;
			}
		}
		//���ֻ��һ�����ϲ���index=charNum-1���ӵ���ͷ
		//���index!=charNum-1��index+1���¼ӻ�ȥ
		if (tempnum == 1) {
			if (index == charNum - 1) {
				pai.insert(pai.begin(), make_pair(temp[0].first, 0));
				offset++;
			}
			else {
				pai.emplace_back(temp[0].first, index + 1);
			}
			paiNum++;
			continue;
		}
		offset = 0;
		//���������ֻ��һ��Ԫ�أ�ֱ�ӷŵ���ͷ
		//����index���㣬���¼ӵ�ĩβ
		for (int j = 0; j < tempnum; j++) {
			if (temp[j].first.size() == 1) {
				pai.insert(pai.begin(), make_pair(temp[j].first, 0));
				offset++;
			}
			else {
				pai.emplace_back(temp[j].first, 0);
			}
			paiNum++;
		}
	}
}

//����pai�е������������
//��ȡ���ս����ʱ��һ��
void getResult() {
	//��pai����һ��ð������
	for (int i = 0; i < paiNum - 1; i++) {
		for (int j = 0; j < paiNum - 1 - i; j++) {
			set<int>::iterator it;
			it = pai[j].first.begin();
			int ai = *it;
			it = pai[j + 1].first.begin();
			int bi = *it;
			if (bi < ai) {
				swap(pai[j], pai[j + 1]);
			}
		}
	}
	for (int i = 0; i < paiNum; i++) {
		bool isend = false;
		int* newset = new int[charNum];
		for (int j = 0; j < charNum; j++) {
			//ȡһ����־�Ե�״̬��
			//���ת����״̬��-1����ȡ��һ��
			//���ת����״̬ȫ��-1���ͱ�ʾ��ת��Ϊ��
			set<int>::iterator it;
			int toS = -1;
			for (it = pai[i].first.begin(); it != pai[i].first.end(); it++) {
				int example = *it;
				if (Dtran[example][j] != -1) {
					toS = Dtran[example][j];
				}
				if (DFAend.find(example) != DFAend.end()) {
					isend = true;
				}
			}
			if (toS != -1) {
				newset[j] = getPaiIndex(toS);
			}
			else {
				newset[j] = -1;
			}
		}
		endState.emplace_back(isend);
		result.emplace_back(newset);
	}
}

//����pai�л�����
void printSimplify() {
	cout << endl << "��ȡ����״̬���ϣ�" << endl;
	cout << "���ջ���Ϊ" << paiNum << "��״̬��" << endl;
	for (int i = 0; i < paiNum; i++) {
		cout << "״̬" << i << ": " << "{";
		set<int>::iterator it;
		for (it = pai[i].first.begin(); it != pai[i].first.end(); it++) {
			cout << *it << ",";
		}
		cout << "}" << endl;
	}
	cout << endl;
	cout << endl << "������״̬ת�ƾ��󣺣���*��ʾΪ��̬��" << endl;
	cout << "S\t\t";
	for (int i = 0; i < charNum; i++) {
		cout << charSET[i] << "\t\t";
	}
	cout << endl;
	for (int i = 0; i < paiNum; i++) {
		cout << i;
		if (endState[i]) {
			cout << "*";
		}
		cout << "\t\t";
		for (int j = 0; j < charNum; j++) {
			cout << result[i][j] << "\t\t";
		}
		cout << endl;
	}
	cout << endl;
}
