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

bool readOccupied; // This variable represent: is read port occupied

bool writeOccupied; // This varible represent: is write port occupied

ifstream fin; // This varibale used to continue read instructions from the file

int start_inst; // This variable represent the start position of the instructions

int inst_count; // This varivale represent how many instructions need to be executed

int W; // This variable represent the width of the pipeline

string location; // Used to store file location (need to be the full location to make sure program can find it)

map<string, int> root; // Used to store all the instructions have been finished. string represent PC. int represent number of finished instructions

// Performance Metrics
int T; // In cycle

double IT[5]; // Store each instructions type percentage

// Help Variables
bool isEXE; // Used to dertemin EX stage if instruction has been executed

bool isMEM; // Used to determin MEM stage if instruction finish memory access

// Help functions
void caculateIT() {
    map<string, int>::iterator iter;
    for(iter = root.begin(); iter != root.end(); iter++) {
        switch (iter->second) {
            case 1:
                IT[0]++;
                break;
            case 2:
                IT[1]++;
                break;
            case 3:
                IT[2]++;
                break;
            case 4:
                IT[3]++;
                break;
            case 5:
                IT[4]++;
                break;
        }
    }
    
    for(int i=0; i<5; i++) IT[i] = IT[i] / root.size();
}

int main(int argc, char* argv[]) {
//    location = str(argv[1]);
//    start_inst = stoi(argv[2]);
//    inst_count = stoi(argv[3]);
//    W = stoi(argv[4]);
    
    location = "/Users/shengyujia/学习/计算机/2022春季/CMPT 305/Assignment/Proj2 Group/Proj2-Group/compute_fp_1";
    start_inst = 10000;
    inst_count = 1000000;
    W = 1;
    
    T = 0;
    for(int i=0; i<5; i++) IT[i] = 0;
    
    pipeline PPL;
    
    bool ifEnd = PPL.simulation();
    
    while(ifEnd) {
        ifEnd = PPL.simulation();
        T++;
    }
    
    PPL.~pipeline();
    
    caculateIT();
    
    cout <<T <<endl;
}
