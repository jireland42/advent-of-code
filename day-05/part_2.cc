#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

struct Range
{
	size_t base;
	size_t length;
};

struct RangeSupport
{
	Range left_unsupported;
	Range support;
	Range right_unsupported;
};

void print_range(Range const& range)
{
	std::cout << "{ base: " << range.base << ", length: " << range.length << " }\n";
}

auto split_range(Range const& range_to_split, Range const& support) -> RangeSupport
{
	size_t const left_endpoint{range_to_split.base};
	size_t const right_endpoint{range_to_split.base + range_to_split.length};
	size_t const left_support_endpoint{support.base};
	size_t const right_support_endpoint{support.base + support.length};

	if (left_endpoint < left_support_endpoint)
	{
		if (right_endpoint < left_support_endpoint)
		{
			// no support
			return RangeSupport{
				range_to_split,
				Range{0, 0} ,
				Range{0, 0}
			};
		}
		else if (right_endpoint == left_support_endpoint)
		{
			// support == [right_endpoint]
			return RangeSupport{
				Range{left_endpoint, range_to_split.length - 1},
				Range{right_endpoint, 1},
				Range{0, 0} 
			};
		}
		else if (right_endpoint < right_support_endpoint)
		{
			// support = [left_support_endpoint, right_endpoint]
			size_t const overlapping_range{right_endpoint - left_support_endpoint + 1};

			return RangeSupport{
				Range{left_endpoint, range_to_split.length - overlapping_range},
				Range{left_support_endpoint, overlapping_range},
				Range{0, 0} 
			};
		}
		else
		{
			// support = [left_support_endpoint, right_support_endpoint]
			return RangeSupport{
				Range{left_endpoint, left_support_endpoint - left_endpoint},
				support,
				Range{right_support_endpoint + 1, right_endpoint - right_support_endpoint} 
			};
		}
	}
	else if (left_endpoint < right_support_endpoint)
	{
		if (right_endpoint <= right_support_endpoint)
		{
			// support = [left_endpoint, right_endpoint]
			return RangeSupport{
				Range{0, 0},
				range_to_split,
				Range{0, 0}
			};
		}
		else
		{
			// support = [left_endpoint, right_support_endpoint]
			size_t const overlapping_range{right_support_endpoint - left_endpoint + 1};

			return RangeSupport{
				Range{0, 0},
				Range{left_endpoint, overlapping_range},
				Range{right_support_endpoint + 1, right_endpoint - right_support_endpoint} 
			};
		}
	}
	else if (left_endpoint == right_support_endpoint)
	{
		// support = [left_endpoint]
		return RangeSupport{
			Range{0, 0} ,
			Range{left_endpoint, 1},
			Range{left_endpoint + 1, range_to_split.length - 1},
		};
	}
	else
	{
		// no support
		return RangeSupport{
			Range{0, 0},
			Range{0, 0},
			range_to_split
		};
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_2 input_file\n";
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
	size_t destination_range_start{};
	size_t source_range_start{};
	size_t range_length{};

	line_stream >> header;

	std::vector<Range> container_1;

	size_t seed_type{};
	size_t seed_range{};

	while (line_stream >> seed_type >> seed_range)
	{
		container_1.emplace_back(Range{seed_type, seed_range});
	}

	std::vector<Range> container_2;
	std::vector<Range> container_3;
	//std::vector<Range> container_2(container_1.size());

	std::vector<Range>& current_container{container_1};
	std::vector<Range>& mapped_container{container_2};
	std::vector<Range>& working_array{container_3};

	//auto mapping_container_start{mapped_container.begin()};

	/*
	std::cout << "Current Container\n";
	for (auto const& range : current_container)
	{
		print_range(range);
	}
	std::cout << '\n';
	*/

	for (;;)
	{
		std::cout << "***** NEW MAP LEVEL *****\n\n";
		// x-to-y map:
		if (!std::getline(input_file, line)) { break; }

		std::sort(
			current_container.begin(),
			current_container.end(),
			[](Range const& lhs, Range const& rhs) { return lhs.base < rhs.base; }
		);

		//auto working_array_start{working_array.begin()};

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

			Range source_range{source_range_start, range_length};

			// determine the range of values to which the current map applies
			auto endpoints{
				std::equal_range(
					current_container.cbegin(),
					current_container.cend(),
					source_range,
					[](Range const& element, Range const& map_range){
						std::cout << "Seed ";
						print_range(element);
						std::cout << "Range ";
						print_range(map_range);
						std::cout << "Seed Min: " << element.base << '\n';
						std::cout << "Map Max: " << map_range.base + map_range.length - 1 << '\n';
						std::cout << "Map Min: " << map_range.base << '\n';
						std::cout << "Seed Max: " << element.base + element.length - 1 << '\n';
						//if (element.base + element.length - 1 < map_range) {std::cout << "true\n"; }
						//else { std::cout << "false\n"; }
						//std::cout << '\n';
						return
							element.base > (map_range.base + map_range.length - 1)
							|| (element.base + element.length - 1) < map_range.base;
					}
				)
			};

			auto left_endpoint = endpoints.first;
			auto right_endpoint = endpoints.second;

			auto print_iter{current_container.begin()};
			auto print_end{current_container.end()};

			std::cout << "Left Endpoint:\n";
			if (left_endpoint == current_container.begin()) { std::cout << "BEGIN\n"; }
			else if (left_endpoint == current_container.end()) { std::cout << "END\n"; }
			else { print_range(*left_endpoint); }

			std::cout << "Right Endpoint:\n";
			if (right_endpoint == current_container.begin()) { std::cout << "BEGIN\n"; }
			else if (right_endpoint == current_container.end()) { std::cout << "END\n"; }
			else { print_range(*right_endpoint); }
			std::cout << "\n";

			if (left_endpoint != current_container.end())
			{
				std::cout << "Left:\n";
				while (print_iter != left_endpoint) { print_range(*print_iter); ++print_iter; }
				std::cout << "\n";

				std::cout << "Supported:\n";
				while (print_iter != right_endpoint) { print_range(*print_iter); ++print_iter; }
				std::cout << "\n";

				std::cout << "Right:\n";
				while (print_iter != print_end) { print_range(*print_iter); ++print_iter; }
				std::cout << "\n";
			}
			else
			{
				std::cout << "Unsupported:\n";
				while (print_iter != print_end) { print_range(*print_iter); ++print_iter; }
				std::cout << "\n";

				continue;
			}
			/*
			*/

			if (left_endpoint == current_container.end()) { continue; }

			auto range_iterator{current_container.begin()};
			auto range_end{current_container.end()};

			/*
			std::cout << "Working Array (Pre-Copy)\n";
			for (Range const& range : working_array)
			{
				print_range(range);
			}
			*/

			for (auto it{current_container.cbegin()}; it != left_endpoint; ++it)
			{
				working_array.emplace_back(*it);
			}

			std::cout << "Working Array (Pre-Copy)\n";
			for (Range const& range : working_array)
			{
				print_range(range);
			}
			std::cout << '\n';

			std::cout << "Current Container (Pre-Loop)\n";
			for (Range const& range : current_container)
			{
				print_range(range);
			}
			std::cout << "Mapped Container (Pre-Loop)\n";
			for (Range const& range : mapped_container)
			{
				print_range(range);
			}
			std::cout << '\n';
			/*
			*/

			// if the map applies to any values, then map them to the mapped
			// values array
			for (; range_iterator != right_endpoint; ++range_iterator)
			{
				RangeSupport support{split_range(*range_iterator, source_range)};

				std::cout << "Split\n";
				print_range(support.left_unsupported);
				print_range(support.support);
				print_range(support.right_unsupported);
				std::cout << "\n";

				if (support.left_unsupported.length != 0)
				{
					working_array.emplace_back(support.left_unsupported);
					//++working_array_start;
				}
				if (support.support.length != 0)
				{
					size_t const offset{support.support.base - source_range_start};

					mapped_container.emplace_back(
						Range{
							destination_range_start + offset,
							support.support.length,
						}
					);
				}
				if (support.right_unsupported.length != 0)
				{
					working_array.emplace_back(support.right_unsupported);
					//++working_array_start;
				}

				std::cout << "Working Array (In-Loop)\n";
				for (Range const& range : working_array)
				{
					print_range(range);
				}
				std::cout << "Mapped Container (In-Loop)\n";
				for (Range const& range : mapped_container)
				{
					print_range(range);
				}
				std::cout << '\n';
				/*
				*/
			}

			//auto const num_supported_ranges{size_t(right_endpoint - left_endpoint)};

			//auto const range_end{working_array.cbegin() + num_supported_ranges};
			//std::copy(working_array.cbegin(), working_array.cend(), working_array.begin());
			for (auto it{right_endpoint}; it != current_container.cend(); ++it)
			{
				working_array.emplace_back(*it);
			}

			std::swap(current_container, working_array);
			working_array.clear();

			// removed mapped items from working array
			//current_container.erase(left_endpoint, right_endpoint);
		}

		/*
		std::cout << "Current Container (Pre-Copy)\n";
		for (Range const& range : current_container)
		{
			print_range(range);
		}
		std::cout << "Mapped Container (Pre-Copy)\n";
		for (Range const& range : mapped_container)
		{
			print_range(range);
		}
		std::cout << '\n';
		*/

		// copy remaining items that map to themselves 
		for (auto const& unmapped_range : current_container)
		{
			mapped_container.emplace_back(unmapped_range);
		}

		// set container references for next pass
		std::swap(current_container, mapped_container);
		mapped_container.clear();
		//mapped_container.resize(current_container.size());
		//mapping_container_start = mapped_container.begin();
		/*

		std::cout << "Current Container (Post-Copy)\n";
		for (Range const& range : current_container)
		{
			print_range(range);
		}
		std::cout << "Mapped Container (Pre-Copy)\n";
		for (Range const& range : mapped_container)
		{
			print_range(range);
		}
		std::cout << '\n';
		*/
	}

	// check the set of locations to find the closest location
	/*
	auto closest_location{
		std::accumulate(
			current_container.begin(),
			current_container.end(),
			std::numeric_limits<size_t>::max(),
			[](size_t lhs, size_t rhs) { return std::min(lhs, rhs); }
		)
	};
	*/

	std::cout << "Current Container (Post-Copy)\n";
	for (Range const& range : current_container)
	{
		print_range(range);
	}
	std::cout << '\n';
	//std::cout << "Closest Location: " << closest_location << '\n';

	return 0;
}

