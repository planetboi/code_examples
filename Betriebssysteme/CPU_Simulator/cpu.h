//
// Created by Cheryl on 06.06.2021.
//

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include "process.h"

#ifndef PRAKTIKUM_2_CPU_H
#define PRAKTIKUM_2_CPU_H

#define INIT "init"
#define QUANTUM 5

class CPU {
public:
    explicit CPU(bool roundRobin=false, bool saveFP=true) {
        this->roundRobin = roundRobin;
        this->saveFP = saveFP;
    };

    ~CPU() = default;
    //User commands
    /**
     * simulates n tacts
     */
    void step(int n = 1);

    /**
     * oldest process in blocked process queue is executed
     */
    void unblock();

    /**
     * display current system state
     */
    void dump();

    /**
     * show avg simulated runtime of each process and end system
     */
    void quit();
    // CPU commands
    /**
     * set integerRegister to n
     */
    void load(int n);

    /**
     * add n to integerRegister
     */
    void add(int n);

    /**
     * subtract n from integerRegister
     */
    void subtract(int n);

    /**
     * block this simulated process
     */
    void block();

    /**
     * create a new simulated process, which executes program from file
     * file is read and content save in instructionMem
     */
    void X(const string& fileName);

    void Z();

    void run();

    //Process *getOldestProcess(){};

    /*bool compareProcess(const Process &p1, const Process &p2) {
        return p1.getStartTime() < p2.getStartTime();
    };
     */

    void runProcess(Process *p);

    void swapProcess(Process *p);

    void executeInstruction(string programLine);

    void waitForUser();

    void scheduler();

private:
    int integerRegister = 0, programCounter = 0, ticks = 0, processAmount = 0, cpuClock = 0;
    std::list<Process *> blockedProcesses, readyProcesses, finishedProcesses;
    Process *runningProcess = nullptr;
    string currentInstruction;
    bool roundRobin, noProcesses = false, saveFP;

    void stopRunningProcess();

    void blockRunningProcess();

    void runProcessRR(Process *pProcess);
};

void CPU::step(int n) {
    ticks = n;
    scheduler();
}

void CPU::unblock() {
    if (blockedProcesses.empty()) {
        cout << "No blocked processes currently" << endl;
    } else {
        //OLDEST FUNCTION ADDED TO READY LIST / UNBLOCKED
        blockedProcesses.front()->setBlocked(false);
        printf("Unblocked process %i \n", blockedProcesses.front()->getID());
        readyProcesses.push_back(blockedProcesses.front());
        blockedProcesses.pop_front();
        currentInstruction = "Unblock";
        stopRunningProcess();
    }
}


void CPU::dump() {
    printf("****************************************************************\n"
           "The current system state is as follows:\n"
           "**************************************************************** \n");
    printf("CURRENT TIME: %i \n", cpuClock);
    printf("RUNNING PROCESS: \n");
    printf("pid\tppid\tvalue\tstart time\tquantum\tname \tCPU time used so far \n");
    if (runningProcess != nullptr) {
        std::cout << *runningProcess;
    }
    printf("BLOCKED PROCESSES: \n");
    printf("pid\tppid\tvalue\tstart time\tquantum\tname \tCPU time used so far \n");
    if (!blockedProcesses.empty()) {
        for (Process *p:blockedProcesses) {
            std::cout << *p;
        }
    }
    printf("PROCESSES READY TO EXECUTE: \n");
    printf("pid\tppid\tvalue\tstart time\tquantum\tname \tCPU time used so far \n");
    if (!readyProcesses.empty()) {
        for (Process *p:readyProcesses) {
            std::cout << *p;
        }
    }
    if(saveFP){
        printf("FINISHED PROCESSES: \n");
        printf("pid\tppid\tvalue\tstart time\tquantum\tname \t Total CPU time used \n");
        if (!finishedProcesses.empty()) {
            for (Process *p:finishedProcesses) {
                std::cout << *p;
            }
        }
    }
    printf("**************************************************************** \n");
}

