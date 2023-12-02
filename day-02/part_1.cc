#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>

bool is_valid_colour_amount(std::string const& colour, int const num_of_colour)
{
	constexpr int max_red{12};
	constexpr int max_green{13};
	constexpr int max_blue{14};

	int max_to_use{};

	if (colour == "red") { max_to_use = max_red; }
	else if (colour == "green") { max_to_use = max_green; }
	else if (colour == "blue") { max_to_use = max_blue; }

	return (num_of_colour <= max_to_use);
}

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
		line.append(";");

		auto search_start{line.cbegin()};
		std::smatch game_id_results;
		std::smatch round_results;
		std::smatch colour_results;

		std::regex_search(search_start, line.cend(), game_id_results, game_id_pattern);
		search_start = game_id_results.suffix().first;

		int id{std::stoi(game_id_results[1])};

		while (std::regex_search(search_start, line.cend(), round_results, game_round_pattern))
		{
			std::string round_result{round_results[1]};

			search_start = round_result.cbegin();
			while (std::regex_search(search_start, round_result.cend(), colour_results, round_colour_pattern))
			{
				int num_of_colour{std::stoi(colour_results[1])};
				std::string colour{colour_results[2]};

				search_start = colour_results.suffix().first;

				if (!is_valid_colour_amount(colour, num_of_colour))
				{
					goto nextGame;
				}
			}

			search_start = round_results.suffix().first;
		}

		sum += id;

		nextGame:;
	}

	std::cout << "Sum: " << sum << '\n';

	return 0;
}
