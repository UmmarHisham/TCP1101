// ********************************************************* 
// Course: TCP1101 PROGRAMMING FUNDAMENTALS 
// Year: Trimester 1, 2022/23 (T2215) 
// Lab: TT1L 
// Names:  AHMAD_LUQMAN_BIN_ZAKARANI     | MUHAMMAD_UMMAR_HISHAM_BIN_AHMAD_MADZLAN
// IDs:    1211101384                    | 1211100415
// Emails: 1211101384@student.mmu.edu.my | 1211100415@student.mmu.edu.my
// Phones: 019-4350096                   | 019-3329702
// ********************************************************* 

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <iomanip>
#include <set>
using namespace std;

void pause()
{
    system("pause");
}

void clear()
{
    system("cls");
}

class Board
{
private:
    vector<vector<char>> map_;
    vector<vector<int>> zombieList_;
    set<pair<int, int>> occupiedMap_;
    vector<int> zombieX_, zombieY_, zombieRange;
    int boardX_, boardY_;
    int alienX_, alienY_, alienHealth, alienAttack;
    int zombieCount_, zombieNo, zombieHealth, zombieAttack;
    

    char alienCh_ = 'A';

public:
    Board(int boardX = 15, int boardY = 5, int zombieCount = 3);
    void init(int boardX, int boardY, int zombieCount);
    void displayMap() const;
    void displayDefaultSetting(int boardX, int boardY, int zombieCount);
    void displayCurrentSetting(int boardX, int boardY, int zombieCount);
    void changeSetting(int boardX, int boardY, int zombCount);
    void commandMenu();
    void restoreMap();
    void helpMenu();

    int getDimX();
    int getDimY();
    void setObject(int x, int y, char ch);

    void alienLand();
    void alienMoveUp();
    void alienMoveDown();
    void alienMoveRight();
    void alienMoveLeft();

    int getZombieCount();
    void zombieSpawn(int zombieCount);
};

Board::Board(int boardX, int boardY, int zombieCount) 
{ 
    init(boardX, boardY, zombieCount);
}

void Board::init(int boardX, int boardY, int zombieCount)
{   
    srand(time(NULL));
    
    boardX_ = boardX;
    boardY_ = boardY;
    zombieCount_ = zombieCount;

    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int noOfObjects = 10; // number of objects in the objects array

    map_.resize(boardY_);
    for (int i = 0; i < boardY_; i++)
    {
        map_[i].resize(boardX_);
    }

    for (int i = 0; i < boardY_; i++)
    {
        for (int j = 0; j < boardX_; j++)
        {
            int objNo = rand() % noOfObjects;
            map_[i][j] = objects[objNo];
        }
    }

    alienLand();
    zombieSpawn(zombieCount_);

    for (int i = 0; i < zombieCount_; i++)
    {
        int zombCoorX = zombieList_[i][1]-1;
        int zombCoorY = zombieList_[i][2]-1;
        //char zombChar = zombieList_[i][0];
        char zombChar = to_string(i+1)[0];
        map_[zombCoorY][zombCoorX] = zombChar;

        occupiedMap_.insert({zombCoorX, zombCoorY}); // mark occupied zombie coordinate
        map_[zombCoorY][zombCoorX] = zombChar;
    }
}

int Board::getDimX()
{
    return boardX_;
}

int Board::getDimY()
{
    return boardY_;
}

void Board::setObject(int x, int y, char ch)
{
    map_[y-1][x-1] = ch;
}

// map display
void Board::displayMap() const
{
    clear();
    cout << " ";
    if (boardX_< 16)
    {
        cout << " =-=-= .: Alien Vs Zombie :. =-=-=" << endl;
    }
    else
    {
        for (int j = 0; j < (boardX_ - 17); j++)
        {
            cout << " ";
        }
        cout << " =-=-= .: Alien Vs Zombie :. =-=-=" << endl;
    }

    // for each row
    for (int i = 0; i < boardY_; i++)
    {
        // display upper border
        cout << "  ";
        for (int j = 0; j < boardX_; j++)
        {
            cout << "+-";
        }
        cout << "+" << endl;

        // display row number
        cout << setw(2) << (i + 1);

        // display cell content n border each column
        for (int j = 0; j < boardX_; j++)
        {
            cout << "|" << map_[i][j];
        }
        cout << "|" << endl;
    }

    // display lower border last row
    cout << "  ";
    for (int j = 0; j < boardX_; j++)
    {
        cout << "+-";
    }
    cout << "+" << endl;

    // display column number
    cout << "  ";
    for (int j = 0; j < boardX_; j++)
    {
        int digit = (j + 1) / 10;
        cout << " ";
        if (digit == 0)
            cout << " ";
        else
            cout << digit;
    }
    cout << endl;
    cout << "  ";
    for (int j = 0; j < boardX_; j++)
    {
        cout << " " << (j + 1) % 10;
    }
    cout << endl << endl;
}

