#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class HumanPlayer : public Player {

public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

};

HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g) {}

bool HumanPlayer::isHuman() const {

    return true;

}

bool HumanPlayer::placeShips(Board& b) {

    cout << name() << " must place " << game().nShips() << " ships." << endl;
    b.display(false);

    bool skip = false;
    char d;

    for (int i = 0; i < game().nShips(); i++)
    {
        if (skip == false) 
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            cin >> d;
            if (d != 'h' && d != 'v')
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Direction must be h or v." << endl;
                i--;
                continue;
            }
        }

        Direction dir;
        if (d == 'h')
        {
            dir = HORIZONTAL;
        }
        else
        {
            dir = VERTICAL;
        }

        bool shipCantBePlaced = true;

        for (int j = 0; j < game().rows(); j++)
        {
            for (int k = 0; k < game().cols(); k++)
            {
                if (b.placeShip(Point(j, k), i, VERTICAL))
                {
                    shipCantBePlaced = false;
                    b.unplaceShip(Point(j, k), i, VERTICAL);
                    break;
                }
                if (b.placeShip(Point(j, k), i, HORIZONTAL))
                {
                    shipCantBePlaced = false;
                    b.unplaceShip(Point(j, k), i, HORIZONTAL);
                    break;
                }
            }

            if (shipCantBePlaced == false)
            {
                break;
            }
        }

        if (shipCantBePlaced)
        {
            return false;
        }

        int r;
        int c;
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        if (getLineWithTwoIntegers(r, c)) {
            if (b.placeShip(Point(r, c), i, dir) == false)
            {
                cout << "The ship can not be placed there." << endl;
                skip = true;
                i--;
                continue;
            }

            b.display(false);
            skip = false;
        }
        else
        {
            cout << "You must enter two integers." << endl;
            skip = true;
            i--;
            continue;
        }
    }

    return true;

}

Point HumanPlayer::recommendAttack() {
    
    while (true) {
        int r;
        int c;
        cout << "Enter the row and column to attack (e.g., 3 5): ";
        if (getLineWithTwoIntegers(r, c))
            return Point(r, c);
        else
        {
            cout << "You must enter two integers." << endl;
        }
    }

}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) {}

void HumanPlayer::recordAttackByOpponent(Point p) {}



//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class MediocrePlayer : public Player {

public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    int mediocrePlacing(Board& b, int shipId);

private:
    int state;
    Point m_lastCellAttacked;
    char m_board[MAXROWS][MAXCOLS];

};

MediocrePlayer::MediocrePlayer(string nm, const Game&g) : Player(nm, g), state(1) {}

bool MediocrePlayer::placeShips(Board& b) {

    int counter = 0;
    while (counter != 50)
    {
        b.block();

        if (mediocrePlacing(b, 0) != 0)
        {
            b.unblock();
            return true;
        }

        b.unblock();
        counter++;
    }

    return false;

}

int MediocrePlayer::mediocrePlacing(Board& b, int shipId) {

    if (shipId == game().nShips())
    {
        return 1;
    }

    int counter = 0;

    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if (b.placeShip(Point(r, c), shipId, VERTICAL))
            {
                counter += mediocrePlacing(b, shipId + 1);
            }

            if (counter > 0)
            {
                return counter;
            }
            
            b.unplaceShip(Point(r, c), shipId, VERTICAL);

        }
    }

    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if (b.placeShip(Point(r, c), shipId, HORIZONTAL))
            {
                counter += mediocrePlacing(b, shipId + 1);
            }

            if (counter > 0)
            {
                return counter;
            }

            b.unplaceShip(Point(r, c), shipId, HORIZONTAL);

        }
    }

    return 0;

}

void MediocrePlayer::recordAttackByOpponent(Point p) {}

