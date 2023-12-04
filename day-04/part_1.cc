#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>


constexpr int num_player_numbers{10};
constexpr int num_winning_numbers{25};
//constexpr int num_player_numbers{5};
//constexpr int num_winning_numbers{8};

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	// locations for strings read from files
	std::string card;
	int card_id_number;
	std::string colon;
	std::string pipe;
	int card_number;

	// each line will be split and stored in an array
	std::vector <int> player_numbers;
	std::vector <int> winning_numbers;

	std::string line{};
	int total_points{0};

	while (std::getline(input_file, line))
	{
		std::istringstream iss{line};

		// read unused line elements
		iss >> card >> card_id_number >> colon;

		// read player numbers
		for (int i{0}; i < num_player_numbers; i += 1)
		{
			iss >> card_number;
			player_numbers.emplace_back(card_number);
			std::sort(player_numbers.begin(), player_numbers.end());
		}

		iss >> pipe;

		// read winning numbers
		for (int i{0}; i < num_winning_numbers; i += 1)
		{
			iss >> card_number;
			winning_numbers.emplace_back(card_number);
			std::sort(winning_numbers.begin(), winning_numbers.end());
		}

		int player_index{0};
		int winners_index{0};
		int matches{0};

		// count matches
		for (;;)
		{
			int player_number{player_numbers[player_index]};
			int winning_number{winning_numbers[winners_index]};

			if (player_number == winning_number)
			{
				player_index += 1;
				winners_index += 1;

				matches += 1;

				if (player_index == num_player_numbers || winners_index == num_winning_numbers) { break; }
			}
			else if (player_number < winning_number)
			{
				player_index += 1;

				if (player_index == num_player_numbers) { break; }
			}
			else
			{
				winners_index += 1;

				if (winners_index == num_winning_numbers) { break; }
			}
		}

		if (matches > 0) { total_points += (1 << (matches - 1));}

		matches = 0;

		player_numbers.clear();
		winning_numbers.clear();
	}

	std::cout << '\n' << "Points: " << total_points << '\n';;

	return 0;
}

