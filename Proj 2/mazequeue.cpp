#include <queue>
#include <string>
#include <cassert>
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
    queue<Coord> visited;
    int dirs[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} }; // east, north, west, south
    Coord start(sr, sc);
    visited.push(start); // Push the starting coordinate (sr,sc) onto the coordinate stack
    maze[sr][sc] = 'X'; // Update maze[sr][sc] to indicate that the algorithm has encountered it
    int i = 1;
    while (!visited.empty()) {
        Coord curr = visited.front(); // Location that is being explored
        int currX = curr.r();
        int currY = curr.c();
        cout << i << ": " << "(" << currX << ", " << currY << ")" << endl;
        i++;
        visited.pop();
        if (curr.r() == er && curr.c() == ec) { //If the current (r,c) coordinate is equal to the ending coordinate, then we've solved the maze 
            return true;
        }
        for (int i = 0; i < 4; i++) {
            int newX = currX + dirs[i][0];
            int newY = currY + dirs[i][1];
            if (newX < 0 || newY < 0 || newX >= nRows || newY >= nCols || maze[newX][newY] != '.') {
                continue;
            }
            Coord newPoint(newX, newY);
            visited.push(newPoint);
            maze[newX][newY] = 'X';
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
}
