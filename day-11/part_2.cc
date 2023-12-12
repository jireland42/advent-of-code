#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Galaxy
{
	size_t row;
	size_t col;
};

auto distance(Galaxy const& g1, Galaxy const& g2) -> size_t
{
	size_t row_distance;
	size_t col_distance;

	if (g1.row > g2.row) { row_distance = g1.row - g2.row; }
	else { row_distance = g2.row - g1.row; }

	if (g1.col > g2.col) { col_distance = g1.col - g2.col; }
	else { col_distance = g2.col - g1.col; }

	return row_distance + col_distance;
}

int main(int argc, char* argv[])
{
	constexpr size_t expansion_factor{1000000};
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::vector<Galaxy> galaxies;

	std::vector<bool> empty_rows;
	std::vector<bool> empty_columns;

	std::string line;

	std::getline(input_file, line);

	size_t row{0};
	bool is_row_empty{true};

	for (size_t i{0}; i < line.size(); i += 1)
	{
		char space_object{line[i]};

		if (space_object == '#')
		{
			galaxies.emplace_back(Galaxy{row, i});
			is_row_empty = false;
			empty_columns.emplace_back(false);
		}
		else
		{
			empty_columns.emplace_back(true);
		}
	}

	if (is_row_empty) { row += (expansion_factor - 1); }

	while (std::getline(input_file, line))
	{
		row += 1;
		is_row_empty = true;

		for (size_t i{0}; i < line.size(); i += 1)
		{
			char space_object{line[i]};

			if (space_object == '#')
			{
				galaxies.emplace_back(Galaxy{row, i});
				is_row_empty = false;
				empty_columns[i] = false;
			}
		}

		if (is_row_empty) { row += (expansion_factor - 1); }
	}

	size_t cumulative_shift{0};
	std::vector<size_t> cumulative_shifts;

	for (auto is_column_empty : empty_columns)
	{
		cumulative_shifts.emplace_back(cumulative_shift);

		if (is_column_empty) { cumulative_shift += (expansion_factor - 1); }
	}

	std::sort(
		galaxies.begin(),
		galaxies.end(),
		[](Galaxy const& lhs, Galaxy const& rhs) { return lhs.col < rhs.col; }
	);

	for (size_t i{0}; i < galaxies.size(); i += 1)
	{
		galaxies[i].col += cumulative_shifts[galaxies[i].col];
	}

	size_t cumulative_distance{0};

	for (size_t i{0}; i < galaxies.size() - 1; i += 1)
	{
		for (size_t j{i + 1}; j < galaxies.size(); j += 1)
		{
			cumulative_distance += distance(galaxies[i], galaxies[j]);
		}
	}

	std::cout << '\n' << "Cumulative Distance: " << cumulative_distance << '\n';;

	// 483845200392 is too high
	// 483844716556
	return 0;
}


