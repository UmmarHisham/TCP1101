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

void mainMenu();

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
    void displayAttributeAlien();
    void displayDefaultSetting(int boardX, int boardY, int zombieCount);
    void displayCurrentSetting(int boardX, int boardY, int zombieCount);
    void changeSetting(int boardX, int boardY, int zombCount);

    // void mainMenu();
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
    void zombieMove();
};

Board::Board(int boardX, int boardY, int zombieCount) 
{ 
    init(boardX, boardY, zombieCount);
}

// initiating board
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
    
    occupiedMap_.clear();
    alienLand();
    zombieSpawn(zombieCount_);

    for (int i = 0; i < zombieCount_; i++)
    {
        int zombCoorX = zombieList_[i][1]-1;
        int zombCoorY = zombieList_[i][2]-1;
        //char zombChar = zombieList_[i][0];
        char zombChar = to_string(i+1)[0];

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
    // upper line title
    cout << "   ";
    for (int j = 0; j < boardX_; j++)
    {
        cout << "=-=-";
    }
    cout << "=" << endl;

    // middle title
    cout << "   ";
    for (int j = 0; j < (((boardX_*4 -34)/2)/2 ); j++)
    {
        cout << "=-";
    }

    cout << "        .: Alien Vs Zombie :.        ";

    for (int j = 0; j < (((boardX_*4 -34)/2)/2); j++)
    {
        cout << "-=";
    }
    cout << endl;

    // bottom line title
    cout << "   ";
    for (int j = 0; j < boardX_; j++)
    {
        cout << "=-=-";
    }
    cout << "=" << endl << endl;

    // for each row
    for (int i = 0; i < boardY_; i++)
    {
        // display upper border
        cout << "   ";
        for (int j = 0; j < boardX_; j++)
        {
            cout << "+---";
        }
        cout << "+" << endl;

        // display row number
        cout << setw(2) << (i + 1) << " | ";

        // display cell content n border each column
        for (int j = 0; j < boardX_; j++)
        {
            cout << map_[i][j] << " | " ;
        }
        cout << endl;
    }

    // display lower border last row
    cout << "   ";
    for (int j = 0; j < boardX_; j++)
    {
        cout << "+---";
    }
    cout << "+" << endl;

    // display column number
    cout << "   ";
    for (int j = 0; j < boardX_; j++)
    {
        int digit = (j + 1) / 10;
        cout << " ";
        if (digit == 0)
            cout << "   ";
        else
            cout << " " << digit << " ";
    }
    cout << endl;
    cout << "    ";
    for (int j = 0; j < boardX_; j++)
    {
        cout << " " << (j + 1) % 10 << "  ";
    }
    cout << endl << endl;
}

void Board::displayAttributeAlien()
{
    cout << "-> Alien    : Life " << setw(3) << alienHealth << ", Attack " << setw(3) << alienAttack << endl;
    for (int i = 0; i < zombieCount_; i++)
    {
        cout << "   Zombie " << zombieList_[i][0] << " : Life " << setw(3) << zombieList_[i][3] << ", Attack " << setw(3) << zombieList_[i][4] << ", Range " << setw(2) << zombieList_[i][5] << endl; 
    }
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

    alienHealth = 100 + 50 * (rand() % 4);
    alienAttack = 0;
    
    setObject(alienX_, alienY_, alienCh_);
    occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
}

void Board::alienMoveUp()
{   
    bool keepLooping = true;

    if (alienY_ > 1)
    {    do
        {
            cout << "\n\n ";
            pause();
            alienY_--;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienY_++;
                displayMap();
                displayAttributeAlien();
                cout << "\n  The Alien has hit an obstacle\n\n " ;
                pause();

                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ ][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
            }

        // map_[alienY_][alienX_] = alienCh_;
        // map_[alienY_ + 1][alienX_] = '.';
        //setObject(alienX_, alienY_, alienCh_);
        //setObject(alienX_, alienY_+1, '.');
            displayMap();
            displayAttributeAlien();
            cout << "\n ";

        // if want to see inside the occupiedMap_
        // for (auto elem : occupiedMap_) 
        // {
        //     cout << "(" << elem.first << ", " << elem.second << ")\n";
        // }

        } while( alienY_ > 1 && keepLooping);

        if (keepLooping)
        {
            cout << " Alien has reach the border\n\n ";
            pause();
        }
    }
    else
    {   
        displayMap();
        displayAttributeAlien();
        cout << "\n Alien has reach the border\n\n ";
        pause();
    }
    restoreMap();
    zombieMove();
    commandMenu();
}

