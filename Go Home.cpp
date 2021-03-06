#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstddef>
using namespace std;

struct Player{
    string firstname;
    string boardpiece;
    int gameposition;
    int boardpiecenumber;
};

void displayRules();
void Play(Player *player, int numPlayers, string cardPile[], string gameBoardposition[]);
void displayBoard(string gameBoardposition[]);
void moveBack(Player *player, int playerindicator, int gamepos, int numPlayer, string gameBoardposition[]);
void resetBoard(string gameBoardposition[]);
void bumpToStart(Player *player, int gamepos, int numPlayer, int playerindication, string gameBoardposition[]);
void printGamePieces();
void printCards(int numcards);
void peformMoveBackAction(Player *player, int playerindicator, int gamepos, int numPlayer, string gameBoardposition[], int moveBackAmount);
bool findSameName(Player *player, int numPlayers, string playerName, int playerchoice);
bool findSameBoardPiece(Player *player, int numPlayers, int playerchoice);
bool taketurn(Player *player, int playerindicator, int &numcards, int numPlayers, string cardPile[], string gameBoardposition[]);
bool revealCard(Player *player, int playerposition, int numcards, int numPlayers, string cardPile[], string gameBoardposition[]);
void readPlayer(Player *player, int numplayer, string &smallestname, string gameBoardposition[]);
void switchPlaces(Player *player, int playerindicator, int numPlayers, string gameBoardposition[]);
void createfirsteightboxes(string gameBoardposition[]);
void createTwoSquares(int &leftgamepieces, int &rightgamepieces, string gameBoardposition[]);
void createTwoSquaresWithLine(int leftgamepieces, int rightgamepieces, string gameBoardposition[]);
void createlasteightboxes(string gameBoardposition[]);
void placeObstacles(string gameBoardposition[]);
void shuffleCard(int numcards, string gameBoardposition[]);
void clearBoard(Player *player, int numPlayers, string gameBoardposition[]);
void initializePlayerValues(Player *player);
void findSmallestName(Player *player, int numPlayers);
void printInfo(Player *player, int numPlayer);

int main()
{
    bool gameLoop;
    char userChoice;
    string smallestname, cardPile[10]{"1", "1", "2", "2", "3", "3", "4", "4", "Lose Turn", "Switch Place"};
    string gameBoardposition[22];
    int numPlayers, positionOfSmallestName;

    srand(time(0));//generates random number generator seed for shuffle card function
    cout << "How many players will be playing? No more than six!: ";
	cin >> numPlayers;

	while(numPlayers > 6){
		cout << "NO MORE THAN SIX. TRY AGAIN: ";
		cin >> numPlayers;
	}


    Player *player = new Player[numPlayers];
	displayRules();
	readPlayer(player, numPlayers, smallestname, gameBoardposition);
	do{
        shuffleCard(10, cardPile);
        Play(player, numPlayers, cardPile, gameBoardposition);
        cout << "Do you want to play again? y/n: ";
        cin >> userChoice;
        system("cls");
        clearBoard(player, numPlayers, gameBoardposition);
	}while(userChoice == 'Y' || userChoice == 'y');

	cout << "Goodbye! Thanks for playing!" << endl;

	delete [] player;

	player = NULL;

	return 0;
}

//clears the board of all the players board pieces, set each players position to zero, and then redraw the obstacles
void clearBoard(Player *player, int numPlayers, string gameBoardposition[])
{
    for(int i = 0; i < 22; i++)
        gameBoardposition[i] = "";
    placeObstacles(gameBoardposition);
    for(int k = 0; k < numPlayers; k++)
        player[k].gameposition = 0;
}


//initializes thee players game positions, and their board pieces
void initializePlayerValues(Player *player, int numPlayers, string gameBoardposition[])
{
    for(int i = 0; i < numPlayers; i++){
        player[i].boardpiece = "";
        player[i].firstname = "";
        player[i].boardpiecenumber = 0;
    }
    clearBoard(player, numPlayers, gameBoardposition);
}

//one of the most important functions here, this little guy will clear the screen, and redraw the board, effectively updating the
//board every time a change to it is made
void resetBoard(string gameBoardposition[])
{
    system("cls");
    displayBoard(gameBoardposition);
}

