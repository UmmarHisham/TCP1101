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
    int alienX_, alienY_, alienHealth, alienMaxHealth, alienAttack;
    int zombieCount_, zombieNo, zombieHealth, zombieAttack;

    char alienCh_ = 'A';

public:
    // board
    Board(int boardX = 15, int boardY = 5, int zombieCount = 3);
    void init(int boardX, int boardY, int zombieCount);
    void displayMap() const;
    void displayAttributeAlien();
    void displayDefaultSetting(int boardX, int boardY, int zombieCount);
    void displayCurrentSetting(int boardX, int boardY, int zombieCount);
    void changeSetting(int boardX, int boardY, int zombCount);

    // menu
    void commandMenu();
    void restoreMap();
    void helpMenu();
    void gameOver();

    // get variable
    int getDimX(), getDimY(), getZombieCount();
    void setObject(int x, int y, char ch);

    // item
    void healthItem(), pod(int podX, int podY), rock();
    void arrowUp(), arrowDown(), arrowLeft(), arrowRight();
    void arrowChange();

    // alien
    void alienLand();
    void alienMoveUp(), alienMoveDown(), alienMoveRight(), alienMoveLeft();

    // zombie
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

    char objects[] = {' ', ' ', 'h', 'r', '^', 'v', '<', '>', 'p', ' '};
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
        int zombCoorX = zombieList_[i][1] - 1;
        int zombCoorY = zombieList_[i][2] - 1;
        // char zombChar = zombieList_[i][0];
        char zombChar = to_string(i + 1)[0];

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
    map_[y - 1][x - 1] = ch;
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
    for (int j = 0; j < (((boardX_ * 4 - 34) / 2) / 2); j++)
    {
        cout << "=-";
    }

    cout << "        .: Alien Vs Zombie :.        ";

    for (int j = 0; j < (((boardX_ * 4 - 34) / 2) / 2); j++)
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
    cout << "=" << endl
         << endl;

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
            cout << map_[i][j] << " | ";
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
    cout << endl
         << endl;
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

    cout << " Number of Columns : " << boardX << endl;
    cout << " Number of Rows    : " << boardY << endl;
    cout << " Number of Zombies : " << zombieCount << endl
         << endl;

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

    cout << " Number of Columns : " << boardX << endl;
    cout << " Number of Rows    : " << boardY << endl;
    cout << " Number of Zombies : " << zombieCount << endl
         << endl;

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
    cout << " ==============================" << endl
         << endl;

    do
    {
        cout << " Enter Columns  => ";
        cin >> boardX;

        if (boardX % 2 == 0)
        {
            cout << " Please enter an Odd number only\n";
        }
    } while (boardX % 2 == 0);

    cout << endl;
    do
    {
        cout << " Enter Rows  => ";
        cin >> boardY;
        ;

        if (boardY % 2 == 0)
        {
            cout << "\n Please enter an Odd number only\n";
        }
    } while (boardY % 2 == 0);

    cout << " ================================" << endl;
    cout << "  Z O M B I E    S E T T I N G S " << endl;
    cout << " ================================" << endl
         << endl;

    cout << " Total zombies => ";
    cin >> zombieCount;
    cout << endl;
    cout << " return to setting...\n ";
    pause();
    clear();
    displayCurrentSetting(boardX, boardY, zombieCount);
}

// item; arrow, health, rock, pod
void Board::healthItem()
{
    displayMap();
    displayAttributeAlien();
    cout << "\n  The Alien found a health pack\n\n ";
    pause();

    int healthItem = 20;
    if (alienHealth + healthItem >= alienMaxHealth)
    {
        alienHealth = alienMaxHealth;
    }
    else
    {
        alienHealth = alienHealth + healthItem;
    }

    // displayMap();
    // displayAttributeAlien();
    // cout << "\n  Alien's life is increased by " << healthItem << "\n\n ";
    // pause();
}

void Board::arrowUp()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20;

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves up\n\n ";

    alienMoveUp();
}

void Board::arrowDown()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20;

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves down\n\n ";

    alienMoveDown();
}

void Board::arrowLeft()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20;

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves left\n\n ";

    alienMoveLeft();
}

void Board::arrowRight()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20;

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves right\n\n ";

    alienMoveRight();
}

