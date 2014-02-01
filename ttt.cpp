#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <cstring>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <cmath>


typedef std::unordered_map<std::string, float> value_map;

const int LIN_BOARD_SIZE = 9;
const char PLAYER1 = 'X';
const char PLAYER2 = 'O';
const char EMPTY = '_';

value_map values;
std::stack<std::string> state_stack;
const float initialValue = 0.f;
const float distanceCoef = 0.1f;
bool explore = true;

const int ROW_SIZE = 3;
const int NUM_WIN_PERMS = 8;

const int winPerms[8][3] =
{
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},
	{0, 4, 8},
	{2, 4, 6}
};

void loadValues(const char* filename)
{
	values.clear();
	std::ifstream valueFile;
	valueFile.open(filename);
	
	std::string state;
	float value;

	while(valueFile.is_open() && !valueFile.eof())
	{
		valueFile >> state;
		if(valueFile.eof())
		{
			break;
		}
		valueFile >> value;
		values[state] = value;
	}
	valueFile.close();
}

void saveValues(const char* filename)
{
	std::ofstream valueFile;
	valueFile.open(filename);
	
	value_map::iterator it;
	for(it = values.begin(); it != values.end() && valueFile.is_open(); it++)
	{
		valueFile << it->first << std::endl;
		valueFile << it->second << std::endl;
	}
	valueFile.close();
}

void initBoard(char* board)
{
	for(unsigned int i = 0; i < LIN_BOARD_SIZE; i++)
	{
		board[i] = EMPTY;
	}
}