//Displays the rules of the game to the screen
void displayRules()
{
    cout << "\t\t\t\t\t*******************" << endl;
    cout << "\t\t\t\t\t*  GoHome Rules   *" << endl;
    cout << "\t\t\t\t\t*******************" << endl << endl;
    cout << "Rule #1: Players take turns in order until one player wins. A turn consists of picking a card. If the" << endl;
    cout << "card has a number, the player moves that number of spaces. \n\n";
    cout << "Rule #2: If the card says 'lose turn', the player does nothing, and the game continues with the next player. \n\n";
    cout << "Rule #3: If the card says 'switch'places, the player must switch places with a player of its choice. \n\n";
    cout << "Rule #4: If the player lands on a space with a number, the player moves back that number of spaces. \n";
}

//This function will randomize the order in which the card pile is shuffled.
void shuffleCard(int numcards, string cardPile[])
{
    int randomize;

    for(int i = 0; i < numcards; i++){
    	randomize = rand () % numcards;
    	swap(cardPile[i], cardPile[randomize]);//swap the card in position i with another card in the same pile
	}
}

//prints selection of game pieces to the screen
void printGamePieces()
{
    cout << "\n1 = #" << "\t2 = @" << endl;
    cout << "3 = $" << "\t4 = %" << endl;
    cout << "5 = &" << "\t6 = +" << endl;
    cout << "7 = :)" << "\t8 = :(" << endl;
    cout << "9 = !" << "\t10 = ?" << endl;
}

//This function will assign the player's chosen game piece to their board piece structure member
void determineGamePiece(Player *player, int i, int choice)
{
    switch(choice){
        case 1:
            player[i].boardpiece = "  #";
            break;
        case 2:
            player[i].boardpiece = "  @";
            break;
        case 3:
            player[i].boardpiece = "  $";
            break;
        case 4:
            player[i].boardpiece = "  %";
            break;
        case 5:
            player[i].boardpiece = "  &";
            break;
        case 6:
            player[i].boardpiece = "  +";
            break;
        case 7:
            player[i].boardpiece = " :)";
            break;
        case 8:
            player[i].boardpiece = " :(";
            break;
        case 9:
            player[i].boardpiece = "  !";
            break;
        default:
            player[i].boardpiece = "  ?";
    }
}

//this function will prevent players from entering two of the same names
bool findSameName(Player *player, int numPlayers, string playerName)
{
    for(int i = 0; i < numPlayers; i++)
        if(playerName == player[i].firstname)
             return true;
    return false;
}

//this function will prevent players from entering two of the same board pieces
bool findSameBoardPiece(Player *player, int numPlayers, int playerchoice)
{
    for(int i = 0; i < numPlayers; i++)
        if(playerchoice == player[i].boardpiecenumber)
            return true;//if the name the board piece entered is the same as the one already entered in the array of players, return true
    return false;
}

//This function will initialize each players position to zero, prompt the players to enter their names, allow them to choose their
//preferred game piece, and will determine who has the smallest name
void readPlayer(Player *player, int numPlayers, string &smallestname, string gameBoardposition[])
{
    int choice;
    string playername;

    initializePlayerValues(player, numPlayers, gameBoardposition);
    for(int i = 0; i < numPlayers; i++){
        cout << "Player " << i+1 << " enter your first name: ";
        cin >> playername;//enter a name, and check to see if it is inn the array of first names
        while(findSameName(player, numPlayers, playername)){
            cout << "Name is taken asshole! Enter a different one: ";//excuse the language hahaha
            cin >> playername;
        }
        player[i].firstname = playername;
        printGamePieces();
        cout << player[i].firstname << ", choose your game piece: ";
        cin >> choice;
        while(findSameBoardPiece(player, numPlayers, choice)){
            cout << "Sorry " << player[i].firstname << ", that game piece is taken. Choose a different one: ";
            printGamePieces();
            cin >> choice;
        }
        player[i].boardpiecenumber = choice;
        determineGamePiece(player, i, choice); //after the player choose the game piece, assign it to the "boardpiece" structure member
        cout << player[i].firstname << " your game piece is '" << player[i].boardpiece << "'" << endl << endl;
    }

    findSmallestName(player, numPlayers);
}

