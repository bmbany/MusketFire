#pragma once
#include "Base.h"

class PlayerStatus : public Engine::Base
{
private:
	explicit PlayerStatus() = default;
	virtual ~PlayerStatus() = default;

private:
	// Base��(��) ���� ��ӵ�
	void Free() override;

public:
	static PlayerStatus* Create();

public:
	bool _isUseBiscuit = false;
	bool _isSacrifice = false;
};

