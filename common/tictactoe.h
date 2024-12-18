#pragma once
#include <cstdint>

namespace ZK {
	class tictactoe
	{
		private:
		//18 bits representing the game, 2 bits describe one (cell: 0b{isO}{isX})
		int32_t _key;

		public:
		tictactoe();
		tictactoe(int32_t key);

		//returns _key
		int32_t key() const;

		//rotates the game 90 degrees
		tictactoe& rotate();
		//flips the game upside-down
		tictactoe& flip();
		//reverses the roles (swaps Xs and Os)
		tictactoe& reverse();

		//reduces the game to it's canonical form and returns a (chain: 0b{reverse}{flip}{rotate twice}{rotate once})
		int8_t reduce();
		//returns the chain that could reverse the effect of the given chain
		int8_t dechainer(int8_t chain) const;

		//transforms the game using a chain
		tictactoe& transform(int8_t chain);
		//returns where a cell would be after going thru a chain
		int8_t transform(int8_t chain, int8_t cell) const;

		//true if draw/lose/win
		bool isdone() const;

		//all comparisons compare canonical forms
		bool operator>(tictactoe other) const;
		bool operator<(tictactoe other) const;
		bool operator>=(tictactoe other) const;
		bool operator<=(tictactoe other) const;
		bool operator==(tictactoe other) const;
		bool operator!=(tictactoe other) const;
	};
}