void Board::alienMoveDown()
{
    bool keepLooping = true;

    if (alienY_ < boardY_)
    {    
        do
        {
            cout << "\n\n ";
            pause();
            alienY_++;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienY_--;
                displayMap();
                displayAttributeAlien();
                cout << "\n  The Alien has hit an obstacle\n\n " ;
                pause();

                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
            }

        // setObject(alienX_, alienY_, alienCh_);
        // setObject(alienX_, alienY_-1, '.');
            displayMap();
            displayAttributeAlien();
            cout << "\n ";

        } while( alienY_ < boardY_ && keepLooping);

        if (keepLooping)
        {
            cout << " Alien has reach the border\n\n ";
            pause();
        }
    
    }
    else
    {   
        displayMap();
        displayAttributeAlien();
        cout << "\n Alien has reach the border\n\n ";
        pause();
    }
    restoreMap();
    zombieMove();
    commandMenu();
}

void Board::alienMoveRight()
{
    bool keepLooping = true;

    if (alienX_ < boardX_)
    {    do
        {
            cout << "\n\n ";
            pause();
            alienX_++;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienX_--;
                displayMap();
                displayAttributeAlien();
                cout << "\n  The Alien has hit an obstacle\n\n " ;
                pause();
            
                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
            }

            // setObject(alienX_, alienY_, alienCh_);
            // setObject(alienX_-1, alienY_, '.');
            displayMap();
            displayAttributeAlien();
            cout << "\n ";

        } while( alienX_ < boardX_ && keepLooping);

        if (keepLooping)
        {
            cout << " Alien has reach the border\n\n ";
            pause();
        }
    }
    else
    {   
        displayMap();
        displayAttributeAlien();
        cout << "\n Alien has reach the border\n\n ";
        pause();
    }
    restoreMap();
    zombieMove();
    commandMenu();
}

void Board::alienMoveLeft()
{
    bool keepLooping = true;

    if (alienX_ > 1)
    {    do
        {
            cout << "\n\n ";
            pause();
            alienX_--;

            if (occupiedMap_.find({alienX_-1, alienY_-1}) != occupiedMap_.end())
            {   
            
                alienX_++;
                displayMap();
                displayAttributeAlien();
                cout << "\n  The Alien has hit an obstacle\n\n " ;
                pause();

                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
            }

            // setObject(alienX_, alienY_, alienCh_);
            // setObject(alienX_+1, alienY_, '.');
            displayMap();
            displayAttributeAlien();
            cout << "\n ";

        } while( alienX_ > 1 && keepLooping);
        
        if (keepLooping)
        {
            cout << " Alien has reach the border\n\n ";
            pause();
        }
    }
    else
    {   
        displayMap();
        displayAttributeAlien();
        cout << "\n Alien has reach the border\n\n ";
        pause();
    }
    restoreMap();
    zombieMove();
    commandMenu();
}

// zombie
int Board::getZombieCount()
{
    return zombieCount_;
}