void Board::arrowChange()
{
    int arrX, arrY;
    string direction;

    displayMap();
    displayAttributeAlien();

    cout << "\n  Enter the coordinate of the arrow that you wish to change (up, down, left, right)\n";
    cout << "  X, Y, and the direction => ";
    cin >> arrX;
    cin >> arrY;
    cin >> direction;

    if (map_[arrY - 1][arrX - 1] == '^' || '>' || 'v' || '<')
    {
        if (direction == "up")
        {
            map_[arrY - 1][arrX - 1] = '^';
        }
        else if (direction == "down")
        {
            map_[arrY - 1][arrX - 1] = 'v';
        }
        else if (direction == "left")
        {
            map_[arrY - 1][arrX - 1] = '<';
        }
        else if (direction == "right")
        {
            map_[arrY - 1][arrX - 1] = '>';
        }
        else
        {
            cout << "\n  Invalid direction, Please try again\n\n ";
            pause();
        }
    }
    else
    {
        cout << "\n  There is no arrow at the given coordinate, Please try again\n\n ";
        pause();
    }
    commandMenu();
}

void Board::pod(int podX, int podY) // x siap lagi
{
    int nearestZombie, nearX, nearY;

    int maxDistZom = abs(boardX_ - 1) + abs(boardY_ - 1);

    nearestZombie = maxDistZom;

    for (int i = 0; i < zombieCount_; i++)
    {
        int diffX = abs(zombieList_[i][1] - podX);
        int diffY = abs(zombieList_[i][2] - podY);

        int zombieDistance = diffX + diffY;

        if (zombieDistance < nearestZombie)
        {
            nearestZombie = zombieDistance;
            nearX = zombieList_[i][1];
            nearY = zombieList_[i][2];
        }
    }
}

void Board::rock()
{
    char objects[] = {' ', ' ', 'h', '^', 'v', '<', '>', 'p', ' '};
    int noOfObjects = 9;

    int objNo = rand() % noOfObjects;
    map_[alienY_ - 1][alienX_ - 1] = objects[objNo];

    displayMap();
    displayAttributeAlien();

    cout << "\n  The Alien hit a rock and found \"" << map_[alienY_ - 1][alienX_ - 1] << "\"\n ";
    pause();
}

// alien
void Board::alienLand()
{
    alienX_ = getDimX() / 2 + 1;
    alienY_ = getDimY() / 2 + 1;

    alienHealth = 150 + 50 * (rand() % 4);
    alienMaxHealth = alienHealth;
    alienAttack = 0;

    setObject(alienX_, alienY_, alienCh_);
    occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
}

