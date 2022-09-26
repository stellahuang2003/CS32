//
//  History.cpp
//  garks
//
//  Created by Stella Huang on 3/31/22.
//

#include "History.h"
#include <iostream>

History::History(int nRows, int nCols)
{
    h_rows = nRows;
    h_cols = nCols;
    for (int r = 0; r < h_rows; r++) {
        for (int c = 0; c < h_cols; c++) {
            timesAttacked[r][c] = 0;
        }
    }
}

bool History::record(int r, int c)
{
    if(r <= h_rows && r > 0 && c <= h_cols && c > 0) {
        timesAttacked[r-1][c-1]++;
        return true;
    }
    return false;
}

void History::display() const
{
    clearScreen();
    for (int r = 0; r < h_rows; r++) {
        for (int c = 0; c < h_cols; c++) {
            if (timesAttacked[r][c] == 0) {
                std::cout << ".";
            } else if (timesAttacked[r][c] > 26) {
                std::cout << "Z";
            } else {
                std::cout << char(timesAttacked[r][c] + 64);
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