Point MediocrePlayer::recommendAttack() {

    switch (state) {
    case 1:
        while (true)
        {
            Point p = game().randomPoint();
            if (m_board[p.r][p.c] != 'o')
            {
                m_board[p.r][p.c] = 'o';
                m_lastCellAttacked = p;
                return p;
            }
        }
    case 2:
        while (true) {
            Point p;
            int direction = randInt(4);
            int distance = randInt(4) + 1;
            switch (direction) {
            case 0: //north
                p.r = m_lastCellAttacked.r - distance; 
                p.c = m_lastCellAttacked.c; 
                break;
            case 1: //east
                p.r = m_lastCellAttacked.r; 
                p.c = m_lastCellAttacked.c + distance; 
                break;
            case 2: //south
                p.r = m_lastCellAttacked.r + distance;
                p.c = m_lastCellAttacked.c; 
                break;
            case 3: //west
                p.r = m_lastCellAttacked.r; 
                p.c = m_lastCellAttacked.c - distance; 
                break;
            }

            if (game().isValid(p) && m_board[p.r][p.c] != 'o')
            {
                m_board[p.r][p.c] = 'o';
                return p;
            }
        }
    }

}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId) {

    switch (state) {
    case 1:
        if (shotHit == true && shipDestroyed == false)
        {
            state = 2;
        }
        break;
    case 2: 
        bool allChosen = true; //for cases where ship lengths are greater than 5
        int rowStart;
        int rowEnd;
        int colStart;
        int colEnd;
        if (m_lastCellAttacked.r - 4 < 0)
            rowStart = 0;
        else
            rowStart = m_lastCellAttacked.r - 4;
        if (m_lastCellAttacked.r + 4 >= game().rows())
            rowEnd = game().rows();
        else
            rowEnd = m_lastCellAttacked.r + 5;
        if (m_lastCellAttacked.c - 4 < 0)
            colStart = 0;
        else
            colStart = m_lastCellAttacked.c - 4;
        if (m_lastCellAttacked.c + 4 >= game().cols())
            colEnd = game().cols();
        else
            colEnd = m_lastCellAttacked.c + 5;

        for (int i = rowStart; i < rowEnd; i++)
        {
            if (m_board[i][m_lastCellAttacked.c] != 'o')
            {
                allChosen = false;
                break;
            }
        }

        for (int j = colStart; j < colEnd; j++)
        {
            if (m_board[m_lastCellAttacked.r][j] != 'o')
            {
                allChosen = false;
                break;
            }
        }

        if (shipDestroyed || allChosen)
        {
            state = 1;
            break;
        }
    }

}

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

class GoodPlayer : public Player {

public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    int goodPlacing(Board& b, int shipId);

private:
    char m_board[MAXROWS][MAXCOLS];
    int state;
    Point m_lastCellAttacked;
    Point m_lastShipPlaced;
    int direction;

};

GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), state(1) {

    for (int i = 0; i < g.rows(); i++)
        for (int j = 0; j < g.cols(); j++)
            m_board[i][j] = '.';

}

bool GoodPlayer::placeShips(Board& b) {

    int counter = 0;
    while (counter != 50)
    {
        b.block();

        if (goodPlacing(b, 0) != 0)
        {
            b.unblock();
            for (int i = 0; i < game().rows(); i++)
                for (int j = 0; j < game().cols(); j++)
                    m_board[i][j] = '.';
            return true;
        }

        for (int i = 0; i < game().rows(); i++)
            for (int j = 0; j < game().cols(); j++)
                m_board[i][j] = '.';
        b.unblock();
        counter++;
    }

    return false;

}

