#include <iostream>
#include <string>
#include <cassert>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    if (sr == er && sc == ec) { //If the current (r,c) coordinate is equal to the ending coordinate, then we've solved the maze 
        return true;
    }
    int dirs[4][2] = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} }; // East, North, West, South
    maze[sr][sc] = 'X'; // Update maze[sr][sc] to indicate that the algorithm has encountered it
    for (int i = 0; i < 4; i++) {
        int newR = sr + dirs[i][0];
        int newC = sc + dirs[i][1];
        //cout << "(" << newR << ", " << newC << ")" << endl;
        if (maze[newR][newC] == '.') {
            if (pathExists(maze, nRows, nCols, newR, newC, er, ec)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    string maze1[10] = {
          "XXXXXXXXXX",
          "X.X..X...X",
          "X.XX.X.XXX",
          "X....X.X.X",
          "XX.X.X...X",
          "XXX..X.X.X",
          "X...X...XX",
          "X.XX..X.XX",
          "X....X...X",
          "XXXXXXXXXX",
    };

    string maze2[10] = {
      "XXXXXXXXXX",
      "X.X..X...X",
      "XXXX.X.XXX",
      "X....X.X.X",
      "XX.X.X...X",
      "XXX..X.X.X",
      "X...X...XX",
      "X.XX..X.XX",
      "X....X...X",
      "XXXXXXXXXX",
    };

    string maze3[10] = {
      "XXXXXXXXXX",
      "XX.....XXX",
      "X..XX....X",
      "X...X...XX",
      "X.X.XXX..X",
      "XXXX..X..X",
      "XX....X..X",
      "X.......XX",
      "X..XXXXXXX",
      "XXXXXXXXXX",
    };

    string maze4[10] = {
      "XXXXXXXXXX",
      "XX.....XXX",
      "X..XX....X",
      "X...X...XX",
      "X.X.XXX..X",
      "XXXX..X..X",
      "XX....X..X",
      "X.X.....XX",
      "X..XXXXXXX",
      "XXXXXXXXXX",
    };
    assert(pathExists(maze1, 10, 10, 8, 6, 1, 1));
    assert(!pathExists(maze2, 10, 10, 8, 6, 1, 1));
    assert(pathExists(maze3, 10, 10, 4, 3, 7, 1));
    assert(!pathExists(maze4, 10, 10, 4, 3, 7, 1));
    cout << "All tests passed" << endl;
}