void CPU::quit() {

    string input;
    int avgRuntime;
    if(ticks > 0){ cpuClock += ticks; }
    avgRuntime = cpuClock / processAmount;
    if(saveFP){dump();}
    std::cout << " The average runtime is: " << avgRuntime << " ticks per process. :)" << endl;
    printf("Press enter to quit \n");
    getline(cin, input);
    exit(0);
}

void CPU::load(int n) {
    integerRegister = n;
}

void CPU::add(int n) {
    integerRegister += n;
}

void CPU::subtract(int n) {
    integerRegister -= n;
}

void CPU::block() {
    if (runningProcess != nullptr) {
        blockRunningProcess();
    }
    //waitForUser();
}

void CPU::blockRunningProcess() {
    runningProcess->integerRegister = integerRegister;
    runningProcess->programCounter = programCounter;
    runningProcess->setRunning(false);
    runningProcess->setBlocked(true);
    if(roundRobin){runningProcess->quantum = QUANTUM;}
    blockedProcesses.push_back(runningProcess);
    runningProcess = nullptr;
    printf("Running process has been blocked because of ");
    cout << currentInstruction << endl;
}

void CPU::stopRunningProcess() {
    if (runningProcess != nullptr) {
        runningProcess->integerRegister = integerRegister;
        runningProcess->programCounter = programCounter;
        runningProcess->setRunning(false);
        if(roundRobin){runningProcess->quantum = QUANTUM;}
        printf("Running process %i stopped due to ", runningProcess->getID());
        cout << currentInstruction << endl;
        readyProcesses.push_back(runningProcess);
        runningProcess = nullptr;
    }
}

void CPU::scheduler() {

    if (runningProcess == nullptr) {
        //if no running process, take oldest ready process
        if (readyProcesses.empty()) {
            if (blockedProcesses.empty()) {
                printf("No processes left \n");
                noProcesses = true;
            } else {
                printf("No processes ready. Please unblock process to continue. \n");
                cpuClock += ticks;
                ticks = 0;
            }
        } else {
            swapProcess(readyProcesses.front());
        }
    } else {
        if (roundRobin) {
            if (runningProcess->quantum == 0) {
                stopRunningProcess();
                swapProcess(readyProcesses.front());
            } else {
                runProcessRR(runningProcess);
            }
        } else {
            runProcess(runningProcess);
        }
    }
}

void CPU::swapProcess(Process *p) {
    runningProcess = p;
    readyProcesses.pop_front();
    if(roundRobin){
        runProcessRR(p);
    }else{
        runProcess(p);
    }

}

void CPU::X(const string& fileName) {
    auto *p = new Process(fileName, cpuClock, processAmount);
    printf("New process created: ");
    cout << fileName << endl;
    processAmount++;
    //ticks++;
    if(roundRobin){p->quantum = QUANTUM;}
    readyProcesses.push_back(p);
    if (runningProcess != nullptr) {
        p->parentID = runningProcess->getID();
        stopRunningProcess();
    }
}

void CPU::Z() {
    printf("Process end reached for: \n");
    printf("pid\tppid\tvalue\tstart time\tquantum\tname \tTotal CPU time used\n");
    cout << *runningProcess;
    if(saveFP){
        finishedProcesses.push_back(runningProcess);
    }
    else{
        delete runningProcess;
    }
    runningProcess = nullptr;
}

void CPU::runProcessRR(Process *p) {
    //do scheduling

    //set up CPU registers and statuses;
    if (!p->getRunning()) {
        if (!p->getBlocked()) {
            p->setRunning(true);
            p->quantum = QUANTUM;
            programCounter = p->programCounter;
            integerRegister = p->integerRegister;
            printf("New process %i running \n", p->getID());
        } else { printf("Can't run blocked process %i \n", p->getID()); }
    }

    int pc = p->programCounter;
    int len = p->getProgramSize();

    for (int i = pc; i < len; i++) {
        //fetch
        if (ticks > 0) {

            if (runningProcess == nullptr) { break; }
            programCounter++;
            cpuClock++;
            ticks--;
            currentInstruction = p->readInstruction(i);
            //execution
            p->quantum--;
            executeInstruction(currentInstruction);
            printf("Current instruction: ");
            cout << currentInstruction << endl;
            // cout << "IR " << integerRegister << endl;

            if (p->quantum < 1) {
             //   printf("Quantum ran out \n");
                currentInstruction = "Quantum ran out";
              //  dump();
                break;

            }
        } else {
            break;
        }
    }
    if (ticks > 0) {
        scheduler();
    }
    waitForUser();

}

