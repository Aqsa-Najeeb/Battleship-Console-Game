#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

int color;

// theme

int primColor = 13;
int secColor = 10;

void setColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

bool readInt(int &value)
{
    cout.flush();

    string line;
    if (!getline(cin, line)) return false;

    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    if (line.empty()) return false;

    stringstream ss(line);
    if (!(ss >> value)) return false;

    char extra;
    while (ss.get(extra))
        if (!isspace((unsigned char)extra)) return false;

    return true;
}

//  Function Prototypes

void menuText();
void showGameMenu();
void initializeBoard(char board[10][10]);
void clearScreen();
void updateScore(int &score);
void getship(char board[10][10]);
void printBoard(char board[10][10], bool hideShips);
void setRandomShips(char board[10][10]);
bool allShipSunk(char board[10][10]);
void vsPlayer(char board1[10][10], char board2[10][10]);
void vsComputer(char playerBoard[10][10], char computerBoard[10][10]);
void playerAttack(char opponentBoard[10][10], int &playerScore);
void computerAttack(char playerBoard[10][10], int &computerScore);
void displayResult(int player1Score, int player2Score, int winner);


void menuText()
{
    clearScreen();

    cout << "\n\n                          |     |     |\n";
    cout << "                         )_)   )_)   )_)\n";
    cout << "                        )___) )___) )___)\n";
    cout << "                       )____))____))_____)\\\n";
   
    cout << "              _________|_____|_____|_______\\\\______\n";
    cout << "  ~~~~~~~~~~~~\\                                    /~~~~\n";
    cout << "  ~~~~~~~~~~~~~\\__________________________________/~~~~~\n";
    cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";


    
    setColor(primColor, 0);
    cout << "  ____    _  _____ _____ _     _____ ____  _   _ ___ ____  \n";
    cout << " | __ )  / \\|_   _|_   _| |   | ____/ ___|| | | |_ _|  _ \\ \n";
    cout << " |  _ \\ / _ \\ | |   | | | |   |  _| \\___ \\| |_| || || |_) |\n";
    cout << " | |_) / ___ \\| |   | | | |___| |___ ___) |  _  || ||  __/ \n";
    cout << " |____/_/   \\_\\_|   |_| |_____|_____|____/|_| |_|___|_|    \n";
    setColor(7, 0);

    
    cout << "\n";
    setColor(8, 0);
    cout << "                    ~ Console Game\n";
    setColor(7, 0);
    cout << "\n\n";
}

void initializeBoard(char board[10][10])
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            board[i][j] = '~';
}

void updateScore(int &score)
{
    score += 10;
}

void printBoard(char board[10][10], bool hideShips)
{
    // Build an attack-only view (hides un-attacked water and ships)
    char display[10][10];
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            char cell = board[i][j];
            display[i][j] = (cell == 'X' || cell == 'O') ? cell : '.';
        }

    cout << "\n\n   Attack Map\t\t\t\t\t\t     Full Board\t\t\t\t\t\t\t  Attack Map\n\n";

    cout << " ";
    for (int i = 0; i < 10; i++) cout << "  ";
    cout << "\t\t\t";
    cout << "      0     1     2     3     4     5     6     7     8     9  ";
    cout << "\n ";

    for (int i = 0; i < 10; i++) cout << " " << i;
    cout << "\t\t\t";
    cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+";
    cout << "\t\t\t";
    for (int i = 0; i < 10; i++) cout << " " << i;
    cout << "\n";

    for (int i = 0; i < 10; i++)
    {
        cout << i;
        for (int j = 0; j < 10; j++)
            cout << " " << display[i][j];
        cout << "\t\t\t";

        cout << "  " << i << " |";
        for (int j = 0; j < 10; j++)
        {
            char cell = board[i][j];

            if (cell == 'S' && !hideShips)
            {
                setColor(7, color);
                cout << "  S  ";
            }
            else if (cell == 'X')
            {
                setColor(15, 12);
                cout << "  X  ";
            }
            else if (cell == 'O')
            {
                setColor(0, 8);
                cout << "  O  ";
            }
            else
            {
                setColor(7, 1);
                cout << "  ~  ";
            }

            setColor(7, 0);
            cout << "|";
        }
        cout << "\t\t\t";

        cout << i;
        for (int j = 0; j < 10; j++)
            cout << " " << display[i][j];
        cout << "\n ";

        for (int k = 0; k < 10; k++) cout << "  ";
        cout << "\t\t\t";
        cout << "    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+";
        cout << "\n";
    }
}

