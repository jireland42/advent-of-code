#include <fstream>
#include <iostream>

#include "dfa.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./simple input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);
	std::string line;

	int first{-1};
	int last{-1};
	int sum{0};

	DFA::State state{DFA::State::start};

	while (std::getline(input_file, line))
	{
		size_t i{0};

		for (; i < line.size(); i += 1)
		{
			state = DFA::transition(state, line[i]);

			if (DFA::is_accepting(state))
			{
				first = DFA::get_int_representation(state);
				last = first;

				break;
			}
		}

		for (; i < line.size(); i += 1)
		{
			state = DFA::transition(state, line[i]);

			if (DFA::is_accepting(state))
			{
				last = DFA::get_int_representation(state);
			}
		}

		if (first == -1) { continue; }

		sum += (10 * first + last);

		first = -1;
		last = -1;
	}

	std::cout << "Sum: " << sum << '\n';

	return 0;
}
