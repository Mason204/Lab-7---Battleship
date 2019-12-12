/*
Mason Henry
C++
December 11, 2019
Create a simple battlship game using four 2D arrays as a grid, specific characters for hits, misses, water, and ships. Letting the
know whats happening by identifying what the computer does each turn and giving the option to hit or surrender.
*/

#include <iostream>
#include <iomanip>
#include <time.h>
#include <string>
//#define Debug

using namespace std;

//The five different ship types.
enum shipType
{
	Carrier,
	Battleship,
	Cruiser,
	Submarine,
	Destroyer
};

struct ship
{
	int size;
	int yposition_start;
	int xposition_start;
	int yposition_end;
	int xposition_end;
	int health;
	string name;
	shipType type;
	int direction;
};

struct Fleet
{ 
	ship carrier;
	ship battleship;
	ship cruiser;
	ship submarine;
	ship destroyer;
	string name;
};

//Preconditions: Stream must be valid, previous calls of function must have come back false, values and startup must have been valid.
//Postconditions: Outputs all the ships one ship at at time alternating between the computer and the user with each call. Makes sure
//ships are placed in valid positions and on the correct grids.
bool placement(ship& carrier, int& counter, char userFriendly[][10], int input, bool display, int placementcheck, char userEnemy[][10], bool& player);
//Preconditions: Stream must be valid, all previous functions must be valid, xposition_start and yposition_start must be valid and
//within range.
//Postconditions:Checks to see if the current ship is able to be placed upward. If returned true can output to let user know that 
//they can place a ship upward or allows the placement of ships upward. If ships are unable to be placed upward returns false 
//not allowing upward placement.
bool Upcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck);
//Preconditions: Stream must be valid, all previous functions must be valid, xposition_start and yposition_start must be valid and
//within range.
//Postconditions:Checks to see if the current ship is able to be placed downward. If returned true can output to let user know that 
//they can place a ship downward or allows the placement of ships downward. If ships are unable to be placed downward returns false 
//not allowing downward placement.
bool Downcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck);
//Preconditions: Stream must be valid, all previous functions must be valid, xposition_start and yposition_start must be valid and
//within range.
//Postconditions:Checks to see if the current ship is able to be placed rightward. If returned true can output to let user know that 
//they can place a ship rightward or allows the placement of ships rightward. If ships are unable to be placed rightward returns false 
//not allowing rightward placement.
bool Rcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck);
//Preconditions: Stream must be valid, all previous functions must be valid, xposition_start and yposition_start must be valid and
//within range.
//Postconditions:Checks to see if the current ship is able to be placed leftward. If returned true can output to let user know that 
//they can place a ship leftward or allows the placement of ships leftward. If ships are unable to be placed leftward returns false 
//not allowing leftward placement.
bool Lcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck);
//Preconditions: Stream must valid, previous placement calls must have returned false.
//Postconditions: Outputs the two arrays the are the user friendly and user enemy grids. If Debug is uncommented certain calls
//output the two arrays that are the computer friendly and computer enemy grids.
void board(int& counter, char userFriendly[][10], char userEnemy[][10]);
//Preconditions: Stream must be valid, placement function must be valid and all ships placed.
//Postconditions: Alternating between user and the computer depending on call parameters outputs whether the user/computer hit ormiss
//letting the user know if they enter in out of range arguments and exiting the program if the stream becomes invalid. Changes the
//arrays to their corresponding hits or misses and outputs it back to the screen.
bool turn(int& xcoordinate, int& ycoordinate, bool display, char compFriendly[][10], char userEnemy[][10], int& input, Fleet fleet[],
	int counter, bool& player);
//Preconditions: Stream must be valid, user or computer must of had a hit, all ships must have been previously placed.
//Postconditions: Changing ships with each call, checks to see if the hit falls on the ship respectively. If so removes one off of that
//ships health returning false. If that ship reaches zero makes the health -1 and returns true letting the user know what ship sank
//of either the user's or computer's fleet.
bool shipcheck(int xcoordinate, int ycoordinate, ship& carrier);
//Preconditions: Stream must be valid, ships must be placed previously.
//Postconditions: Counts the number of hits on the grid its called to. If there are 17 hits returns true letting the user know who won
//otherwise it returns false;
bool check(char userEnemy[][10]);
//Preconditions: Stream must be valid.
//Postconditions: Outputs the starting screen and instructions for the game.
void startup();
//Preconditions: Stream must be valid.
//Postconditions: Initializes the ship values for both the user and computers fleets.
void values(Fleet fleet[]);
//Preconditions: Stream must be valid, settingUp must return false, all ships must have been placed.
//Postconditions: Performs turn and check playing the game until either side wins or loses. Adding 1 to either wins or losses depending
//on whether the user won or lost. If an invalid character is entered returns true, returning 1 ending the program.
bool turns(int& xcoordinate, int& ycoordinate, bool& display, char compFriendly[][10], char userEnemy[][10], int input, Fleet fleet[], int& counter,
	char userFriendly[][10], bool& game, bool skip, int& gwon, int& glost, char compEnemy[][10], bool& player);
