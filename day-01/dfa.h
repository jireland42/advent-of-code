#ifndef _DFA_H_
#define _DFA_H_

namespace DFA
{
	enum class State
	{
		start,
		o, on, one,
		t, tw, two,
		th, thr, thre, three,
		f, fo, fou, four,
		fi, fiv, five,
		s, si, six,
		se, sev, seve, seven,
		e, ei, eig, eigh, eight,
		n, ni, nin, nine,
		z, ze, zer, zero,
		one_l,
		two_l,
		three_l,
		four_l,
		five_l,
		six_l,
		seven_l,
		eight_l,
		nine_l,
		zero_l
	};

	int get_int_representation(State state);
	bool is_accepting(State state);
	State transition(State start_state, char input);
}

#endif