void drawBoard(char* board)
{
	for(unsigned int i = 0; i < 3; i++)
	{
		for(unsigned int j = 0; j < 3; j++)
		{
			unsigned int index = 3 * i + j;
			std::cout << board[index];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;
}

void makeMove(char* board, unsigned int space)
{
	unsigned int p1Count = 0;
	unsigned int p2Count = 0;
	char player = EMPTY;

	for(unsigned int i = 0; i < LIN_BOARD_SIZE; i++)
	{
		if(board[i] == PLAYER1)
		{
			p1Count++;
		}
		else if(board[i] == PLAYER2)
		{
			p2Count++;
		}
	}

	if(p1Count == p2Count)
	{
		player = PLAYER1;
	}

	else if(p1Count > p2Count)
	{
		player = PLAYER2;
	}

	else
	{
		std::cout << "I DON'T KNOW WHAT I'M DOING";
	}

	if(space < LIN_BOARD_SIZE)
	{
		if(board[space] == EMPTY)
		{
			board[space] = player;
		}
	}
	
}

unsigned int findNumEmptySpaces(char* board)
{
	unsigned int count = 0;
	for(unsigned int i = 0; i < LIN_BOARD_SIZE; i++)
	{
		if(board[i] == EMPTY)
		{
			count++;
		}
	}
	return count;
}

bool isGameOver(char* board, char& winner)
{
	winner = EMPTY;
	
	//Win with 3 horizontally...
	if(board[0] == board[1] && board[0] == board[2] && board[0] != EMPTY)
	{
		winner = board[0];
		return true;
	}

	else if(board[3] == board[4] && board[3] == board[5] && board[3] != EMPTY)
	{
		winner = board[3];
		return true;
	}
	
	else if(board[6] == board[7] && board[6] == board[8] && board[6] != EMPTY)
	{
		winner = board[6];
		return true;
	}
	
	//Win with 3 vertically...
	else if(board[0] == board[3] && board[0] == board[6] && board[0] != EMPTY)
	{
		winner = board[0];
		return true;
	}

	else if(board[1] == board[4] && board[1] == board[7] && board[1] != EMPTY)
	{
		winner = board[1];
		return true;
	}

	else if(board[2] == board[5] && board[2] == board[8] && board[2] != EMPTY)
	{
		winner = board[2];
		return true;
	}

	//Win with 3 diagonally...
	else if(board[0] == board[4] && board[0] == board[8] && board[0] != EMPTY)
	{
		winner = board[0];
		return true;
	}

	else if(board[2] == board[4] && board[2] == board[6] && board[2] != EMPTY)
	{
		winner = board[2];
		return true;
	}

	//Tie case
	else if(findNumEmptySpaces(board) == 0)
	{
		winner = EMPTY;
		return true;
	}
}

void printWinMessage(char winner)
{
	char message[25];
	switch(winner)
	{
	case PLAYER1:
		strcpy(message, "PLAYER 1 WINS");
		break;
	case PLAYER2:
		strcpy(message, "PLAYER 2 WINS");
		break;
	case EMPTY:
		strcpy(message, "TIE");
		break;
	default:
		strcpy(message, "SOMETHING WENT WRONG");
	}
	std::cout << message << std::endl;
}

void generateNextStates(char* board, std::vector<std::string>& nextStates)
{
	nextStates.clear();
	std::string nextState;
	nextState = board;

	for(unsigned int i = 0; i < LIN_BOARD_SIZE; i++)
	{
		if(board[i] == EMPTY)
		{
			nextState[i] = PLAYER1;
			nextStates.push_back(nextState);
			nextState = board;
		}
	}
}

unsigned int findBestMove(char* board, std::vector<std::string>& nextStates)
{
	std::string bestState;
	float highestValue = -1.0f;
	
	for(unsigned int i = 0; i < nextStates.size(); i++)
	{
		if(values.find(nextStates[i]) == values.end())
		{
			continue;
		}
		
		if(values[nextStates[i]] > highestValue)
		{
			highestValue = values[nextStates[i]];
			bestState = nextStates[i];
		}
	}

	for(unsigned int i = 0; i < LIN_BOARD_SIZE; i++)
	{
		if(board[i] == EMPTY)
		{
			if(bestState[i] != EMPTY)
			{
				return i;
			}
		}
	}
}

void machineTurn(char* board)
{
	std::string state;
	state = board;
	state_stack.push(state);
	unsigned int space = 0;
	//Are we in explore or exploit mode?
	do
	{
		if(explore)
		{
			space = rand() % LIN_BOARD_SIZE;
		}
		else
		{
			std::vector<std::string> nextStates;
			generateNextStates(board, nextStates);
			space = findBestMove(board, nextStates);
		}
	} while(board[space] != EMPTY);

	makeMove(board, space);
	state = board;
	state_stack.push(state);
}

void randomTurn(char* board)
{
	unsigned int space = 0;
	do
	{
		space = rand() % LIN_BOARD_SIZE;
	} while(board[space] != EMPTY);

	makeMove(board, space);
}

void strategyTurn(char* board, char player)
{
	unsigned int space = 0;
	bool foundMove = false;

	for(int i = 0; i < NUM_WIN_PERMS; i++)
	{
		unsigned int a = winPerms[i][0];
		unsigned int b = winPerms[i][1];
		unsigned int c = winPerms[i][2];

		if(board[a] == board[b] && board[a] != EMPTY && board[c] == EMPTY)
		{
			space = c;
			foundMove = true;
			if(board[a] == player)
			{
				makeMove(board, space);
				return;
			}
		}

		if(board[a] == board[c] && board[a] != EMPTY && board[b] == EMPTY)
		{
			space = b;
			foundMove = true;
			if(board[a] == player)
			{
				makeMove(board, space);
				return;
			}
		}

		if(board[b] == board[c] && board[b] != EMPTY && board[a] == EMPTY)
		{
			space = a;
			foundMove = true;
			if(board[b] == player)
			{
				makeMove(board, space);
				return;
			}
		}
	}

	if(foundMove)
	{
		makeMove(board, space);
	}

	else
	{
		randomTurn(board);
	}
}

void playerTurn(char* board)
{
	unsigned int space = 0;
	do
	{
		std::cout << "Enter a space (0 - 8)";
		std::cin >> space;
		
		if(space >= LIN_BOARD_SIZE || board[space] != EMPTY)
		{
			std::cout << "That space is not available" << std::endl;
		}
	} while (space < LIN_BOARD_SIZE && board[space] != EMPTY);
	makeMove(board, space);
}

void valueUpdate(char winner)
{
	//Set the initial value based on the outcome of the game
	float principal = 0.0f;
	switch(winner)
	{
	case PLAYER1:
		principal = 1.0f;
		break;
	case PLAYER2:
		principal = -1.0f;
		break;
	case EMPTY:
		principal = -0.5f;
		break;
	default:
		std::cout << "Something went wrong with game over function!" << std::endl;
	}
	
	int turns = 0;
	//Step through and update all state values
	while(!state_stack.empty())
	{
		//If the state is not currently in the state system add it with an initial value of 0.5
		std::string topState = state_stack.top();
		if(values.find(topState) == values.end())
		{
			values[topState] = initialValue;
		}
		
		//Update the states value. Hopefully it converges!
		float value = principal;
		if(principal > 0.0f)
		{
			value -= distanceCoef * turns;
		}

		else
		{
			value += distanceCoef * turns;
		}

		if(principal * value < 0.0f)
		{
			value = 0.0f;
		}

		values[topState] +=  value;

		state_stack.pop();
		turns++;
	}
}

void playGame(unsigned int player)
{
	const char* filename = "values.txt";
	loadValues(filename);
	char board[LIN_BOARD_SIZE];
	initBoard(board);

	char winner;

	
	while(!isGameOver(board, winner))
	{
		drawBoard(board);
		machineTurn(board);
		drawBoard(board);

		if(!isGameOver(board, winner))
		{
			switch(player)
			{
			case 0:
				randomTurn(board);
				break;
			case 1:
				strategyTurn(board, PLAYER2);
				break;
			case 2:
				playerTurn(board);
				break;

			}
		}	
	}
	
	drawBoard(board);
	printWinMessage(winner);
	valueUpdate(winner);
	saveValues(filename);
}

int main()
{
	unsigned int numGames = 0;
	unsigned int mode = 0;
	unsigned int player = 0;
	std::cout << "Play machine in what mode? Exploit(0), Explore(1): ";
	std::cin >> mode;
	explore = (bool)mode;
	std::cout << "Play machine against what kind of player? Random(0), Strategy(1), Human(2): ";
	std::cin >> player;
	std::cout << "Enter the number of games to simulate: ";
	std::cin >> numGames;
	
	srand(time(NULL));

	for(unsigned int i = 0; i < numGames; i++)
	{
		playGame(player);
	}
	return 0;
}
