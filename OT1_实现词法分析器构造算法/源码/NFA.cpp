#include <stack>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//定义存储NFA的结构
//NFA的边(状态->连接的状态)
struct NFALine {
	int ID = -1;
	int link; //后续转移的状态数,-1表示该状态是结束状态
	//下一个转移的节点
	//转移边的字符，'#'表示空
	vector<pair<char, NFALine*>> next;
	NFALine() {
		link = 0;
	}
	NFALine(int l) {
		link = l;
	}
};
//NFA有关操作
struct NFA {
	int num; //当前NFA中的状态数
	vector<NFALine*> State; //为当前所有的状态编号
	NFALine* start; //当前子NFA的开始状态
	NFALine* end;  //当前子NFA的结束状态
	NFA() {
		start = new NFALine();
		State.emplace_back(start);
		end = start;
		num = 1;
	}
	//加一个字符a的情况
	void addLineA(char c) {
		NFALine* e = new NFALine(-1);
		start->link++;
		start->next.emplace_back(c, e);
		end = e;
		num = 2;
		State.emplace_back(end);
	}
	//并的情况
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
	//连接的情况
	void linkLineA(NFA* linka) {
		State.insert(State.end(), linka->State.begin()+1, linka->State.end());
		end->link = linka->start->link;
		end->next = linka->start->next;
		end = linka->end;
		num = num + linka->num - 1;
	}
	//闭包
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
	//设置状态号
	void SetID() {
		for (int i = 0; i < num; i++) {
			NFALine* temp = State[i];
			int ID = i;
			temp->ID = ID;
		}
	}
};
//创建NFA的函数
NFA buildNFA(string s) {
	//存储当前已构造的子NFA
	stack<NFA> stk;
	for (auto c : s) {
		//如果是用一个字符连接
		if (c != '|' && c != '*' && c != '-') {
			NFA a;
			a.addLineA(c);
			stk.push(a);
		}
		//如果是*
		else if (c == '*') {
			NFA a = stk.top();
			stk.pop();
			a.eLinkA();
			stk.push(a);
		}
		//如果是|
		else if (c == '|') {
			NFA b = stk.top();
			stk.pop();
			NFA a = stk.top();
			stk.pop();
			a.orLineA(&b);
			stk.push(a);
		}
		//如果是-
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
//检验
void testprint(NFA r) {
	r.SetID();
	for (int i = 0; i < r.num; i++) {
		NFALine* temp = r.State[i];
		cout << "状态" << i << ":";
		int k = temp->link;
		if (k != -1) {
			cout << "连接了" << k << "条边" << endl << '\t';
			for (int j = 0; j < k; j++) {
				cout << temp->next[j].first << "->" << temp->next[j].second->ID << ", ";
			}
		}
		else {
			cout << "终止状态";
		}
		cout << endl;
	}
}