//finds the smallest first name
void findSmallestName(Player *player, int numPlayers)
{
    int smallestNameSoFar, positionOfSmallestName;

    smallestNameSoFar = player[0].firstname.length();
    positionOfSmallestName = 0;
    for(int i = 1; i < numPlayers; i++){
        if (player[i].firstname.length() < smallestNameSoFar){
            smallestNameSoFar = player[i].firstname.length();
            positionOfSmallestName = i;//assign the position of the player with the smallest name to this variable
        }
    }
    swap(player[0], player[positionOfSmallestName]);//swaps the player with the smallest name with the first player.
    cout << "\n" << player[0].firstname << ", You will go first (You have the shortest name!)." << endl;
    system("pause");
    system("cls");
}

//this function plays the actual game, using boolean values and a while loop to control the flow of
// the game and each players turn
void Play(Player *player, int numPlayers, string cardPile[], string gameBoardposition[])
{
    int  numcards = 10, turnNumber = 1;
    bool continueGame = true;//set continueGame to true to enter while loop, and will fall out if taketurn returns false

    placeObstacles(gameBoardposition);
	displayBoard(gameBoardposition);
	while(continueGame)
        for(int playerindicator = 0; playerindicator < numPlayers && continueGame == true; playerindicator++)
            continueGame = taketurn(player, playerindicator, numcards, numPlayers, cardPile, gameBoardposition);

    for(int winner = 0; winner < numPlayers; winner++)
        if(player[winner].gameposition == 21)
            cout << player[winner].firstname << " won the game!" << endl;
}

//this function will allow each player to take their turn, and return true or false depending on where they are
bool taketurn(Player *player, int playerindicator, int &numcards, int numPlayers, string cardPile[], string gameBoardposition[])
{
    bool turnEnd;

    turnEnd = revealCard(player, playerindicator, numcards, numPlayers, cardPile, gameBoardposition);
    numcards--;
    if(numcards == 0){//after reducing the number of cards by one after a turn is taken, check to see if the card pile is reduced to zero
        numcards = 10;//if true, recover card deck, and shuffle them
        shuffleCard(numcards, cardPile);
    }

    if(turnEnd)
        return true;//if the reveal card function returned true, return true to play function to signal that game is still in play
    else
        return false;//otherwise, return false to signal that someone won the game (landed on home)
}

//determines which card the program chooses for the player is which i.e. whether or not it is
bool revealCard(Player *player, int playerindicator, int numCards, int numPlayers, string cardPile[], string gameBoardposition[])
{
    int previousPosition, gamepos = 0, overShootDistance, moveBackDistance;
    int otherPlayerPosition;

    previousPosition = player[playerindicator].gameposition;//Saves the players previous position so it can be erased later on.

    cout << "Game piece: " << player[playerindicator].boardpiece << endl;
    cout << player[playerindicator].firstname << ", Your card is \"" << cardPile[numCards-1] << "\"." << endl;

    //since the cards are shuffled, which cards are which are unknown, so we need to process each possible choice of card
    if (cardPile[numCards-1] == "1"){
        cout << "You will move one space on the board" << endl;
        gamepos = ++player[playerindicator].gameposition;
    }else if(cardPile[numCards-1] == "2"){
        cout << "You will move two spaces on the board" << endl;
        gamepos = player[playerindicator].gameposition += 2;
    }else if(cardPile[numCards-1] == "3"){
        cout << "You will move three spaces on the board" << endl;
        gamepos = player[playerindicator].gameposition += 3;
    }else if(cardPile[numCards-1] == "4"){
        cout << "You will move four spaces on the board" << endl;
        gamepos = player[playerindicator].gameposition += 4;
    }else if(cardPile[numCards-1] == "Lose Turn"){
        cout << "Oh no! you lose your turn! :(" << endl;
        system("pause");
        return true;
    }else{
        switchPlaces(player, playerindicator, numPlayers, gameBoardposition);
        return true;//once positions have been swapped, end turn immediately
	}


    //After the players position is incremented by the number of the card they chose, check to see if their is greater than
    //or equal to 21. This is to ensure that the program does not go out of bounds, as the game board array only has 22 positions(0-21)
    system("pause");
    if(gamepos == 21){//in order to win game, player must land on home EXACTLY, which is position 21
        gameBoardposition[21] = player[playerindicator].boardpiece;
        gameBoardposition[previousPosition] = "";
        resetBoard(gameBoardposition);
        return false;
    }else if (gamepos > 21){
        player[playerindicator].gameposition = previousPosition;//if the player's position is not 21, they will not progress.
        cout << "You need to land EXACTLY on home to win! No overshooting allowed!" << endl;
        return true;
    }
    else{
        gameBoardposition[gamepos] = player[playerindicator].boardpiece;
        gameBoardposition[previousPosition] = "";
        bumpToStart(player, gamepos, numPlayers, playerindicator, gameBoardposition);
        resetBoard(gameBoardposition);
        moveBack(player, playerindicator, gamepos, numPlayers, gameBoardposition);
        return true;//after the board pieces have been placed, end player turn, and return true to take turn function
    }
}

