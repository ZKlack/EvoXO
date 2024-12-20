#include"../common/tictactoe.h"
#include<fstream>
#include<iostream>
#include<vector>
using namespace std;

struct memorycell{
	uint32_t gamestate;
	uint8_t wieghts[9];
	bool operator<(memorycell other) { return gamestate<other.gamestate; }
	bool operator==(memorycell other) { return gamestate==other.gamestate; }
};

int calculate(string name, ZK::tictactoe game);
void readmemory(string name, vector<memorycell>& memory);
void learn(string name, ZK::tictactoe game, int move, string cond);

int main(int argc, char* argv[])
{
	ZK::tictactoe game;
	int move;
	string name, cond;
	switch(argc)
	{
		case 3:
		name = argv[1];
		game = strtol(argv[2],nullptr,10);
		move = calculate(name,game);
		cout<<move;
		return 0;
		case 5:
		name = argv[1];
		game = strtol(argv[2],nullptr,10);
		move = strtol(argv[3],nullptr,10);
		cond = argv[4];
		learn(name,game,move,cond);
		return 0;
	}
	return 1;
}

int calculate(string name, ZK::tictactoe game)
{
	game.reduce();
	vector<memorycell> memory;
	readmemory(name,memory);

}

void readmemory(string name, vector<memorycell>& memory)
{
	//if the memory file doesn't exist leave memory empty
	ifstream memfile(name,ios::binary);
    if (!memfile.is_open())
	{
		memfile.close();
		return;
	}

	//validate file format
	char header[5];
	memfile.read(header,5);
	if(string(header,5)!="EvoXO")
	{
		cerr<<"Error: incorrect file format.";
		memfile.close();
		throw(1);
	}
	memfile.seekg(0,ios::end);
	size_t filesize = memfile.tellg();
	if((filesize-5)%12)
	{
		cerr<<"Error: incorrect file format.";
		memfile.close();
		throw(1);
	}

	//preserve space for data
	memory.reserve((filesize-5)/12);

	//read data
	memfile.seekg(5,ios::beg);
	memorycell memcell;
	while(memfile.peek() != EOF)
	{
		memfile.seekg(-1);
		memfile.read(reinterpret_cast<char*>(&memcell), 13);
		memcell.gamestate &= ~(0b11111111<<24);
		memory.push_back(memcell);
	}
}