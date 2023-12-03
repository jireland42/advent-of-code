#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr char empty_symbol{'.'};

auto does_identify_part(
		std::string const& top_line,
		std::string const& middle_line,
		std::string const& bottom_line,
		size_t i,
		int value_start) -> bool
{
	size_t check_index{i};
	char check_symbol{};

	// check diagonally below and to the right
	check_symbol = bottom_line[check_index];
	if (check_symbol != empty_symbol && !std::isdigit(check_symbol)) { return true; }

	check_index -= 1;

	// check below the number and diagonally to the left
	for (; int(check_index) >= value_start - 1; check_index -= 1)
	{
		check_symbol = bottom_line[check_index];

		if (check_symbol != empty_symbol && !std::isdigit(check_symbol)) { return true; }
		if (check_index == 0 || int(check_index) == (value_start - 1)) { break; }
	}

	// check to the left
	check_symbol = middle_line[check_index];

	if (check_symbol != empty_symbol && !std::isdigit(check_symbol)) { return true; }

	// check diagonally above and to the left
	check_symbol = top_line[check_index];

	if (check_symbol != empty_symbol && !std::isdigit(check_symbol)) { return true; }

	// check above the number and diagonally to the right
	for (; check_index <= i; check_index += 1)
	{
		check_symbol = top_line[check_index];

		if (check_symbol != empty_symbol && !std::isdigit(check_symbol)) { return true; }
	}

	return false;
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

	int sum{0};

	for (;;)
	{
		int value{0};
		int value_start{-1};

		for (size_t i{0}; i < line_length; i += 1)
		{
			char const current_symbol{middle_line[i]};

			if (current_symbol == empty_symbol)
			{
				// reading an empty placeholder symbol
				if (value > 0)
				{
					if (does_identify_part(top_line, middle_line, bottom_line, i, value_start)) { sum += value; };

					value = 0;
					value_start = -1;
				}
			}
			else if (std::isdigit(current_symbol))
			{
				// reading a number
				if (value == 0) { value_start = i; }

				value *= 10;
				value += current_symbol - '0';
			}
			else
			{
				if (value > 0)
				{
					// read a part symbol directly after a part number

					sum += value;
					if (does_identify_part(top_line, middle_line, bottom_line, i, value_start)) { sum += value; };

					value = 0;
					value_start = -1;
				}
			}
		}
		if (value != 0)
		{
			// line ended with a number

			if (does_identify_part(top_line, middle_line, bottom_line, line_length - 1, value_start)) { sum += value; };

			value = 0;
			value_start = -1;
		}

		top_line = middle_line;
		middle_line = bottom_line;
		if (std::getline(input_file, bottom_line)) { continue; }
		else { bottom_line = empty_line_2; }

		if (middle_line == bottom_line) { break; }
	}

	std::cout << "Sum: " << sum << '\n';
	// 542422
	// 578472
	// don't add part numbers twice

	return 0;
}
