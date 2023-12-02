#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::string line;

	std::regex game_id_pattern{"Game ([[:d:]]+):"};
	std::regex game_round_pattern{"([^;]+;)"};
	std::regex round_colour_pattern{" ([[:d:]]+) ([[:lower:]]+)[,;]"};

	int sum{};

	while(std::getline(input_file, line))
	{
		int max_red_seen{0};
		int max_green_seen{0};
		int max_blue_seen{0};

		line.append(";");

		auto search_start{line.cbegin()};
		std::smatch game_id_results;
		std::smatch round_results;
		std::smatch colour_results;

		std::regex_search(search_start, line.cend(), game_id_results, game_id_pattern);
		search_start = game_id_results.suffix().first;

		while (std::regex_search(search_start, line.cend(), round_results, game_round_pattern))
		{
			std::string round_result{round_results[1]};

			search_start = round_result.cbegin();
			while (std::regex_search(search_start, round_result.cend(), colour_results, round_colour_pattern))
			{
				int num_of_colour{std::stoi(colour_results[1])};
				std::string colour{colour_results[2]};

				int* max_seen_of_colour{&max_red_seen};

				if (colour == "green") { max_seen_of_colour = &max_green_seen; }
				else if (colour == "blue") { max_seen_of_colour = &max_blue_seen; }

				if (*max_seen_of_colour < num_of_colour) { *max_seen_of_colour = num_of_colour; }

				search_start = colour_results.suffix().first;
			}

			search_start = round_results.suffix().first;
		}

		int power{1};
		if (max_red_seen != 0) { power *= max_red_seen; }
		if (max_green_seen != 0) { power *= max_green_seen; }
		if (max_blue_seen != 0) { power *= max_blue_seen; }

		sum += power;
	}

	std::cout << "Sum: " << sum << '\n';

	return 0;
}

