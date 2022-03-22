//
// Created by Cheryl on 19.06.2021.
//

#ifndef PRAKTIKUM_3_SIMULATION_H
#define PRAKTIKUM_3_SIMULATION_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Components.h"
#include <list>

using namespace std;
#define NUMBEROFSTEPS 10

class Simulation {
public:

    Simulation(int addressSize, int pageSize, int physicalSize, int addrGeneratorMode, int pageAlgorithm) {
        this->addressSize = addressSize;
        this->pageSize = pageSize;
        this->physicalSize = physicalSize;
        this->addrGeneratorMode = addrGeneratorMode;
        this->pageAlgorithm = pageAlgorithm;
        addrNum = pow(2, addressSize);

        pageNumber = addrNum/ pageSize;    //Virtual Memory
        pageFrameNumber = physicalSize / pageSize;         //Physical Memory
    }

    ~Simulation(){
        delete [] ram;
    };

    void setup();

    void cpu();

    int mmu(int address);

    void os(int page);

    int addressGenerator(int mode);

    void report();

    void read(int address);

    void write(int address);

    void changeProcess();

    bool pagePresent(int page);

    void replacePageFrame(int page);

    void run();

    void addProcess(char id);

    void putPageInFreePageFrame(int page);

    void fifoAlg(int page);

    void secondChanceAlg(int page);

    PageFrame* pageToPageFrame(Page *p);
    bool pageFrameFree();

private:
    int addressSize, pageSize, pageNumber, physicalSize, pageFrameNumber, addrNum = 0, pageFaults = 0, addrGeneratorMode, lastAddr = 0, pageAlgorithm;
    vector<vector<Page*>> hd;
    vector<Process *> processes;
    Process *activeProcess = NULL;
    int physical_addr;


    vector<PageFrame*> framePagesInUse; //RAM
    vector<PageFrame*> freePageFrames; //RAM
    char* ram;



};

void Simulation::setup() {

    addProcess('A');
    addProcess('B');

    //ALLOCATE MEMORY FOR RAM
    this->ram = new char[physicalSize];
    for(int i = 0; i < physicalSize; i++){
        ram[i] = NULL;
    }

  for(int i = 0; i < pageFrameNumber; i++){

      PageFrame *pf = new PageFrame(i, pageSize);
      freePageFrames.push_back(pf);

  }

  activeProcess = processes[0];
    run();

}

void Simulation::run() {

    int counter = 0;

    while(counter < NUMBEROFSTEPS){

        //CPU PICKS A STEP AND EXECUTES IT
        cpu();
        counter++;
    }
    report();
}

void Simulation::addProcess(char id) {

    Process *p = new Process(id, processes.size(), pageNumber, pageSize);
    processes.push_back(p);

    hd.push_back(p->pages);

}

void Simulation::cpu() {

    srand(time(NULL));
    int step = 0;
    //CPU WORKS WITH VIRTUAL/LOGICAL ADDRESSES
    int address = 0;

    //DETERMINE STEP STOCHASTICALLY
    //READ has 1/2 chance, WRITE has 1/3 chance, Process Change has 1/6 chance
    int steps[] = {1, 1, 1, 2, 2, 3};

    step = steps[rand() % 6];

    switch (step) {

        //1 = READ, 2 = WRITE, 3 = PROCESS CHANGE
        case 1:
            printf("Read \n");
            address = addressGenerator(addrGeneratorMode);
            read(address);
            break;
        case 2:
            printf("Write \n");
            address = addressGenerator(addrGeneratorMode);
            write(address);
            break;
        case 3:
            printf("Change Process \n");
            changeProcess();
            break;
        default:
            break;

    }

}

void Simulation::read(int address) {

    int physical_addr;
    physical_addr = mmu(address);
    int value = ram[physical_addr];

    cout << "READ \n Virtual address: " << address << "\n Physical address: " << physical_addr << "\n Value: " << value
         << endl;

}

void Simulation::write(int address) {

    int physical_addr;
    physical_addr = mmu(address);
    ram[physical_addr] = activeProcess->processID;

}

