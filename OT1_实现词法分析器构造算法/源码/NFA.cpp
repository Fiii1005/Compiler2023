#include <stack>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//����洢NFA�Ľṹ
//NFA�ı�(״̬->���ӵ�״̬)
struct NFALine {
	int ID = -1;
	int link; //����ת�Ƶ�״̬��,-1��ʾ��״̬�ǽ���״̬
	//��һ��ת�ƵĽڵ�
	//ת�Ʊߵ��ַ���'#'��ʾ��
	vector<pair<char, NFALine*>> next;
	NFALine() {
		link = 0;
	}
	NFALine(int l) {
		link = l;
	}
};
//NFA�йز���
struct NFA {
	int num; //��ǰNFA�е�״̬��
	vector<NFALine*> State; //Ϊ��ǰ���е�״̬���
	NFALine* start; //��ǰ��NFA�Ŀ�ʼ״̬
	NFALine* end;  //��ǰ��NFA�Ľ���״̬
	NFA() {
		start = new NFALine();
		State.emplace_back(start);
		end = start;
		num = 1;
	}
	//��һ���ַ�a�����
	void addLineA(char c) {
		NFALine* e = new NFALine(-1);
		start->link++;
		start->next.emplace_back(c, e);
		end = e;
		num = 2;
		State.emplace_back(end);
	}
	//�������
	void orLineA(NFA* linka) {
		State.insert(State.end(), linka->State.begin(), linka->State.end());
		NFALine* e = new NFALine(-1);
		end->link = 1;
		end->next.emplace_back('#', e);
		linka->end->link = 1;
		linka->end->next.emplace_back('#', e);
		end = e;
		NFALine* s = new NFALine();
		s->link += 2;
		s->next.emplace_back('#', start);
		s->next.emplace_back('#', linka->start);
		start = s;
		num = num + linka->num + 2;
		State.insert(State.begin(), start);
		State.emplace_back(end);
	}
	//���ӵ����
	void linkLineA(NFA* linka) {
		State.insert(State.end(), linka->State.begin()+1, linka->State.end());
		end->link = linka->start->link;
		end->next = linka->start->next;
		end = linka->end;
		num = num + linka->num - 1;
	}
	//�հ�
	void eLinkA() {
		NFALine* e = new NFALine(-1);
		end->link = 2;
		end->next.emplace_back('#', e);
		end->next.emplace_back('#', start);
		end = e;
		NFALine* s = new NFALine();
		s->link += 2;
		s->next.emplace_back('#', start);
		s->next.emplace_back('#', end);
		start = s;
		State.insert(State.begin(), start);
		num += 2;
		State.emplace_back(end);
	}
	//����״̬��
	void SetID() {
		for (int i = 0; i < num; i++) {
			NFALine* temp = State[i];
			int ID = i;
			temp->ID = ID;
		}
	}
};
//����NFA�ĺ���
NFA buildNFA(string s) {
	//�洢��ǰ�ѹ������NFA
	stack<NFA> stk;
	for (auto c : s) {
		//�������һ���ַ�����
		if (c != '|' && c != '*' && c != '-') {
			NFA a;
			a.addLineA(c);
			stk.push(a);
		}
		//�����*
		else if (c == '*') {
			NFA a = stk.top();
			stk.pop();
			a.eLinkA();
			stk.push(a);
		}
		//�����|
		else if (c == '|') {
			NFA b = stk.top();
			stk.pop();
			NFA a = stk.top();
			stk.pop();
			a.orLineA(&b);
			stk.push(a);
		}
		//�����-
		else if (c == '-') {
			NFA b = stk.top();
			stk.pop();
			NFA a = stk.top();
			stk.pop();
			a.linkLineA(&b);
			stk.push(a);
		}
	}
	return stk.top();
}
//����
void testprint(NFA r) {
	r.SetID();
	for (int i = 0; i < r.num; i++) {
		NFALine* temp = r.State[i];
		cout << "״̬" << i << ":";
		int k = temp->link;
		if (k != -1) {
			cout << "������" << k << "����" << endl << '\t';
			for (int j = 0; j < k; j++) {
				cout << temp->next[j].first << "->" << temp->next[j].second->ID << ", ";
			}
		}
		else {
			cout << "��ֹ״̬";
		}
		cout << endl;
	}
}