//
// Created by Cheryl on 29.11.2021.
//

#include "Lager.h"

int Lager::getProductID(string product){
    int id;
    if(product == "Schuhe"){
        id = 1;
    }
    else if(product == "Hose"){
        id = 2;
    }
    else if(product == "Jeans"){
        id = 3;
    }
    else if(product == "Shirt"){
        id = 4;
    }
    else if(product == "Pullover"){
        id = 5;
    }
    else if(product == "Hemd"){
        id = 6;
    }
    else if(product == "Kleid"){
        id = 7;
    }
    else if(product == "Rock"){
        id = 8;
    }
    else if(product == "Jacke"){
        id = 9;
    }
    else if(product == "Mantel"){
        id = 10;
    }

    return id;
}

int Lager::countStock(string product){

    int id;
    id = getProductID(product);

    switch (id) {
        case 1:
            return schuhe;
        case 2:
            return hose;
        case 3:
            return jeans;
        case 4:
            return shirt;
        case 5:
            return pullover;
        case 6:
            return hemd;
        case 7:
            return kleid;
        case 8:
            return rock;
        case 9:
            return jacke;
        case 10:
            return mantel;
        default:
            return -1;
    }

}

void Lager::changeStock(string product, int amount){

    int * initial;
    int id;

    id = getProductID(product);

    switch (id) {
        case 1:
            initial = &schuhe;
            break;
        case 2:
            initial = &hose;
            break;
        case 3:
            initial = &jeans;
            break;
        case 4:
            initial = &shirt;
            break;
        case 5:
            initial = &pullover;
            break;
        case 6:
            initial = &hemd;
            break;
        case 7:
            initial = &kleid;
            break;
        case 8:
            initial = &rock;
            break;
        case 9:
            initial = &jacke;
            break;
        case 10:
            initial = &mantel;
            break;
        default:
            break;
    }
    //check if stock is being increased or decreased
    if(amount < 0){
        if(((*initial) + amount) < 0){
            cout << "Not enough stock. Current stock is " << *initial << " and amount to be delivered is " << amount << endl;
            return;
        }
    }
    //cout << endl << "TESTAUSGABE - " << product << ": " << (*initial) << " + " << amount << " = ";

    int test = *initial;
    (*initial) += amount;

    /*
    cout << (*initial) << endl;

    
    if(*initial == (test+amount))
    {
        cout << "CALC TEST SUCCESSFUL" << endl << endl;
    }

    else
    {
        cout << "ERROR: CALC TEST UNSUCCESSFUL" << endl << endl;
    }
    */

}

void Lager::saveStock(string timestamp){

    int stock[10] = {schuhe, hose, jeans, shirt, pullover, hemd, kleid, rock, jacke, mantel};
    int i = 0;

    ofstream logfile;
    this->mtx->lock();
    logfile.open("stock.txt", ios::trunc);
    if (logfile.is_open()) {
      //  logfile << timestamp;
        while(i < 10){
            logfile << stock[i] << "\n";
            i++;
        }
        logfile.close();
    } else {
        cout << "Unable to open stock file" << endl;
    }
    this->mtx->unlock();

}

void Lager::saveOverview(){

    int stock[12] = {in, out, schuhe, hose, jeans, shirt, pullover, hemd, kleid, rock, jacke, mantel};
    int i = 0;

    ofstream logfile;
    this->mtx2->lock();
    logfile.open("overview.txt", ios::trunc);
    if (logfile.is_open()) {
        logfile << this->timestamp;
        while(i < 12){
            logfile << stock[i] << "\n";
            i++;
        }
        logfile.close();
    } else {
        cout << "Unable to open overview file" << endl;
    }
    this->mtx2->unlock();
}