void Simulation::changeProcess() {

    srand(time(NULL));
    int newID;

    newID = rand() % processes.size();

    activeProcess = processes[newID];
    printf("Now running process %i \n", newID);

}

int Simulation::mmu(int address) {

    //takes virtual addresses and translates to physical address
    //checks if address page is in RAM (page frame is NULL in physical memory)
    //if not in physical memory throw page fault
    //enter page into free page frame from hard drive
    int page = address / pageSize; //calculate page
    int offset = address % pageSize;
    if (!pagePresent(page)) { //is page in ram rn (in pageframes in use)

        //PAGE FAULT!!
        printf("Page fault triggered. Address %i is not allowed to be accessed.", address);
        pageFaults++;
        os(page);

    }
    activeProcess->pages[page]->referenceBit = true;
    physical_addr = activeProcess->pages[page]->pageframe->frameID * pageSize + offset;

    return physical_addr;


}

void Simulation::os(int page){

    //CHECK IF ALL FREE PAGE FRAMES ARE NULL
    if (freePageFrames.empty()) {
        replacePageFrame(page);
    }else{
        putPageInFreePageFrame(page);
    }

}

bool Simulation::pageFrameFree(){

    /*for(int i = 0; i < pageFrameNumber; i++) {
        //IF SPOT IS NULL THE PAGE FRAME THERE IS CURRENTLY OCCUPIED
        if (freePageFrames[i] != NULL) {
            return true;
        }
    }
    return false;
     */


}

void Simulation::replacePageFrame(int page){

    //PAGE REPLACEMENT ALGORITHM
    //FIFO = 0, Second Chance = 1
    switch (pageAlgorithm) {
        case 0:
            fifoAlg(page);
            break;
        case 1:
            secondChanceAlg(page);
            break;
        default:
            break;
    }


}

void Simulation::fifoAlg(int page) {

    Page *pg = hd[activeProcess->processNumber][page];
    if(pg->pageID != page)
    { printf("Wrong page, fix! \n"); return;}

    freePageFrames.push_back(framePagesInUse.front());
    framePagesInUse.erase(framePagesInUse.begin());

}

void Simulation::secondChanceAlg(int page) {

}

PageFrame* Simulation::pageToPageFrame(Page *pg) {

    PageFrame *pf = new PageFrame(framePagesInUse.size(), pageSize);
    pf->copyStorage(pg->storage);

    return pf;

}

void Simulation::putPageInFreePageFrame(int page) {

    //Load page from Hard Drive
    Page *pg = hd[activeProcess->processNumber][page-1];
    if(pg->pageID != page)
    { printf("Wrong page, fix! \n"); return;}

    //TAKE PAGEFRAME THAT S FREE; USUALLY YOU TAKE THE ONE WITH SMALLEST ID
    for(int i= 0; freePageFrames.size(); i++){
        if(freePageFrames[i] != NULL){
            //go from not in use to in used
            framePagesInUse.push_back(freePageFrames[i]);
            freePageFrames.erase(freePageFrames.begin() + i);
            break;
        }
    }

    //set bit
    pg->presentBit = true;

}

bool Simulation::pagePresent(int page) {

    return activeProcess->pages[page-1]->presentBit;

}

int Simulation::addressGenerator(int mode) {

    switch (mode) {

        //0 - completely random, 1-bubble sort
        case 0:
            lastAddr = rand() % addrNum;
            break;
        case 1:
            if (lastAddr < addrNum) {
                lastAddr++;
            } else {
                lastAddr = 0;
            }
            break;
        default:
            break;
    }

    return lastAddr;


}

void Simulation::report() {

    printf("This is a report of the storage management simulation \n");
    printf("The simulation had the following configuration \n");
    printf("Address size: %i \n", addressSize);
    printf("Page size: %i \n", pageSize);
    printf("RAM size: %i \n", physicalSize);
    printf("The number of page faults was %i \n", pageFaults);


}



#endif //PRAKTIKUM_3_SIMULATION_H
