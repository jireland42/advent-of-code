#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::string line{};

	// seeds: 565778304 ...
	//
	std::getline(input_file, line);
	std::istringstream line_stream{line};
	std::getline(input_file, line);

	std::string header{};
	std::string colon{};
	size_t seed_type{};
	size_t destination_range_start{};
	size_t source_range_start{};
	size_t range_length{};

	line_stream >> header;

	std::vector<size_t> container_1;

	while (line_stream >> seed_type)
	{
		container_1.emplace_back(seed_type);
	}

	std::vector<size_t> container_2(container_1.size());

	std::vector<size_t>& current_container{container_1};
	std::vector<size_t>& mapped_container{container_2};

	auto mapping_container_start{mapped_container.begin()};

	for (;;)
	{
		// x-to-y map:
		if (!std::getline(input_file, line)) { break; }

		std::sort(current_container.begin(), current_container.end());

		// 1136439539 28187015 34421000
		// ...
		while (std::getline(input_file, line))
		{
			// reset stream for reading
			line_stream.str(line);
			line_stream.clear();

			// read the value into variables
			line_stream >> destination_range_start;
			if (line_stream.fail()) { break; }
			line_stream >> source_range_start;
			line_stream >> range_length;

			// if we finished mapping all the elements for this round we don't
			// have to look at any more of the maps, just process the input file
			// to find the next mapping stage
			if (current_container.size() == 0) { continue; }

			// determine the range of values to which the current map applies
			auto left_endpoint{
				std::lower_bound(
					current_container.cbegin(),
					current_container.cend(),
					source_range_start
				)
			};
			auto right_endpoint{
				std::upper_bound(
					current_container.cbegin(),
					current_container.cend(),
					source_range_start + range_length - 1
				)
			};

			// if the map applies to any values, then map them to the mapped
			// values array
			if (left_endpoint != right_endpoint)
			{
				mapping_container_start = std::transform(
					left_endpoint,
					right_endpoint,
					mapping_container_start,
					[source_range_start, destination_range_start](size_t value){
						size_t offset{value - source_range_start};
						return destination_range_start + offset;
					}
				);
			}

			// removed mapped items from working array
			current_container.erase(left_endpoint, right_endpoint);
		}

		// copy remaining items that map to themselves 
		std::copy(
			current_container.cbegin(),
			current_container.cend(),
			mapping_container_start
		);

		// set container references for next pass
		std::swap(current_container, mapped_container);
		mapped_container.resize(current_container.size());
		mapping_container_start = mapped_container.begin();
	}

	// check the set of locations to find the closest location
	auto closest_location{
		std::accumulate(
			current_container.begin(),
			current_container.end(),
			std::numeric_limits<size_t>::max(),
			[](size_t lhs, size_t rhs) { return std::min(lhs, rhs); }
		)
	};

	std::cout << "Closest Location: " << closest_location << '\n';

	return 0;
}

