#include"../common/tictactoe.h"
#include<fstream>
#include<iostream>
#include<vector>
using namespace std;

struct memorycell{
	uint32_t gamestate;
	uint8_t wieghts[9];
	memorycell()
	{
		gamestate=0;
		for(int i=0;i<9;++i)
			wieghts[i]=1;
	}
	memorycell(ZK::tictactoe game)
	{
		game.reduce();
		gamestate = game.key();
		for(int i=0;i<9;++i)
			wieghts[i]=1;
	}
	bool operator<(memorycell other) { return gamestate<other.gamestate; }
	bool operator==(memorycell other) { return gamestate==other.gamestate; }
	int pick()
	{
		//pick a random choice taking wieghts into account
	}
};

int calculate(string name, ZK::tictactoe game);
void readmemory(string name, vector<memorycell>& memory);
void learn(string name, ZK::tictactoe game, int move, string cond);
vector<memorycell>::iterator search(vector<memorycell>& memory, ZK::tictactoe game);
void insertmemory(string name, vector<memorycell>& memory);

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
	int8_t chain = game.reduce();
	chain = game.dechainer(chain);
	vector<memorycell> memory;
	readmemory(name,memory);
	vector<memorycell>::iterator cell = search(memory,game);
	if(cell!=memory.end())
		return game.transform(chain,cell->pick());
	//remember the game if you can't find it
	memory.push_back(game);
	int result = game.transform(chain,memory[memory.size()-1].pick());
	insertmemory(name,memory);
	return result;
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
	filesize-=5;
	if(filesize%12)
	{
		cerr<<"Error: incorrect file format.";
		memfile.close();
		throw(1);
	}

	//preserve space for data
	filesize/=12;
	memory.reserve(filesize+1);

	//read data
	memfile.seekg(5,ios::beg);
	memorycell memcell;
	while(filesize--)
	{
		memfile.seekg(-1,ios::cur);
		memfile.read(reinterpret_cast<char*>(&memcell), 13);
		memcell.gamestate &= ~(0b11111111<<24);
		memory.push_back(memcell);
	}
}

vector<memorycell>::iterator search(vector<memorycell>& memory, ZK::tictactoe game)
{
	memorycell target = game;
	vector<memorycell>::iterator l=memory.begin(),r=memory.end(),m;
	while(l<r)
	{
		m=l+(r-l)/2;
		if(*m==target)
			return m;
		if(*m<target)
			l=m+1;
		else
			r=m;
	}
	return memory.end();
}

void insertmemory(string name, vector<memorycell>& memory)
{
	//insert the last memorycell remembering the new location

	//overwrite the memory file starting from the new location since the rest is the same
}