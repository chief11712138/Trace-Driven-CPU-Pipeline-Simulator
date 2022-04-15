//
//  instructions.hpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#ifndef instructions_hpp
#define instructions_hpp

#include "necessaryHeader.hpp"
#include "record.hpp"

class instructions {
private:
    string PC; // Instruction program counter (PC)
    int type; // Instruction type
    string* depends; // A list of PC values for instructions that the current instruction depends on
    instructions* next; // A list of
    
    instructions(); // Default constructor
    ~instructions(); // Deconstructor
    
    void readFile(string tempS); // Read input trace into program
    
    void reload(); // Reload instructions from the file. Each time reload 10 instructions
    
    // Help functions
    
public:
    instructions(string s); // Constructor
    
    void pop(); // Pop instructions
    
    void push(); // Push instructions
    
    instructions top(); // Return top instuctions
};

#endif /* instructions_hpp */
