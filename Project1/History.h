//
//  History.h
//  garks
//
//  Created by Stella Huang on 3/31/22.
//

#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    int h_rows;
    int h_cols;
    int timesAttacked[MAXROWS][MAXCOLS];
};

#endif
