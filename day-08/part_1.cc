#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::string sequence_line;
	std::getline(input_file, sequence_line);

	std::string line{};
	std::unordered_map<std::string, std::pair<std::string, std::string>> network;

	// clear blank line
	std::getline(input_file, line);

	std::regex pattern{"(.*) = \\(([[:upper:]]{3}), ([[:upper:]]{3})\\)"};

	while (std::getline(input_file, line))
	{
		auto pos{line.cbegin()};
		auto end{line.cend()};
		std::smatch match_results;

		std::regex_search(pos, end, match_results, pattern);

		network[match_results.str(1)] = std::pair{match_results.str(2), match_results.str(3)};
	}

	size_t i{0};

	std::string start{"AAA"};
	std::string& network_position{start};

	for (;;)
	{
		size_t index{i % sequence_line.size()};
		char step_direction{sequence_line[index]};

		auto map_fragment{network[network_position]};

		switch(step_direction)
		{
			case 'L':
				network_position = map_fragment.first;
				break;
			case 'R':
				network_position = map_fragment.second;
				break;
		}

		i += 1;

		if (network_position == "ZZZ") { break; }
	}

	std::cout << '\n' << "Steps: " << i << '\n';

	return 0;
}