void CPU::runProcess(Process *p) {
    //do scheduling

    //set up CPU registers and statuses;
    if (!p->getRunning()) {
        if (!p->getBlocked()) {
            p->setRunning(true);
            programCounter = p->programCounter;
            integerRegister = p->integerRegister;
            printf("New process %i running \n", p->getID());
        } else { printf("Can't run blocked process %i \n", p->getID()); }
    }

    int pc = p->programCounter;
    int len = p->getProgramSize();

    for (int i = pc; i < len; i++) {
        //fetch
        if (ticks > 0) {

            if (runningProcess == nullptr) { break; }
            programCounter++;
            cpuClock++;
            ticks--;
            currentInstruction = p->readInstruction(i);
            //execution
            executeInstruction(currentInstruction);
            printf("Current instruction: ");
            cout << currentInstruction << endl;
            // cout << "IR " << integerRegister << endl;

        } else {
            break;
        }
    }
    if (ticks > 0) {
        scheduler();
    }
    waitForUser();

}

void CPU::executeInstruction(string programLine) {
    char instruction;
    string valueStr;

    instruction = programLine[0];

    size_t pos = programLine.find(' ');
    valueStr = programLine.substr(pos + 1);

    switch (instruction) {
        case 'L':
            load(stoi(valueStr));
            break;
        case 'A':
            add(stoi(valueStr));
            break;
        case 'S':
            subtract(stoi(valueStr));
            break;
        case 'X':
            X(valueStr);
            break;
        case 'B':
            block();
            break;
        case 'Z':
            Z();
            break;
        default:
            printf("The instruction %i does not exist.", instruction);
            break;
    }


    if (runningProcess != nullptr) {
        runningProcess->integerRegister = integerRegister;

    }

}

void CPU::waitForUser() {

    string input;

    if (noProcesses) {
        printf("There are no more processes to execute. \n");
        quit();
    } else {

        printf("**************************************************************** \n");
        printf("Please enter command to continue simulation... \n");
        printf("S n/Step n: \t simulate n ticks \n");
        printf("U/Unblock: \t unblock first process \n");
        printf("D/Dump: \t system status \n");
        printf("Q/Quit: \t show average runtime per process and end system \n");
        printf("**************************************************************** \n");

        while (!noProcesses) {

            input = "";
            printf("$");
            getline(cin, input);

            if ((input[0] == 'S') || ((input.find("Step", 0)) != string::npos)) { // sets value n to

                int value = 1;
                if (input.length() > 2) {
                    size_t pos = input.find(' ');
                    string valueStr = input.substr(pos);
                    value = stoi(valueStr);
                }

                //cout << " Step command " << endl;
                step(value);
                // return;

            } else if ((input[0] == 'U') ||
                       ((input.find("Unblock", 0)) != string::npos)) { // adds value n to integerregister

                // cout << " Unblock command " << endl;
                unblock();

            } else if ((input[0] == 'D') ||
                       ((input.find("Dump", 0)) != string::npos)) { // subtracts value n from intergerregsiter

                // cout << " Dump command " << endl;
                dump();

            } else if ((input[0] == 'Q') || ((input.find("Quit", 0)) != string::npos)) { // blocks simulative process
                // cout << " Quit command " << endl;
                quit();
            } else {
                cout << "Unknown command" << endl;
            }
        }
    }

}

void CPU::run() {

    printf("Welcome to the CPU simulator :) \n");
    X(INIT);
    waitForUser();
}

#endif //PRAKTIKUM_2_CPU_H
