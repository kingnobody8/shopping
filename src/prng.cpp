#include "prng.h"
#include <stdio.h>
#include <limits.h>

PRNG::PRNG(seed_t seed)
{
	SetSeed(seed);
}

void PRNG::SetSeed(seed_t seed)
{
	m_seed = seed;

	unsigned int x = m_seed >> 48;
	unsigned int y = (m_seed & 0x0000FFFFFFFFFFFF) >> 32;
	unsigned int z = (m_seed & 0x00000000FFFFFFFF) >> 16;
	unsigned int w = (m_seed & 0x000000000000FFFF);

	// Note:
	// If these are 0 then the rng will produce undefined results
	if (x == 0 && y == 0 && z == 0 && w == 0)
	{
		printf("Invalid seed: %X (using default seed)\n", seed);
		SetSeed(0xDEAD0BEAF0F4F3F2);
		return;
	}

	// u64 was used as a seed but is missing upper bytes
	if (x == 0 && y != 0 && z != 0 && w != 0)
	{
		x = z * 2;
	}
	// u32 was used as a seed
	else if (x == 0 && y == 0 && z != 0 && w != 0)
	{
		x = z * 2;
		y = w * 4;
	}
	// u16 or u8 was used as a seed
	else if (x == 0 && y == 0 && z == 0 && w != 0)
	{
		x = w * 2;
		y = w * 4;
		z = w * 8;
	}

	x *= y;
	y *= z;
	z *= w;
	w *= x;

	m_generator[0] = x;
	m_generator[1] = y;
	m_generator[2] = z;
	m_generator[3] = w;

	// Poll rng once to get things in place
	Rand();
}

float PRNG::Random() const
{
	return static_cast<float>(Rand()) / UINT_MAX;
}

int PRNG::NextInt() const
{
	return static_cast<int>(Rand());
}

unsigned int PRNG::NextUInt() const
{
	return Rand();
}

size_t PRNG::NextIndex(size_t length) const
{
	return length == 0 ? 0 : NextUInt() % length;
}

bool PRNG::NextBool() const
{
	return (Rand() & 0x00000001) == 0;
}

unsigned int PRNG::Rand() const
{
	// An Xorshift, see: https://en.wikipedia.org/wiki/Xorshift

	unsigned int temp = m_generator[0] ^ (m_generator[0] << 11);

	m_generator[0] = m_generator[1];
	m_generator[1] = m_generator[2];
	m_generator[2] = m_generator[3];

	return m_generator[3] = m_generator[3] ^ (m_generator[3] >> 19) ^ (temp ^ (temp >> 8));
}