#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::string time_line{};
	std::string distance_line{};

	std::getline(input_file, time_line);
	std::getline(input_file, distance_line);

	std::istringstream time_line_stream(time_line);
	std::istringstream distance_line_stream(distance_line);

	std::string heading{};
	time_line_stream >> heading;
	distance_line_stream >> heading;

	int race_duration{};
	int record_distance{};

	size_t winning_combo_product{1};

	while (time_line_stream >> race_duration)
	{
		distance_line_stream >> record_distance;

		int discriminant{race_duration * race_duration - 4 * record_distance};

		auto sol_left{(race_duration - std::sqrt(discriminant)) / 2};

		bool exclude_boundary{false};
		float integer_part{};
		if (std::modf(sol_left, &integer_part) == 0.0) { exclude_boundary = true; }

		double hold_time_left{ceil((race_duration - std::sqrt(discriminant)) / 2)};
		double hold_time_right{std::floor((race_duration + std::sqrt(discriminant)) / 2)};

		double winning_times{hold_time_right - hold_time_left + 1.0};
		if (exclude_boundary) { winning_times -= 2; }

		winning_combo_product *= winning_times;
	}

	std::cout << '\n' << "Product: " << winning_combo_product << '\n';;

	return 0;
}

