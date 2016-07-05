#pragma once

#include <vector>
#include <string>

class Tokenizer
{
public:
	Tokenizer(const std::string& str, const std::string& delimiter);
	const std::string& operator[](int i) const;
	size_t size() const;

private:
	std::vector<std::string> m_tokens;
};


inline Tokenizer::Tokenizer(const std::string& str, const std::string& delimiter)
{
	size_t start = 0;
	size_t end = std::string::npos;

	for (;;)
	{
		end = str.find(delimiter, start);
		m_tokens.push_back(str.substr(start, end - start));

		if (end == std::string::npos)
		{
			break;
		}

		start = end + delimiter.size();
	}
}

inline const std::string& Tokenizer::operator[](int i) const
{
	return m_tokens.at(i);
}

inline size_t Tokenizer::size() const
{
	return m_tokens.size();
}