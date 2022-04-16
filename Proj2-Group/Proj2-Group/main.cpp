//
//  main.cpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#include "necessaryHeader.hpp"
#include "pipeline.hpp"

// Necessary Global Variables
bool intOccupied; // This variable represent: is integer ALU unit occupied

bool floatOccupied; // This variable represent: is floating point unit occupied

bool branchExeOccupied; // This variable represent: is branch execution unit occupied

bool readOcuupied; // This variable represent: is read port occupied

bool writeOccupied; // This varible represent: is write port occupied

ifstream fin; // This varibale used to continue read instructions from the file

int start_inst; // This variable represent the start position of the instructions

int inst_count; // This varivale represent how many instructions need to be executed

int W; // This variable represent the width of the pipeline

string location; // Used to store file location (need to be the full location to make sure program can find it)

map<string, int> root; // Used to store all the instructions have been finished

int main(int argc, char* argv[]) {
    start_inst = stoi(argv[1]);
    inst_count = stoi(argv[2]);
    W = stoi(argv[3]);
    
    pipeline pl;
    
    int t = 1; // Cycles spend
    bool ifEnd = pl.simulation();
    
    while(ifEnd) {
        ifEnd = pl.simulation();
        t++;
    }
}