void Board::alienMoveUp()
{
    bool keepLooping = true;

    if (alienY_ > 1)
    {
        do
        {
            cout << "\n\n ";
            pause();
            alienY_--;

            if (occupiedMap_.find({alienX_ - 1, alienY_ - 1}) != occupiedMap_.end())
            {
                for (int i = 0; i < zombieCount_; i++)
                {
                    // if (zombieList_[i][3] > 0) // if zombie is still alive
                    // {
                    if (alienX_ == zombieList_[i][1] && alienY_ == zombieList_[i][2])
                    {

                        zombieList_[i][3] = zombieList_[i][3] - alienAttack;
                        int zombX = zombieList_[i][1], zombY = zombieList_[i][2];

                        displayMap();
                        displayAttributeAlien();
                        cout << "\n  The Alien hit zombie " << zombieList_[i][0] << ", deal " << alienAttack << " damage\n\n ";

                        if (zombieList_[i][3] <= 0)
                        {
                            zombieList_[i][3] = 0;
                            map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                            map_[alienY_][alienX_ - 1] = '.';

                            occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                            occupiedMap_.erase({alienX_ - 1, alienY_});
                        }
                        else
                        {
                            alienY_++;
                            keepLooping = false;
                        }
                    }
                    //}
                }

                pause();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'h')
            {
                healthItem();

                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});

                displayMap();
                displayAttributeAlien();
                cout << "\n  Alien's life is increased by 20 \n\n ";
                pause();
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                // continue;
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
                arrowUp();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '>')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
                arrowRight();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
                arrowDown();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
                arrowLeft();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'r')
            {
                rock();

                alienY_++;
                keepLooping = false;
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});
            }

            // map_[alienY_][alienX_] = alienCh_;
            // map_[alienY_ + 1][alienX_] = '.';
            // setObject(alienX_, alienY_, alienCh_);
            // setObject(alienX_, alienY_+1, '.');
            displayMap();
            displayAttributeAlien();
            cout << "\n ";

            // if want to see inside the occupiedMap_
            // for (auto elem : occupiedMap_)
            // {
            //     cout << "(" << elem.first << ", " << elem.second << ")\n";
            // }

        } while (alienY_ > 1 && keepLooping);

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

            if (occupiedMap_.find({alienX_ - 1, alienY_ - 1}) != occupiedMap_.end())
            {
                for (int i = 0; i < zombieCount_; i++)
                {
                    if (alienX_ == zombieList_[i][1] && alienY_ == zombieList_[i][2])
                    {

                        zombieList_[i][3] = zombieList_[i][3] - alienAttack;
                        int zombX = zombieList_[i][1], zombY = zombieList_[i][2];

                        displayMap();
                        displayAttributeAlien();
                        cout << "\n  The Alien hit zombie " << zombieList_[i][0] << ", deal " << alienAttack << " damage\n\n ";

                        if (zombieList_[i][3] <= 0)
                        {
                            zombieList_[i][3] = 0;
                            map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                            map_[alienY_ - 2][alienX_ - 1] = '.';

                            occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                            occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
                        }
                        else
                        {
                            alienY_--;
                            keepLooping = false;
                        }
                    }
                }

                pause();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'h')
            {
                healthItem();

                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});

                displayMap();
                displayAttributeAlien();
                cout << "\n  Alien's life is increased by 20 \n\n ";
                pause();
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                // continue;
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
                arrowUp();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '>')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
                arrowRight();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
                arrowDown();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
                arrowLeft();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'r')
            {
                rock();

                alienY_--;
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

        } while (alienY_ < boardY_ && keepLooping);

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
    {
        do
        {
            cout << "\n\n ";
            pause();
            alienX_++;

            if (occupiedMap_.find({alienX_ - 1, alienY_ - 1}) != occupiedMap_.end())
            {
                for (int i = 0; i < zombieCount_; i++)
                {
                    if (alienX_ == zombieList_[i][1] && alienY_ == zombieList_[i][2])
                    {

                        zombieList_[i][3] = zombieList_[i][3] - alienAttack;
                        int zombX = zombieList_[i][1], zombY = zombieList_[i][2];

                        displayMap();
                        displayAttributeAlien();
                        cout << "\n  The Alien hit zombie " << zombieList_[i][0] << ", deal " << alienAttack << " damage\n\n ";

                        if (zombieList_[i][3] <= 0)
                        {
                            zombieList_[i][3] = 0;
                            map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                            map_[alienY_ - 1][alienX_ - 2] = '.';

                            occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                            occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
                        }
                        else
                        {
                            alienX_--;
                            keepLooping = false;
                        }
                    }
                }

                pause();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'h')
            {
                healthItem();

                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});

                displayMap();
                displayAttributeAlien();
                cout << "\n  Alien's life is increased by 20 \n\n ";
                pause();
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                // continue;
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
                arrowUp();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '>')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
                arrowRight();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
                arrowDown();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
                arrowLeft();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'r')
            {
                rock();

                alienX_--;
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

        } while (alienX_ < boardX_ && keepLooping);

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
    {
        do
        {
            cout << "\n\n ";
            pause();
            alienX_--;

            if (occupiedMap_.find({alienX_ - 1, alienY_ - 1}) != occupiedMap_.end())
            {
                for (int i = 0; i < zombieCount_; i++)
                {
                    if (alienX_ == zombieList_[i][1] && alienY_ == zombieList_[i][2])
                    {

                        zombieList_[i][3] = zombieList_[i][3] - alienAttack;
                        int zombX = zombieList_[i][1], zombY = zombieList_[i][2];

                        displayMap();
                        displayAttributeAlien();
                        cout << "\n  The Alien hit zombie " << zombieList_[i][0] << ", deal " << alienAttack << " damage\n\n ";

                        if (zombieList_[i][3] <= 0)
                        {
                            zombieList_[i][3] = 0;
                            map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                            map_[alienY_ - 1][alienX_] = '.';

                            occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                            occupiedMap_.erase({alienX_, alienY_ - 1});
                        }
                        else
                        {
                            alienX_++;
                            keepLooping = false;
                        }
                    }
                }

                pause();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'h')
            {
                healthItem();

                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});

                displayMap();
                displayAttributeAlien();
                cout << "\n  Alien's life is increased by 20 \n\n ";
                pause();
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                // continue;
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
                arrowUp();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '>')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
                arrowRight();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
                arrowDown();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
                arrowLeft();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'r')
            {
                rock();

                alienX_++;
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

        } while (alienX_ > 1 && keepLooping);

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
    // zombieList[] = {zombieNo, zombieX_, zombieY_, zombieHealth, zombieAttack, zombieRange, zombieMoveTurn};
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
                // zombNo
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

        } while (occupiedCoor.find({x, y}) != occupiedCoor.end()); // check if the zombie occupied already

        occupiedCoor.insert({x, y});

        zombieMap[y - 1][x - 1] = 'z';

        zombieList_[i][1] = x;
        zombieList_[i][2] = y;
    }
}

