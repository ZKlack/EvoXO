#include"../common/tictactoe.h"
#include<fstream>
#include<iostream>
#include<vector>
#include<random>
using namespace std;

struct memorycell{
	uint32_t gamestate;
	uint8_t wieghts[9];
	memorycell()
	{
		gamestate=0;
		for(int i=0;i<9;++i)
			if((gamestate>>(i<<1))&0b11)
				wieghts[i]=0;
			else
				wieghts[i]=1;
	}
	memorycell(ZK::tictactoe game)
	{
		game.reduce();
		gamestate = game.key();
		for(int i=0;i<9;++i)
			if((gamestate>>(i<<1))&0b11)
				wieghts[i]=0;
			else
				wieghts[i]=1;
	}
	bool operator<(memorycell other) { return gamestate<other.gamestate; }
	bool operator==(memorycell other) { return gamestate==other.gamestate; }
	int pick()
	{
		int total = 0;
		for(int i=0;i<9;++i)
			total+=wieghts[i];

		if(total==0)
		{
			*this = memorycell(gamestate);
			total = 0;
			for(int i=0;i<9;++i)
				total+=wieghts[i];
		}
		if(total==0)
			throw(1);

		// <chatGPT code block>
		mt19937 gen(random_device{}());
		uniform_int_distribution<> dist(0,total-1);
		int random_number = dist(gen);
		// </chatGPT code block>		*I have no idea how this works

		int consumed_chance = 0;
		for(int i=0;i<9;++i)
		{
			consumed_chance += wieghts[i];
			if(random_number<consumed_chance)
				return i;
		}

		throw(1);
	}
};

int calculate(string name, ZK::tictactoe game);
void readmemory(string name, vector<memorycell>& memory);
void learn(string name, ZK::tictactoe game, int move, string cond);
vector<memorycell>::iterator search(vector<memorycell>& memory, ZK::tictactoe game);
vector<memorycell>::iterator insertmemory(string name, vector<memorycell>& memory);
void rewrite(string name, vector<memorycell>& memory, vector<memorycell>::iterator begin, vector<memorycell>::iterator end);

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
	int8_t chain = game.reduce(), result;
	chain = game.dechainer(chain);
	vector<memorycell> memory;
	readmemory(name,memory);
	vector<memorycell>::iterator cell = search(memory,game);
	if(cell!=memory.end())
	{
		result = game.transform(chain,cell->pick());
		rewrite(name,memory,cell,cell+1);
	}
	else
	{
		memory.push_back(game);
		result = game.transform(chain,memory[memory.size()-1].pick());
		cell = insertmemory(name,memory);
		rewrite(name,memory,cell,memory.end());
	}
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

void rewrite(string name, vector<memorycell>& memory, vector<memorycell>::iterator begin, vector<memorycell>::iterator end)
{
	size_t index = begin - memory.begin();
	size_t starting_byte = 5 + 12*index;

	fstream memfile(name,ios::binary|ios::in|ios::out);
	if(!memfile.is_open())
	{
		cerr<<"Error: unable to open file.";
		throw(1);
	}
	memfile.seekp(starting_byte,ios::beg);

	while(begin<end)
	{
		memfile.write(reinterpret_cast<char*>(&(*begin))+1,12);
		++begin;
	}
	memfile.close();
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

vector<memorycell>::iterator insertmemory(string name, vector<memorycell>& memory)
{
	if(memory.size()==0)
		throw(0);
	vector<memorycell>::iterator itr = memory.end();
	if(memory.size()<2)
		return --itr;
	
	memorycell temp;
	while(memory.begin()<--itr)
	{
		if(*(itr-1)<*itr)
			break;
		temp = *itr;
		*itr = *(itr-1);
		*(itr-1) = temp;
	}
	return itr;
}

void learn(string name, ZK::tictactoe game, int move, string cond)
{
	int8_t chain = game.reduce();
	chain = game.dechainer(chain);
	move = game.transform(chain,move);
	vector<memorycell> memory;
	readmemory(name,memory);
	vector<memorycell>::iterator cell = search(memory,game);
	vector<memorycell>::iterator end = cell+1;
	if(cell==memory.end())
	{
		memory.push_back(game);
		cell = insertmemory(name,memory);
		end = memory.end();
	}
	/*
		do the learning
	*/
	rewrite(name,memory,cell,end);
}