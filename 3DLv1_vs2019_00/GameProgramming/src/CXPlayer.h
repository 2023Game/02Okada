#pragma once
#include"CXCharacter.h"
#include "CInput.h"

class CXPlayer :public CXCharacter
{
public:
	//XVˆ—
	void Update();
private:
	CInput mInput;

};