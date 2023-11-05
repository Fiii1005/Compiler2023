#include <iostream>
#include <stack>
#include <string>
using namespace std;

//�㷨ʵ�ֵ�������ʽ����()|*�����
//��������ʽ��׺ת��׺
//������-��ʾ

//Ԥ����
//�ж�ĳ���ַ��Ƿ����ַ�
char isText(char c) { //���������������������򷵻�'a'
	if (c == '(' || c == ')' || c == '*' || c == '|') {
		return c;
	}
	return 'a';
}

//�жϱ��ʽ�Ƿ��ǺϷ���������ʽ
bool isRegex(string &s) {
	if (!s.size()) {
		//�����Ƿ�Ϊ0
		return false;
	}
	int l = 0;
	for (const auto &t : s) {
		//�ж������Ƿ�ƥ��
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
	//��-����a-a,a*-a,a-(,)-a,)-(,a*-(
	for (int i = 0; i < s.size()-1; i++) {
		char a = isText(s[i]);
		char b = isText(s[i + 1]);
		if ((a == 'a' && b == 'a') || (a == '*' && b == 'a') || (a == 'a' && b == '(') || (a == ')' && b == 'a') || (a == ')' && b == '(') || (a == '*' && b == '(')) {
			s.insert(i + 1, "-");
			i++;
		}
	}
	cout << s << "�ǺϷ�������ʽ��" << endl;
	return true;
}



//�ж��ַ��Ƿ��������
bool isOperator(char c) {
	if (c == '|' || c == '*'||c=='-') {
		return true;
	}
	return false;
}

//��ȡ��������ȼ�
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
	stack<char> op; //�洢�����
	string suffix; //�洢���ʽ
	//����������ʽ
	for (const auto& c : expr) {
		if (isOperator(c)) {
			//�����������Ļ�
			if (op.empty()) { //���ջ�գ�ֱ����ջ
				op.push(c);
			}
			else {
				//���ջ����������ȼ����ڵ��ڵ�ǰ�ķ���
				//��ȫ������ջ������suffixĩβ
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
				//ֱ�������������ȼ����ڵ�ǰ������ķ���ʱ
				//����ǰ�����ѹ��ջ
				op.push(c);
			}
		}
		else { //������������
			if (c == '(') { //������ֱ����ջ
				op.push(c);
			}
			else if (c == ')') {
				//�����������
				//���ε���ջ�����������suffix��ֱ������������
				while (op.top() != '(') {
					suffix.push_back(op.top());
					op.pop();
				}
				op.pop(); //��������
			}
			else {
				//������ֱ�Ӽ���suffix
				suffix.push_back(c);
			}
		}
	}
	//ȡ��ʣ�������
	while (!op.empty()) {
		suffix.push_back(op.top());
		op.pop();
	}
	return suffix;
}

string Suffix(string s) {
	if (!isRegex(s)) {
		cout << "���ǺϷ���������ʽ��" << endl;
		system("pause");
	}
	return toSuffix(s);
}