void getship(char board[10][10])
{
    const int ships[5]       = {5, 4, 3, 3, 2};
    const char *names[5]     = {
        "Aircraft Carrier (5)", "Battleship (4)",
        "Destroyer (3)",        "Submarine (3)",
        "Patrol Boat (2)"
    };

    cout << "Place your ships:\n\n";

    for (int i = 0; i < 5; i++)
    {
        cout << "\nPlacing: " << names[i] << "\n";

        bool placed = false;

        while (!placed)
        {
            int X_axis, Y_axis, position;
            bool positionCorrect = true;

            // Row input
            while (true)
            {
                cout << "\nEnter starting row (0-9): ";
                if (!readInt(X_axis))
                {
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }
                if (X_axis >= 0 && X_axis <= 9) break;
                cout << "Invalid input. Row must be between 0 and 9.\n";
            }

            // Column input
            while (true)
            {
                cout << "Enter starting column (0-9): ";
                if (!readInt(Y_axis))
                {
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }
                if (Y_axis >= 0 && Y_axis <= 9) break;
                cout << "Invalid input. Column must be between 0 and 9.\n";
            }

            // Orientation input
            while (true)
            {
                cout << "1. Vertical\n2. Horizontal\nEnter position: ";
                if (!readInt(position))
                {
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    continue;
                }
                position--;
                if (position == 0 || position == 1) break;
                cout << "Invalid input. Enter 1 or 2.\n";
            }

            // Validate placement
            int X = X_axis, Y = Y_axis;
            for (int j = 0; j < ships[i]; j++)
            {
                if (X >= 10 || Y >= 10)
                {
                    cout << "Ship exceeds board limits. Try again.\n";
                    positionCorrect = false;
                    break;
                }
                if (board[X][Y] != '~')
                {
                    cout << "Another ship is already there. Try again.\n";
                    positionCorrect = false;
                    break;
                }
                if (position == 0) X++;
                else               Y++;
            }

            if (!positionCorrect) continue;

            // Place ship
            X = X_axis;
            Y = Y_axis;
            for (int j = 0; j < ships[i]; j++)
            {
                board[X][Y] = 'S';
                if (position == 0) X++;
                else               Y++;
            }

            placed = true;
            printBoard(board, false);
        }
    }

    cout << "\nAll ships have been placed.\n\n";
}

void setRandomShips(char board[10][10])
{
    const int ships[5] = {5, 4, 3, 3, 2};

    for (int i = 0; i < 5; i++)
    {
        bool placed = false;

        while (!placed)
        {
            int X       = rand() % 10;
            int Y       = rand() % 10;
            int position = rand() % 2;

            int startX = X, startY = Y;
            bool positionCorrect = true;

            for (int j = 0; j < ships[i]; j++)
            {
                if (X >= 10 || Y >= 10)
                {
                    positionCorrect = false;
                    break;
                }
                if (board[X][Y] != '~')
                {
                    positionCorrect = false;
                    break;
                }
                if (position == 0) X++;
                else               Y++;
            }

            if (!positionCorrect) continue;

            X = startX;
            Y = startY;
            for (int j = 0; j < ships[i]; j++)
            {
                board[X][Y] = 'S';
                if (position == 0) X++;
                else               Y++;
            }

            placed = true;
        }
    }
}

bool allShipSunk(char board[10][10])
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (board[i][j] == 'S') return false;
    return true;
}

void playerAttack(char opponentBoard[10][10], int &playerScore)
{
    bool attack = false;

    while (!attack)
    {
        int X_axis, Y_axis;

        cout << "Attack opponent's ships:\n\n";

        cout << "Enter row (0-9): ";
        if (!readInt(X_axis) || X_axis < 0 || X_axis > 9)
        {
            cout << "Invalid input. Row must be between 0 and 9.\n";
            continue;
        }

        cout << "Enter column (0-9): ";
        if (!readInt(Y_axis) || Y_axis < 0 || Y_axis > 9)
        {
            cout << "Invalid input. Column must be between 0 and 9.\n";
            continue;
        }

        if (opponentBoard[X_axis][Y_axis] == 'X' || opponentBoard[X_axis][Y_axis] == 'O')
        {
            cout << "This coordinate has already been attacked. Try again.\n";
            continue;
        }

        if (opponentBoard[X_axis][Y_axis] == 'S')
        {
            opponentBoard[X_axis][Y_axis] = 'X';
            updateScore(playerScore);
            cout << "HIT!\n";
        }
        else
        {
            opponentBoard[X_axis][Y_axis] = 'O';
            playerScore -= 1;
            cout << "MISS!\n";
        }

        attack = true;
    }
}

