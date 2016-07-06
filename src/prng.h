#pragma once

class PRNG
{
public:
	typedef unsigned long long int seed_t;

	PRNG(seed_t seed = 0xDEAD0BEAF0F4F3F2);

	void SetSeed(seed_t seed);
	seed_t GetSeed() const;

	// [0, 1]
	float Random() const;

	int NextInt() const;
	unsigned int NextUInt() const;
	size_t NextIndex(size_t length) const;
	bool NextBool() const;

	// Get a random number in the given range (be careful of numeric limits)
	// Value is in [min, max)
	template<typename TReal>
	TReal RandomInRange(TReal min, TReal max) const
	{
		return static_cast<TReal>(min + (max - min) * Random());
	}

private:
	unsigned int Rand() const;
	seed_t m_seed;
	mutable unsigned int m_generator[4];
};

inline PRNG::seed_t PRNG::GetSeed() const
{
	return m_seed;
}