//if two players share the same space after a certain action, the player who was originally there will get sent back to start
void bumpToStart(Player *player, int gamepos, int numPlayer, int playerindicator, string gameBoardposition[])
{
    for(int index = 0; index < numPlayer; index++){
        if(gamepos == player[index].gameposition && index != playerindicator){
            player[index].gameposition = 0;
            gameBoardposition[player[index].gameposition] = player[index].boardpiece;
            resetBoard(gameBoardposition);
            cout << "Sorry " << player[index].firstname << ", you got bumped back to start!" << endl;
            system("pause");
        }
    }
}

//This function serves the purpose of moving the player's game piece by the number of the obstacle they landed on
//i.e. if the player lands on a "2", they have to move back two spaces
void moveBack(Player *player, int playerindicator, int gamepos, int numPlayer, string gameBoardposition[])
{
	//if the player's position is the same as that of an obstacle, move the player back by the amount of spaces by the number in that position.
    if(gamepos == 6 || gamepos == 20){// position 6 and 20 on the board are the positions of the obstacle "1"
        cout << "Woops! You landed on a \"1\" You have to move back one space!" << endl;
        peformMoveBackAction(player, playerindicator, gamepos, numPlayer, gameBoardposition, 1);
    }
    else if(gamepos == 3 || gamepos == 14|| gamepos == 18){// positions 3, 14 and 20 on the board are the positions of the obstacle "2"
        cout << "Woops! You landed on a \"2\" You have to move back two spaces!" << endl;
        peformMoveBackAction(player, playerindicator, gamepos, numPlayer, gameBoardposition, 2);
    }
    else if(gamepos == 8){// position 8 on the board is the position of the obstacle "3"
        cout << "Woops! You landed on a \"3\" You have to move back three spaces!" << endl;
        peformMoveBackAction(player, playerindicator, gamepos, numPlayer, gameBoardposition, 3);
    }
}

//performs the action of actually moving back
void peformMoveBackAction(Player *player, int playerindicator, int gamepos, int numPlayer, string gameBoardposition[], int moveBackAmount)
{
    player[playerindicator].gameposition -= moveBackAmount;
    gameBoardposition[gamepos] = "";
    gameBoardposition[player[playerindicator].gameposition] = player[playerindicator].boardpiece;
    placeObstacles(gameBoardposition);
    system("pause");
    resetBoard(gameBoardposition);
    bumpToStart(player, player[playerindicator].gameposition, numPlayer, playerindicator, gameBoardposition);
}

//if a "switch place" card is drawn, have that player switch places with the player furthest along the board.
void switchPlaces(Player *player, int playerindicator, int numPlayers, string gameBoardposition[])
{
    int largestValue = player[0].gameposition, positionOfLargest = 0;

    for(int i = 1; i < numPlayers; i++)
        if(player[i].gameposition > largestValue){
            largestValue = player[i].gameposition;
            positionOfLargest = i;//takes index of player furthest along the board
        }

    //only swap if possible, if the person who drew the card is NOT the farthest along.
    if(player[playerindicator].gameposition < player[positionOfLargest].gameposition){
        cout << player[playerindicator].firstname << ", you will be switching places with " << player[positionOfLargest].firstname << "!" << endl;
        system("pause");
        swap(player[playerindicator].gameposition, player[positionOfLargest].gameposition);
        gameBoardposition[player[playerindicator].gameposition] = player[playerindicator].boardpiece;
        gameBoardposition[player[positionOfLargest].gameposition] = player[positionOfLargest].boardpiece;
        resetBoard(gameBoardposition);
    }
    else{
        cout << player[playerindicator].firstname << ", you're the farthest along, so you won't switch with any player. Lucky Bastard!" << endl;
        system("pause");
    }
}

