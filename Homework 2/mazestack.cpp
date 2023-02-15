#include <stack>
#include <string>
#include <iostream>
using namespace std;

class Coord
{
    public:
        Coord(int r, int c) : m_row(r), m_col(c) {}
        int r() const { return m_row; }
        int c() const { return m_col; }
    private:
        int m_row;
        int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    stack<Coord> visited;
    Coord start(sr, sc);
    visited.push(start); // Push the starting coordinate (sr,sc) onto the coordinate stack
    maze[sr][sc] = 'X'; // Update maze[sr][sc] to indicate that the algorithm has encountered it
    while (!visited.empty()) {
        Coord curr = visited.top(); // Location that is being explored
        int r = curr.r();
        int c = curr.c();
        visited.pop(); 
        if (r == er && c == ec) { //If the current (r,c) coordinate is equal to the ending coordinate, then we've solved the maze 
            return true;
        }
        // East
        if (maze[r][c + 1] == '.') {
            Coord newPoint(r, c + 1);
            visited.push(newPoint);
            maze[r][c + 1] = 'X';
        }
        // North
        if (maze[r - 1][c] == '.') {
            Coord newPoint(r - 1, c);
            visited.push(newPoint);
            maze[r - 1][c] = 'X';
        }
        // West
        if (maze[r][c - 1] == '.') {
            Coord newPoint(r, c - 1);
            visited.push(newPoint);
            maze[r][c - 1] = 'X';
        }
        // South
        if (maze[r + 1][c] == '.') {
            Coord newPoint(r + 1, c);
            visited.push(newPoint);
            maze[r + 1][c] = 'X';
        }
    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X..X...X.X",
        "X.XXXX.X.X",
        "X.X.X..X.X",
        "X...X.XX.X",
        "XXX......X",
        "X.X.XXXX.X",
        "X.XXX....X",
        "X...X..X.X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10, 10, 5, 3, 8, 8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    return 0;
}