void computerAttack(char playerBoard[10][10], int &computerScore)
{
    bool attack = false;

    while (!attack)
    {
        int X_axis = rand() % 10;
        int Y_axis = rand() % 10;

        if (playerBoard[X_axis][Y_axis] == 'O' || playerBoard[X_axis][Y_axis] == 'X')
            continue;

        if (playerBoard[X_axis][Y_axis] == 'S')
        {
            playerBoard[X_axis][Y_axis] = 'X';
            updateScore(computerScore);
        }
        else
        {
            playerBoard[X_axis][Y_axis] = 'O';
            computerScore -= 1;
        }

        attack = true;
    }
}

void displayResult(int player1Score, int player2Score, int winner)
{
    cout << "\n================================================\n";
    cout << "                   GAME OVER\n";
    cout << "================================================\n";
    cout << "  Player 1 Score: " << player1Score << "\n";
    cout << "  Player 2 Score: " << player2Score << "\n";
    cout << "------------------------------------------------\n";

    if(winner == 1) 
        cout << "  PLAYER 1 WINS!\n";
    else if (winner == 2) 
        cout << "  PLAYER 2 WINS!\n";
    else
        cout << "  IT'S A DRAW!\n";

    cout << "================================================\n\n";
}

void vsPlayer(char board1[10][10], char board2[10][10])
{
    int p1score = 0, p2score = 0;
    int winner = 0;

    cout << "PLAYER 1 Place your ships.\n";
    getship(board1);
    cout << "Press Enter when Player 2 is ready: ";
    cin.get();
    clearScreen();

    cout << "PLAYER 2 Place your ships.\n";
    getship(board2);
    cout << "Press Enter to start the game: ";
    cin.get();
    clearScreen();

    cout << "\nAll ships have been placed. Let the battle begin!\n\n";

    while (winner == 0)
    {
        clearScreen();

        cout << "PLAYER 1'S TURN\n";
        printBoard(board2, true);
        playerAttack(board2, p1score);
        printBoard(board2, true);

        if (allShipSunk(board2))
        {
            winner = 1;
            cout << "\nPlayer 1 sank all enemy ships!\n";
            break;
        }

        cout << "\nPlayer 1 has attacked. Press Enter to pass to Player 2\n";
        cin.get();

        clearScreen();
        cout << "PLAYER 2'S TURN\n";
        printBoard(board1, true);
        playerAttack(board1, p2score);
        printBoard(board1, true);

        if (allShipSunk(board1))
        {
            winner = 2;
            cout << "\nPlayer 2 sank all enemy ships!\n";
        }
        else
        {
            cout << "\nPlayer 2 has attacked. Press Enter to pass to Player 1\n";
            cin.get();
        }
    }

    displayResult(p1score, p2score, winner);
}

void vsComputer(char playerBoard[10][10], char computerBoard[10][10])
{
    int playerScore = 0, computerScore = 0;
    int winner = 0;

    getship(playerBoard);
    setRandomShips(computerBoard);

    cout << "\nAll ships have been placed. Let the battle begin!\n\n";

    while (winner == 0)
    {
        clearScreen();

        cout << "YOUR TURN\n";
        printBoard(computerBoard, true);
        playerAttack(computerBoard, playerScore);
        printBoard(computerBoard, true);

        if (allShipSunk(computerBoard))
        {
            winner = 1;
            cout << "\nYou sank all enemy ships!\n";
            break;
        }

        cout << "\nYou have attacked. Press Enter to see the computer's move\n";
        cin.get();

        clearScreen();
        cout << "COMPUTER'S TURN\n";
        computerAttack(playerBoard, computerScore);
        printBoard(playerBoard, false);

        if (allShipSunk(playerBoard))
        {
            winner = 2;
            cout << "\nThe computer sank all your ships!\n";
        }
        else
        {
            cout << "\nComputer has attacked. Press Enter for your next turn\n";
            cin.get();
        }
    }

    displayResult(playerScore, computerScore, winner);
}

void clearScreen()
{
    system("cls");
}

