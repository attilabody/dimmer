//============================================================================
// Name        : buttontest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include "ButtonLogic.h"

using namespace std;

bool 		g_pinStatus;
uint16_t	g_now;

const char* StateToString(uint8_t st)
{
	static const char *names[4] = { "inactive", "clicked", "doubleclicked", "tripleclicked" };
	if(st == 255) return "active";
	else if(st > 3) return "wtf";
	else return names[(uint8_t)st];
}

inline uint8_t Tick(ButtonLogic &btn, bool newStatus)
{

	auto st = btn.Tick(newStatus, g_now++);
	cout << g_now << ", " << g_pinStatus << ", " << StateToString(st) << ", " << StateToString(btn.GetState()) << ", " << StateToString(btn.GetState()) << endl;
	return st;
}

inline uint8_t _Tick(ButtonLogic &btn, bool newStatus)
{
	g_pinStatus = newStatus;
	auto rsb = btn.GetRawState();
	auto st = btn.Tick(newStatus, g_now++);
	auto rsa = btn.GetRawState();
//	auto st2 = btn.GetState();
//	auto st3 = btn.GetState();
	cout << g_now << ", " << g_pinStatus << ", " << StateToString(rsb) << ", > " << StateToString(st) << " < , " << StateToString(rsa) << endl;//<< ", " << StateToString(st2) << ", " << StateToString(st3) << endl;
	return st;
}

int main() {
	ButtonLogic	btn;

	cout << hex;
	btn.Init(1, 2);

	//

	auto rslt = _Tick(btn, false);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);

	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);

	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);

	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, false);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	
	rslt = btn.GetState();

	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, true);
	rslt = _Tick(btn, false);
	return 0;
}