// restore map after alienMove 
void Board::restoreMap()
{

    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int noOfObjects = 10;

    for (int i = 0; i < boardY_; i++)
    {
        for (int j = 0; j < boardX_; j++)
        {
            if (map_[i][j] == '.')
            {
                int objNo = rand() % noOfObjects;
                map_[i][j] = objects[objNo];
            }
        }
    }
}

// customize board size
void Board::displayDefaultSetting(int boardX, int boardY, int zombieCount)
{
    clear();
    cout << " =============================================" << endl;
    cout << "  D E F A U L T    G A M E    S E T T I N G S " << endl;
    cout << " =============================================" << endl;

    cout << " Number of Rows    : " << boardX << endl;
    cout << " Number of Columns : " << boardY << endl;
    cout << " Number of Zombies : " << zombieCount << endl << endl;

    string confirmSetting;

    cout << " Do you want to change the game settings (y/N) ? ";
    cin >> confirmSetting;

    if (confirmSetting == "n" || confirmSetting == "N")
    {
        init(boardX, boardY, zombieCount);
    }
    else if (confirmSetting == "y" || confirmSetting == "Y")
    {
        changeSetting(boardX, boardY, zombieCount);
    }
    else
    {
        cout << "\n You enter the wrong input \n Please try again...\n ";
        pause();
        displayDefaultSetting(boardX, boardY, zombieCount);
    }
}

void Board::displayCurrentSetting(int boardX, int boardY, int zombieCount)
{
    clear();
    cout << " =============================================" << endl;
    cout << "  C U R R E N T    G A M E    S E T T I N G S " << endl;
    cout << " =============================================" << endl;

    cout << " Number of Rows    : " << boardX << endl;
    cout << " Number of Columns : " << boardY << endl;
    cout << " Number of Zombies : " << zombieCount << endl << endl;

    string confirmSetting;

    cout << " Do you want to change the game settings (y/N) ? ";
    cin >> confirmSetting;

    if (confirmSetting == "n" || confirmSetting == "N")
    {
        init(boardX, boardY, zombieCount);
    }
    else if (confirmSetting == "y" || confirmSetting == "Y")
    {
        changeSetting(boardX, boardY, zombieCount);
    }
    else
    {
        cout << "\n You enter the wrong input \n Please try again...\n ";
        pause();
        displayCurrentSetting(boardX, boardY, zombieCount);
    }
}

void Board::changeSetting(int boardX, int boardY, int zombieCount)
{   
    clear();
    cout << " ==============================" << endl;
    cout << "  B O A R D    S E T T I N G S " << endl;
    cout << " ==============================" << endl << endl;

    do
    {
        cout << " Enter rows    => "; cin >> boardX; 

        if (boardX % 2 == 0)
        {
            cout << " Please enter an Odd number only\n";
        }
    } while (boardX % 2 == 0);

    cout << endl;
    do
    {
        cout << " Enter columns => "; cin >> boardY;; 

        if (boardY % 2 == 0)
        {
            cout << "\n Please enter an Odd number only\n";
        }
    } while (boardY % 2 == 0);

    cout << " ================================" << endl;
    cout << "  Z O M B I E    S E T T I N G S " << endl;
    cout << " ================================" << endl << endl;

    cout << " Total zombies => "; cin >> zombieCount;
    cout << endl;
    cout << " return to setting...\n ";
    pause();
    clear();
    displayCurrentSetting(boardX, boardY, zombieCount);
}

// alien
void Board::alienLand()
{
    alienX_ = getDimX() / 2 + 1;
    alienY_ = getDimY() / 2 + 1;
    
    setObject(alienX_, alienY_, alienCh_);
    occupiedMap_.insert({alienX_, alienY_});
}

void Board::alienMoveUp()
{   
    bool keepLooping = true;

    if (alienY_ > 1)
    {    do
        {
            pause();
            alienY_--;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienY_++;
                displayMap();
                cout << "The Alien has hit an obstacle\n" ;
                pause();

                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ ][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_, alienY_});
                occupiedMap_.erase({alienX_, alienY_ + 1});
            }

        // map_[alienY_][alienX_] = alienCh_;
        // map_[alienY_ + 1][alienX_] = '.';
        //setObject(alienX_, alienY_, alienCh_);
        //setObject(alienX_, alienY_+1, '.');
            displayMap();

        // if want to see inside the occupiedMap_
        // for (auto elem : occupiedMap_) 
        // {
        //     cout << "(" << elem.first << ", " << elem.second << ")\n";
        // }

        } while( alienY_ > 1 && keepLooping);

        if (keepLooping)
        {
            cout << " Alien has reach the border\n";
            pause();
        }
    }
    else
    {   
        displayMap();
        cout << " Alien has reach the border\n";
        pause();
    }
    restoreMap();
    commandMenu();
}