int GoodPlayer::goodPlacing(Board& b, int shipId) {

    if (shipId == game().nShips())
    {
        return 1;
    }

    int counter = 0;

    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if (game().rows() == 10 && game().cols() == 10 && game().nShips() == 5)
            {
                if (b.placeShip(Point(r, c), shipId, VERTICAL))
                {
                    if (c == 0)
                    {
                        for (int i = 0; i < game().shipLength(shipId); i++)
                        {
                            if (m_board[r + i][c + 1] != '.')
                            {
                                b.unplaceShip(Point(r, c), shipId, VERTICAL);
                                return 0;
                            }
                        }
                    }
                    else if (c > 0 && c < game().cols() - 1)
                    {
                        for (int i = 0; i < game().shipLength(shipId); i++)
                        {
                            if (m_board[r + i][c - 1] != '.' || m_board[r + i][c + 1] != '.')
                            {
                                b.unplaceShip(Point(r, c), shipId, VERTICAL);
                                return 0;
                            }
                        }
                    }
                    else if (c == game().cols() - 1)
                    {
                        for (int i = 0; i < game().shipLength(shipId); i++)
                        {
                            if (m_board[r + i][c - 1] != '.')
                            {
                                b.unplaceShip(Point(r, c), shipId, VERTICAL);
                                return 0;
                            }
                        }
                    }
                    if (r > 0)
                    {
                        if (m_board[r - 1][c] != '.')
                        {
                            b.unplaceShip(Point(r, c), shipId, VERTICAL);
                            return 0;
                        }
                    }
                    if (r + game().shipLength(shipId) < game().rows())
                    {
                        if (m_board[r + game().shipLength(shipId)][c] != '.')
                        {
                            b.unplaceShip(Point(r, c), shipId, VERTICAL);
                            return 0;
                        }
                    }
                    for (int i = 0; i < game().shipLength(shipId); i++)
                    {
                        m_board[r + i][c] = game().shipSymbol(shipId);
                    }
                    counter += goodPlacing(b, shipId + 1);
                }
            }
            else
            {
                if (b.placeShip(Point(r, c), shipId, VERTICAL))
                {
                    counter += goodPlacing(b, shipId + 1);
                }
            }

            if (counter > 0)
            {
                return counter;
            }

            if (b.unplaceShip(Point(r, c), shipId, VERTICAL))
            {
                for (int i = 0; i < game().shipLength(shipId); i++)
                {
                    m_board[r + i][c] = '.';
                }
            }

        }
    }

    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if (game().rows() == 10 && game().cols() == 10 && game().nShips() == 5)
            {
                if (b.placeShip(Point(r, c), shipId, HORIZONTAL))
                {
                    if (r == 0)
                    {
                        for (int i = 0; i < game().shipLength(shipId); i++)
                        {
                            if (m_board[r + 1][c + i] != '.')
                            {
                                b.unplaceShip(Point(r, c), shipId, HORIZONTAL);
                                return 0;
                            }
                        }
                    }
                    else if (r > 0 && r < game().rows() - 1)
                    {
                        for (int i = 0; i < game().shipLength(shipId); i++)
                        {
                            if (m_board[r - 1][c + i] != '.' || m_board[r + 1][c + i] != '.')
                            {
                                b.unplaceShip(Point(r, c), shipId, HORIZONTAL);
                                return 0;
                            }
                        }
                    }
                    else if (r == game().rows() - 1)
                    {
                        for (int i = 0; i < game().shipLength(shipId); i++)
                        {
                            if (m_board[r - 1][c + i] != '.')
                            {
                                b.unplaceShip(Point(r, c), shipId, HORIZONTAL);
                                return 0;
                            }
                        }
                    }
                    if (c > 0)
                    {
                        if (m_board[r][c - 1] != '.')
                        {
                            b.unplaceShip(Point(r, c), shipId, HORIZONTAL);
                            return 0;
                        }
                    }
                    if (c + game().shipLength(shipId) < game().cols())
                    {
                        if (m_board[r][c + game().shipLength(shipId)] != '.')
                        {
                            b.unplaceShip(Point(r, c), shipId, HORIZONTAL);
                            return 0;
                        }
                    }
                    for (int i = 0; i < game().shipLength(shipId); i++)
                    {
                        m_board[r][c + i] = game().shipSymbol(shipId);
                    }
                    counter += goodPlacing(b, shipId + 1);
                }
            }
            else
            {
                if (b.placeShip(Point(r, c), shipId, HORIZONTAL))
                {
                    counter += goodPlacing(b, shipId + 1);
                }
            }

            if (counter > 0)
            {
                return counter;
            }

            if (b.unplaceShip(Point(r, c), shipId, HORIZONTAL))
            {
                for (int i = 0; i < game().shipLength(shipId); i++)
                {
                    m_board[r][c + i] = '.';
                }
            }

        }
    }

    return 0;

}