void Board::zombieMove()
{
    // int zombieMove[] = {1, 2, 3 ,4};
    //  1 - up
    //  2 - down
    //  3 - left
    //  4 - right
    alienAttack = 0;
    for (int i = 0; i < zombieCount_; i++) // loop each zombie
    {
        int x, y, zombieMoveRan;
        string move;
        int prevX = zombieList_[i][1];
        int prevY = zombieList_[i][2];
        int zombieTurn = 1;

        if (zombieList_[i][3] == 0)
        {
            continue;
        }

        bool isBlocked = false;
        bool upBlocked = false, downBlocked = false, leftBlocked = false, rightBlocked = false;

        if (occupiedMap_.find({prevX - 2, prevY - 1}) != occupiedMap_.end()) // Check cell to the left
        {
            leftBlocked = true;
        }
        if (occupiedMap_.find({prevX, prevY - 1}) != occupiedMap_.end()) // Check cell to the right
        {
            rightBlocked = true;
        }
        if (occupiedMap_.find({prevX - 1, prevY - 2}) != occupiedMap_.end()) // Check cell above
        {
            upBlocked = true;
        }
        if (occupiedMap_.find({prevX - 1, prevY}) != occupiedMap_.end()) // Check cell below
        {
            downBlocked = true;
        }

        // condition if zombie Blocked or not
        if (prevX == 1 && prevY == 1 && downBlocked && rightBlocked) // top-left corner
        {
            isBlocked = true;
        }
        else if (prevX == 1 && prevY == boardY_ && upBlocked && rightBlocked) // bottom-left corner
        {
            isBlocked = true;
        }
        else if (prevX == boardX_ && prevY == 1 && downBlocked && leftBlocked) // top-right corner
        {

            isBlocked = true;
        }
        else if (prevX == boardX_ && prevY == boardY_ && upBlocked && leftBlocked) // bottom-right corner
        {
            isBlocked = true;
        }
        else if (prevX == 1 && upBlocked && downBlocked && rightBlocked) // left edge
        {
            isBlocked = true;
        }
        else if (prevX == boardX_ && upBlocked && downBlocked && leftBlocked) // right edge
        {
            isBlocked = true;
        }
        else if (prevY == 1 && leftBlocked && downBlocked & rightBlocked) // top edge
        {
            isBlocked = true;
        }
        else if (prevY == boardY_ && upBlocked && rightBlocked && leftBlocked) // bottom edge
        {
            isBlocked = true;
        }
        else if (upBlocked && downBlocked && leftBlocked && rightBlocked) // surrounded
        {
            isBlocked = true;
        }

        do
        {

            zombieMoveRan = rand() % 5;

            if (zombieMoveRan == 1) // up
            {
                zombieList_[i][2]--;
            }
            else if (zombieMoveRan == 2) // down
            {
                zombieList_[i][2]++;
            }
            else if (zombieMoveRan == 3) // left
            {
                zombieList_[i][1]--;
            }
            else if (zombieMoveRan == 4) // right
            {
                zombieList_[i][1]++;
            }

            x = zombieList_[i][1];
            y = zombieList_[i][2];

            if (isBlocked || occupiedMap_.find({x - 1, y - 1}) != occupiedMap_.end() || x < 1 || x > boardX_ || y < 1 || y > boardY_)
            {
                if (isBlocked)
                {
                    zombieList_[i][1] = prevX;
                    zombieList_[i][2] = prevY;

                    zombieMoveRan = 5;
                    break;
                }
                zombieList_[i][1] = prevX;
                zombieList_[i][2] = prevY;
            }

            // cout << "current zombie " << zombieList_[i][0] << " new coordinate " << x << y << endl;

        } while (occupiedMap_.find({x - 1, y - 1}) != occupiedMap_.end() || x < 1 || x > boardX_ || y < 1 || y > boardY_);

        char zombChar = to_string(i + 1)[0];

        if (zombieMoveRan == 1) // up
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x - 1, y});

            map_[y - 1][x - 1] = zombChar;
            map_[y][x - 1] = ' ';

            move = "up";
        }
        else if (zombieMoveRan == 2) // down
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x - 1, y - 2});

            map_[y - 1][x - 1] = zombChar;
            map_[y - 2][x - 1] = ' ';

            move = "down";
        }
        else if (zombieMoveRan == 3) // left
        {
            occupiedMap_.insert({x - 1, y - 1});
            occupiedMap_.erase({x, y - 1});

            map_[y - 1][x - 1] = zombChar;
            map_[y - 1][x] = ' ';

            move = "left";
        }
        else if (zombieMoveRan == 4) // right
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

        if (zombieMoveRan == 5)
        {
            cout << "\n  zombie " << zombieList_[i][0] << " can't move\n\n";
        }
        else
        {
            cout << "\n  zombie " << zombieList_[i][0] << " moving " << move << "\n\n";
        }

        int diffX = abs(alienX_ - zombieList_[i][1]);
        int diffY = abs(alienY_ - zombieList_[i][2]);

        if (diffX + diffY <= zombieList_[i][5])
        {
            alienHealth = alienHealth - zombieList_[i][4];

            cout << "  Zombie " << zombieList_[i][0] << " deal " << zombieList_[i][4] << " damage to the Alien\n\n ";
            if (alienHealth <= 0)
            {
                alienHealth = 0;
                break;
            }
        }
        else
        {
            cout << "  The Alien is outside Zombie " << zombieList_[i][0] << "'s range\n\n ";
        }

        pause();
    }
    if (alienHealth <= 0)
    {
        cout << "The alien is  D E A D\n\n ";
        pause();
        gameOver();
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
    cout << "\n command => ";
    cin >> command;

    if (command == "up")
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
    else if (command == "arrow")
    {
        arrowChange();
    }
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

void Board::gameOver()
{

    clear();
    cout << " +-------------------------------------------+" << endl;
    cout << " |                                           |" << endl;
    cout << " |             G A M E   O V E R             |" << endl;
    cout << " |                                           |" << endl;
    cout << " +-------------------------------------------+\n\n";

    cout << " Do you want to Play Again? or \n return to the Main Menu? (y/N) => ";

    string confirm;
    cin >> confirm;

    if (confirm == "y" || confirm == "Y")
    {
        cout << "\n Returning to the Default Game Setting\n\n ";
        pause();

        int boardX = getDimX(), boardY = getDimY();
        int zombCount = getZombieCount();

        displayDefaultSetting(boardX, boardY, zombCount);
        commandMenu();
    }
    else if (confirm == "n" || confirm == "N")
    {
        cout << "\n Returning to the Main Menu\n\n ";
        pause();
        mainMenu();
    }
    else
    {
        cout << "\n Invalid input, please try again\n\n ";
        pause();
        gameOver();
    }
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
    cout << " +-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+" << endl
         << endl;

    cout << " Enter => ";
    cin >> input;

    if (input == "1")
    {
        gameStart(board);
    }
    else if (input == "2")
    {
        // loadGame();
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
            exit(0);
        }
        else if (confirmExit == "n" || confirmExit == "N")
        {
            cout << " Returning to the Main Menu\n\n ";
            pause();
            mainMenu();
        }
        else
        {
            cout << " Invalid input, return to the Main Menu\n ";
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
