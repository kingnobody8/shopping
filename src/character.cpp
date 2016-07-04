#include "character.h"
#include "character_controller.h"
#include "gfx_util.h"
#include "tilemap.h"
#include "event.h"

static const int FRAME_WIDTH = 32;
static const int FRAME_HEIGHT = 32;
static const int SKIN_ROW = 2;
static const int SKIN_COL = 2;
static const int SKIN_COUNT = SKIN_ROW * SKIN_COL;
static const int SKIN_STEP_X = 3 * FRAME_WIDTH;
static const int SKIN_STEP_Y = 4 * FRAME_HEIGHT;

static const float MOVE_TIME = 0.32f;

static int s_characterFrameMapping[][4][2] =
{
	// North
	{
		{2, 0},
		{0, 0},
		{1, 3},
		{0, 0},
	},

	// South
	{
		{2, 3},
		{2, 1},
		{2, 2},
		{2, 1},
	},

	// East
	{
		{1, 2},
		{1, 0},
		{1, 1},
		{1, 0},
	},

	// West
	{
		{0, 3},
		{0, 2},
		{0, 1},
		{0, 2},
	},
};

static sf::IntRect s_characterCollisionOffsets[] = 
{
	// North
	{10, 23, 13, 7},

	// South
	{10, 23, 13, 7},

	// East
	{10, 23, 13, 7},

	// West
	{10, 23, 13, 7},
};

// [skin][direction][frame]
static sf::IntRect s_characterFrames[SKIN_COUNT][4][4];
/* static */ void Character::InitializeCharacterFrameMap()
{
	for (int i = 0; i < SKIN_COUNT; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			for (int j = 0; j < 4; ++j)
			{
				int offsetX = i % SKIN_COL;
				int offsetY = i / SKIN_COL;

				s_characterFrames[i][k][j].left = s_characterFrameMapping[k][j][0] * FRAME_WIDTH + offsetX * SKIN_STEP_X;
				s_characterFrames[i][k][j].top = s_characterFrameMapping[k][j][1] * FRAME_HEIGHT + offsetY * SKIN_STEP_Y;
				s_characterFrames[i][k][j].width = FRAME_WIDTH;
				s_characterFrames[i][k][j].height = FRAME_HEIGHT;
			}
		}
	}
}
/*=
{
	// North
	{
		{64, 0, 32, 32},
		{0, 0, 32, 32},
		{32, 96, 32, 32},
		{0, 0, 32, 32},
	},

	// South
	{
		{64, 96, 32, 32},
		{64, 32, 32, 32},
		{64, 64, 32, 32},
		{64, 32, 32, 32},
	},

	// East
	{
		{32, 64, 32, 32},
		{32, 0, 32, 32},
		{32, 32, 32, 32},
		{32, 0, 32, 32},
	},

	// West
	{
		{0, 96, 32, 32},
		{0, 64, 32, 32},
		{0, 32, 32, 32},
		{0, 64, 32, 32},
	},
};*/

Character::Character()
	: m_frame(0)
	, m_totalFrames(4)
	, m_animTimer(0.0f)
	, m_skin(0)
	, m_speed(32.0f)
	, m_controller(nullptr)
	, m_speedMultiplier(1.0f)
	, m_fMoveTimer(-1.0f)
	, m_eMoveDir(EDirection::ED_INVALID)
{
	LoadTexture("assets/textures/characters.png", m_texture);
	m_sprite.setTexture(m_texture);

	SetFacing(EDirection::ED_NORTH);
}

Character::~Character()
{
	delete m_controller;
}

void Character::Update(float dt)
{
	MoveUpdatePreInput(dt);
	m_controller->Update(dt);
	MoveUpdatePostInput(dt);

	if (!IsMoving())
	{
		//m_frame = 1;
		SetFacing(m_facing);
	}
	else
	{
		m_animTimer += dt;
		if (m_animTimer > 0.16f)
		{
			m_animTimer = 0;
			++m_frame;
			if (m_frame == m_totalFrames)
			{
				m_frame = 0;
			}
			SetFacing(m_facing);
		}
	}
}

void Character::Draw(sf::RenderWindow& window)
{
	GridEntity::Draw(window);

	sf::IntRect rect = GetRect();

	sf::VertexArray verts;
	verts.resize(5);
	verts[0].position.x = rect.left;
	verts[0].position.y = rect.top;
	verts[1].position.x = rect.left + rect.width;
	verts[1].position.y = rect.top;
	verts[2].position.x = rect.left + rect.width;
	verts[2].position.y = rect.top + rect.height;
	verts[3].position.x = rect.left;
	verts[3].position.y = rect.top + rect.height;
	verts[4].position.x = rect.left;
	verts[4].position.y = rect.top;

	for (size_t i = 0; i < verts.getVertexCount(); ++i)
	{
		verts[i].color = sf::Color::Black;
	}

	window.draw(&verts[0], verts.getVertexCount(), sf::PrimitiveType::LinesStrip);
}

/*virtual*/ void Character::Move(const EDirection& eDir)
{
	m_oldPos = m_pGridNode->position;
	GridEntity::Move(eDir);

	//if (IsMoving()) //only allow movement changes in the opposite direction while we are moving
	//{
	//	if ((m_eMoveDir == North || m_eMoveDir == South) && !(eDir == North || eDir == South))
	//	{
	//		return;
	//	}
	//	else if ((m_eMoveDir == East || m_eMoveDir == West) && !(eDir == East || eDir == West))
	//	{
	//		return;
	//	}
	//}

	SetFacing(eDir);
	m_fMoveTimer = MOVE_TIME;
	m_eMoveDir = eDir;
	//AlignPositionToNode();
}

