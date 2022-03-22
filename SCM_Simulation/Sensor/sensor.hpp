#include <iostream>
#include <cstdio>      //for printf
#include <cstdlib>     //for srand(), rand()
#include <unistd.h>
#include <cstring>     //for memset()
#include <vector>
#include <sys/types.h>
#include <sys/socket.h> //for socket(), bind(), sendto(), close()
#include <arpa/inet.h>  //for htons()
#include <netinet/in.h>
#include <ctime>  

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

//====================CLASS PRODUKT====================
class Produkt
{
public:
    Produkt();
    Produkt(string n, double p, int i)
    {
        name = n;
        price = p;
        id = i;
    }
    int getID();
    string getName();

private:
    string name;
    double price;
    int id;
};

void fillKatalog(std::vector<Produkt> &katalog);     //Vector mit Produkten befüllen

int Produkt::getID() {
    return this->id;
}

string Produkt::getName() {
    return this->name;
}

string scanProduct()
{
    int randNum = rand() % 10;

   // cout << "Random product is: " << randNum << endl;

    vector<Produkt> katalog;
    fillKatalog(katalog);

    //return to_string(katalog.at(randNum).getID());
    return katalog.at(randNum).getName();
}

string setScannerType(){
    string scannerTypes[3] = {"Einlieferung", "Auslieferung", "Inventarisierung"};
    int randType = rand() % 3;

   // cout << "Random type is: " << randType << endl;

    return scannerTypes[randType];

}


void fillKatalog(std::vector<Produkt> &katalog) {
    Produkt schuhe("Schuhe", 50.99, 1);
    Produkt hose("Hose", 35.0, 2);
    Produkt jeans("Jeans", 25.75, 3);
    Produkt shirt("Shirt", 15.0, 4);
    Produkt pullover("Pullover", 40.0, 5);
    Produkt hemd("Hemd", 60.50, 6);
    Produkt kleid("Kleid", 70.0, 7);
    Produkt rock("Rock", 20.45, 8);
    Produkt jacke("Jacke", 100.0, 9);
    Produkt mantel("Mantel", 150.50, 10);

    for (int i = 1; i < 11; i++) {
        switch (i) {
            case 1:
                katalog.push_back(schuhe);
                break;
            case 2:
                katalog.push_back(hose);
                break;
            case 3:
                katalog.push_back(jeans);
                break;
            case 4:
                katalog.push_back(shirt);
                break;
            case 5:
                katalog.push_back(pullover);
                break;
            case 6:
                katalog.push_back(hemd);
                break;
            case 7:
                katalog.push_back(kleid);
                break;
            case 8:
                katalog.push_back(rock);
                break;
            case 9:
                katalog.push_back(jacke);
                break;
            case 10:
                katalog.push_back(mantel);
                break;
        }

    }
}