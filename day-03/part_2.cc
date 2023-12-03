#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr char empty_symbol{'.'};
constexpr char gear_symbol{'*'};

struct Position
{
	size_t row;
	size_t column;
};

struct PossibleGear
{
	int count;
	int value;
};

auto update_possible_gear(
		size_t row,
		size_t column,
		int value,
		std::unordered_map <int, std::unordered_map <int, PossibleGear>>& possible_gears,
		std::vector <Position>& gear_positions)
{
	PossibleGear& gear{possible_gears[row][column]};

	int initial_count{gear.count};

	gear.count += 1;

	if (initial_count == 0)
	{
		gear.value = value;
		gear_positions.emplace_back(Position{row, column});
	}
	else
	{
		gear.value *= value;
	}
}

auto check_if_possible_gear(
		std::string const& top_line,
		std::string const& middle_line,
		std::string const& bottom_line,
		std::unordered_map <int, std::unordered_map <int, PossibleGear>>& possible_gears,
		std::vector <Position>& gear_positions,
		size_t line_number,
		size_t i,
		int value,
		int value_start)
{
	size_t check_index{i};
	char check_symbol{};

	size_t const line_length{top_line.size()};

	// check to the right
	if (check_index != line_length)
	{
		check_symbol = middle_line[check_index];
		if (check_symbol == gear_symbol)
		{
			update_possible_gear(line_number, check_index, value, possible_gears, gear_positions);
		}

		// check diagonally below and to the right
		check_symbol = bottom_line[check_index];
		if (check_symbol == gear_symbol)
		{
			update_possible_gear(line_number + 1, check_index, value, possible_gears, gear_positions);
		}
	}

	check_index -= 1;

	// check below the number and diagonally to the left
	for (; int(check_index) >= value_start - 1; check_index -= 1)
	{
		check_symbol = bottom_line[check_index];

		if (check_symbol == gear_symbol)
		{
			update_possible_gear(line_number + 1, check_index, value, possible_gears, gear_positions);
		}

		if (check_index == 0 || int(check_index) == (value_start - 1)) { break; }
	}

	// check to the left
	check_symbol = middle_line[check_index];

	if (check_symbol == gear_symbol)
	{
		update_possible_gear(line_number, check_index, value, possible_gears, gear_positions);
	}

	// check diagonally above and to the left
	check_symbol = top_line[check_index];

	if (check_symbol == gear_symbol)
	{
		update_possible_gear(line_number, check_index, value, possible_gears, gear_positions);
	}

	// check above the number and diagonally to the right
	for (; check_index <= i; check_index += 1)
	{
		check_symbol = top_line[check_index];

		if (check_symbol == gear_symbol)
		{
			update_possible_gear(line_number - 1, check_index, value, possible_gears, gear_positions);
		}

		if (check_index == line_length) { break; }
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part1 input_file\n";
		return 1;
	}

	std::ifstream input_file (argv[1]);

	std::string empty_line_1;
	std::string empty_line_2;
	std::string first_line;
	std::string second_line;

	std::getline(input_file, first_line);
	std::getline(input_file, second_line);

	size_t const line_length{first_line.size()};
	empty_line_1.resize(line_length);
	empty_line_2.resize(line_length);

	for (size_t i{0}; i < line_length; i += 1)
	{
		empty_line_1[i] = empty_symbol;
		empty_line_2[i] = empty_symbol;
	}

	std::string& top_line{empty_line_1};
	std::string& middle_line{first_line};
	std::string& bottom_line{second_line};

	int line_number{0};

	std::unordered_map <int, std::unordered_map <int, PossibleGear>> possible_gears;
	std::vector <Position> gear_positions{};

	for (;;)
	{
		int value{-1};
		int value_start{-1};

		for (size_t i{0}; i < line_length; i += 1)
		{
			char const current_symbol{middle_line[i]};

			if (std::isdigit(current_symbol))
			{
				// reading a number
				int digit{current_symbol - '0'};

				if (value == -1)
				{
					value_start = i;
					value = digit;
				}
				else
				{
					value *= 10;
					value += digit;
				}
			}
			else
			{
				if (value > 0)
				{
					// read a part symbol directly after a part number

					check_if_possible_gear(
						top_line,
						middle_line,
						bottom_line,
						possible_gears,
						gear_positions,
						line_number,
						i,
						value,
						value_start
					);

					value = -1;
					value_start = -1;
				}
			}
		}
		if (value > 0)
		{
			// line ended with a number

			check_if_possible_gear(
				top_line,
				middle_line,
				bottom_line,
				possible_gears,
				gear_positions,
				line_number,
				line_length,
				value,
				value_start
			);

			value = -1;
			value_start = -1;
		}

		line_number += 1;

		top_line = middle_line;
		middle_line = bottom_line;
		if (std::getline(input_file, bottom_line)) { continue; }
		else { bottom_line = empty_line_2; }

		if (middle_line == bottom_line) { break; }
	}

	int sum{0};

	for (auto& position : gear_positions)
	{
		PossibleGear& gear{possible_gears[position.row][position.column]};

		if (gear.count == 2) { sum += (gear.value); }
	}

	std::cout << "Sum: " << sum << '\n';

	return 0;
}

