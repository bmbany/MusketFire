#include "Section.h"
#include "Client_Define.h"

void Section::OnCollision(CollisionInfo info)
{
	if (*info.other == "Mouse")
		SetActive(true);
}

void Section::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
		SetActive(false);
}
