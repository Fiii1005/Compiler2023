#pragma once
#include <string>
#include <vector>
using namespace std;
struct NFALine {
	int ID;
	int link; //����ת�Ƶ�״̬��,-1��ʾ��״̬�ǽ���״̬
	//��һ��ת�ƵĽڵ�
	//ת�Ʊߵ��ַ���'#'��ʾ��
	vector<pair<char, NFALine*>> next;
	NFALine();
	NFALine(int l);
};
//NFA�йز���
struct NFA {
	int num; //��ǰNFA�е�״̬��
	vector<NFALine*> State; //Ϊ��ǰ���е�״̬���
	NFALine* start; //��ǰ��NFA�Ŀ�ʼ״̬
	NFALine* end;  //��ǰ��NFA�Ľ���״̬
	NFA();
	//��һ���ַ�a�����
	void addLineA(char c);
	//�������
	void orLineA(NFA* linka);
	//���ӵ����
	void linkLineA(NFA* linka);
	//�հ�
	void eLinkA();
	//����״̬��
	void SetID();
};
NFA buildNFA(string s);
//����
void testprint(NFA r);