#include "encryption.h"

std::string Encryption::encWeakKeyXOR(std::string input, std::string key)
{
	// Make sure the key is at least as long as the message
	std::string tmp(key);
	while (key.size() < input.size())
		key += tmp;

	// And now for the encryption part
	for (std::string::size_type i = 0; i < input.size(); ++i)
		input[i] ^= key[i];
	return input;
}
std::string Encryption::decWeakKeyXOR(std::string input, std::string key)
{
	return encWeakKeyXOR(input, key); // lol
}