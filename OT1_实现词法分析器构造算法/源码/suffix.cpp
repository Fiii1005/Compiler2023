#include <iostream>
#include <stack>
#include <string>
using namespace std;

//算法实现的正则表达式包含()|*运算符
//将正则表达式中缀转后缀
//连接用-表示

//预处理
//判断某个字符是否是字符
char isText(char c) { //如果是运算符返回自身，否则返回'a'
	if (c == '(' || c == ')' || c == '*' || c == '|') {
		return c;
	}
	return 'a';
}

//判断表达式是否是合法的正则表达式
bool isRegex(string &s) {
	if (!s.size()) {
		//长度是否为0
		return false;
	}
	int l = 0;
	for (const auto &t : s) {
		//判断括号是否匹配
		if (t == '(') {
			l++;
		}
		else if (t == ')') {
			l--;
		}
		if (l < 0) {
			return false;
		}
	}
	if (l != 0) {
		return false;
	}
	//用-连接a-a,a*-a,a-(,)-a,)-(,a*-(
	for (int i = 0; i < s.size()-1; i++) {
		char a = isText(s[i]);
		char b = isText(s[i + 1]);
		if ((a == 'a' && b == 'a') || (a == '*' && b == 'a') || (a == 'a' && b == '(') || (a == ')' && b == 'a') || (a == ')' && b == '(') || (a == '*' && b == '(')) {
			s.insert(i + 1, "-");
			i++;
		}
	}
	cout << s << "是合法正则表达式！" << endl;
	return true;
}



//判断字符是否是运算符
bool isOperator(char c) {
	if (c == '|' || c == '*'||c=='-') {
		return true;
	}
	return false;
}

//获取运算符优先级
int getPriority(char c) {
	if (c == '(') {
		return 1;
	}
	if (c == '|') {
		return 2;
	}
	if (c == '-') {
		return 3;
	}
	if (c == '*') {
		return 4;
	}
	return 0;
}

string toSuffix(const string& expr) {
	stack<char> op; //存储运算符
	string suffix; //存储表达式
	//遍历正则表达式
	for (const auto& c : expr) {
		if (isOperator(c)) {
			//如果是运算符的话
			if (op.empty()) { //如果栈空，直接入栈
				op.push(c);
			}
			else {
				//如果栈顶运算符优先级大于等于当前的符号
				//则全部弹出栈，加入suffix末尾
				while (!op.empty()) {
					char t = op.top();
					if (getPriority(t) >= getPriority(c)) {
						op.pop();
						suffix.push_back(t);
					}
					else {
						break;
					}
				}
				//直到弹出所有优先级大于当前运算符的符号时
				//将当前运算符压入栈
				op.push(c);
			}
		}
		else { //如果不是运算符
			if (c == '(') { //左括号直接入栈
				op.push(c);
			}
			else if (c == ')') {
				//如果是右括号
				//依次弹出栈顶运算符加入suffix，直到遇到左括号
				while (op.top() != '(') {
					suffix.push_back(op.top());
					op.pop();
				}
				op.pop(); //括号舍弃
			}
			else {
				//操作符直接加入suffix
				suffix.push_back(c);
			}
		}
	}
	//取出剩余运算符
	while (!op.empty()) {
		suffix.push_back(op.top());
		op.pop();
	}
	return suffix;
}

string Suffix(string s) {
	if (!isRegex(s)) {
		cout << "不是合法的正则表达式！" << endl;
		system("pause");
	}
	return toSuffix(s);
}