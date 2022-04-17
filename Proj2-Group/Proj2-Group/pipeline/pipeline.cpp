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
    fin.open(location);
    isEXE = false;
    isMEM = false;
    
    string s;
    for(int i=0; i<start_inst; i++) getline(fin, s);
}

// Simulate IF stage
void pipeline::S_IF() {
    
    // Not reach the pipeline width
    // Then check if in queue
    if(IF->length() < W && waiting->length()) {
        // If not reach the end of the instructions queue and read port free
        if(waiting->length() > 0 && !readOccupied) {
            instructions INS = waiting->top();
            readOccupied = true;
            waiting->pop(waiting);
            
            // If this is the first instruction of IF
            if(IF->getType() == -1){
                if(!INS.getDepends()) {
                    string* tempS = INS.getDepends();
                    string* s = new string[INS.getDependsLength()];
                    for(int i=0; i<INS.getDependsLength(); i++) s[i] = tempS[i];
                
                    IF->setInstructions(INS.getPC(),    INS.getType(), s, INS.getDependsLength());
                }
                else
                    IF->setInstructions(INS.getPC(),    INS.getType(), nullptr, INS.getDependsLength());
            }
            else
                IF->push(INS);
        }
    }
    cout <<"B" <<endl;
    // Reach the pipeline width (or after push)
    // Then out queue
    // If ID not reach the pipline width
    if(ID->length() < W && IF->length()) {
        instructions INS = IF->top();
        IF->pop(IF);
        
        // If this is the first instruction of ID
        if(ID->getType() == -1){
            if(!INS.getDepends()) {
                string* tempS = INS.getDepends();
                string* s = new string[INS.getDependsLength()];
                for(int i=0; i<INS.getDependsLength(); i++) s[i] = tempS[i];
            
                ID->setInstructions(INS.getPC(), INS.getType(), s, INS.getDependsLength());
            }
            else
                ID->setInstructions(INS.getPC(), INS.getType(), nullptr, INS.getDependsLength());
        }
        else
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
        // If this is the first instruction
        if(EX->getType() == -1){
            if(!EX->getDepends()) {
                string* tempS = INS.getDepends();
                string* s = new string[INS.getDependsLength()];
                for(int i=0; i<INS.getDependsLength(); i++) s[i] = tempS[i];
            
                EX->setInstructions(INS.getPC(), INS.getType(), s, INS.getDependsLength());
            }
            else
                EX->setInstructions(INS.getPC(), INS.getType(), nullptr, INS.getDependsLength());
        }
        else
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
    if(MEM->length() < W && isEXE && ifOut) {
        isEXE = false;
        EX->pop(EX);
        // If this is the first instruction
        if(MEM->getType() == -1){
            if(!MEM->getDepends()) {
                string* tempS = INS.getDepends();
                string* s = new string[INS.getDependsLength()];
                for(int i=0; i<INS.getDependsLength(); i++) s[i] = tempS[i];
            
                MEM->setInstructions(INS.getPC(), INS.getType(), s, INS.getDependsLength());
            }
            else
                MEM->setInstructions(INS.getPC(), INS.getType(), nullptr, INS.getDependsLength());
        }
        else
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
    if(WB->length() < W && ifOut) {
        isMEM = false;
        MEM->pop(MEM);
        // If this is the first instruction
        if(WB->getType() == -1){
            if(!WB->getDepends()) {
                string* tempS = INS.getDepends();
                string* s = new string[INS.getDependsLength()];
                for(int i=0; i<INS.getDependsLength(); i++) s[i] = tempS[i];
            
                WB->setInstructions(INS.getPC(), INS.getType(), s, INS.getDependsLength());
            }
            else
                WB->setInstructions(INS.getPC(), INS.getType(), nullptr, INS.getDependsLength());
        }
        else
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
    waiting = new instructions();
    IF = new instructions();
    ID = new instructions();
    EX = new instructions();
    MEM = new instructions();
    WB = new instructions();
    
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
    
    intOccupied = false;
    floatOccupied = false;
    branchExeOccupied = false;
    readOccupied = false;
    writeOccupied = false;
    writeOccupied = false;
    
    return true;
}
