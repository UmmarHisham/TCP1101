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
#include <fstream>
#include <limits>
#include <ios>
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
    int zombieDied;
    bool saveGame = false;

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
    void gameWin();
    void gameSave();
    void gameLoad();

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
    zombieDied = 0;

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
    if (boardX_ <= 7)
    {
        cout << "   =-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "      .: Alien Vs Zombie :.   " << endl;
        cout << "   =-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    }
    else
    {
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
    }

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

    char objects[] = {'<', '^', '>', 'v', 'h', 'p', 'r', ' ', ' '};
    int noOfObjects = 9;

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
        cout << " Enter columns    => ";
        cin >> boardX;

        if (boardX % 2 == 0)
        {
            cout << " Please enter an Odd number only\n";
        }
    } while (boardX % 2 == 0);

    cout << endl;
    do
    {
        cout << " Enter rows => ";
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
}

void Board::arrowUp()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20; // increasing alien attack damage

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves up\n\n ";

    alienMoveUp();
}

void Board::arrowDown()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20; // increasing alien attack damage

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves down\n\n ";

    alienMoveDown();
}

void Board::arrowLeft()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20; // increasing alien attack damage

    cout << "\n  Alien's attack is increased by 20 \n";
    cout << "\n  Alien moves left\n\n ";

    alienMoveLeft();
}

void Board::arrowRight()
{
    displayMap();
    displayAttributeAlien();
    alienAttack = alienAttack + 20; // increasing alien attack damage

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
    cout << "  Column, Row and the direction => ";
    cin >> arrX;
    cin >> arrY;
    cin >> direction;

    if (map_[arrY - 1][arrX - 1] == '^' || map_[arrY - 1][arrX - 1] == '>' || map_[arrY - 1][arrX - 1] == 'v' || map_[arrY - 1][arrX - 1] == '<') // check if the location of the given coordinate was an arrow or not
    {
        if (direction == "up") // change arrow according to the given direction
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
        else // if direction is invalid
        {
            cout << "\n  Invalid direction, Please try again\n\n ";
            pause();
        }
    }
    else // if there is no arrow
    {
        cout << "\n  There is no arrow at the given coordinate, Please try again\n\n ";
        pause();
    }
    commandMenu(); // after finish, back to commandMenu
}

void Board::pod(int podX, int podY)
{
    displayMap();
    displayAttributeAlien();
    cout << "\n  The Alien found an Attack Pod\n\n ";
    pause();

    int nearestZombie, nearX, nearY, podDamage = 10;
    vector<int> nearPod;
    int maxDistZom = abs(boardX_ - 1) + abs(boardY_ - 1);

    nearestZombie = maxDistZom;

    for (int i = 0; i < zombieCount_; i++)
    {
        if (zombieList_[i][3] > 0)
        {
            int diffX = abs((zombieList_[i][1] - 1) - podX);
            int diffY = abs((zombieList_[i][2] - 1) - podY);

            int zombieDistance = diffX + diffY;

            if (zombieDistance < nearestZombie)
            {
                nearestZombie = zombieDistance;
                nearPod.clear();
                nearPod.resize(1);
                nearPod[0] = zombieList_[i][0];
            }
            else if (zombieDistance == nearestZombie)
            {
                nearPod.resize(nearPod.size() + 1);
                nearPod[nearPod.size() - 1] = zombieList_[i][0];
            }
        }
    }

    int nearPodRan = rand() % nearPod.size();

    for (int i = 0; i < zombieCount_; i++)
    {
        if (nearPod[nearPodRan] == zombieList_[i][0])
        {
            zombieList_[i][3] = zombieList_[i][3] - podDamage;

            displayMap();
            displayAttributeAlien();
            cout << "\n  Zombie " << zombieList_[i][0] << " receive " << podDamage << " damage from the Pod\n";

            if (zombieList_[i][3] <= 0)
            {
                zombieList_[i][3] = 0;
                map_[zombieList_[i][2] - 1][zombieList_[i][1] - 1] = ' '; // emptying the space

                occupiedMap_.erase({zombieList_[i][1] - 1, zombieList_[i][2] - 1}); // remove from occupiedMap_

                cout << "  Zombie " << zombieList_[i][0] << " is dead\n ";
                zombieDied = zombieDied++;
                pause();
                break;
            }
            else
            {
                cout << "  Zombie " << zombieList_[i][0] << " is still alive\n ";
                pause();
                break;
            }
        }
    }
}

