#include"../common/tictactoe.h"
#include<iostream>
using namespace std;

int pickmove(string name, int player);
int assessgame(ZK::tictactoe game);

int main(int argc, char* argv[])
{
	//setup: 
	/*
		in case no arguments are passed:
			ask user for who should play.
		in case one argument is passed:
			if argument is "_":
				play a ("_","_") game -normal 2 player game-
			else:
				the argument is the name of the AI that should play 2nd(O)
				play a ("_",arg[1]) game -player is 1st(X) arg[1] is 2nd(O)-
		in case two arguments are passed:
			play a (arg[1],arg[2]) game -arg[1] is 1st(X) arg[2] is 2nd(O)-
	*/
	string players[2];
	switch(argc)
	{
		case 1:
		cout<<"*typing \"_\" as the name of the AI will switch it to human controls.\n\n";
		cout<<"enter the name of the AI playing 1st(X): ";
		cin>>players[0];
		cout<<"enter the name of the AI playing 2nd(O): ";
		cin>>players[1];
		break;
		case 2:
		players[0]="_";
		players[1]=argv[1];
		break;
		case 3:
		players[0]=argv[1];
		players[1]=argv[2];
		break;
		default:
		cerr<<"Error: too many arguments!";
		return 1;
	}

	ZK::tictactoe game,history;
	int currentplayer=1;

	do{
		//record game
		history=game;

		//print game
		for(int i=0;i<9;++i)
		{
			char cell = 3&(game.key()>>(i<<1));
			switch(cell)
			{
				case 0:
				cell=' ';
				break;
				case 1:
				cell='X';
				break;
				case 2:
				cell='O';
				break;
				case 3:
				cell='?';
			}
			cout<<cell;
			if(i==8)
				cout<<'\t'<<i-1<<'|'<<i<<'|'<<i+1<<'\n';
			else if(i%3==2)
				cout<<'\t'<<i-1<<'|'<<i<<'|'<<i+1<<"\n-+-+-\t-+-+-\n";
			else
				cout<<'|';
		}

		//ask for next move
		int move = pickmove(players[currentplayer-1],currentplayer);

		//apply move
		game=game.key()|(currentplayer<<(move<<1));

		//switch players
		currentplayer=3-currentplayer;

	}while(!game.isdone());
	int result = assessgame(game);
	currentplayer=3-currentplayer;
	if(result)
		cout<<"player"<<currentplayer<<"("<<(currentplayer==1?'X':'O')<<") won!\n";
	else
		cout<<"draw\n";
	system("pause");
}

int pickmove(string name, int player)
{
	int move;
	if(name!="_")
	{
		cerr<<"Error: not yet implemented";
		throw(1);
	}
	cout<<(player==1?'X':'O')<<" to move: ";
	cin>>move;
	return (--move)%9;
}

int assessgame(ZK::tictactoe game)
{
	int board[9];
	bool draw=true;
	for(int i=0;i<18;i+=2)
	{
		board[i>>1]=0b11&(game.key()>>i);
		if(board[i>>1]==0)
			draw=false;
	}
	if(draw)
		return 0;
	if(( board[0]==board[4] && board[0]==board[8] && board[0]!=0 ) || ( board[2]==board[4] && board[2]==board[6] && board[2]!=0 ))
		return board[4];
	for(int8_t i=0;i<3;++i)
		if(( board[i]==board[i+3] && board[i]==board[i+6] && board[i]!=0 ) || ( board[i*3]==board[i*3+1] && board[i]==board[i*3+2] && board[i*3]!=0 ))
			return board[i];
	return false;
}