void Character::SetFacing(EDirection direction)
{
	m_sprite.setTextureRect(s_characterFrames[m_skin][direction][m_frame]);
	m_facing = direction;
}

void Character::MoveUpdatePreInput(float dt)
{
	m_bMoveMarker = false;

	if (m_fMoveTimer < 0)
		return;

	m_fMoveTimer -= dt;

	if (m_fMoveTimer < 0)
	{
		m_bMoveMarker = true;
		m_fMoveTimer = -1.0f;
		m_eMoveDir = GridEntity::EDirection::ED_INVALID;
		AlignPositionToNode();
		return;
	}

	float lerp = (MOVE_TIME - m_fMoveTimer) / MOVE_TIME;
	sf::Vector2i lerpPos;
	lerpPos.x = m_oldPos.x + lerp * (m_pGridNode->position.x - m_oldPos.x);
	lerpPos.y = m_oldPos.y + lerp * (m_pGridNode->position.y - m_oldPos.y);

	sf::IntRect rect = GetRect();
	SetPosition(sf::Vector2f(lerpPos.x - rect.width / 2, lerpPos.y - rect.height / 2));

}


void Character::MoveUpdatePostInput(float dt)
{
	if (m_bMoveMarker && m_fMoveTimer < 0)
	{
		m_frame = 1;
		SetFacing(m_facing);
	}
}

//void Character::Move(int x, int y, float dt)
//{
//	m_velocity.x = x * m_speed * dt * m_speedMultiplier;
//	m_velocity.y = y * m_speed * dt * m_speedMultiplier;
//
//	if (x != 0)
//	{
//		SetFacing(x < 0 ? CharacterDirection::West : CharacterDirection::East);
//	}
//	else if (y != 0)
//	{
//		SetFacing(y < 0 ? CharacterDirection::North : CharacterDirection::South);
//	}
//}

void Character::SetSkin(short skin)
{
	if (skin >= 0 && skin < SKIN_COUNT)
	{
		m_skin = skin;
	}
}

void Character::ApplyMotion(sf::Vector2f velocity)
{
	static const int MAX_ITERATIONS = 100;
	sf::Vector2f positionStep;
	sf::Vector2f position = m_sprite.getPosition();

	positionStep.x = velocity.x * 0.16f;
	positionStep.y = velocity.y * 0.16f;

	for (int i = 0; i < MAX_ITERATIONS; ++i)
	{
		// Collide on x
		if (ValidatePosition(position.x + positionStep.x, position.y))
		{
			position.x += positionStep.x;
		}
		else
		{
			m_velocity.x = 0;
		}

		// Collide on y
		if (ValidatePosition(position.x, position.y + positionStep.y))
		{
			position.y += positionStep.y;
		}
		else
		{
			m_velocity.y = 0;
		}

		velocity -= positionStep;

		// Break conditions
		if (positionStep.x > 0 && velocity.x <= 0) positionStep.x = 0;	// Got stopped moving left
		if (positionStep.x < 0 && velocity.x >= 0) positionStep.x = 0;	// Got stopped moving right

		if (positionStep.y > 0 && velocity.y <= 0) positionStep.y = 0;	// Got stopped moving up
		if (positionStep.y < 0 && velocity.y >= 0) positionStep.y = 0;	// Got stopped moving down

		if (velocity.x == 0) positionStep.x = 0;						// Not moving anymore
		if (velocity.y == 0) positionStep.y = 0;

		// Break if not moving or no change in position
		if (velocity.x   == 0 && velocity.y   == 0)   break;
		if (positionStep.x  == 0 && positionStep.y  == 0)   break;
	}

	m_sprite.setPosition(position);
}

bool Character::ValidatePosition(float x, float y)
{
	bool posValid = true;
	const TileMap& map = GetCurrentMap();
	
	// Collision check on map
	int startX = (int) ((x + s_characterCollisionOffsets[m_facing].left) / map.GetTileWidth());
	int startY = (int) ((y + s_characterCollisionOffsets[m_facing].top) / map.GetTileHeight());
	int endX   = (int) ((x + m_sprite.getLocalBounds().width - s_characterCollisionOffsets[m_facing].width) / map.GetTileWidth());
	int endY   = (int) ((y + m_sprite.getLocalBounds().height - s_characterCollisionOffsets[m_facing].height) / map.GetTileHeight());

	for (int iy = startY; iy <= endY; ++iy)
	{
		for (int ix = startX; ix <= endX; ++ix)
		{
			Actor* tileActor = map.GetTileActorAt(ix, iy, 1);
			if (tileActor)
			{
				if (tileActor->GetType() == "TileActor")
				{
					posValid = false;
				}
				else if (tileActor->GetType() == "ItemActor")
				{
					pItemActor = static_cast<ItemActor*>(tileActor);
				}
			}
		}
	}

	return posValid;
}

/*virtual*/ sf::IntRect Character::GetRect()
{
	//TODO (daniel) make sprite have actual hitbox

	sf::IntRect rect = SpriteActor::GetRect();
	//rect.left += 8;
	//rect.top += 16;
	//rect.width -= 16;
	//rect.height -= 16;
	return rect;
}