void Board::rock()
{
    char objects[] = {' ', ' ', 'h', '^', 'v', '<', '>', 'p', ' '}; // list of possible item the rock will change to
    int noOfObjects = 9;

    int objNo = rand() % noOfObjects;
    map_[alienY_ - 1][alienX_ - 1] = objects[objNo]; // change the item

    displayMap();
    displayAttributeAlien();

    cout << "\n  The Alien hit a rock and found \"" << map_[alienY_ - 1][alienX_ - 1] << "\"\n ";
    pause();
}

// alien
void Board::alienLand()
{
    alienX_ = getDimX() / 2 + 1;
    alienY_ = getDimY() / 2 + 1; // get the middle coordinate

    alienHealth = 150 + 50 * (rand() % 4); // random health from 150 - 300
    alienMaxHealth = alienHealth;          // max health for healing
    alienAttack = 0;                       // alien attack start with 0

    setObject(alienX_, alienY_, alienCh_);           // put alien in map_
    occupiedMap_.insert({alienX_ - 1, alienY_ - 1}); // put in occupiedMap_ with zombies
}

void Board::alienMoveUp() // the comment only in here, ither just copy paste with slight modifier
{
    bool keepLooping = true;

    if (alienY_ > 1)
    {
        do
        {
            cout << "\n\n ";
            pause();
            alienY_--; // for move up

            if (occupiedMap_.find({alienX_ - 1, alienY_ - 1}) != occupiedMap_.end())
            {
                for (int i = 0; i < zombieCount_; i++) // run through the zombieList_
                {
                    if (alienX_ == zombieList_[i][1] && alienY_ == zombieList_[i][2]) // find the same zombie that occupied the spot
                    {

                        zombieList_[i][3] = zombieList_[i][3] - alienAttack; // zombie health after alien attack
                        int zombX = zombieList_[i][1], zombY = zombieList_[i][2];

                        displayMap();
                        displayAttributeAlien();
                        cout << "\n  The Alien hit zombie " << zombieList_[i][0] << ", deal " << alienAttack << " damage\n\n ";

                        if (zombieList_[i][3] <= 0) // if alien atttack more than zombie health
                        {
                            zombieList_[i][3] = 0;
                            map_[alienY_ - 1][alienX_ - 1] = alienCh_; // change the loaction in map_ with Alien 'A'
                            map_[alienY_][alienX_ - 1] = '.';          // trail

                            occupiedMap_.insert({alienX_ - 1, alienY_ - 1}); // the alien and zombie location same place
                            occupiedMap_.erase({alienX_ - 1, alienY_});      // remove the before ALien location
                            zombieDied = zombieDied++;                       // zombie killed count
                            break;
                        }
                        else
                        {
                            alienY_++;           // if zombie not dead, ALien blocked
                            keepLooping = false; // will stop looping
                            cout << " The Alien has stop moving\n\n ";
                            break;
                        }
                    }
                }

                pause();
                break;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'h') // if the next spot is 'h'
            {
                healthItem(); // will get heal, because calling the function

                map_[alienY_ - 1][alienX_ - 1] = alienCh_; // also will move to the spot
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1}); // in occupied map, will insert the new move and
                occupiedMap_.erase({alienX_ - 1, alienY_});      // removed the old location

                displayMap();
                displayAttributeAlien();
                cout << "\n  Alien's life is increased by 20 \n\n ";
                continue;
            }

            else if (map_[alienY_ - 1][alienX_ - 1] == 'p') // if it is pod
            {
                pod(alienX_ - 1, alienY_ - 1); // will call pod function
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_][alienX_ - 1] = '.'; // same move, trail

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1}); // insert and remove
                occupiedMap_.erase({alienX_ - 1, alienY_});
                if (zombieDied == zombieCount_)
                {
                    displayMap();
                    displayAttributeAlien();
                    cout << "\n  The Alien has successfully killed all the zombies\n\n ";
                    pause();
                    gameWin();
                    break;
                }
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^' || map_[alienY_ - 1][alienX_ - 1] == '>' || map_[alienY_ - 1][alienX_ - 1] == 'v' || map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_});

                if (map_[alienY_ - 1][alienX_ - 1] == '^')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowUp();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '>')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowRight();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowDown();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '<')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowLeft();
                    break;
                }
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'r') // if it is rock
            {
                rock(); // call rock function

                alienY_++;           // will return to the original position
                keepLooping = false; // will stop looping
                break;
            }
            else
            {
                map_[alienY_ - 1][alienX_ - 1] = alienCh_; // if empty, just move to the spot
                map_[alienY_][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1}); // insert and remove
                occupiedMap_.erase({alienX_ - 1, alienY_});
            }

            displayMap();
            displayAttributeAlien();
            cout << "\n ";

        } while (alienY_ > 1 && keepLooping);

        if (keepLooping) // if break the loop because reaching the border
        {
            cout << " Alien has reach the border\n\n ";
            pause();
        }
    }
    else // if the Alien already at the border
    {
        displayMap();
        displayAttributeAlien();
        cout << "\n Alien has reach the border\n\n ";
        pause();
    }
    if (zombieDied == zombieCount_)
    {
        displayMap();
        displayAttributeAlien();
        cout << "\n  The Alien has successfully killed all the zombies\n\n ";
        pause();
        gameWin();
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
                            zombieDied = zombieDied++; // zombie killed count
                            break;
                        }
                        else
                        {
                            alienY_--;
                            keepLooping = false;
                            cout << " The Alien has stop moving\n\n ";
                            break;
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
                continue;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                pod(alienX_ - 1, alienY_ - 1);
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});
                if (zombieDied == zombieCount_)
                {
                    displayMap();
                    displayAttributeAlien();
                    cout << "\n  The Alien has successfully killed all the zombies\n\n ";
                    pause();
                    gameWin();
                    break;
                }
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^' || map_[alienY_ - 1][alienX_ - 1] == '>' || map_[alienY_ - 1][alienX_ - 1] == 'v' || map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 2][alienX_ - 1] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 1, alienY_ - 2});

                if (map_[alienY_ - 1][alienX_ - 1] == '^')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowUp();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '>')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowRight();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowDown();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '<')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowLeft();
                    break;
                }
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
            displayMap();
            displayAttributeAlien();
            cout << "\n Alien has reach the border\n\n ";
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
    if (zombieDied == zombieCount_)
    {
        displayMap();
        displayAttributeAlien();
        cout << "\n  The Alien has successfully killed all the zombies\n\n ";
        pause();
        gameWin();
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
                            zombieDied = zombieDied++; // plus zombie killed count
                            break;
                        }
                        else
                        {
                            alienX_--;
                            keepLooping = false;
                            cout << " The Alien has stop moving\n\n ";
                            break;
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
                continue;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                pod(alienX_ - 1, alienY_ - 1);
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});
                if (zombieDied == zombieCount_)
                {
                    displayMap();
                    displayAttributeAlien();
                    cout << "\n  The Alien has successfully killed all the zombies\n\n ";
                    pause();
                    gameWin();
                    break;
                }
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^' || map_[alienY_ - 1][alienX_ - 1] == '>' || map_[alienY_ - 1][alienX_ - 1] == 'v' || map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 1][alienX_ - 2] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_ - 2, alienY_ - 1});

                if (map_[alienY_ - 1][alienX_ - 1] == '^')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowUp();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '>')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowRight();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowDown();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '<')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowLeft();
                    break;
                }
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
            displayMap();
            displayAttributeAlien();
            cout << "\n Alien has reach the border\n\n ";
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
    if (zombieDied == zombieCount_)
    {
        displayMap();
        displayAttributeAlien();
        cout << "\n  The Alien has successfully killed all the zombies\n\n ";
        pause();
        gameWin();
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
                            zombieDied = zombieDied++; // zombie killed count
                            break;
                        }
                        else
                        {
                            alienX_++;
                            keepLooping = false;
                            cout << " The Alien has stop moving\n\n ";
                            break;
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
                continue;
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == 'p')
            {
                pod(alienX_ - 1, alienY_ - 1);
                map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});
                if (zombieDied == zombieCount_)
                {
                    displayMap();
                    displayAttributeAlien();
                    cout << "\n  The Alien has successfully killed all the zombies\n\n ";
                    pause();
                    gameWin();
                    break;
                }
            }
            else if (map_[alienY_ - 1][alienX_ - 1] == '^' || map_[alienY_ - 1][alienX_ - 1] == '>' || map_[alienY_ - 1][alienX_ - 1] == 'v' || map_[alienY_ - 1][alienX_ - 1] == '<')
            {
                map_[alienY_ - 1][alienX_] = '.';

                occupiedMap_.insert({alienX_ - 1, alienY_ - 1});
                occupiedMap_.erase({alienX_, alienY_ - 1});

                if (map_[alienY_ - 1][alienX_ - 1] == '^')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowUp();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '>')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowRight();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == 'v')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowDown();
                    break;
                }
                else if (map_[alienY_ - 1][alienX_ - 1] == '<')
                {
                    map_[alienY_ - 1][alienX_ - 1] = alienCh_;
                    arrowLeft();
                    break;
                }
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
            displayMap();
            displayAttributeAlien();
            cout << "\n Alien has reach the border\n\n ";
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
    if (zombieDied == zombieCount_)
    {
        displayMap();
        displayAttributeAlien();
        cout << "\n  The Alien has successfully killed all the zombies\n\n ";
        pause();
        gameWin();
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

        // zombie move
        displayMap();
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

        if (zombieMoveRan == 5)
        {
            cout << "\n  zombie " << zombieList_[i][0] << " can't move\n\n ";
        }
        else
        {
            cout << "\n  zombie " << zombieList_[i][0] << " moves " << move << "\n\n ";
        }
        pause();

        // zombie attack
        displayMap();
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

        int diffX = abs(alienX_ - zombieList_[i][1]);
        int diffY = abs(alienY_ - zombieList_[i][2]);

        if (diffX + diffY <= zombieList_[i][5])
        {
            alienHealth = alienHealth - zombieList_[i][4];

            cout << "\n  Zombie " << zombieList_[i][0] << " deal " << zombieList_[i][4] << " damage to the Alien\n\n";
            if (alienHealth <= 0)
            {
                alienHealth = 0;
                cout << "  The alien is  D E A D\n\n ";
                break;
            }
            else if (alienHealth > 0)
            {
                cout << "  The Alien is still alive\n\n ";
                pause();
            }
        }
        else
        {
            cout << "\n  The Alien is outside Zombie " << zombieList_[i][0] << "'s range\n\n ";
            pause();
        }
    }
    if (alienHealth <= 0)
    {
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
    else if (command == "save")
    {
        gameSave();
        commandMenu();
    }
    else if (command == "load")
    {
        displayMap();
        displayAttributeAlien();
        gameLoad();
    }
    else if (command == "quit")
    {
        if (!saveGame)
        {
            string confirmSave;
            cout << "\n Are you sure you want to quit without saving the game? (y/N)\n confirming =>";
            cin >> confirmSave;

            if (confirmSave == "y" || confirmSave == "Y")
            {
                cout << " Noted :)\n";
                cout << " Returning to the Main Menu,\n\n ";
                pause();
                mainMenu();
            }
            else if (confirmSave == "n" || confirmSave == "N")
            {
                pause();
                gameSave();
                mainMenu();
            }
            else
            {
                cout << " Invalid input, return to the Game\n\n ";
                pause();
                commandMenu();
            }
        }
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

void Board::gameSave()
{
    displayMap();
    displayAttributeAlien();
    string filename;
    cout << "\n  Enter the name of the file that you wish to save (without the extension)=> ";
    cin >> filename;
    filename = filename + ".txt";

    ofstream saveFile(filename);

    // uniques element
    saveFile << boardX_ << " " << boardY_ << " " << zombieCount_ << " " << alienX_ << " " << alienY_ << " " << alienHealth << " " << alienMaxHealth << " " << zombieDied << endl;

    // map_
    for (int i = 0; i < boardY_; i++)
    {
        for (int j = 0; j < boardX_; j++)
        {
            saveFile << map_[i][j];
        }
        saveFile << endl;
    }

    // zombie attributes
    for (int i = 0; i < zombieCount_; i++)
    {
        saveFile << zombieList_[i][0] << " " << zombieList_[i][1] << " " << zombieList_[i][2] << " " << zombieList_[i][3] << " " << zombieList_[i][4] << " " << zombieList_[i][5] << " " << zombieList_[i][6] << endl;
    }

    saveFile.close();

    cout << "  The game has been saved in file " << filename << endl;

    saveGame = true;
}

void Board::gameLoad()
{
    string filename;
    cout << "\n Enter the name of the file that you wish to load (without the extension)=> ";
    cin >> filename;
    filename = filename + ".txt";

    ifstream loadFile(filename);

    if (!loadFile.is_open())
    {
        cout << "\n The name of the file doesn't exist, Please try again\n\n ";
        pause();
        mainMenu();
    }

    // load unique element
    loadFile >> boardX_ >> boardY_ >> zombieCount_ >> alienX_ >> alienY_ >> alienHealth >> alienMaxHealth >> zombieDied;

    loadFile.ignore(numeric_limits<streamsize>::max(), '\n');

    // resize map_
    map_.resize(boardY_);
    for (int i = 0; i < boardY_; i++)
    {
        map_[i].resize(boardX_);
    }

    // load map_
    for (int i = 0; i < boardY_; i++)
    {
        string row;
        getline(loadFile, row);
        for (int j = 0; j < boardX_; j++)
        {
            map_[i][j] = row[j];
        }
    }

    // resize zombieList_
    zombieList_.resize(zombieCount_);
    for (int i = 0; i < zombieCount_; i++)
    {
        zombieList_[i].resize(7);
    }
    // load zombie attribute
    for (int i = 0; i < zombieCount_; i++)
    {
        loadFile >> zombieList_[i][0] >> zombieList_[i][1] >> zombieList_[i][2] >> zombieList_[i][3] >> zombieList_[i][4] >> zombieList_[i][5] >> zombieList_[i][6];
    }

    occupiedMap_.clear();
    occupiedMap_.insert({alienX_ - 1, alienY_ -1});
    for (int i = 0; i < zombieCount_; i++)
    {
        if (zombieList_[i][3] > 0)
        {
            occupiedMap_.insert({zombieList_[i][1] - 1, zombieList_[1][2] - 1});
        }
    }

    loadFile.close();

    clear();
    cout << " +-------------------------------------------+" << endl;
    cout << " |                                           |" << endl;
    cout << " |             L O A D   G A M E             |" << endl;
    cout << " |                                           |" << endl;
    cout << " +-------------------------------------------+\n\n";

    cout << "               THE GAME SETTING              " << endl;
    cout << " --------------------------------------------" << endl;
    cout << " Row: " << boardX_ << endl;
    cout << " Column: " << boardY_ << endl;
    cout << " Zombie Count: " << zombieCount_ << endl;

    cout << " Do you wish to load the game? (y/N) => ";

    string confirm;
    cin >> confirm;

    if (confirm == "y" || confirm == "Y")
    {
        cout << "\n Resuming the game\n\n ";
        pause();
        commandMenu();
        saveGame = false;
    }
    else if (confirm == "n" || confirm == "N")
    {
        cout << "\n Returning to the Main Menu\n\n ";
        pause();
        mainMenu();
    }
    else
    {
        cout << "\n Invalid input, return to the Main Menu\n\n ";
        pause();
        mainMenu();
    }
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

void Board::gameWin()
{
    clear();
    cout << " +-------------------------------------------+" << endl;
    cout << " |                                           |" << endl;
    cout << " |        C O N G R A T U L A T I O N        |" << endl;
    cout << " |            Y O U   W I N  !!!             |" << endl;
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
        board.gameLoad();
        pause();
        mainMenu();
    }
    else if (input == "3")
    {
        cout << " Credits to:\n ";
        cout << " 1. Ahmad Luqman Bin Zakarani - 1211101384\n ";
        cout << " 2. Muhammad Ummar Hisham Bin Ahmad Madzlan - 1211100415\n ";
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
    mainMenu();

    return 0;
}

