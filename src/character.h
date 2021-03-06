#pragma once
#include "grid_entity.h"

class CharacterController;
class Character : public GridEntity
{
public:
	static void InitializeCharacterFrameMap();

	Character();
	virtual ~Character();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);


	virtual void Move(const EDirection& eDir);
	bool IsMoving() const { return m_fMoveTimer > 0.0f; }

	void SetFacing(EDirection direction);
	void SetSkin(short skin);

	virtual sf::IntRect GetRect();


private:
	void MoveUpdatePreInput(float dt);
	void MoveUpdatePostInput(float dt);

	short m_frame;
	short m_totalFrames;
	EDirection m_facing;
	float m_animTimer;
	short m_skin;
	
	float m_speed;
	sf::Vector2f m_velocity;

	float m_fMoveTimer;
	EDirection m_eMoveDir;
	sf::Vector2i m_oldPos;
	bool m_bMoveMarker;

protected:
	CharacterController* m_controller;

public:
	float m_speedMultiplier;
};