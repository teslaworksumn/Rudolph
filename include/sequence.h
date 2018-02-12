#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>       // std::string
#include <vector>       //std::vector
#include <iostream>
#include <fstream>

class Sequence
{
private:
    std::vector< std::vector<int>> grid;
    int ROWS;
    int COLS;
    std::string sequenceName;

public:
    Sequence() ;
    Sequence(int rows, int cols, std::string name);
    int getRows();
    int getCols();
    std::string getName();
    std::vector< std::vector<int>> getGrid();
    void save();
    void print();
};


#endif // SEQUENCE_H
