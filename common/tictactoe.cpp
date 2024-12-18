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
}