// хаффман.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cassert>
#include <string>
#include <tuple>
#include <cmath>
#include <vector>
#include <queue>
#include <cstddef>
#include <unordered_map>
#include <algorithm>
#include <fstream>


class Huffman
{
	struct CharSetFrequency
	{
		std::string char_set;
		int frequency;
		bool operator < (const CharSetFrequency& other) const
		{
			return std::tie(frequency, char_set) > std::tie(other.frequency, other.char_set);
		}
	};
public:
	static std::unordered_map<char, std::string> encode(const std::string& text);
	static std::string decode(const std::string& text, const std::unordered_map<char, std::string>& huffman_encoding);
};



std::unordered_map<char, std::string> Huffman::encode(const std::string& text)
{
	std::unordered_map<char, int> char_frequencies;
	for (auto c : text)
	{
		char_frequencies[c]++;
	}
	std::vector<CharSetFrequency> frequencies;
	for (auto char_frequency : char_frequencies)
	{
		frequencies.push_back({ std::string(1, char_frequency.first), char_frequency.second });
	}


	if (frequencies.size() == 1)
	{
		std::unordered_map<char, std::string> result;
		result[frequencies[0].char_set[0]] = "0";
		return result;
	}
	std::unordered_map<char, std::string> result;
	std::priority_queue<CharSetFrequency> q(frequencies.begin(), frequencies.end());
	while (q.size() >= 2)
	{
		auto first = q.top();
		q.pop();
		auto second = q.top();
		q.pop();
		for (auto c : first.char_set)
		{
			result[c] = "0" + result[c];
		}
		for (auto c : second.char_set)
		{
			result[c] = "1" + result[c];
		}
		q.push({ first.char_set + second.char_set, first.frequency + second.frequency });
	}
	return result;

}

std::string Huffman::decode(const std::string& text, const std::unordered_map<char, std::string>& huffman_encoding)
{
	size_t len = text.size();
	size_t pos = 0;
	std::string result;
	while (pos < len)
	{
		for (auto& encoded : huffman_encoding)
		{
			if (text.substr(pos, encoded.second.size()) == encoded.second)
			{
				result += encoded.first;
				pos += encoded.second.size();
				break;
			}
		}
	}
	return result;
}

int main(void)
{
	std::string text;
	
	std::string file;
	std::cout << "Enter filename: ";
	std::cin >> file;
	std::ifstream in(file);
	
	std::string txt = "";
	if (in.is_open())
	{
		while (getline(in, text))
		{
			std::cout << text << std::endl;			
		}
		
	}
	int num = text.size();   
	
	//int x = huffman_encoding.size();
	auto huffman_encoding = Huffman::encode(text);
	std::string encoded_text;
	for (auto c : text)
	{
		encoded_text += huffman_encoding[c];
	}
	std::cout << huffman_encoding.size() << " " << encoded_text.size() << std::endl;
	for (auto& encoded : huffman_encoding)
	{
		std::cout << encoded.first << ": " << encoded.second << std::endl;
	}
	int x = huffman_encoding.size();
	float entropy = ceil(std::log2(num / x));
	float entropy_ravnom = ceil(std::log2( encoded_text.size() / x )); 
	std::cout << encoded_text << std::endl;

	std::cout << entropy_ravnom  * encoded_text.size()<< std::endl;
	std::cout << entropy * encoded_text.size() << std::endl;
	std::cout << entropy_ravnom * encoded_text.size() - entropy * encoded_text.size() << std::endl;
}