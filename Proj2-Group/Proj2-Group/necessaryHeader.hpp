//
//  necessaryHeader.hpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#ifndef necessaryHeader_hpp
#define necessaryHeader_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <map>
using namespace std;

// Necessary Global Variables
extern bool intOccupied; // This variable represent: is integer ALU unit occupied

extern bool floatOccupied; // This variable represent: is floating point unit occupied

extern bool branchExeOccupied; // This variable represent: is branch execution unit occupied

extern bool readOccupied; // This variable represent: is read port occupied

extern bool writeOccupied; // This varible represent: is write port occupied

extern ifstream fin; // This varibale used to continue read instructions from the file

extern int start_inst; // This variable represent the start position of the instructions

extern int inst_count; // This varivale represent how many instructions need to be executed

extern int W; // This variable represent the width of the pipeline

extern string location; // Used to store file location (need to be the full location to make sure program can find it)

extern map<string, int> root; // Used to store all the instructions have been finished. string represent PC. int represent number of finished instructions

// Performance Metrics
extern int T; // In cycle

extern double IT[5]; // Store each instructions type percentage

// Help Variables
extern bool isEXE; // Used to dertemin EX stage if instruction has been executed

extern bool isMEM; // Used to determin MEM stage if instruction finish memory access

#endif /* necessaryHeader_hpp */
