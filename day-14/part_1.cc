#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Chain
{
	size_t length;
	size_t start_weight;
};

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::vector<size_t> tops;
	std::vector<Chain> partial_chains;
	std::vector<Chain> all_chains;
	size_t line_weight{0};

	std::string line;

	std::getline(input_file, line);

	for (char const ground_symbol : line)
	{
		switch (ground_symbol)
		{
			case 'O':
				tops.emplace_back(0);
				partial_chains.emplace_back(Chain{1, line_weight});
				break;
			case '#':
				tops.emplace_back(1);
				partial_chains.emplace_back(Chain{0, 0});
				break;
			case '.':
				tops.emplace_back(0);
				partial_chains.emplace_back(Chain{0, 0});
				break;
			default:
				throw std::runtime_error("invalid format\n");
		}
	}

	while (std::getline(input_file, line))
	{
		line_weight += 1;

		for (size_t i{0}; i < line.size(); i += 1)
		{
			char const ground_symbol{line[i]};
			Chain& current_chain{partial_chains[i]};

			switch (ground_symbol)
			{
				case 'O':
					if (current_chain.length == 0) { current_chain.start_weight = tops[i]; }
					current_chain.length += 1;
					break;
				case '#':
					tops[i] = line_weight + 1;
					if (current_chain.length != 0)
					{
						all_chains.emplace_back(std::move(current_chain));
						current_chain = Chain{0,0};
					}
					break;
				case '.':
					break;
				default:
					throw std::runtime_error("invalid format\n");
			}
		}
	}

	size_t const max_weight{line_weight + 1};
	size_t cumulative_wieght{0};

	for (Chain const& chain : partial_chains)
	{
		if (chain.length == 0) { continue; }

		size_t const new_weight{
			(chain.length
				 * (2*(max_weight - chain.start_weight) - chain.length + 1))
				/ 2};

		cumulative_wieght += new_weight;
	}

	for (Chain const& chain : all_chains)
	{
		size_t const new_weight{
			(chain.length
				 * (2*(max_weight - chain.start_weight) - chain.length + 1))
				/ 2};

		cumulative_wieght += new_weight;
	}

	std::cout << "Total Weight: " << cumulative_wieght << '\n';;

	return 0;
}