Point GoodPlayer::recommendAttack() {

    switch (state) {
    case 1:
        while (true)
        {
            Point p = game().randomPoint();
            if (m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
            {
                bool blockedNorth = false;
                bool blockedEast = false;
                bool blockedSouth = false;
                bool blockedWest = false;
                if (game().isValid(Point(p.r - 1, p.c)) == false || m_board[p.r - 1][p.c] == 'o')
                {
                    blockedNorth = true;
                }
                if (game().isValid(Point(p.r, p.c + 1)) == false || m_board[p.r][p.c + 1] == 'o')
                {
                    blockedEast = true;
                }
                if (game().isValid(Point(p.r + 1, p.c)) == false || m_board[p.r + 1][p.c] == 'o')
                {
                    blockedSouth = true;
                }
                if (game().isValid(Point(p.r, p.c - 1)) == false || m_board[p.r][p.c - 1] == 'o')
                {
                    blockedWest = true;
                }

                if (blockedNorth && blockedEast && blockedWest && blockedSouth)
                {
                    continue;
                }

                m_board[p.r][p.c] = 'o';
                m_lastCellAttacked = p;
                return p;
            }
        }
    case 2:
        direction = 0;
        while (true) {
            Point p;
            switch (direction) {
            case 0: //north
                p.r = m_lastCellAttacked.r - 1;
                p.c = m_lastCellAttacked.c;
                break;
            case 1: //east
                p.r = m_lastCellAttacked.r;
                p.c = m_lastCellAttacked.c + 1;
                break;
            case 2: //south
                p.r = m_lastCellAttacked.r + 1;
                p.c = m_lastCellAttacked.c;
                break;
            case 3: //west
                p.r = m_lastCellAttacked.r;
                p.c = m_lastCellAttacked.c - 1;
                break;
            }

            if (game().isValid(p) && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
            {
                m_board[p.r][p.c] = 'o';
                return p;
            }
            else if (game().isValid(p) && m_board[p.r][p.c] == 'X')
            {
                state = 3;
                break;
            }

            direction++;
        }
    case 3:
        Point p = m_lastCellAttacked;
        bool directionChanged = false;
        while (true) {
            switch (direction) {
            case 0: //north
                p.r -= 1;
                if (game().isValid(p) && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
                {
                    m_board[p.r][p.c] = 'o';
                    return p;
                }
                else if (game().isValid(p) && m_board[p.r][p.c] == 'X')
                {
                    continue;
                }
                else
                {
                    if (directionChanged)
                    {
                        direction = 1;
                        directionChanged = false;
                        p = m_lastCellAttacked;
                        continue;
                    }
                    direction = 2;
                    directionChanged = true;
                }
                break;
            case 1: //east
                p.c += 1;
                if (game().isValid(p) && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
                {
                    m_board[p.r][p.c] = 'o';
                    return p;
                }
                else if (game().isValid(p) && m_board[p.r][p.c] == 'X')
                {
                    continue;
                }
                else
                {
                    if (directionChanged)
                    {
                        direction = 2;
                        directionChanged = false;
                        p = m_lastCellAttacked;
                        continue;
                    }
                    direction = 3;
                    directionChanged = true;
                }
                break;
            case 2: //south
                p.r += 1;
                if (game().isValid(p) && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
                {
                    m_board[p.r][p.c] = 'o';
                    return p;
                }
                else if (game().isValid(p) && m_board[p.r][p.c] == 'X')
                {
                    continue;
                }
                else
                {
                    if (directionChanged)
                    {
                        direction = 3;
                        directionChanged = false;
                        p = m_lastCellAttacked;
                        continue;
                    }
                    direction = 0;
                    directionChanged = true;
                }
                break;
            case 3: //west
                p.c -= 1;
                if (game().isValid(p) && m_board[p.r][p.c] != 'o' && m_board[p.r][p.c] != 'X')
                {
                    m_board[p.r][p.c] = 'o';
                    return p;
                }
                else if (game().isValid(p) && m_board[p.r][p.c] == 'X')
                {
                    continue;
                }
                else
                {
                    if (directionChanged)
                    {
                        direction = 0;
                        directionChanged = false;
                        p = m_lastCellAttacked;
                        continue;
                    }
                    direction = 1;
                    directionChanged = true;
                }
                break;
            }
        }
    }
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId) {

    switch (state) {
    case 1:
        if (shotHit == true && shipDestroyed == false)
        {
            state = 2;
            m_board[p.r][p.c] = 'X';
        }
        else if (shipDestroyed)
        {
            m_board[p.r][p.c] = 'X';
        }
        break;
    case 2:

        if (shipDestroyed)
        {
            state = 1;
            m_board[p.r][p.c] = 'X';
        }
        else if (shotHit)
        {
            state = 3;
            m_board[p.r][p.c] = 'X';
        }
        break;
    case 3:
        if (shipDestroyed)
        {
            state = 1;
            m_board[p.r][p.c] = 'X';
        }
        else if (shotHit)
        {
            m_board[p.r][p.c] = 'X';
        }
    }

}

void GoodPlayer::recordAttackByOpponent(Point p) {



}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
