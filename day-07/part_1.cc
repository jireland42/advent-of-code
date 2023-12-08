#include <cctype>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

struct Hand
{
	std::string cards;
	size_t bid;
};

enum class HandType
{
	FiveOfAKind,
	FourOfAKind,
	FullHouse,
	ThreeOfAKind,
	TwoPair,
	OnePair,
	HighCard
};

// *********************************************************************
auto get_card_from_index(size_t const index) -> char
{
	char card;

	switch(index)
	{
		case (0):
		case (1):
		case (2):
		case (3):
		case (4):
		case (5):
		case (6):
		case (7):
			// fallthrough
			card = '2' + index;
			break;
		case (8):
			card = 'T';
			break;
		case (9):
			card = 'J';
			break;
		case (10):
			card = 'Q';
			break;
		case (11):
			card = 'K';
			break;
		case (12):
			card = 'A';
			break;
	}

	return card;
}

// *********************************************************************
auto get_card_counts(std::string const& hand) -> std::array<size_t, 13>
{
	std::array<size_t,13> cards {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (char card : hand)
	{
		switch(card)
		{
			case ('2'):
			case ('3'):
			case ('4'):
			case ('5'):
			case ('6'):
			case ('7'):
			case ('8'):
			case ('9'):
				// fallthrough
				cards[card - '2'] += 1; // 2 is the lowest card, so maps to 0 index
				break;
			case ('T'):
				cards[8] += 1;
				break;
			case ('J'):
				cards[9] += 1;
				break;
			case ('Q'):
				cards[10] += 1;
				break;
			case ('K'):
				cards[11] += 1;
				break;
			case ('A'):
				cards[12] += 1;
				break;
		}
	}

	return cards;
}

// *********************************************************************
auto get_hand_type(std::string const& hand) -> HandType
{
	std::array<size_t,13> cards {get_card_counts(hand)};

	size_t pair_1_count{0};
	size_t pair_2_count{0};

	for (size_t i{0}; i < cards.size(); i += 1)
	{
		size_t card_count{cards[i]};

		if (card_count == 0 || card_count == 1) { continue; }

		if (card_count == 2 || card_count == 3)
		{
			if (pair_1_count == 0)
			{
				pair_1_count = card_count;
			}
			else
			{
				pair_2_count = card_count;

				// can't find more than two pairs, so stop looking
				break;
			}
		}
		else if (card_count == 4)
		{
			return HandType::FourOfAKind;
		}
		else
		{
			return HandType::FiveOfAKind;
		}
	}

	HandType hand_type{HandType::HighCard};

	if (pair_2_count != 0)
	{
		// two pair, full house
		if (pair_2_count == 3 || pair_1_count == 3) { hand_type = HandType::FullHouse; }
		else { hand_type = HandType::TwoPair; }

	}
	else if (pair_1_count != 0)
	{
		// three-of-a-kind, pair
		if (pair_1_count == 2) { hand_type = HandType::OnePair; }
		else { hand_type = HandType::ThreeOfAKind; }
	}

	return hand_type;
}

// *********************************************************************
auto compare_high_cards(char const stored_char, char const new_char) -> bool
{
	switch (stored_char)
	{
		case 'A':
			switch (new_char)
			{
				case 'K':
				case 'Q':
				case 'J':
				case 'T':
					return true;
			}
			break;
		case 'K':
			switch (new_char)
			{
				case 'Q':
				case 'J':
				case 'T':
					return true;
			}
			break;
		case 'Q':
			switch (new_char)
			{
				case 'J':
				case 'T':
					return true;
			}
			break;
		case 'J':
			switch (new_char)
			{
				case 'T':
					return true;
			}
			break;
	}
	return false;
}

// *********************************************************************
auto insert_card_into_sorted_of_type(
	std::string const& hand,
	std::vector<Hand>& sorted_hands)
{
	auto insert_position{
		std::lower_bound(
			sorted_hands.cbegin(),
			sorted_hands.cend(),
			hand,
			[](Hand const& stored_hand, std::string const& new_hand)
			{
				for (size_t i{0}; i < new_hand.size(); i += 1)
				{
					char next_of_stored{stored_hand.cards[i]};
					char next_of_new{new_hand[i]};

					if (next_of_stored == next_of_new) { continue; }

					bool is_stored_digit{std::isdigit(next_of_stored) != 0};
					bool is_new_digit{std::isdigit(next_of_new) != 0};

					if (!is_stored_digit && is_new_digit) { return true; }
					if (is_stored_digit && is_new_digit) { return (next_of_stored > next_of_new); }

					return compare_high_cards(next_of_stored, next_of_new);
				}

				return false;
			}
		)
	};

	return insert_position;
}

// *********************************************************************
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./part_1 input_file\n";
		return 1;
	}

	std::ifstream input_file(argv[1]);

	std::string line{};
	std::string hand;
	size_t bid;

	std::vector<std::vector<Hand>> sorted_hands (7);

	size_t num_hands{0};

	while (std::getline(input_file, line))
	{
		std::istringstream line_stream(line);

		line_stream >> hand;
		line_stream >> bid;

		HandType const hand_type{get_hand_type(hand)};

		auto insert_position{insert_card_into_sorted_of_type(
			hand,
			sorted_hands[static_cast<size_t> (hand_type)]
		)};

		sorted_hands[static_cast<size_t> (hand_type)].insert(insert_position, Hand{hand, bid});

		num_hands += 1;
	}

	size_t standing{num_hands};
	constexpr size_t num_hand_types{7};
	size_t total_winnings{0};

	for (size_t i{0}; i < num_hand_types; i += 1)
	{
		std::vector<Hand>& hands_at_current_strength{sorted_hands[i]};
		size_t num_hands_at_current_strength{hands_at_current_strength.size()};

		for (size_t j{0}; j < num_hands_at_current_strength; j += 1)
		{
			total_winnings += (standing * hands_at_current_strength[j].bid);
			standing -= 1;
		}
	}

	std::cout << '\n' << "Winnings: " << total_winnings << '\n';

	// 248835789 is too high
	// 249579981 is too high
	return 0;
}

