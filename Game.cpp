#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);

private:
    int m_nRows;
    int m_nCols; 
    int m_nShips;
    vector<int> shipLengths;
    vector<char> shipSymbols;
    vector<string> shipNames;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : m_nRows(nRows), m_nCols(nCols), m_nShips(0)
{
    
}

int GameImpl::rows() const
{
    return m_nRows;
}

int GameImpl::cols() const
{
    return m_nCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{

    shipLengths.push_back(length);
    shipSymbols.push_back(symbol);
    shipNames.push_back(name);
    m_nShips++;
    return true;

}

int GameImpl::nShips() const
{
    return m_nShips;
}

int GameImpl::shipLength(int shipId) const
{
    return shipLengths[shipId];
}

char GameImpl::shipSymbol(int shipId) const
{
    return shipSymbols[shipId];
}

string GameImpl::shipName(int shipId) const
{
    return shipNames[shipId];
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause = true)
{
    
    if (p1->placeShips(b1) == false || p2->placeShips(b2) == false)
    {
        return nullptr;
    }
    
    while (b1.allShipsDestroyed() == false && b2.allShipsDestroyed() == false)
    {
        //p1 turn
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        if (p1->isHuman())
        {
            b2.display(true);
        }
        else
        {
            b2.display(false);
        }

        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId = -1;
        Point p = p1->recommendAttack();
        bool validShot = b2.attack(p, shotHit, shipDestroyed, shipId);
        p1->recordAttackResult(p, validShot, shotHit, shipDestroyed, shipId);

        if (validShot) {
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and ";
            if (shipDestroyed)
            {
                cout << "destroyed the " << shipName(shipId) << ", resulting in:" << endl;
            }
            else if (shotHit)
            {
                cout << "hit something, resulting in:" << endl;
            }
            else
            {
                cout << "missed, resulting in:" << endl;
            }

            if (p1->isHuman())
            {
                b2.display(true);
            }
            else
            {
                b2.display(false);
            }
        }
        else
            cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;

        if (b2.allShipsDestroyed())
            break;

        if (shouldPause)
        {
            waitForEnter();
        }

        //p2 turn
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;

        if (p2->isHuman())
        {
            b1.display(true);
        }
        else
        {
            b1.display(false);
        }

        bool shotHit2 = false;
        bool shipDestroyed2 = false;
        int shipId2 = -1;
        Point p3 = p2->recommendAttack();
        bool validShot2 = b1.attack(p3, shotHit2, shipDestroyed2, shipId2);
        p2->recordAttackResult(p3, validShot2, shotHit2, shipDestroyed2, shipId2);

        if (validShot2) {
            cout << p2->name() << " attacked (" << p3.r << "," << p3.c << ") and ";
            if (shipDestroyed2)
            {
                cout << "destroyed the " << shipName(shipId2) << ", resulting in:" << endl;
            }
            else if (shotHit2)
            {
                cout << "hit something, resulting in:" << endl;
            }
            else
            {
                cout << "missed, resulting in:" << endl;
            }

            if (p2->isHuman())
            {
                b1.display(true);
            }
            else
            {
                b1.display(false);
            }
        }
        else
            cout << p2->name() << " wasted a shot at (" << p3.r << "," << p3.c << ")." << endl;

        if (b1.allShipsDestroyed())
            break;

        if (shouldPause)
        {
            waitForEnter();
        }
        
    }

    if (b1.allShipsDestroyed())
    {
        cout << p2->name() << " wins!" << endl;
        if (p1->isHuman())
        {
            b2.display(false);
        }
        return p2;
    }
    else
    {
        cout << p1->name() << " wins!" << endl;
        if (p2->isHuman())
        {
            b1.display(false);
        }
        return p1;
    }
        

}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