//Preconditions:Stream must be valid. The game must have either been won or lost.
//Postconditions: Asks the user whether they wish to play again. Sets the value of play to either true or false to play the game again.
//If an invalid character is entered ends the program.
bool playagain(int input, bool& play, bool& game, bool& setup, int& gwon, int& glost, int& counter, bool& player);
//Preconditions: Stream must be valid.
//Postconditions: Performs the placement function for each ship for each of the two fleets. Ends the program if the user placement
//functions return true which means an invalid charactedr was entered.
bool settingUp(int& counter, char userFriendly[][10], char userEnemy[][10], char compFriendly[][10], char compEnemy[][10], bool& display,
	Fleet fleet[], int& input, bool& setup, int placementcheck, bool player);

int main()
{
	srand(time(NULL));
	//used to play again or not
	bool play = true;
	//used when determing whether to display something and various others
	bool display = true;
	//used for the ship placement part of the program
	bool setup = true;
	//used for whenever the user needs to input something or when the computer needs a value set to it
	int input = 0;
	//used in settingUp for the placement functions
	int counter = 0;
	//used when placing ships and when shooting for both the user and computer.
	int xcoordinate = 0;
	//used when placing ships and when shooting for both the user and computer.
	int ycoordinate = 0;
	//used to determing whether a ship can be placed on the grid. (Prevents ships from being boxed in)
	int placementcheck = 0;
	//used to continue playing the game until someone wins.
	bool game = true;
	//if the user wins skips over the computers turn.
	bool skip = true;
	//keeps track of wins.
	int gwon = 0;
	//keps track of losses
	int glost = 0;
	//helps keep track of whether its the users turn or not allowing certain extra actions to be done.
	bool player = false;
	//The four grids
	char userFriendly[10][10];
	char userEnemy[10][10];
	char compFriendly[10][10];
	char compEnemy[10][10];
	//The two fleets
	Fleet fleet[2];

	//Start up scrren
	startup();

	do
	{
		//sets values
		values(fleet);
		//Places the ships
		if (settingUp(counter, userFriendly, userEnemy, compFriendly, compEnemy, display, fleet, input, setup, placementcheck, player))
			return 1;
		cout << "The game will now commence.\n";
		//Main part of the games taking turns firing at the opponetns ships.
		if (turns(xcoordinate, ycoordinate, display, compFriendly, userEnemy, input, fleet, counter, userFriendly, game,
			skip, gwon, glost, compEnemy, player))
			return 1;
		//Determines whether the game is to be played again.
		if (playagain(input, play, game, setup, gwon, glost, counter, player))
			return 1;
	} while (play == true);

	return 0;
}

