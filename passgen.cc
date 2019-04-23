#include <iostream>
#include <fstream>

#ifdef _WIN32
	#define _CRT_RAND_S
	#include <stdlib.h>
#endif

using namespace std::literals;

class RandomGenerator
{
	using random_value_t = unsigned int;

	const std::string char_pool;

	#ifdef __linux__
	std::ifstream furandom;
	#endif

	void initRandomDevice()
	{
		#ifdef __linux__
			if (not furandom.is_open())
				furandom.open("/dev/urandom", std::ios::binary | std::ios::in);
			else
				throw std::runtime_error("Random generator is already initilized!");
		#endif
	}

	random_value_t getRandomValue()
	{
		random_value_t random_value;

		#ifdef _WIN32
		rand_s(&random_value);

		if(!random_value)
			throw std::runtime_error("System random number generator failed to generate value.");
		
		return random_value;
		#else

		this->furandom.read(reinterpret_cast<char *>(&random_value), sizeof(random_value));
		return random_value;

		#endif
	}

public:

	template <class T>
	RandomGenerator(T&& char_pool)
	: char_pool{std::forward<T>(char_pool)}
	{
		this->initRandomDevice();
	}

	RandomGenerator() = delete;
	RandomGenerator& operator=(RandomGenerator&&) = delete;
	RandomGenerator& operator=(const RandomGenerator&) = delete;

	unsigned char getRandomChar()
	{
		return this->char_pool[this->getRandomValue()%this->char_pool.size()];
	}
};

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

template<typename T, std::size_t N>
std::size_t array_length(const T array[N])
{return N;}

int main(int argc, char**argv)
{
	const char * def_charpool = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
	std::string passwd;
	std::string availChars;
	int32_t passwdLength;

	if (argc <= 2)
	{	
		usage(argv[0]);
		return 1;
	}
	else
	{
		if(*argv[1] == '?')
			availChars = def_charpool;
		else
			availChars = argv[1];

		passwdLength = sstoi(std::string(argv[2]));
	}

	RandomGenerator random_generator(availChars);

	for(auto i=0; i < passwdLength; ++i)
	{
		unsigned char ch = random_generator.getRandomChar();
		putchar(ch);
	}
	putchar('\n');

	return 0;
}
