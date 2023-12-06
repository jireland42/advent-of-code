#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

auto f() -> size_t
{
	static size_t i{0};
	return i++;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);
	std::ostringstream read_only_file_string;
	std::istringstream file_string;

	std::string line{};
	std::string header{};

	// seeds: 565778304 ...
	//
	std::getline(input_file, line);
	std::istringstream line_stream{line};
	std::getline(input_file, line);
	line_stream >> header;

	while(std::getline(input_file, line)) { read_only_file_string << line << '\n'; }

	size_t seed_type{};
	size_t seed_range{};

	size_t destination_range_start{};
	size_t source_range_start{};
	size_t range_length{};

	std::vector<std::pair<size_t,size_t>> seed_pairs;

	while (line_stream >> seed_type >> seed_range)
	{
		seed_pairs.emplace_back(seed_type, seed_range);
	}

	std::sort(
		seed_pairs.begin(),
		seed_pairs.end(),
		[](auto const& lhs, auto const& rhs){ return lhs.second < rhs.second; }
	);

	//size_t largest_range{seed_base_range_pairs[0].second};
	//std::vector<size_t> seed_container(largest_range);

	constexpr size_t batch_size{100000000};

	std::vector<size_t> container_1(batch_size);
	std::vector<size_t> container_2(batch_size);

	std::vector<size_t> closest_locations;

	while (seed_pairs.size() > 0)
	{
		file_string.str(read_only_file_string.str());
		file_string.clear();

		std::cout << "Num Pairs: " << seed_pairs.size() << '\n';

		size_t seeds_to_process{batch_size};
		size_t const current_index{seed_pairs.size() - 1};

		size_t current_seed{seed_pairs[current_index].first};
		size_t current_range{seed_pairs[current_index].second};

		std::cout << "    Current Seed: " << seed_pairs[current_index].first << '\n';
		std::cout << "    Current Rnge: " << seed_pairs[current_index].second << '\n';

		if (current_range <= batch_size)
		{
			seeds_to_process = seed_pairs[current_index].second;
			seed_pairs.pop_back();
		}
		else
		{
			seed_pairs[current_index].first += batch_size;
			seed_pairs[current_index].second -= batch_size;
		}

		std::generate(
			container_1.begin(),
			container_1.begin() + seeds_to_process,
			[current_seed](){ static size_t i{0}; return current_seed + i++; }
		);

		std::vector<size_t>& current_container{container_1};
		std::vector<size_t>& mapped_container{container_2};

		auto mapping_container_start{mapped_container.begin()};

		for (;;)
		{
			//std::cout << "Current Container: ";
			//for (auto elem : current_container) { std::cout << elem << ", "; }
			//std::cout << '\n';

			//std::cout << "Mapped Container: ";
			//for (auto elem : mapped_container) { std::cout << elem << ", "; }
			//std::cout << '\n' << '\n';


			// x-to-y map:
			if (!std::getline(file_string, line)) { break; }

			std::sort(current_container.begin(), current_container.end());

			// 1136439539 28187015 34421000
			// ...
			while (std::getline(file_string, line))
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
			//std::cout << '\n' << '\n';
		}

		//std::cout << "Final Locations: ";
		//for (auto elem : current_container) { std::cout << elem << ", "; }
		//std::cout << '\n';

		// check the set of locations to find the closest location
		auto closest_location{
			std::accumulate(
				current_container.begin(),
				current_container.end(),
				std::numeric_limits<size_t>::max(),
				[](size_t lhs, size_t rhs) { return std::min(lhs, rhs); }
			)
		};

		closest_locations.emplace_back(closest_location);
	}

	std::sort(closest_locations.begin(), closest_locations.end());

	std::cout << "Closest Location: " << closest_locations[0] << '\n';

	return 0;
}