void Board::alienMoveDown()
{
    bool keepLooping = true;

    if (alienY_ < boardY_)
    {    
        do
        {
            pause();
            alienY_++;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienY_--;
                displayMap();
                cout << "The Alien has hit an obstacle\n" ;
                pause();

                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_, alienY_});
                occupiedMap_.erase({alienX_, alienY_ - 1});
            }

        // setObject(alienX_, alienY_, alienCh_);
        // setObject(alienX_, alienY_-1, '.');
            displayMap();

        } while( alienY_ < boardY_ && keepLooping);

        if (keepLooping)
        {
            cout << " Alien has reach the border\n";
            pause();
        }
    
    }
    else
    {   
        displayMap();
        cout << " Alien has reach the border\n";
        pause();
    }
    restoreMap();
    commandMenu();
}

void Board::alienMoveRight()
{
    bool keepLooping = true;

    if (alienX_ < boardX_)
    {    do
        {
            pause();
            alienX_++;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienX_--;
                displayMap();
                cout << "The Alien has hit an obstacle\n" ;
                pause();
            
                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_, alienY_});
                occupiedMap_.erase({alienX_ - 1, alienY_});
            }

            // setObject(alienX_, alienY_, alienCh_);
            // setObject(alienX_-1, alienY_, '.');
            displayMap();

        } while( alienX_ < boardX_ && keepLooping);

        if (keepLooping)
        {
            cout << " Alien has reach the border\n";
            pause();
        }
    }
    else
    {   
        displayMap();
        cout << " Alien has reach the border\n";
        pause();
    }
    restoreMap();
    commandMenu();
}

void Board::alienMoveLeft()
{
    bool keepLooping = true;

    if (alienX_ > 1)
    {    do
        {
            pause();
            alienX_--;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienX_++;
                displayMap();
                cout << "The Alien has hit an obstacle\n" ;
                pause();

                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_, alienY_});
                occupiedMap_.erase({alienX_ + 1, alienY_});
            }

            // setObject(alienX_, alienY_, alienCh_);
            // setObject(alienX_+1, alienY_, '.');
            displayMap();

        } while( alienX_ > 1 && keepLooping);
        
        if (keepLooping)
        {
            cout << " Alien has reach the border\n";
            pause();
        }
    }
    else
    {   
        displayMap();
        cout << " Alien has reach the border\n";
        pause();
    }
    restoreMap();
    commandMenu();
}

// zombie
int Board::getZombieCount()
{
    return zombieCount_;
}