bool placement(ship& carrier, int& counter, char userFriendly[][10], int input, bool display, int placementcheck, char userEnemy[][10], bool& player)
{
	//used to determine whether new x and y coordinates need to be inputed
	bool replace;
	//prevents user from forcing a ship in an invalid direction
	bool valid = false;
	do
	{
		placementcheck = 0;
		replace = false;
		do
		{
			do
			{
				//If it is the users turn informs them to input a x and then y starting positions within the range.
				if (player == true)
				{
					cout << "You will now be placing the " << carrier.name << ". Please input the x position between 1 and 10.\n";
					cin >> carrier.xposition_start;
					cout << "You will now be placing the " << carrier.name << ". Please input the y position between 1 and 10.\n";
					cin >> carrier.yposition_start;
					//If invalid lets user know and exits the program.
					if (!carrier.xposition_start || !carrier.yposition_start)
					{
						cout << "An invalid argument has been entered. Exiting the program.\n";
						return true;
					}
					//Checks to see if x and y inputs are in range. If not informs user and allows them to input new coordinates.
					else if (carrier.xposition_start > 10 || carrier.xposition_start < 1 || carrier.yposition_start > 10 || carrier.yposition_start < 1)
					{
						system("CLS");
						board(counter, userFriendly, userEnemy);
						cout << "A value outside the the range has been inputted. Please input a valid input.\n";
						replace = true;
					}
					else
						replace = false;
					//Subtracts one off of both the x and y coordinates so they can be used in the array.
					carrier.xposition_start -= 1;
					carrier.yposition_start -= 1;
				}
				//If it is the computers turn randomly assigns values to x and y starting positions.
				else if (player == false)
				{
					carrier.xposition_start = rand() % 10 + 0;
					carrier.yposition_start = rand() % 10 + 0;
				}

			} while (replace == true);
			//Checks to see if the entered x and y starting positions are on top of an already placed ship and informs user if so
			//allowing them to input new coordinates.
			if (userFriendly[carrier.yposition_start][carrier.xposition_start] == '#')
			{
				if (player == true)
					cout << "You can't place ships on top of each other. Input another point.\n";
				display = false;
			}
			else
				display = true;
		} while (display == false);

		do
		{
			//If it is the players turn checks the directions that the ship can be placed and informs the user along with length 
			//of the ship. 
			if (player == true)
			{
				cout << "Select which direction you would like your ship to face. The " << carrier.name << " has a length of " << carrier.size << endl;
				if (Upcheck(carrier, userFriendly, display, placementcheck))
					cout << "1 for up\n";
				if (Downcheck(carrier, userFriendly, display, placementcheck))
					cout << "2 for down\n";
				if (Lcheck(carrier, userFriendly, display, placementcheck))
					cout << "3 for left\n";
				if (Rcheck(carrier, userFriendly, display, placementcheck))
					cout << "4 for right\n";
				//If there are no possible directions automatically sets input to 5 otherwise allows user input.
				if (placementcheck == 4)
					input = 5;
				else
					cin >> input;
			}
			//If computers turn randomly assigns direction and performs check later.
			else if (player == false)
				input = rand() % 4 + 1;
			//sets the ship direction to input.
			carrier.direction = input;
			//For first four cases checks to see if it is possible. If so sets the ship end position and fills in between. Otherwise
			//Allows new input of coordinates.
			do
			{
				switch (input)
				{
				case 1: carrier.yposition_end = carrier.yposition_start - (carrier.size - 1);
					if (Upcheck(carrier, userFriendly, display, placementcheck))
					{
						for (int i = carrier.yposition_start; i >= carrier.yposition_end; i--)
							userFriendly[i][carrier.xposition_start] = '#';
						valid = false;
					}
					else
						valid = true;
					display = true;
					break;
				case 2: carrier.yposition_end = carrier.yposition_start + (carrier.size - 1);
					if (Downcheck(carrier, userFriendly, display, placementcheck))
					{
						for (int i = carrier.yposition_start; i <= carrier.yposition_end; i++)
							userFriendly[i][carrier.xposition_start] = '#';
						valid = false;
					}
					else
						valid = true;
					display = true;
					break;
				case 3: carrier.xposition_end = carrier.xposition_start - (carrier.size - 1);
					if (Lcheck(carrier, userFriendly, display, placementcheck))
					{
						for (int j = carrier.xposition_start; j >= carrier.xposition_end; j--)
							userFriendly[carrier.yposition_start][j] = '#';
						valid = false;
					}
					else
						valid = true;
					display = true;
					break;
				case 4: carrier.xposition_end = carrier.xposition_start + (carrier.size - 1);
					if (Rcheck(carrier, userFriendly, display, placementcheck))
					{
						for (int j = carrier.xposition_start; j <= carrier.xposition_end; j++)
							userFriendly[carrier.yposition_start][j] = '#';
						valid = false;
					}
					else
						valid = true;
					display = true;
					break;
				//If there are no possible placements informs user and allows them to enter new coordinates.
				case 5:system("CLS");
					board(counter, userFriendly, userEnemy);
					cout << "You can't place your ship in that location. Please try again.\n";
					display = true;
					replace = true;
					break;
				//If an invalid character is entered. Informs user and them exits the program.
				default:system("CLS");
					board(counter, userFriendly, userEnemy);
					cout << "An invalid input has been entered. Please enter in a valid input.\n";
					display = false;
					return true;
				}
				//If the user tries to pick a direction they can't go lets them know and allows them to pick a new direction.
				if (valid == true)
				{
					if (player == true)
					{
						system("CLS");
						board(counter, userFriendly, userEnemy);
						cout << "Come on now, you know you can't go that direction. Pick a valid direction.\n";
					}
					display = true;
				}
			} while (display == false);
		} while (valid == true);
	} while (replace == true);
	//User is always second to place so adds one to counter in outside switch for placement and sets it so player is no longer true.
	if (player == true)
	{
		counter++;
		system("CLS");
		player = false;
	}
	//If the computer finished their placement sets player to true so next iteration the user will place.
	else if (player == false)
		player = true;
	return false;
}
//Checks to see if the ship can be placed upward. If not adds one to placementcheck.
bool Upcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck)
{
	if (carrier.yposition_start - (carrier.size - 1) >= 0)
	{
		for (int i = carrier.yposition_start; i > (carrier.yposition_start - carrier.size); i--)
		{
			if (userFriendly[i][carrier.xposition_start] != '#')
				display = true;
			else
			{
				display = false;
				break;
			}
		}
		if (display == true)
			return true;
		else
		{
			placementcheck++;
			return false;
		}
	}
	else
		placementcheck++;
	return false;
}
//Checks to see if the ship can be placed downward. If not adds one to placementcheck.
bool Downcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck)
{
	if (carrier.yposition_start + (carrier.size - 1) <= 9)
	{
		for (int i = carrier.yposition_start; i < (carrier.yposition_start + carrier.size); i++)
		{
			if (userFriendly[i][carrier.xposition_start] != '#')
				display = true;
			else
			{
				display = false;
				break;
			}
		}
		if (display == true)
			return true;
		else
		{
			placementcheck++;
			return false;
		}
	}
	else
		placementcheck++;
	return false;
}
//Checks to see if the ship can be placed leftward. If not adds one to placementcheck.
bool Lcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck)
{
	if (carrier.xposition_start - (carrier.size - 1) >= 0)
	{
		for (int i = carrier.xposition_start; i > (carrier.xposition_start - carrier.size); i--)
		{
			if (userFriendly[carrier.yposition_start][i] != '#')
				display = true;
			else
			{
				display = false;
				break;
			}
		}
		if (display == true)
			return true;
		else
		{
			placementcheck++;
			return false;
		}
	}
	else
		placementcheck++;
	return false;
}
//Checks to see if the ship can be placed rightward. If not adds one to placementcheck.
bool Rcheck(ship& carrier, char userFriendly[][10], bool display, int& placementcheck)
{
	if (carrier.xposition_start + (carrier.size - 1) <= 9)
	{
		for (int i = carrier.xposition_start; i < (carrier.xposition_start + carrier.size); i++)
		{
			if (userFriendly[carrier.yposition_start][i] != '#')
				display = true;
			else
			{
				display = false;
				break;
			}
		}
		if (display == true)
			return true;
		else
		{
			placementcheck++;
			return false;
		}
	}
	else
		placementcheck++;
	return false;
}

