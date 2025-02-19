#include <string>

#pragma once
class State
{
public:
	State();
	~State();

private:
	string name;
	bool isAccept;
	bool isStart;
};
