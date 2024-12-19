#include"../common/tictactoe.h"
#include<iostream>
using namespace std;

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
	string player1, player2;
	switch(argc)
	{
		case 1:
		cout<<"*typing \"_\" as the name of the AI will switch it to human controls.\n\n";
		cout<<"enter the name of the AI playing 1st(X): ";
		cin>>player1;
		cout<<"enter the name of the AI playing 2nd(O): ";
		cin>>player2;
		break;
		case 2:
		player1="_";
		player2=argv[1];
		break;
		case 3:
		player1=argv[1];
		player2=argv[2];
		break;
		default:
		cerr<<"Error: too many arguments!";
		return 1;
	}
}