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
using namespace std;

// Necessary Global Variables
extern bool intOccupied = false; // This variable represent: is integer ALU unit occupied

extern bool floatOccupied = false; // This variable represent: is floating point unit occupied

extern bool branchExeOccupied = false; // This variable represent: is branch execution unit occupied

extern bool readOcuupied = false; // This variable represent: is read port occupied

extern bool writeOccupied = false; // This varible represent: is write port occupied

extern ifstream fin; // This varibale used to continue read instructions from the file

// Performance Metrics

#endif /* necessaryHeader_hpp */
