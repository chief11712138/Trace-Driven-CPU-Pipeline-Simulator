//
//  pipeline.cpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#include "pipeline.hpp"

// Private:
// Initialize all the global variable
void pipeline::initialize() {
    intOccupied = false;
    floatOccupied = false;
    branchExeOccupied = false;
    readOcuupied = false;
    writeOccupied = false;
    writeOccupied = false;
    fin.open(location);
    
    string s;
    for(int i=0; i<inst_count; i++) getline(fin, s);
}

// Simulate IF stage
void pipeline::S_IF() {
    
}

// Simulate ID stage
void pipeline::S_ID() {
    
}

// Simulate EX stage
void pipeline::S_EX() {
    
}

// Simulate MEM stage
void pipeline::S_MEM() {
    
}

// Simulate WB stage
void pipeline::S_WB() {
    
}

// Public:
// Default constructor
pipeline::pipeline() {
    this->initialize();
    
    waiting->reload();
}

// Deconstructor
pipeline::~pipeline() {
    waiting->~instructions();
    IF->~instructions();
    ID->~instructions();
    EX->~instructions();
    MEM->~instructions();
    WB->~instructions();
}

// Simulate system. If end return 0. Otherwise, return 1
bool pipeline::simulation() {
    if(root.size() == inst_count)
        return false;
    
    this->S_IF();
    this->S_ID();
    this->S_EX();
    this->S_MEM();
    this->S_WB();
    
    return true;
}
