#include <vector>
#include <iostream>
#include <string>
#include "suffix.h"
using namespace std;
int charNum = 0;
vector<char> charSET;
//判断在集合中是否存在该字符
int findChar(char c) {
	for (int i = 0; i < charNum; i++) {
		if (charSET[i] == c) {
			return i;
		}
	}
	return -1;
}
//从输入的正则表达式中获取字符集
void getChar(string s) {
	for (const auto c : s) {
		if ((isText(c)=='a') && findChar(c) == -1) {
			charSET.emplace_back(c);
			charNum++;
		}
	}
}