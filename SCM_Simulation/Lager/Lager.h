//
// Created by Cheryl on 29.11.2021.
//

#include "utils.h"
#include <iostream>
#include <fstream>
#include <mutex>

#ifndef LAGER_LAGER_H
#define LAGER_LAGER_H

using namespace std;

class Lager {

public:
    Lager(mutex * mtx1, mutex * mtx2){

this->mtx = mtx1;
this->mtx2 = mtx2;

char* initialStock = getenv ("INITIALSTOCK");
        if (initialStock==nullptr) {
            printf ("No initial stock set");
            throw runtime_error ("Lager error: stock missing");
        }

        int initial = atoi(initialStock);
        schuhe = initial; hose = initial;jeans = initial;
        shirt = initial; pullover = initial; hemd = initial;
        kleid = initial; rock = initial; jacke = initial; mantel = initial;

        saveStock("");
    };
    ~Lager(){delete mtx;
    delete mtx2;}

    /**
     * stock for a certain product is returned
     * @param product
     * @return
     */
    int countStock(string product);
    /**
     * stock for certain product is changed (added or subtracted)
     * @param product
     * @param amount
     */
    void changeStock(string product, int amount);
    /**
     * amount of stock is saved in logfile
     */
    void saveStock(string timestamp);

    int getProductID(string product);

    void saveOverview();

    int schuhe = 0, hose = 0, jeans = 0, shirt = 0, pullover = 0, hemd = 0, kleid = 0, rock = 0, jacke = 0, mantel = 0;
    int in = 0, out = 0;

mutex * mtx;
mutex * mtx2;
string timestamp;

};


#endif //LAGER_LAGER_H
