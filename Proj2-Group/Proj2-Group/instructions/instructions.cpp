//
//  instructions.cpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#include "instructions.hpp"

// Private:
// Default constructor
instructions::instructions() {
    PC = "Error";
    type = -1;
    depends = nullptr;
    dependsLength = -1;
    next = nullptr;
}

// Help functions
// This is a help function of reload. It will put s into current instructions
void instructions::__reloadH(string s) {
    this->dependsLength = 0;
    int i = 0;
    
    int start = 0;
    int end = (int)s.find(',');
    
    this->PC = s.substr(start, end);
    
    start += (end - start + 1);
    end = (int)s.find(',', start);
    
    this->type = stoi(s.substr(start, end-start));
    
    int tempStart = start;
    int tempEnd = end;
    
    while(s.find(',', tempStart) != -1) {
        tempStart += (tempEnd + 1);
        tempEnd = (int)s.find(',', tempStart);
        this->dependsLength++;
    }

    this->depends = new string[this->dependsLength];

    while (s.find(',', start) != -1) {
        start += (end - start + 1);
        end = (int)s.find(',', start);

        this->depends[i] = s.substr(start, end-start);
        i++;
    }
}

// Public:
// Deconstructor
instructions::~instructions() {
    if(this->next == nullptr) {
        delete depends;
        return;
    }
    
    instructions* temp = this->next;
    delete this->depends;
    delete this;
    temp->~instructions();
}

// Constructor
instructions::instructions(string s) {
    s = location;
    this->reload();
}

// Constructor
instructions::instructions(string PC, int type, string depends[], int dependsLength) {
    this->PC = PC;
    this->type = type;
    this->depends = new string[dependsLength];
    for(int i=0; i<dependsLength; i++) this->depends[i] = depends[i];
    this->dependsLength = dependsLength;
}

// Pop instructions
void instructions::pop(instructions* INS) {
    if(INS == nullptr) {
        cout <<"Error! Cannot pop empty instructions queue" <<endl;
        exit(10);
    }
    
    if(INS->next == nullptr) {
        this->~instructions();
        return;
    }
    
    instructions* temp = INS->next;
    delete INS->depends;
    delete INS;
    INS = temp;
}

// Push instructions
void instructions::push(instructions INS) {
    if(this->type == -1) {
        this->PC = INS.PC;
        this->type = INS.type;
        this->depends = new string[INS.dependsLength];
        for(int i=0; i<INS.dependsLength; i++) this->depends[i] = INS.depends[i];
        this->dependsLength = INS.dependsLength;
        return;
    }
    
    instructions* temp = this;
    while(temp->next) temp = temp->next;
    
    temp->next = new instructions;
    
    temp->PC = INS.PC;
    temp->type = INS.type;
    temp->depends = new string[INS.dependsLength];
    for(int i=0; i<INS.dependsLength; i++) temp->depends[i] = INS.depends[i];
    temp->dependsLength = INS.dependsLength;
}

// Return top instructions
instructions instructions::top() {
    string s[this->dependsLength];
    for(int i=0; i<this->dependsLength; i++) s[i] = this->depends[i];
    
    instructions result(this->PC, this->type, s, this->dependsLength);
    
    return result;
}

// Reload instructions from the file. Each time reload 30 instructions
void instructions::reload() {
    string s;
    int i = 0;
    
    if(this->type == -1) {
        getline(fin ,s);
        this->__reloadH(s);
    }
        
    instructions* temp = this;
    while(temp->next) temp = temp->next;
    
    while(getline(fin, s) && i<30) {
        temp->next = new instructions;
        temp->next->__reloadH(s);
        temp = temp->next;
        i++;
    }
}

// Return the length of current instructions queue
int instructions::length() {
    if(!this)
        return 0;
    if(!this->next)
        return 1;
    
    int result = 1;
    instructions* temp = this;
    
    while(temp->next) {
        temp = temp->next;
        result++;
    }
    
    return result;
}

// Help functions
// Get the top instruction depends
string* instructions::getDepends() {
    return this->depends;
}

// Get the top instruction depends length
int instructions::getDependsLength() {
    return this->dependsLength;
}

// Get the top instruction type
int instructions::getType() {
    return this->type;
}

// Get the top instruction PC
string instructions::getPC() {
    return this->PC;
}
