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

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    
    maze[sr][sc] = '/';
    
    if ((sc-1)>0 && maze[sr][sc - 1] == '.')
        if (pathExists(maze, sr, sc-1, er, ec))
           return true;
    
    if ((sr-1)>0 && maze[sr-1][sc] == '.')
        if (pathExists(maze, sr-1, sc, er, ec))
            return true;
    
    if((sc+1)<10 && maze[sr][sc + 1] == '.')
        if (pathExists(maze, sr, sc+1, er, ec))
            return true;
    
    if((sr+1)<10 && maze[sr+1][sc] == '.')
        if (pathExists(maze, sr+1, sc, er, ec))
            return true;
    
    return false;
}
