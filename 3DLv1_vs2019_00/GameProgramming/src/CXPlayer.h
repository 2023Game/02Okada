#pragma once
#include"CXCharacter.h"
#include "CInput.h"

class CXPlayer :public CXCharacter
{
public:
	//�X�V����
	void Update();
private:
	CInput mInput;

};