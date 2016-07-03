#pragma once

class CharacterController
{
public:
	CharacterController() {}
	virtual ~CharacterController() {}

	virtual void Update(float dt) = 0;
};