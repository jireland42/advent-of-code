#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

auto predict_next_reading(std::vector<int>& sensor_readings) -> int
{
	bool all_zero{true};

	size_t i{sensor_readings.size() - 1};
	size_t j{i};

	// destruct
	for(; i > 0; i -= 1)
	{
		for(j = 0; j < i; j += 1)
		{
			int reading_delta{sensor_readings[j + 1] - sensor_readings[j]};

			if (reading_delta != 0) { all_zero = false; }

			sensor_readings[j] = reading_delta;
		}

		if (all_zero) { break; }

		all_zero = true;
	}

	int predicted_sensor_reading{0};

	// rebuild
	for(; i < sensor_readings.size(); i += 1)
	{
		for(j = i - 1;; j -= 1)
		{
			int reading_difference{sensor_readings[j + 1] - sensor_readings[j]};

			sensor_readings[j] = reading_difference;

			if (j == 0)
			{
				predicted_sensor_reading = sensor_readings[0] - predicted_sensor_reading;
				break;
			}
		}
	}

	return predicted_sensor_reading;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::istringstream line_stream;
	std::string line{};
	std::vector<int> sensor_readings;

	std::getline(input_file, line);
	line_stream.str(line);

	int reading;
	while(line_stream >> reading)
	{
		sensor_readings.emplace_back(reading);
	}

	size_t num_readings_per_sensor{sensor_readings.size()};

	std::vector<int> predicted_sensor_readings;

	for(;;)
	{
		int predicted_sensor_reading{predict_next_reading(sensor_readings)};
		predicted_sensor_readings.emplace_back(predicted_sensor_reading);

		if (!std::getline(input_file, line)) { break; }

		line_stream.str(line);
		line_stream.clear();

		for(size_t i{0}; i < num_readings_per_sensor; i += 1)
		{
			line_stream >> reading;
			sensor_readings[i] = reading;
		}
	}

	int sum_of_predicitons{
		std::reduce(predicted_sensor_readings.cbegin(), predicted_sensor_readings.cend())
	};

	std::cout << '\n' << "Prediction Sum: " << sum_of_predicitons << '\n';

	return 0;
}


