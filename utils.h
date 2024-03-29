#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <ctime>

bool is_a_number(string input){
    bool check_number(string str);

    for (int i = 0; i < input.length(); i++)
        if(!isdigit(input[i]))
            return false;
    
    return true;
}

string string_tolower(string input){
    for(int i=0; i < input.size(); i++)
        input[i] = tolower(input[i]);

    return input;
}

bool exists_file(const char* file_name){
    ifstream ifile;
    ifile.open(file_name);
    
    if(ifile.is_open()){
        ifile.close();
        return true;
    }
    return false;
}

#endif