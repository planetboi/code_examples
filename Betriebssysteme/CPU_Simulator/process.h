//
// Created by Cheryl on 06.06.2021.
//
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#ifndef PRAKTIKUM_2_PROCESS_H
#define PRAKTIKUM_2_PROCESS_H

class Process{
public:
    Process(string filename, int startTime, int processID){
        readFile(filename);
        this->processID = processID;
        this->parentID = parentID;
        this->startTime = startTime;
        this->filename = filename;
    }
    ~Process(){};
    int getStartTime() const;
    friend std::ostream& operator<<(std::ostream &out, const Process &p);
    void setBlocked(bool b);
    void readFile(string filename);
    bool getBlocked();
    bool getRunning();
    string readInstruction(int n);
    int getProgramSize();
    void setRunning(bool b);
    int activeCycles = 0, parentID = 0, programCounter = 0, integerRegister = 0, quantum = -1;
    int getID();

private:
    bool running = false, blocked = false;
int processID, startTime;
    vector<string> instructionMem; string filename;

};

int Process::getStartTime() const{
    return startTime;
}

std::ostream& operator<<(std::ostream &out, const Process &p){
    out << p.processID << '\t' << p.parentID << '\t' << p.integerRegister << '\t' << p.startTime << '\t' << '\t' << p.quantum << '\t' << p.filename << '\t'  << p.activeCycles  << '\n';

}

int Process::getID(){
    return processID;
}

void Process::setBlocked(bool value){
    blocked = value;
}

void Process::setRunning(bool value){
    running = value;
}

void Process::readFile(string filename){

    ifstream input(filename);
    char c;
    string line;

    if (!input.is_open()) {
        std::cout << "Datei konnte nicht geoeffnet werden." << std::endl;
    } else {

       while (std::getline(input,line, '\r')){
          if((c=input.get())!='\n') {input.rdbuf()->sputbackc(c);}
          //cout << line << endl;
           instructionMem.push_back(line);
        }


        input.close();

    }



}

bool Process::getBlocked() {
    return blocked;
}

bool Process::getRunning() {
    return running;
}

string Process::readInstruction(int n) {
    programCounter++;
    activeCycles++;
    return instructionMem[n];
}

int Process::getProgramSize() {
    return instructionMem.size();
}

#endif //PRAKTIKUM_2_PROCESS_H
