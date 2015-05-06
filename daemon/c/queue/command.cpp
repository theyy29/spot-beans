#include "command.h"

#include <iostream>

Command::Command(std::string command){
    verbatim = command;
}

Command::~Command(){
}

std::string Command::DBGECHO(){
    std::cout << verbatim;
    return verbatim;
}
