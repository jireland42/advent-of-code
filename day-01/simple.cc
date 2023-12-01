#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./simple input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);
	std::string line;

	int first{-1};
	int last{-1};
	int sum{0};

	while (std::getline(input_file, line))
	{
		size_t i{0};

		// set first and last
		for (; i < line.size(); i += 1)
		{
			char character = line[i];

			if (std::isdigit(character))
			{
				first = character - '0';
				last = first;

				i += 1;

				break;
			}
		}

		// set last if there is more than one digit on the line
		for (; i < line.size(); i += 1)
		{
			char character = line[i];

			if (std::isdigit(character)) { last = character - '0'; }
		}

		if (first == -1) { continue; }

		sum += (10 * first + last);

		std::cout << (10 * first + last) << '\n';

		first = -1;
		last = -1;
	}

	std::cout << "\nSum: " << sum << '\n';

	return 0;
}