void board(int& counter, char userFriendly[][10], char userEnemy[][10])
{
	//Outputs column axis
	cout << "| x | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|" << setw(54) << "| x | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|\n";
	for (int i = 0; i < 10; i++)
	{
		//Outputs the lower half of the grid setion
		cout << "|___|___|___|___|___|___|___|___|___|___|___|" << setw(54) << "|___|___|___|___|___|___|___|___|___|___|___|\n";
		//outputs the row axises
		cout << "|" << setw(2) << i + 1 << " |";
		for (int j = 0; j < 10; j++)
		{
			//If counter is 0 then 0 ships have been placed and sets all values of the two grids to zero.
			if (counter == 0)
			{
				userFriendly[i][j] = '~';
				userEnemy[i][j] = '~';
			}
			//outputs first grid characters
			cout << setw(2) << userFriendly[i][j] << " " << "|";
		}
		//outputs second grid row axis
		cout << setw(9) << "|" << setw(2) << i + 1 << " |";
		for (int j = 0; j < 10; j++)
		{
			//outputs the values in the second grid
			cout << setw(2) << userEnemy[i][j] << " |";
		}
		cout << endl;
	}
	//outputs final bottom section.
	cout << "|___|___|___|___|___|___|___|___|___|___|___|" << setw(54) << "|___|___|___|___|___|___|___|___|___|___|___|\n";
}

