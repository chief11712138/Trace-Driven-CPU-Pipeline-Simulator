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
    readOccupied = false;
    writeOccupied = false;
    writeOccupied = false;
    fin.open(location);
    isEXE = false;
    isMEM = false;
    
    string s;
    for(int i=0; i<inst_count; i++) getline(fin, s);
}

// Simulate IF stage
void pipeline::S_IF() {
    // Not reach the pipeline width
    // Then check if in queue
    if(IF->length() < 4 && !waiting->length()) {
        // If not reach the end of the instructions queue and read port free
        if(waiting->length() > 0 && !readOccupied) {
            instructions INS = waiting->top();
            readOccupied = true;
            waiting->pop(waiting);
            IF->push(INS);
        }
    }
    
    // Reach the pipeline width (or after push)
    // Then out queue
    // If ID not reach the pipline width
    if(ID->length() < 4 && IF->length()) {
        instructions INS = IF->top();
        IF->pop(IF);
        ID->push(INS);
    }
}

// Simulate ID stage
void pipeline::S_ID() {
    // If ID queue no element
    if(!ID->length())
        return;
    
    // If EX not reach the pipeline width
    if(EX->length()) {
        instructions INS = ID->top();
        ID->pop(ID);
        EX->push(INS);
    }
}

// Simulate EX stage
void pipeline::S_EX() {
    if(!EX->length())
        return;
    
    // Check dependence
    instructions INS = EX->top();
    string* D = INS.getDepends();
    int DL = INS.getDependsLength();
    
    if(!D)
        for(int i=0; i<DL; i++)
            if(!root.count(D[i]))
                isEXE = false;
    
    // Execute it
    bool ifOut = false;
    
    if(isEXE) {
        switch (INS.getType()) {
            case 1:
                if(!intOccupied) {
                    intOccupied = true;
                    ifOut = true;
                }
                break;
            case 2:
                if(!floatOccupied) {
                    floatOccupied = true;
                    ifOut = true;
                }
                break;
            case 3:
                if(!branchExeOccupied) {
                    branchExeOccupied = true;
                    // After branch instruciton executed, it can be dependence
                    pair<map<string, int>::iterator, bool> insertPair;
                    insertPair = root.insert(pair<string, int>(INS.getPC(), 1));
                    // If insert fail, same instruction appears
                    if(!insertPair.second)
                        root[INS.getPC()] += root[INS.getPC()] + 1;
                }
            
            default:
                isEXE = true;
                ifOut = true;
                break;
        }
    }
    
    // Out instruction
    if(MEM->length() < 4 && isEXE && ifOut) {
        isEXE = false;
        EX->pop(EX);
        MEM->push(INS);
    }
}

// Simulate MEM stage
void pipeline::S_MEM() {
    if(!MEM->length())
        return;
    
    instructions INS = MEM->top();
    
    // Access memory
    bool ifOut = false;
    
    switch (INS.getType()) {
        case 4:
            if(!readOccupied) {
                readOccupied = true;
                ifOut = true;
                isMEM = true;
            }
            break;
        case 5:
            if(!writeOccupied) {
                writeOccupied = true;
                ifOut = true;
                isMEM = true;
            }
            break;
                
        default:
            isMEM = true;
            ifOut = true;
            break;
    }
    
    // Out instruction
    if(WB->length() < 4 && ifOut) {
        isMEM = false;
        MEM->pop(MEM);
        WB->push(INS);
    }
}

// Simulate WB stage
void pipeline::S_WB() {
    if(!WB->length())
        return;
    
    // Check if write port free
    // If write port free
    // Pop and insert into tree
    if(!writeOccupied) {
        writeOccupied = false;
        
        instructions INS = WB->top();
        WB->pop(WB);
        pair<map<string, int>::iterator, bool> insertPair;
        insertPair = root.insert(pair<string, int>(INS.getPC(), 1));
        // If insert fail, same instruction appears
        if(!insertPair.second)
            root[INS.getPC()] += root[INS.getPC()] + 1;
    }
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
