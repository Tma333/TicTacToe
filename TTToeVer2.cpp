#include <iostream>
#include <time.h>
using namespace std;

char GameBoard[9];
int SpaceTakenCounter =0;

void DisplayBoard();
void GetInput();
void AIMove(char XorO);
void AIMoveRandom();
bool WinCond(bool &GameOver);
bool WinCond();

void CheckLine(int X, int O, int &XValue, int & OValue);
int ValueCalc();
int MiniMove(int Alpha, int Beta);
int MaxMove(int &MoveLocation);
int MaxMove(int Alpha, int Beta);
void StartGame();

int main(){
	bool PlayAgain = true;
	char Answer;
	
	while (PlayAgain==true){
		StartGame();
		cout << "Play again?(y/n):";
		cin >> Answer;
		Answer = tolower(Answer);
		if (Answer == 'y'){
			PlayAgain=true;
			SpaceTakenCounter=0;
		}
		else {
			PlayAgain=false;
			cout << "\nThanks for playing!" << endl << endl;
		}
	}
	system("pause");
	return 0;
}

void StartGame(){
	// fill board with numbers
	for (int i=0;i<9;i++){
		GameBoard[i]=i+49;
	}
	DisplayBoard();

	bool GameOver = false;

	// game over is changed to true when win condition is meet.  Space taken account for draw games that go all the way to turn 9.  Game over is check before each move.  The board is displayed after a move is made and the win conditon is checked which will update true or false accordingly.

	while (GameOver==false&&SpaceTakenCounter<9){
		if (GameOver==false){
			GetInput();
			DisplayBoard();

			WinCond(GameOver);
		}
		if (GameOver==false){
			AIMove('X');
			DisplayBoard();
			WinCond(GameOver);
		}
	}
}

int MiniMove(int Alpha, int Beta){

	int MiniValue = 1000;
	int Value=1000;
	int PlyCount=0;

	for (int i=0;i<9;i++){
		if (GameBoard[i]=='X'||GameBoard[i]=='O'){
			PlyCount++;
		}
	}

	// at the end return bottom value calculation
	if (PlyCount>=9){
		return ValueCalc();
	}

	for (int i=0;i<9;i++){
		if (GameBoard[i]!='X'&&GameBoard[i]!='O'){
			
			if (Alpha >= Beta){
				return Beta;
			}
			else if (Alpha <=Beta){
				GameBoard[i]='O';
		
				bool WinConditionResult = WinCond();

				if (WinConditionResult==false){
					Value = MaxMove(Alpha, Beta);
					if (Value < Beta){
						Beta = Value;
					}
				}
				else if (WinConditionResult==true&&GameBoard[i]=='O'){
					Value = -10;
					if (Value < Beta){
						Beta = Value;
					}
				}
				GameBoard[i] = i+49;
			}
		}
	}

	return Beta;
	//return MiniValue;
}

int MaxMove(int Alpha, int Beta){
	int MaxValue = -1000;
	int Value;
	int PlyCount = 0;

	// count the depth, also used to determine no more moves are to be made.
	for (int i=0;i<9;i++){
		if (GameBoard[i]=='X'||GameBoard[i]=='O'){
			PlyCount++;
		}
	}

	// if board full return the value of board
	if (PlyCount>=9){
		return ValueCalc();
	}

	// find empty space fill with x and spawn children to return find values.
	for (int i=0;i<9;i++){
		if (GameBoard[i]!='X'&&GameBoard[i]!='O'){

			// if condition is meet no need to check values prune remaining branches.
			if (Alpha >=Beta){
				return Alpha;
			}
			// if not pruned proceed
			else if (Alpha<=Beta){
				// fill empty space temporarily with X
				GameBoard[i]='X';

				// Check to see if the filled space result in a win
				bool WinConditionResult = WinCond();

				// not a win then check for children values
				if (WinConditionResult==false){
					Value = MiniMove(Alpha, Beta);
					if (Value > Alpha){
						Alpha = Value;
					}
				}
				// check to see if there is a win condition present with current move, record value if present.
				else if (WinConditionResult==true){
					Value = 10;
					if (Value > Alpha){
						Alpha = Value;
					}
				}
				// revert board back original state before temporary x fill
				GameBoard[i] = i+49;
			}
		}
	}
	// return the Max value
	return Alpha;
}

int MaxMove(int &MoveLocation){
	//int MaxValue = -1000;
	int Value;
	int PlyCount = 0;
	int Alpha = -1000;
	int Beta = 1000;
	
	for (int i=0;i<9;i++){
		if (GameBoard[i]=='X'||GameBoard[i]=='O'){
			PlyCount++;
		}
	}
	// if board if full return value of board.
	if (PlyCount>=9){
		return ValueCalc();
	}

	for (int i=0;i<9;i++){
		if (GameBoard[i]!='X'&&GameBoard[i]!='O'){

			GameBoard[i]='X';

			bool WinConditionResult = WinCond();
			if (WinConditionResult==false){
				Value = MiniMove(Alpha, Beta);
			}
			else if (WinConditionResult==true&&GameBoard[i]=='X'){
				Value = 10;
			}
			
			if (Alpha < Value){
				Alpha = Value;
				MoveLocation = i;
			}

			GameBoard[i] = i+49;
		}
	}
	
	// no more free spaces return the value of the board

	return MoveLocation;
}

void AIMove(char XorO){
	XorO = toupper(XorO);
	int MoveLocation=11;
	SpaceTakenCounter++;

	if (SpaceTakenCounter<9){
		if (XorO=='X'){
			MoveLocation=MaxMove(MoveLocation);
			GameBoard[MoveLocation]=XorO;
			cout << "AI move location:" << MoveLocation+1 << endl;
		}

		// this is for future option of choosing to play as x or o
		else if (XorO=='O'){
			//MiniMove(Alpha, Beta);
		}
	}
}