bool turn(int& xcoordinate, int& ycoordinate, bool display, char userFriendly[][10], char userEnemy[][10], int& input,
	Fleet fleet[], int counter, bool& player)
{
	//used to determine which fleets is being worked with
	int fleetnumber;
	//used to output either "Your" or "The enemeies"
	string whosefleet;
	//flips value of player.
	if (player == true)
		player = false;
	else if (player == false)
		player = true;
	//Asks user if they would like to shoot or surrender. If they shoot function ontinues. If they surrender it ends current game.
	//If an invalid character is enter asks user to input a valid number.
	if (player == true)
	{
		do
		{
			cout << "Would you like to shoot or surrender?\n1 to shoot, 2 to surrender.\n";
			cin >> input;
			if (!input)
				return true;
			switch (input)
			{
			case 1: display = true;
				break;
			case 2: return false;
				break;
			default: cout << "Please input a valid number.\n";
				display = false;
				break;
			}
		} while (display == false);
		//sets the fleet number value and whosefleet name if it is the users turn.
		fleetnumber = 1;
		whosefleet = "The enemies";
	}
	do
	{
		//Asks the user to input the corrding of where they would like to fire. Checking if it is in range and a valid input. 
		//If invalid character is entered it exits the program.
		if (player == true)
		{
			do
			{
				cout << "Please input the x coordinate between 1 and 10 of where you would like to shoot.\n";
				cin >> xcoordinate;
				cout << "Now please input the y coordinate between 1 and 10 of where you would like to shoot.\n";
				cin >> ycoordinate;
				if (xcoordinate > 10 || xcoordinate < 1 || ycoordinate > 10 || ycoordinate < 1)
				{
					display = false;
					system("CLS");
					board(counter, userFriendly, userEnemy);
					cout << "One of the coordinates was out of range. Please input a value in range.\n";
				}
				else if (!xcoordinate || !ycoordinate)
				{
					cout << "An invalid argument has been entered. Exiting the program.\n";
					return true;
				}
				else
					display = true;
			} while (display == false);
			//Subtracts 1 off of both coordinates so that the array can use them.
			xcoordinate -= 1;
			ycoordinate -= 1;
		}
		//if it is computers turn sets fleetnumber value to correct fleet and whosefleet name. Also sets coordinates to random number
		//between 0 and 9 including those two.
		else if (player == false)
		{
			xcoordinate = rand() % 10 + 0;
			ycoordinate = rand() % 10 + 0;
			fleetnumber = 0;
			whosefleet = "Your";
		}
		//Checks to see if the coordinates where a hit and lets the user know accordingly if they were hit or got a hit.
		//Also lets user know if any ships have sank.
		if (userFriendly[ycoordinate][xcoordinate] == '#')
		{
			if (player == true)
				cout << "That's a hit.\n";
			userEnemy[ycoordinate][xcoordinate] = 'H';
			userFriendly[ycoordinate][xcoordinate] = 'H';
			if (shipcheck(xcoordinate, ycoordinate, fleet[fleetnumber].carrier))
				cout << whosefleet << " " << fleet[fleetnumber].carrier.name << " has been sunk.\n";
			if (shipcheck(xcoordinate, ycoordinate, fleet[fleetnumber].battleship))
				cout << whosefleet << " " << fleet[fleetnumber].battleship.name << " has been sunk.\n";
			if (shipcheck(xcoordinate, ycoordinate, fleet[fleetnumber].cruiser))
				cout << whosefleet << " " << fleet[fleetnumber].cruiser.name << " has been sunk.\n";
			if (shipcheck(xcoordinate, ycoordinate, fleet[fleetnumber].submarine))
				cout << whosefleet << " " << fleet[fleetnumber].submarine.name << " has been sunk.\n";
			if (shipcheck(xcoordinate, ycoordinate, fleet[fleetnumber].destroyer))
				cout << whosefleet << " " << fleet[fleetnumber].destroyer.name << " has been sunk.\n";

			if (player == false)
				cout << "You have been hit at (" << ++xcoordinate << "," << ++ycoordinate << ").\n";
		}
		//Checks to see if the coordinates missed and lets the user know accordingly, if they missed or if the computer missed.
		else if (userFriendly[ycoordinate][xcoordinate] == '~')
		{
			userEnemy[ycoordinate][xcoordinate] = 'M';
			userFriendly[ycoordinate][xcoordinate] = 'M';
			if (player == true)
				cout << "That's a miss.\n";
			else if (player == false)
				cout << "The enemy shot at (" << ++xcoordinate << "," << ++ycoordinate << ") and missed.\n";
			display = true;
		}
		//Makes sure that the user and computer don't fire where they have previously and lets user know if they did.
		else if (userFriendly[ycoordinate][xcoordinate] == 'M' || userFriendly[ycoordinate][xcoordinate] == 'H')
		{
			if (player == true)
				cout << "You can't shoot on top of where you already shot. Please input a new location to fire.\n";
			display = false;
		}
	} while (display == false);

	return false;
}
//Checks to see if the number of hits on the user and computers girds have reached 17 hits. If so ends the game.
bool check(char userEnemy[][10])
{
	int counter = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (userEnemy[i][j] == 'H')
				counter++;
		}
	}
	if (counter == 17)
		return true;
	else
		return false;
}
//Checks to see which direction the ship was placed. Once that is determined checks to see if the hit falls on that specific ship.
bool shipcheck(int xcoordinate, int ycoordinate, ship& carrier)
{
	if (carrier.direction == 1)
	{
		if (xcoordinate == carrier.xposition_start && ycoordinate <= carrier.yposition_start && ycoordinate >= carrier.yposition_end)
			carrier.health--;
	}
	else if (carrier.direction == 2)
	{
		if (xcoordinate == carrier.xposition_start && ycoordinate >= carrier.yposition_start && ycoordinate <= carrier.yposition_end)
			carrier.health--;
	}
	else if (carrier.direction == 3)
	{
		if (ycoordinate == carrier.yposition_start && xcoordinate <= carrier.xposition_start && xcoordinate >= carrier.xposition_end)
			carrier.health--;
	}
	else if (carrier.direction == 4)
	{
		if (ycoordinate == carrier.yposition_start && xcoordinate >= carrier.xposition_start && xcoordinate <= carrier.xposition_end)
			carrier.health--;
	}
	//Once the ship health reaches zero sets value to -1 and returns true letting user know that a ship has sunk.
	if (carrier.health == 0)
	{
		carrier.health = -1;
		return true;
	}
	return false;
}
//Outputs my cool starting screen.
void startup()
{
	cout << "\n";
	cout << "					              |    |    |                 \n";
	cout << "					             )_)  )_)  )_)                \n";
	cout << "					            )___))___))___)\			  \n";
	cout << "					           )____)____)_____)\\			  \n";
	cout << "					         _____|____|____|____\\\__		  \n";
	cout << "					---------\                   /---------	  \n";
	cout << "					  ^^^^^ ^^^^^^^^^^^^^^^^^^^^^			  \n";
	cout << "					    ^^^^      ^^^^     ^^^    ^^		  \n";
	cout << "					         ^^^^      ^^^					  \n";
	cout << "\n";
	cout << "					    	WELCOME TO BATTLESHIP	\n";

	system("PAUSE");

	cout << "\nThe game is simple to play.\n\n";
	cout << "First you will place your ships ships of varying lengths on a 10 by 10 grid.\n\n";
	cout << "Next you will begin the game by taking turns shooting at the enemies grid trying to sink their ships.\n\n";
	cout << "Each round you will either choose to shoot or surrender. If you shoot enter in your x and y coordinates to fire.\n\n";
	cout << "The winner is whoever sinks all of their opponents ships first.\n";

	system("PAUSE");
	system("CLS");
}
//Sets the values of the two fleets for each ship.
void values(Fleet fleet[])
{
	for (int i = 0; i <= 1; i++)
	{
		fleet[i].battleship.type = Battleship;
		fleet[i].battleship.size = 4;
		fleet[i].battleship.health = 4;
		fleet[i].battleship.name = "battleship";
		fleet[i].carrier.type = Carrier;
		fleet[i].carrier.size = 5;
		fleet[i].carrier.health = 5;
		fleet[i].carrier.name = "carrier";
		fleet[i].cruiser.type = Cruiser;
		fleet[i].cruiser.size = 3;
		fleet[i].cruiser.health = 3;
		fleet[i].cruiser.name = "cruiser";
		fleet[i].submarine.type = Submarine;
		fleet[i].submarine.size = 3;
		fleet[i].submarine.health = 3;
		fleet[i].submarine.name = "submarine";
		fleet[i].destroyer.type = Destroyer;
		fleet[i].destroyer.size = 2;
		fleet[i].destroyer.health = 2;
		fleet[i].destroyer.name = "destroyer";
	}
}
//Performs the turn function alternating between the user and the computer checking after each turn whether the games was won or not.
bool turns(int& xcoordinate, int& ycoordinate, bool& display, char compFriendly[][10], char userEnemy[][10], int input, Fleet fleet[], int& counter,
	char userFriendly[][10], bool& game, bool skip, int& gwon, int& glost, char compEnemy[][10], bool& player)
{
	do
	{
		if (turn(xcoordinate, ycoordinate, display, compFriendly, userEnemy, input, fleet, counter, player))
			return true;
		if (check(userEnemy))
		{
			cout << "Congratulations, you won.\n";
			game = false;
			skip = true;
			gwon++;
		}
		else if (input == 2)
		{
			skip = true;
			game = false;
			glost++;
		}
		else
			skip = false;
		while (skip == false)
		{
			turn(xcoordinate, ycoordinate, display, userFriendly, compEnemy, input, fleet, counter, player);
			if (check(compEnemy))
			{
				cout << "Sorry you lost.\n";
				game = false;
				glost++;
			}
			skip = true;
		}
		system("PAUSE");
		system("CLS");
		cout << setw(30) << "Friendly Shipyard" << setw(53) << "Enemy Shipyard\n";
		board(counter, userFriendly, userEnemy);
#ifdef Debug
		cout << setw(30) << "C Friendly Shipyard" << setw(53) << "C Enemy Shipyard\n";
		board(counter, compFriendly, compEnemy);
#endif
	} while (game);

	return false;
}
//Asks the user if they would like to play again resetting certain if so. Otherwise outputing how many games were won or loss and 
//ending the program. If an invalid value is entered exits the program.
bool playagain(int input, bool& play, bool& game, bool& setup, int& gwon, int& glost, int& counter, bool& player)
{
	cout << "Would you like to play again? 1 for yes and 2 for no.\n";
	cin >> input;
	switch (input)
	{
	case 1: play = true;
		counter = 0;
		game = true;
		setup = true;
		player = false;
		break;
	case 2:play = false;
		cout << "You won " << gwon << " games and lost " << glost << " games.\nThank you for playing.\n";
		break;
	default: cout << "Inputted an incorrect value exiting program.\n";
		return true;
	}
	return false;
}
//Performs the placement of the ships for the game responding accordingly for the functions inside this function. The placement
//function is performed 10 times. 5 for each fleet.
bool settingUp(int& counter, char userFriendly[][10], char userEnemy[][10], char compFriendly[][10], char compEnemy[][10], bool& display,
	Fleet fleet[], int& input, bool& setup, int placementcheck, bool player)
{
	do
	{
		cout << setw(30) << "Friendly Shipyard" << setw(53) << "Enemy Shipyard\n";
		board(counter, userFriendly, userEnemy);
#ifdef Debug
		cout << setw(30) << "C Friendly Shipyard" << setw(53) << "C Enemy Shipyard\n";
		board(counter, compFriendly, compEnemy);
#endif

		switch (counter)
		{
		case 0: placement(fleet[1].carrier, counter, compFriendly, input, display, placementcheck, compEnemy, player);
			if (placement(fleet[0].carrier, counter, userFriendly, input, display, placementcheck, userEnemy, player))
				return true;
			break;
		case 1: placement(fleet[1].battleship, counter, compFriendly, input, display, placementcheck, compEnemy, player);
			if (placement(fleet[0].battleship, counter, userFriendly, input, display, placementcheck, userEnemy, player))
				return true;
			break;
		case 2: placement(fleet[1].cruiser, counter, compFriendly, input, display, placementcheck, compEnemy, player);
			if (placement(fleet[0].cruiser, counter, userFriendly, input, display, placementcheck, userEnemy, player))
				return true;
			break;
		case 3: placement(fleet[1].submarine, counter, compFriendly, input, display, placementcheck, compEnemy, player);
			if (placement(fleet[0].submarine, counter, userFriendly, input, display, placementcheck, userEnemy, player))
				return true;
			break;
		case 4: placement(fleet[1].destroyer, counter, compFriendly, input, display, placementcheck, compEnemy, player);
			if (placement(fleet[0].destroyer, counter, userFriendly, input, display, placementcheck, userEnemy, player))
				return true;
			break;
		case 5: setup = false;
		}
	} while (setup == true);
	return false;
}