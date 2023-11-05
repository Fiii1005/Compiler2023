#include "DFA.h"
#include <set>
#include <vector>
#include "charset.h"
using namespace std;
//存储完成划分的位移
int offset = 0;
//存储划分pai
vector<pair<set<int>,int>> pai;
int paiNum = 0;
//最后化简的状态-》转换的状态
vector<int*> result;
//状态是否是终态数组
vector<bool> endState;

//判断非终态集合中的某个状态是否是死状态
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
//存储包含终态的状态号
set<int> DFAend;

//初始划分
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

//返回某个状态在哪个状态集合中
int getPaiIndex(int s) {
	for (int i = 0; i < paiNum; i++) {
		if (pai[i].first.find(s) != pai[i].first.end()) {
			return i;
		}
	}
	return -1;
}

//状态划分
//offset表示已经完成分类的状态
//paiNum表示数组最后一个状态
//当全部完成检验时最小化完成
void dividePai() {
	if (paiNum == 1) {
		cout << "只有终态" << endl;
		return;
	}
	while (offset < paiNum) {
		set<int> now = pai[paiNum - 1].first;
		//index表示该集合之前确认过几个字符
		int index = pai[paiNum - 1].second;
		//用来存储有几个新的状态<状态集合，转移的状态号所在集合>
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
		//如果转移后有空状态，并且只有两个状态的话，不用区别
		//否则报错，让我再研究下怎么处理
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
		//如果只有一个集合并且index=charNum-1，加到开头
		//如果index!=charNum-1，index+1重新加回去
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
		//如果集合中只有一个元素，直接放到开头
		//否则index清零，重新加到末尾
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

//由于pai中的排列是逆序的
//获取最终结果的时候倒一下
void getResult() {
	//对pai进行一个冒泡排序
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
			//取一个标志性的状态号
			//如果转换的状态是-1，就取下一个
			//如果转换的状态全是-1，就表示该转移为空
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

//检验pai中化简结果
void printSimplify() {
	cout << endl << "获取的新状态集合：" << endl;
	cout << "最终化简为" << paiNum << "个状态：" << endl;
	for (int i = 0; i < paiNum; i++) {
		cout << "状态" << i << ": " << "{";
		set<int>::iterator it;
		for (it = pai[i].first.begin(); it != pai[i].first.end(); it++) {
			cout << *it << ",";
		}
		cout << "}" << endl;
	}
	cout << endl;
	cout << endl << "化简后的状态转移矩阵：（带*表示为终态）" << endl;
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
