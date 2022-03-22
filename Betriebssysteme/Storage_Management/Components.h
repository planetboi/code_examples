//
// Created by Cheryl on 21.06.2021.
//

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <list>

using namespace std;

#ifndef PRAKTIKUM_3_COMPONENTS_H
#define PRAKTIKUM_3_COMPONENTS_H

class PageFrame {
public:
    PageFrame(){}
    PageFrame(int frameID, int pageSize){
        this->frameID = frameID;
        this->pageSize = pageSize;
        storage = new int[pageSize];

        freeStorage();
    }
    void writeToAddress(int address, int value){
        storage[address] = value;
    }
    int readFromAddress(int address){
        if(storage[address] != NULL){
            return storage[address];
        }
        else{
            printf("No value stored at this address \n");
        }
    }
    void copyStorage(int values[]){
        for(int i = 0; i < pageSize; i++){
            storage[i] = values[i];
        }
    }
    void freeStorage(){
        for (int i = 0; i < pageSize; i++){
            storage[i] = NULL;
        }
    }

    void advanceFrame(int newID){
        this->frameID = newID;
        freeStorage();
    }
    ~PageFrame(){
        delete[] storage;
    }
    int *storage, frameID, pageSize;


};

class Page {
public:
    Page(int processID, int pageID, int pageSize){
        this->processID = processID;
        this->pageID = pageID;
        storage = new int[pageSize];
    }
    ~Page(){
        delete[] storage;
    }
    void initialisePage(int pageSize, int value){
        for(int i = 0; i < pageSize; i++){
            storage[i] = value;
        }
    }
    int *storage, pageID, processID;
    //present = true/absent = false
    //rwBit read = 0/write = 1
    //dirtyBit if page is written on -> must be saved in HD
    //referenceBit if page is accessed by read or write
    bool presentBit = false, rwBit, dirtyBit, referenceBit;
    //page is virtual, pageFrame is physical
    int pageFault;
    PageFrame* pageframe = NULL;
};

class Process{

public:
    Process(char id, int processNumber, int pageNumber, int pageSize){
        this->processID = id;
        this->processNumber = processNumber;

        for(int i = 0; i < pageNumber; i++){
            Page *pg = new Page(id, i, pageSize);
            pages.push_back(pg);
        }

    }
    ~Process() = default;
    void definePageTable(int pageNumber, int pageSize);
    char processID;
    int pageNumber, processNumber;
    vector<Page*> pages;

};

#endif //PRAKTIKUM_3_COMPONENTS_H
