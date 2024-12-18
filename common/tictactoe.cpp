#include"tictactoe.h"

namespace ZK {
	tictactoe::tictactoe()
	:_key(0) { }
	tictactoe::tictactoe(int32_t key)
	:_key(key&0b111111111111111111) { }

	int32_t tictactoe::key() const
	{ return _key; }

	tictactoe& tictactoe::rotate()
	{
		static int8_t matrix[] = { 2, 5, 8,
		                           1, 4, 7,
								   0, 3, 6 };
		int32_t copy = _key;
		_key = 0;
		for(int8_t i=0;i<9;++i)
			_key |= ((copy>>(i<<1))&3)<<(matrix[i]<<1); //I have no idea if this shit even works LMAO
		return *this;
	}
	tictactoe& tictactoe::flip()
	{
		int32_t copy = _key;
		_key &= 0b111111000000;
		_key |= copy>>12;
		_key |= (copy&0b111111)<<12;
		return *this;
	}
	tictactoe& tictactoe::reverse()
	{
		for(int8_t i=0;i<18;i+=2)
			if((_key>>i)&3)
				_key ^= 3<<i;
		return *this;
	}

	int8_t tictactoe::reduce()
	{
		int32_t lowest=_key;
		int8_t chain=0;
		for(int8_t i=1;i<0b10000;++i)
		{
			tictactoe copy(_key);
			copy.transform(i);
			if(lowest>copy._key)
			{
				chain=i;
				lowest=copy._key;
			}
		}
		_key=lowest;
		return chain;
	}
	int8_t tictactoe::dechainer(int8_t chain) const
	{
		return (0b1100&chain) | (0b100&chain?(0b11&chain):(0b11&(4-(0b11&chain)))); //I love bitwise stuff
	}

	tictactoe& tictactoe::transform(int8_t chain)
	{
		for(int8_t i=0b11&chain;i>0;--i)
			rotate();
		if(0b100&chain)
			flip();
		if(0b1000&chain)
			reverse();
		return *this;
	}
	int8_t tictactoe::transform(int8_t chain, int8_t cell) const
	{
		cell%=9;
		tictactoe board(1<<(cell<<1));
		board.transform(0b111&chain);
		for(int8_t i=0;i<18;i+=2)
			if(0b11&(board.key()>>i))
				return i>>1;
		return -1;
	}

	bool tictactoe::isdone() const
	{
		int8_t board[9];
		bool playable=false;
		for(int8_t i=0;i<18;i+=2)
		{
			board[i>>1]=0b11&(_key>>i);
			if(board[i>>1]==0)
				playable=true;
		}
		if(!playable)
			return true;
		if(( board[0]==board[4] && board[0]==board[8] && board[0]!=0 ) || ( board[2]==board[4] && board[2]==board[6] && board[2]!=0 ))
			return true;
		for(int8_t i=0;i<3;++i)
			if(( board[i]==board[i+3] && board[i]==board[i+6] && board[i]!=0 ) || ( board[i*3]==board[i*3+1] && board[i]==board[i*3+2] && board[i*3]!=0 ))
				return true;
		return false;
	}

	bool tictactoe::operator>(tictactoe other) const
	{
		tictactoe copy(_key);
		copy.reduce();
		other.reduce();
		return copy._key>copy._key;
	}
	bool tictactoe::operator<(tictactoe other) const
	{
		tictactoe copy(_key);
		copy.reduce();
		other.reduce();
		return copy._key<copy._key;
	}
	bool tictactoe::operator>=(tictactoe other) const
	{
		tictactoe copy(_key);
		copy.reduce();
		other.reduce();
		return copy._key>=copy._key;
	}
	bool tictactoe::operator<=(tictactoe other) const
	{
		tictactoe copy(_key);
		copy.reduce();
		other.reduce();
		return copy._key<=copy._key;
	}
	bool tictactoe::operator==(tictactoe other) const
	{
		tictactoe copy(_key);
		copy.reduce();
		other.reduce();
		return copy._key==copy._key;
	}
	bool tictactoe::operator!=(tictactoe other) const
	{
		tictactoe copy(_key);
		copy.reduce();
		other.reduce();
		return copy._key!=copy._key;
	}
}