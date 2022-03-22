//
// Created by tabea on 16.05.21.
//

#include <iostream>
#include "process.h"
#include "cpu.h"

using namespace std;

int main() {

    bool roundRobin = true;
    bool saveFP = true;

    CPU cpu1;
    cpu1.run();

    //CPU cpu2(roundRobin, saveFP);
    //cpu2.run();

}
