#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"
#include"CCollider.h"

//�Փˏ����͈͂�藣��Ă���R���C�_�͏Փˏ������Ȃ�
#define COLLISIONRANGE 30


class CCollisionManager : public CTaskManager
{
public:
	//�Փˏ���
	virtual void Collision();
	void Collision(CCollider* c, int range);
	//�C���X�^���X�̎擾
	static CCollisionManager* Instance();
private:
	//�}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;
};

#endif

