#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    ~BoardImpl();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    char m_board[MAXROWS][MAXCOLS];
    const Game& m_game;
    int* m_placedShips;
};

BoardImpl::BoardImpl(const Game& g)
    : m_game(g)
{

    for (int i = 0; i < m_game.rows(); i++)
        for (int j = 0; j < m_game.cols(); j++)
            m_board[i][j] = '.';

    m_placedShips = new int[m_game.nShips()];

    for (int k = 0; k < m_game.nShips(); k++)
    {
        m_placedShips[k] = -1;
    }

}

BoardImpl::~BoardImpl() {

    delete[] m_placedShips;

}

void BoardImpl::clear()
{
    for (int i = 0; i < m_game.rows(); i++)
        for (int j = 0; j < m_game.cols(); j++)
            m_board[i][j] = '.';
}

void BoardImpl::block()
{
    int counter = 0;
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (randInt(2) == 0 && m_board[r][c] == '.')
            {
                m_board[r][c] = 'X';
                counter++;
            }

            if (counter == (m_game.rows() * m_game.cols() / 2))
            {
                return;
            }

            if (r == m_game.rows() - 1 && c == m_game.cols() - 1 && counter != (m_game.rows() * m_game.cols() / 2))
            {
                r = 0;
                c = -1;
                continue;
            }
        }
    }
}

void BoardImpl::unblock()
{
 
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_board[r][c] == 'X')
            {
                m_board[r][c] = '.';
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    
    if (shipId < 0 || shipId >= m_game.nShips() || m_placedShips[shipId] != -1)
    {
        return false;
    }

    switch (dir) {
    case VERTICAL:
        
        if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
        {
            return false;
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (m_board[topOrLeft.r + i][topOrLeft.c] != '.')
            {
                return false;
            }
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_board[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }

        m_placedShips[shipId] = m_game.shipLength(shipId);
        return true;

    case HORIZONTAL:
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
        {
            return false;
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (m_board[topOrLeft.r][topOrLeft.c + i] != '.')
            {
                return false;
            }
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
        }

        m_placedShips[shipId] = m_game.shipLength(shipId);
        return true;
    }
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0 || shipId >= m_game.nShips() || m_placedShips[shipId] == -1)
    {
        return false;
    }

    switch (dir) {
    case VERTICAL:

        if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
        {
            return false;
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (m_board[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_board[topOrLeft.r + i][topOrLeft.c] = '.';
        }

        m_placedShips[shipId] = -1;
        return true;

    case HORIZONTAL:
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
        {
            return false;
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (m_board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }

        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_board[topOrLeft.r][topOrLeft.c + i] = '.';
        }

        m_placedShips[shipId] = -1;
        return true;
    }
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for (int k = 0; k < m_game.cols(); k++)
    {
        cout << k;
    }
    cout << endl;

    for (int i = 0; i < m_game.rows(); i++)
    {
        cout << i << " ";
        for (int j = 0; j < m_game.cols(); j++)
        {
            if (shotsOnly)
            {
                if (m_board[i][j] == 'X' || m_board[i][j] == 'o')
                {
                    cout << m_board[i][j];
                }
                else
                {
                    cout << '.';
                }
            }
            else
            {
                cout << m_board[i][j];
            }
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    
    if (m_game.isValid(p) == false || m_board[p.r][p.c] == 'o' || m_board[p.r][p.c] == 'X')
    {
        return false;
    }

    if (m_board[p.r][p.c] != '.')
    {
        shotHit = true;
        for (int i = 0; i < m_game.nShips(); i++)
        {
            if (m_board[p.r][p.c] == m_game.shipSymbol(i))
            {
                m_placedShips[i]--;
                shipDestroyed = false;
                if (m_placedShips[i] == 0)
                {
                    shipId = i;
                    shipDestroyed = true;
                }
                m_board[p.r][p.c] = 'X';
                return true;
            }
        }
    }

    shotHit = false;
    m_board[p.r][p.c] = 'o';
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int i = 0; i < m_game.nShips(); i++)
    {
        if (m_placedShips[i] != 0)
        {
            return false;
        }
    }

    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
