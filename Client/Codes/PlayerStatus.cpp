#include "PlayerStatus.h"

void PlayerStatus::Free()
{
}

PlayerStatus* PlayerStatus::Create()
{
	return new PlayerStatus;
}