void showGameMenu()
{
    srand(static_cast<unsigned int>(time(0)));

    int choice;

    do
    {
        menuText();

        setColor(secColor, 0); //Light Green
        cout << "  +====================================+\n";
        cout << "  |                                    |\n";
        setColor(primColor, 0); // Light Purple
        cout << "  |         M A I N   M E N U         |\n";
        setColor(secColor, 0);
        cout << "  |                                    |\n";
        cout << "  +====================================+\n";
        cout << "  |                                    |\n";
        setColor(7, 0); //Light Gray
        cout << "  |  ";
        setColor(primColor, 0); cout << " [1] ";
        setColor(7, 0);  cout << " Start New Game              |\n";
        cout << "  |                                    |\n";
        cout << "  |  ";
        setColor(primColor, 0); cout << " [2] ";
        setColor(7, 0);  cout << " Instructions                |\n";
        cout << "  |                                    |\n";
        cout << "  |  ";
        setColor(primColor, 0); cout << " [3] ";
        setColor(7, 0);  cout << " Exit                        |\n";
        cout << "  |                                    |\n";
        setColor(secColor, 0);
        cout << "  +====================================+\n";
        setColor(7, 0);
        cout << "\n  Enter your choice: ";

        if (!readInt(choice))
        {
            cout << "Invalid input. Please enter 1, 2, or 3.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                string playername1, playername2;
                int mode;

                cout << endl;
                cout << "=========================\n";
                cout << "Enter Player 1 name: ";
                cin >> playername1;
                cin.ignore(10000, '\n');

                cout << endl;
                cout << "+===============================================+\n";
                cout << "|     G A M E   M O D E   S E L E C T I O N     | \n";
                cout << "+===============================================+\n";
                cout << "|                                               |\n";
                cout << "|  [1]       Player vs Player                   |\n";
                cout << "|  [2]       Player vs Computer                 |\n";
                cout << "|                                               |\n";
                cout << "+===============================================+\n\n";
                cout << "Your choice: ";

                while (!readInt(mode) || (mode != 1 && mode != 2))
                    cout << "Invalid input. Enter 1 or 2: ";

                do
                {
                    cout << endl;
                    cout << "+=============================+\n";
                    cout << "|      ";
                    setColor(12, 0); cout << "S H I P   C O L O R";
                    setColor(15, 0); cout << "    | \n";
                    cout << "+=============================+\n";
                    cout << "|                             |\n";
                    cout << "|  [1]       ";
                    cout << "Black";
                    cout << "            |\n";
                    cout << "|  [2]       ";
                    setColor(1, 0); cout << "Blue"; setColor(15, 0);
                    cout << "             |\n";
                    cout << "|  [3]       ";
                    setColor(2, 0); cout << "Green"; setColor(15, 0);
                    cout << "            |\n";
                    cout << "|  [4]       ";
                    setColor(3, 0); cout << "Cyan"; setColor(15, 0);
                    cout << "             |\n";
                    cout << "|  [5]       ";
                    setColor(4, 0); cout << "Red"; setColor(15, 0);
                    cout << "              |\n";
                    cout << "|  [6]       ";
                    setColor(5, 0); cout << "Magenta"; setColor(15, 0);
                    cout << "          |\n";
                    cout << "|  [7]       ";
                    setColor(6, 0); cout << "Yellow"; setColor(15, 0);
                    cout << "           |\n";
                    cout << "|                             |\n";
                    cout << "+=============================+\n\n";
                    cout << "Enter colour (1-7): ";

                    if (!readInt(color))
                    {
                        cout << "\nInvalid input. Try again.\n";
                        continue;
                    }

                    color--;

                    if (color < 0 || color > 6)
                        cout << "\nInvalid input. Try again.\n";
                    else
                        break;
                } while (true);

                char board1[10][10], board2[10][10];
                initializeBoard(board1);
                initializeBoard(board2);
                clearScreen();

                if (mode == 1)
                {
                    cout << endl;
                    cout << "=========================\n";
                    cout << "Enter Player 2 name: ";
                    cin >> playername2;
                    cin.ignore(10000, '\n');
                    vsPlayer(board1, board2);
                }
                else
                {
                    vsComputer(board1, board2);
                }

                break;
            }

            case 2:
            {
                clearScreen();
                cout << "\n";
                cout << "  +====================================+\n";
                cout << "  |                                    |\n";
                cout << "  |      I N S T R U C T I O N S       |\n";
                cout << "  |                                    |\n";
                cout << "  +====================================+\n";
                cout << "  |                                    |\n";
                cout << "  |  1. Each player places 5 ships.    |\n";
                cout << "  |     Ships cannot overlap.          |\n";
                cout << "  |                                    |\n";
                cout << "  |  2. Players take turns attacking   |\n";
                cout << "  |     the opponent's grid.           |\n";
                cout << "  |                                    |\n";
                cout << "  |  3. X = Hit    O = Miss            |\n";
                cout << "  |                                    |\n";
                cout << "  |  4. Sink ALL enemy ships to win!   |\n";
                cout << "  |                                    |\n";
                cout << "  |  5. Score: +10 hit  -1 miss        |\n";
                cout << "  |                                    |\n";
                cout << "  +====================================+\n";
                cout << "\n  Press Enter to return to the menu...";
                cin.get();
                clearScreen();
                break;
            }

            case 3:
            {
                clearScreen();
                setColor(13, 0);
                cout << "\n\n  Thanks for playing BATTLESHIP.\n\n";
                setColor(7, 0);
                break;
            }

            default:
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    } while (choice != 3);
}

int main()
{
    showGameMenu();
    return 0;
}
