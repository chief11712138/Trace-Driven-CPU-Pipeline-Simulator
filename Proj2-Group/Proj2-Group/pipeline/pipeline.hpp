//
//  pipeline.hpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//

#ifndef pipeline_hpp
#define pipeline_hpp

#include "necessaryHeader.hpp"
#include "instructions.hpp"

class pipeline {
private:
    instructions* waiting;
    instructions* IF;
    instructions* ID;
    instructions* EX;
    instructions* MEM;
    instructions* WB;
    
    void S_IF(); // Simulate IF stage
    void S_ID(); // Simulate ID stage
    void S_EX(); // Simulate EX stage
    void S_MEM(); // Simulate MEM stage
    void S_WB(); // Simulate WB stage
    
    // Help functions
    void initialize(); // Initialize all the global variable
    
public:
    pipeline(); // Default constructor
    ~pipeline(); // Deconstructor
    
    bool simulation(); // Simulate system. If end return 0. Otherwise, return 1
};

#endif /* pipeline_hpp */
