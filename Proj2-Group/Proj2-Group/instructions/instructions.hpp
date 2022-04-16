//
//  instructions.hpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#ifndef instructions_hpp
#define instructions_hpp

#include "necessaryHeader.hpp"

class instructions {
private:
    string PC; // Instruction program counter (PC)
    int type; // Instruction type
    string* depends; // A list of PC values for instructions that the current instruction depends on
    int dependsLength; // Depends array length
    instructions* next; // A list of
    
    instructions(); // Default constructor

    // Help functions
    void __reloadH(string s); // This is a help function of reload. It will put s into current instructions
    
public:
    ~instructions(); // Deconstructor
    
    instructions(string s); // Constructor
    instructions(string PC, int type, string depends[], int dependsLength); // Constructor
    
    void pop(instructions* INS); // Pop instructions
    
    void push(instructions INS); // Push instructions
    
    instructions top(); // Return top instuctions
    
    void reload(); // Reload instructions from the file. Each time reload 30 instructions. Assume never reach the end of the file
    
    int length(); // Return the length of current instructions queue
};

#endif /* instructions_hpp */
