//
//  record.hpp
//  Proj2-Group
//
//  Created by Shengyu Jia on 2022-04-15.
//
// record is avl tree that used to store all of instructions that finished 

#ifndef record_hpp
#define record_hpp

#include "necessaryHeader.hpp"

class record {
private:
    string s;
    record* left; // Left child
    record* right; // Right child
    
    record(); // Default constructor
    ~record(); // Deconstructor
    
    // Rotations
    void LL();
    void LR();
    void RR();
    void RL();
    
    void balance();
    
    // Help functions
    int height(record* r); // Return node r's height. Must ensure r in the tree. If not return -1
    
public:
    void insert(string key); // Insert "key" into tree
    
    bool find(string key); // Return if "key" found in the tree
    
    int nodesNumber(); // Return how many nodes in the tree
};

#endif /* record_hpp */
