#pragma once
#include "NFA.h"
#include <set>
using namespace std;
//�洢״̬��
extern int StateNum;
extern int NFAend;
//�洢״̬�ͱ�־
extern vector<pair<bool, set<int>>> Dstates;
//�洢״̬ת�ƾ���
extern vector<int*> Dtran;
void getTransition(NFA nfar);
void printDtran();
void printMu();