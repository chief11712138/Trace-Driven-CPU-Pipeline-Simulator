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
    instructions* next; // Next instructions

    // Help functions
    void __reloadH(string s); // This is a help function of reload. It will put s into current instructions
    
public:
    instructions(); // Default constructor
    ~instructions(); // Deconstructor

    instructions(string PC, int type, string* depends, int dependsLength); // Constructor
    
    void pop(instructions* INS); // Pop instructions
    
    void push(instructions INS); // Push instructions
    
    instructions top(); // Return top instuctions
    
    void reload(); // Reload instructions from the file. Each time reload 30 instructions. Assume never reach the end of the file
    
    int length(); // Return the length of current instructions queue
    
    // Help functions
    string* getDepends(); // Get the top instruction depends
    
    int getDependsLength(); // Get the top instruction depends length
    
    int getType(); // Get the top instruction type
    
    string getPC(); // Get the top instruction PC
    
    void setInstructions(string PC, int type, string* s, int dependensLength); // Set the top instruction
};

#endif /* instructions_hpp */
