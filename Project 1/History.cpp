#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int rows, int cols) {
	m_rows = rows;
	m_cols = cols;
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			grid[i][j] = 0;
		}
	}
}

bool History::record(int r, int c) {
	if (r < 1 || c < 1 || r > m_rows || c > m_cols) {
		return false;
	}
	grid[r - 1][c - 1]++;
	return true;
}

void History::display() const{
	clearScreen();
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			int car = 64 + grid[i][j];
			if (grid[i][j] == 0) {
				cout << '.';
			}
			else if (car <= 90){
				cout << char(car);
			}
			else {
				cout << 'Z';
			}
		}
		cout << endl;
	}
}

