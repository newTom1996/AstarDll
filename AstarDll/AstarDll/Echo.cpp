#include "Echo.h"
#include <iostream>
#include "pch.h"
using namespace std;

extern "C" {

	_declspec(dllexport) Echo* GenerateEcho(float x);
	_declspec(dllexport) void SetX(Echo* echo, float x);
	_declspec(dllexport) float GetX(Echo* echo);
	_declspec(dllexport) void ReleaseEcho(Echo* echo);


	Echo::Echo(float xx) : x(xx) {
		cout << "echo¹¹Ôì" << endl;
	}

	Echo::~Echo() {
		cout << "echoÎö¹¹" << endl;
	}

	float Echo::getX() {
		return x;
	}

	void Echo::setX(float x) {
		this->x = x;
	}

	Echo* GenerateEcho(float x)
	{
		return new Echo(x);
	}

	void ReleaseEcho(Echo* echo)
	{
		delete echo;
	}

	void SetX(Echo* echo, float x)
	{
		echo->setX(x);
	}

	float GetX(Echo* echo)
	{
		return echo->getX();
	}
}
