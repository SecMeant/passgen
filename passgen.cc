#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cstring>
#include <cctype>

using namespace std::literals;

const char * const def_charrange = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";

inline void usage(const char *programname)
{std::cout << "Usage: " << programname << " <password-chars-pool> <password-length>\n";}

// safe stoi
int sstoi(const std::string& str)
{
	for(auto itb=str.begin(), ite=str.end(); itb != ite; ++itb)
	{
		if(!isdigit(*itb)) throw std::invalid_argument("password length cannot contains chars");
	}
	return std::stoi(str);
}

int main(int argc, char**argv)
{
	std::ifstream furandom;
	std::string passwd;
	std::string availChars;
	int32_t passwdLength;

	if (argc < 3)
	{	
		usage(argv[0]);
		return 1;
	}
	else
	{
		*argv[1] == '?' ? availChars = def_charrange: availChars = argv[1];
		passwdLength = sstoi(std::string(argv[2]));
	}

	furandom.open("/dev/urandom", std::ios::binary | std::ios::in);

	for(auto i=0; i < passwdLength;)
	{
		unsigned char ch = furandom.get();
		if (availChars.find(ch) != std::string::npos)
		{
			std::cout << ch;
			i++;
		}
	}
	std::cout << "\n";

	return 0;
}