void DisplayBoard(){
	system("cls");
	
	cout << GameBoard[0] << '|' << (GameBoard[1]) << '|' << (GameBoard[2]) << endl;
	cout << "=====\n";
	cout << GameBoard[3] << '|' << GameBoard[4] << '|' << GameBoard[5] << endl;
	cout << "=====\n";
	cout << GameBoard[6] << '|' << GameBoard[7] << '|' << GameBoard[8] << endl;
	cout << endl;
}

void GetInput(){
	cout << "Enter your next move: ";
	int temp;
	cin >> temp;

	if (cin.fail()){
		cout << "Invalid input, please try again." << endl << endl;
		cin.clear();
		cin.ignore();
		GetInput();
	}
	else if (temp > 9||temp < 1){
		cout << "Please pick between 1 and 9\n" << endl;
		GetInput();
	}
	else if (GameBoard[temp-1]!='X'&&GameBoard[temp-1]!='O'){
		GameBoard[temp-1]='O';
		SpaceTakenCounter++;
	}
	else {
		cout << endl << "Spot " << temp << " is already taken, pick another.\n\n";
		GetInput();
	}
}

void AIMoveRandom(){
	int MoveLocation = -1;
	MoveLocation = MaxMove(MoveLocation);
	cout << "MoveLocation:" << MoveLocation << endl;
	GameBoard[MoveLocation]= 'O';
	SpaceTakenCounter++;
	
	/*
	int RandomNumber;
	srand(time(0));
	bool Check = false;
	while (Check==false){
		RandomNumber = rand() %9;
	
		if (GameBoard[RandomNumber]!='X'&&GameBoard[RandomNumber]!='O'){
			GameBoard[RandomNumber]='O';
			DisplayBoard();
			cout << "Computer's move at location " << RandomNumber+1 << endl;
			SpaceTakenCounter++;
			Check=true;
		}
	}
	*/

}

bool WinCond(bool &GameOver){
	if (GameBoard[0]==GameBoard[1]&&GameBoard[1]==GameBoard[2]){
		cout << GameBoard[0] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[3]==GameBoard[4]&&GameBoard[4]==GameBoard[5]){
		cout << GameBoard[3] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[6]==GameBoard[7]&&GameBoard[7]==GameBoard[8]){
		cout << GameBoard[6] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[0]==GameBoard[3]&&GameBoard[3]==GameBoard[6]){
		cout << GameBoard[0] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[1]==GameBoard[4]&&GameBoard[4]==GameBoard[7]){
		cout << GameBoard[1] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[2]==GameBoard[5]&&GameBoard[5]==GameBoard[8]){
		cout << GameBoard[2] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[0]==GameBoard[4]&&GameBoard[4]==GameBoard[8]){
		cout << GameBoard[0] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (GameBoard[2]==GameBoard[4]&&GameBoard[4]==GameBoard[6]){
		cout << GameBoard[2] << " Wins!" << endl;
		GameOver=true;
		return true;
	}
	else if (SpaceTakenCounter>9){
		cout << "It's a draw" << endl;
		GameOver=true;
		return true;
	}
	GameOver = false;
	return false;
}

bool WinCond(){
	if (GameBoard[0]==GameBoard[1]&&GameBoard[1]==GameBoard[2]){
		return true;
	}
	else if (GameBoard[3]==GameBoard[4]&&GameBoard[4]==GameBoard[5]){
		return true;
	}
	else if (GameBoard[6]==GameBoard[7]&&GameBoard[7]==GameBoard[8]){
		return true;
	}
	else if (GameBoard[0]==GameBoard[3]&&GameBoard[3]==GameBoard[6]){
		return true;
	}
	else if (GameBoard[1]==GameBoard[4]&&GameBoard[4]==GameBoard[7]){
		return true;
	}
	else if (GameBoard[2]==GameBoard[5]&&GameBoard[5]==GameBoard[8]){
		return true;
	}
	else if (GameBoard[0]==GameBoard[4]&&GameBoard[4]==GameBoard[8]){
		return true;
	}
	else if (GameBoard[2]==GameBoard[4]&&GameBoard[4]==GameBoard[6]){
		return true;
	}
	else if (SpaceTakenCounter>9){
		return true;
	}
	return false;
}

int ValueCalc(){
	int XValue=0;
	int OValue=0;
	int LineX=0;
	int LineO=0;
	
	for (int i=0;i<3;i++){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);
	
	LineX=0;
	LineO=0;
	for (int i=3;i<6;i++){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);

	LineX=0;
	LineO=0;
	for (int i=6;i<9;i++){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);

	LineX=0;
	LineO=0;
	for (int i=0;i<7;i+=3){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);

	LineX=0;
	LineO=0;
	for (int i=1;i<8;i+=3){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);

	LineX=0;
	LineO=0;
	for (int i=2;i<9;i+=3){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);

	LineX=0;
	LineO=0;
	for (int i=0;i<9;i+=4){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);

	LineX=0;
	LineO=0;
	for (int i=2;i<7;i+=2){
		if (GameBoard[i]=='X'){LineX++;}
		else if (GameBoard[i]=='O'){LineO++;}
	}
	CheckLine(LineX, LineO, XValue, OValue);
	// cout << "[" <<  XValue << ":" << OValue << "]";
	return XValue-OValue;
}

void CheckLine(int X, int O, int &XValue, int & OValue){
	if (X>=0&&O==0){
		XValue++;
	}
	if (O>=0&&X==0){
		OValue++;
	}
}
