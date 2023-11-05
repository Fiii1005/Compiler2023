#pragma once
#include "NFA.h"
#include <set>
using namespace std;
//´æ´¢×´Ì¬Êý
extern int StateNum;
extern int NFAend;
//´æ´¢×´Ì¬ºÍ±êÖ¾
extern vector<pair<bool, set<int>>> Dstates;
//´æ´¢×´Ì¬×ªÒÆ¾ØÕó
extern vector<int*> Dtran;
void getTransition(NFA nfar);
void printDtran();
void printMu();