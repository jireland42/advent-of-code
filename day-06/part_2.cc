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

	size_t race_duration{};
	size_t record_distance{};

	time_line_stream >> race_duration;
	distance_line_stream >> record_distance;

	size_t discriminant{race_duration * race_duration - 4 * record_distance};

	size_t hold_time_left{size_t(ceil((race_duration - std::sqrt(discriminant)) / 2))};
	size_t hold_time_right{size_t(std::floor((race_duration + std::sqrt(discriminant)) / 2))};

	size_t winning_times{hold_time_right - hold_time_left + 1};

	std::cout << '\n' << "Winning Hold Times: " << winning_times << '\n';;

	return 0;
}

