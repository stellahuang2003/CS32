// HW 2 PROBLEM 1
#include <stack>
#include <iostream>
using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
  private:
    int m_row;
    int m_col;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec);

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = '/';
    
    while(!coordStack.empty())
    {
        int row = coordStack.top().r();
        int col = coordStack.top().c();
//        cout << "(" << row << ", " << col << ")" << endl;
        coordStack.pop();
        
        if(row == er && col == ec) {
            return true;
        }
        if((col-1)>0 && maze[row][col-1] != '/' && maze[row][col-1] != 'X') { // left
            coordStack.push(Coord(row, col-1));
            maze[row][col-1] = '/';
        }
        if((row-1)>0 && maze[row-1][col] != '/' && maze[row-1][col] != 'X') { // up
            coordStack.push(Coord(row-1, col));
            maze[row-1][col] = '/';
        }
        if((col+1)<10 && maze[row][col+1] != '/' && maze[row][col+1] != 'X') { // right
            coordStack.push(Coord(row, col+1));
            maze[row][col+1] = '/';
        }
        if((row+1)<10 && maze[row+1][col] != '/' && maze[row+1][col] != 'X') { // down
            coordStack.push(Coord(row+1, col));
            maze[row+1][col] = '/';
        }
    }
    return false;
}

int main()
{
    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X' },
        { 'X','.','X','.','.','.','X','.','.','X' },
        { 'X','.','X','.','X','X','X','X','.','X' },
        { 'X','.','X','.','.','X','.','X','.','X' },
        { 'X','.','X','X','.','X','.','.','.','X' },
        { 'X','.','.','.','.','.','.','X','X','X' },
        { 'X','.','X','X','X','X','.','X','.','X' },
        { 'X','.','.','.','.','X','X','X','.','X' },
        { 'X','.','X','.','.','X','.','.','.','X' },
        { 'X','X','X','X','X','X','X','X','X','X' }
    };

    if (pathExists(maze, 5,6, 8,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