//places the obstacles on the board
void placeObstacles(string gameBoardposition[])
{
	gameBoardposition[0] = "start";
	gameBoardposition[3] = "  2";
	gameBoardposition[6] = "  1";
	gameBoardposition[8] = "  3";
	gameBoardposition[14] = "  2";
	gameBoardposition[18] = "  2";
	gameBoardposition[20] = "  1";
	gameBoardposition[21] = "home";
}

void createfirsteightboxes(string gameBoardposition[])
{
	int topBoard = 18;//this number will be the index of the gameBoardposition, allowing the elements to be print in each box
	//in this case, this variable will be the index for elements 18 -11

	cout << " ";
    for(int topBoundary = 0; topBoundary < 63; topBoundary ++)
        cout << "_";
	cout << endl;

	for(int j = 0; j < 9; j++)
        cout << "|\t";
    cout << endl;

    //continues to print the box boundaries, and prints the game piece to show where each player is
    for(int boxBoundary = 0; boxBoundary < 8; boxBoundary++){
        cout << "| " << gameBoardposition[topBoard] << "\t";//prints array positions 18-11 (left to right) for each box on the top of the game board
        topBoard--;
    }
    cout << "|" << endl;

    //finishes bottom boundary of the bottom half of the game board
    for(int bottomboundary = 0; bottomboundary < 8; bottomboundary++)
        cout << "|_______";
    cout << "|\n";
}

//creates a square on either side of the board
void createTwoSqaures(int &leftgamepieces, int &rightgamepieces, string gameBoardposition[])
{
	cout << "|       |" << "\t\t\t\t\t\t" << "|       |" << endl;
    cout << "| "  << gameBoardposition[leftgamepieces] <<"\t|\t\t\t\t\t\t" << "| " << gameBoardposition[rightgamepieces] << "\t|"<< endl;
    cout << "|_______|" << "\t\t\t\t\t\t" << "|_______|\n";

    leftgamepieces++;
	rightgamepieces--;
}

//creates the third set of squares, with a long line at the bottom to create the top portion of the last row of boxes
void createTwoSquaresWithLine(int leftgamepieces, int rightgamepieces, string gameBoardposition[])
{
	cout << "|       |" << "\t\t\t\t\t\t" << "|       |" << endl;
    cout << "| "  << gameBoardposition[leftgamepieces] <<"\t|\t\t\t\t\t\t" << "| " << gameBoardposition[rightgamepieces] << "\t|"<< endl;
    cout << "|_______|";
	for(int i = 0; i < 47; i++)
	    cout << "_";
	cout << "|_______|\n";
}

//create the very last set of boxes, thus completing the game board. Does the same thing as createfirsteightboxes
//, but ignores the top line and prints the rest of the boxes
void createlasteightboxes(string gameBoardposition[])
{
	for(int j = 0; j < 9; j++)
        cout << "|\t";
    cout << endl;

    for(int boxBoundary = 0; boxBoundary < 8; boxBoundary++){
        cout << "| " << gameBoardposition[boxBoundary] << "\t"; // in
    }
    cout << "|" << endl;

    //finishes bottom boundary of the bottom half of the game board
    for(int bottomboundary = 0; bottomboundary < 8; bottomboundary++)
        cout << "|_______";
    cout << "|\n";
}

void displayBoard(string gameBoardposition[])
{
	int leftgamepieces = 19, rightgamepieces = 10;

	createfirsteightboxes(gameBoardposition);
	for(int i = 0; i < 2; i++)
		createTwoSqaures(leftgamepieces, rightgamepieces, gameBoardposition);
	createTwoSquaresWithLine(leftgamepieces, rightgamepieces, gameBoardposition);
	createlasteightboxes(gameBoardposition);
}