void Board::zombieSpawn(int zombieCount)
{
    //zombieList[] = {zombieNo, zombieX_, zombieY_, zombieHealth, zombieAttack, zombieRange, zombieMoveTurn};
    int zombieListSize_ = 7; 
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

    occupiedCoor.insert({alienX_, alienY_});
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

void Board::zombieMove()
{
    //int zombieMove[] = {1, 2, 3 ,4};
    // 1 - up
    // 2 - down
    // 3 - left
    // 4 - right
    for (int i = 0; i < zombieCount_; i++) // loop each zombie
    {
        int x, y, zombieMoveRan;
        string move;
        int prevX = zombieList_[i][1];
        int prevY = zombieList_[i][2];
        int zombieTurn = 1;
        do
        {

            zombieMoveRan = rand() % 5;

            if (zombieMoveRan == 1) //up
            {
                zombieList_[i][2]--;
            }
            else if (zombieMoveRan == 2) //down
            {
                zombieList_[i][2]++;
            }
            else if (zombieMoveRan == 3) //left
            {
                zombieList_[i][1]--;
            }
            else if (zombieMoveRan == 4) //right
            {
                zombieList_[i][1]++;
            }

            x = zombieList_[i][1];
            y = zombieList_[i][2];

            if (occupiedMap_.find({x - 1, y - 1}) != occupiedMap_.end() || x < 1 || x > boardX_ || y < 1 || y > boardY_)
            {
                zombieList_[i][1] = prevX;
                zombieList_[i][2] = prevY;
            }

            // cout << "current zombie " << zombieList_[i][0] << " new coordinate " << x << y << endl;

        } while (occupiedMap_.find({x - 1, y - 1}) != occupiedMap_.end() || x < 1 || x > boardX_ || y < 1 || y > boardY_);

        char zombChar = to_string(i+1)[0];

        if (zombieMoveRan == 1) //up
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x - 1, y});

            map_[y- 1][x - 1] = zombChar;
            map_[y ][x - 1] = ' ';

            move = "up";
        }
        else if (zombieMoveRan == 2) //down
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x - 1, y - 2});

            map_[y - 1][x - 1] = zombChar;
            map_[y - 2][x - 1] = ' ';

            move = "down";
        }
        else if (zombieMoveRan == 3) //left
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x, y - 1});

            map_[y - 1][x - 1] = zombChar;
            map_[y - 1][x] = ' ';

            move = "left";
        }
        else if (zombieMoveRan == 4) //right
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x - 2, y - 1});

            map_[y - 1][x - 1] = zombChar;
            map_[y - 1][x - 2] = ' ';

            move = "right";
        }
        zombieList_[i][6] = zombieTurn;
        displayMap();

        // for (auto elem : occupiedMap_) 
        // {
        //     cout << "(" << elem.first + 1 << ", " << elem.second + 1 << ")\n";
        // }

        cout << "   Alien    : Life " << setw(3) << alienHealth << ", Attack " << setw(3) << alienAttack << endl;
        for (int j = 0; j < zombieCount_; j++)
        {
            if (zombieList_[j][6] == 1)
            {
                cout << "-> Zombie " << zombieList_[j][0] << " : Life " << setw(3) << zombieList_[j][3] << ", Attack " << setw(3) << zombieList_[j][4] << ", Range " << setw(2) << zombieList_[j][5] << endl; 
            }
            else
            {
                cout << "   Zombie " << zombieList_[j][0] << " : Life " << setw(3) << zombieList_[j][3] << ", Attack " << setw(3) << zombieList_[j][4] << ", Range " << setw(2) << zombieList_[j][5] << endl;
            }
        }
        zombieList_[i][6] = 0;

        cout << "\n  zombie " << zombieList_[i][0] << " move to the " << move << "\n\n ";

        pause();
    }
}

// menu
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
    displayAttributeAlien();
    string command;
    cout << "\n command => "; cin >> command;

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
            cout << " Returning to the Main Menu,\n\n ";
            pause();
            mainMenu();
        }
        else if (confirmQuit == "n" || confirmQuit == "N")
        {
            cout << " Returning to the Game\n\n ";
            pause();
            commandMenu();
        }
        else
        {
            cout << " Invalid input, return to the Game\n\n ";
            pause();
            commandMenu();
        }
    }
    else 
    {
        cout << " Invalid input, please try again\n\n ";
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

void mainMenu()
{
    clear();
    Board board;
    string input;
    cout << " +-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+" << endl;
    cout << " |              W E L C O M E                |" << endl;
    cout << " |                   T O                     |" << endl;
    cout << " |       A L I E N   V S   Z O M B I E S     |" << endl;
    cout << " +-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+" << endl;
    cout << " | Select:                                   |" << endl;
    cout << " | 1 - New Game                              |" << endl;
    cout << " | 2 - Load Game                             |" << endl;
    cout << " | 3 - Credits                               |" << endl;
    cout << " | 4 - Exit                                  |" << endl;
    cout << " +-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+" << endl << endl;

    cout << " Enter => "; cin >> input;

    if (input == "1")
    {
        gameStart(board);
    }
    else if (input == "2")
    {
        //loadGame();
        cout << " The option has yet to be implemented, please try again later ;)\n\n ";
        pause();
        mainMenu();
    }
    else if (input == "3")
    {
        cout << " Credits to \n\n ";
        pause();
        mainMenu();
    }
    else if (input == "4")
    {
        string confirmExit;
        cout << "\n Are you sure to exit the game? (y/N)\n confirming => ";
        cin >> confirmExit;

        if (confirmExit == "y" || confirmExit == "Y")
        {
            clear();
            cout << " Have a nice day :)\n ";
            pause();
            exit;
        }
        else if (confirmExit == "n" || confirmExit == "N")
        {
            cout << " Returning to the Main Menu\n\n ";
            pause();
            mainMenu();
        }
        else
        {
            cout << " Invalid input, return to the Main Menu\n\n ";
            pause();
            mainMenu();
        }
    }
    else
    {
        cout << " Invalid input, please try again\n\n ";
        pause();
        mainMenu();
    }
}

// main
int main()
{
    // Board board;
    // gameStart(board);
    mainMenu();

    return 0;
}