void Board::zombieSpawn(int zombieCount)
{
    //zombieList[] = {zombieNo, zombieX_, zombieY_, zombieHealth, zombieAttack, zombieRange};
    int zombieListSize_ = 6; 
    zombieCount_ = zombieCount;

    zombieList_.resize(zombieCount_);
    for (int i = 0; i < zombieCount_; i++)
    {
        zombieList_[i].resize(zombieListSize_);
    }

    // list of coordinate X
    zombieX_.resize(boardX_);
    for (int j = 0; j < boardX_; j++) 
    {
        zombieX_[j] = j + 1;
    }
    int zombieXSize = zombieX_.size();

    // list of coordinate Y
    zombieY_.resize(boardY_);
    for (int i = 0; i < boardY_; i++) 
    {
        zombieY_[i] = i + 1;
    }
    int zombieYSize = zombieY_.size();

    // respective list
    int zombieHealth[] = {50, 75, 100, 125, 150, 175, 200, 225, 250};
    int zombieHealthSize = 9;

    int zombieAttack[] = {10, 15, 20, 25, 30};
    int zombieAttackSize = 5;
    
    // zombieRange based on the board lowest X Y value
    if (boardX_ > boardY_)
    {
        zombieRange.resize(boardY_);
        for (int i = 0; i < boardY_; i++) 
        {
            zombieRange[i] = i + 1;
        }
    } 
    else if (boardY_ > boardX_)
    {
        zombieRange.resize(boardX_);
        for (int j = 0; j < boardX_; j++) 
        {
            zombieRange[j] = j + 1;
        }
    }

    int zombieRangeSize = zombieRange.size();

    // column resize based on zombieCOunt_
    zombieList_.resize(zombieCount_);
    for (int i = 0; i < zombieCount_; i++)
    {   
        // rows resize based on ZombieListSize_
        zombieList_[i].resize(zombieListSize_);
    }

    for (int i = 0; i < zombieCount_; i++)
    {
        for (int j = 0; j < zombieListSize_; j++)
        {
            if (j == 0)
            {
                //zombNo
                zombieList_[i][j] = i + 1; 
            }
            else if (j == 3)
            {
                // random health
                int zomRanHealth = rand() % zombieHealthSize;
                zombieList_[i][j] = zombieHealth[zomRanHealth]; 
            }
            else if (j == 4)
            {
                // random attack
                int zomRanAtt = rand() % zombieAttackSize;
                zombieList_[i][j] = zombieAttack[zomRanAtt]; 
            }
            else if (j == 5)
            {
                // random range
                int zomRanRange = rand() % zombieRangeSize;
                zombieList_[i][j] = zombieRange[zomRanRange]; 
            }
        }
    }


    // empty zombieMap to check if occupied
    vector<vector<char>> zombieMap;
    set<pair<int, int>> occupiedCoor;
    char objects[] = {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'};
    int noOfObjects = 10; // number of objects in the objects array

    zombieMap.resize(boardY_);
    for (int i = 0; i < boardY_; i++)
    {
        zombieMap[i].resize(boardX_);
    }

    for (int i = 0; i < boardY_; i++)
    {
        for (int j = 0; j < boardX_; j++)
        {
            int objNo = rand() % noOfObjects;
            zombieMap[i][j] = objects[objNo];
        }
    }

    // check for column 1 and 2 for zombieX and zombieY
    for (int i = 0; i < zombieCount_; i++)
    {
        int x;
        int y;

        do
        {   
            // random zomX
            int zXNo = rand() % zombieXSize;

            // random zomY
            int zYNo = rand() % zombieYSize;

            x = zombieX_[zXNo];

            y = zombieY_[zYNo];

        } while (occupiedCoor.find({x, y}) != occupiedCoor.end()); //check if the zombie occupied already

        occupiedCoor.insert({x, y});

        zombieMap[y-1][x-1] = 'z';

        zombieList_[i][1] = x;
        zombieList_[i][2] = y;

    }
}

void Board::helpMenu()
{
    cout << "\n Commands: \n";
    cout << " 1. up    - Move Up\n";
    cout << " 2. down  - Move Down\n";
    cout << " 3. left  - Move Left\n";
    cout << " 4. right - Move RIght\n";
    cout << " 5. arrow - Change an Arrow Direction\n";
    cout << " 6. help  - Display This Help Menu\n";
    cout << " 7. save  - Save the current Game\n";
    cout << " 8. load  - Load a Game\n";
    cout << " 9. quit  - Quit the Game\n \n"; 
    cout << " Returning to the game. \n ";
    pause();
    commandMenu();
}

void Board::commandMenu()
{
    displayMap();
    string command;
    cout << " command => "; cin >> command;

    if (command == "up" )
    {
        alienMoveUp();
    }
    else if (command == "down")
    {
        alienMoveDown();
    }
    else if (command == "right")
    {
        alienMoveRight();
    }
    else if (command == "left")
    {
        alienMoveLeft();
    }
    // else if (command == "arrow")
    // {
    //     arrowChange();
    // }
    else if (command == "help")
    {
        helpMenu();
    }
    // else if (command == "save")
    // {
    //     gameSave();
    // }
    // else if (command == "load")
    // {
    //     gameLoad();
    // }
    else if (command == "quit")
    {
        string confirmQuit;
        cout << "\n Are you sure to quit the game? (y/N)\n confirming => ";
        cin >> confirmQuit;

        if (confirmQuit == "y" || confirmQuit == "Y")
        {
            clear();
            cout << " Have a nice day :)\n ";
            pause();
            exit;
        }
        else if (confirmQuit == "n" || confirmQuit == "N")
        {
            cout << " Returning to the main menu\n ";
            pause();
            commandMenu();
        }
        else
        {
            cout << " Invalid input, return to the main menu\n ";
            pause();
            commandMenu();
        }
    }
    else 
    {
        cout << " Invalid input, please try again\n";
        pause();
        commandMenu();
    }
}

void gameStart(Board &board)
{
    int boardX = board.getDimX(), boardY = board.getDimY();
    int zombCount = board.getZombieCount();

    board.displayDefaultSetting(boardX, boardY, zombCount);
    board.commandMenu();
}

// main
int main()
{
    Board board;
    gameStart(board);

    return 0;
}


