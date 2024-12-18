#include"tictactoe.h"

namespace ZK {
	tictactoe::tictactoe()
	:_key(0) {}
	tictactoe::tictactoe(int32_t key)
	:_key(key&0b111111111111111111) {}
	
}