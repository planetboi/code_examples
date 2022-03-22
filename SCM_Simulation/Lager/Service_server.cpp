// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Service.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::Service;
using namespace std;

class ServiceHandler : virtual public ServiceIf {
 public:
  ServiceHandler() {
    // Your initialization goes here
  }

  void seeProducts(std::vector<Product> & _return, const std::string& warehouse) {
    // Your implementation goes here
    //printf("seeProducts\n");

      ifstream logfile;

      logfile.open("stock.txt");
      if (!logfile) {
          cerr << "Error: file could not be opened" << endl;
      } else {
          string line;
          int i = 1;
          while (!logfile.eof()) {
              getline(logfile, line);
bool isNumber = true;
if(line.size() == 0){
    isNumber = false;
}

        if(isNumber){
             
              //read data from text file into vector

string name ="";

    switch (i) {
        case 1:
            name = "Schuhe";
            break;
        case 2:
            name = "Hose";
            break;
        case 3:
            name = "Jeans";
            break;
        case 4:
            name = "Shirt";
            break;
        case 5:
            name = "Pullover";
            break;
        case 6:
            name = "Hemd";
            break;
        case 7:
            name = "Kleid";
            break;
        case 8:
            name = "Rock";
            break;
        case 9:
            name = "Jacke";
            break;
        case 10:
            name = "Mantel";
            break;
        default:
            name ="";
            break;
    }
              Product p;
              p.__set_name(name);
              p.__set_id(i);
              p.__set_amount(stoi(line));
             // cout << "Amount : " << line << endl;
              _return.push_back(p);
              i++;
          }
          }
          logfile.close();
      }

  }

  


  void orderProduct(std::string& _return, const std::string& product, const int32_t amount, const std::string& warehouse)
  {
    // Your implementation goes here

   // printf("orderProduct\n");

    ifstream logfile;
    ofstream overwrite;
    string line;
    vector<string> lineVec;
    int productID = 0;

    //assigning productIDs to product (name)
    if (product == "Schuhe")
    {
        productID = 1;
    }
    else if (product == "Hose")
    {
        productID = 2;
    }
    else if (product == "Jeans")
    {
        productID = 3;
    }
    else if (product == "Shirt")
    {
        productID = 4;
    }
    else if (product == "Pullover")
    {
        productID = 5;
    }
    else if (product == "Hemd")
    {
        productID = 6;
    }
    else if (product == "Kleid")
    {
        productID = 7;
    }
    else if (product == "Rock")
    {
        productID = 8;
    }
    else if (product == "Jacke")
    {
        productID = 9;
    }
    else if (product == "Mantel")
    {
        productID = 10;
    }

    bool stockChanged;


/*overview.txt
Timestamp:
Einlieferungen:
Auslieferungen:
Schuhe: 
Hose:
Jeans:
Shirt:
Pullover:
Hemd:
Kleid:
Rock:
Jacke:
Mantel: 
*/

    logfile.open("overview.txt");      //open file for reading

    if (!logfile)
    {
        cerr << "Error: file could not be opened for reading" << endl;
    }

    else
    {
        //cout << endl << "file is open" << endl;
        while (!logfile.eof())
        {
            getline(logfile, line);     //read data from text file into line
            lineVec.push_back(line);    //write into vector
        }

        for (size_t i = 3; i < lineVec.size(); i++)                //go through vector to find correct line
        {
            if (static_cast<int>(i) == productID)                               //if line with the wanted product is found
            {
                if(stoi(lineVec.at(i)) < amount)
                {
                    stringstream msg;
                    msg << "Error: available amount of " << product << " is too little in " << warehouse << endl
                         << "Needed amount: " << amount << endl
                         << "Available amount: " << lineVec.at(i) << endl;
                         cerr << msg.str();
                         _return = msg.str();
                    break;
                }

                else
                {
                  int calc = stoi(lineVec.at(i)) - amount;        //subtract needed amount from available amount
                  string newLine = to_string(calc);               //turn result into string for .insert()

                  /*
                  cout << "TESTAUSGABE: " << stoi(lineVec.at(i)) << " - " << amount << " = " << calc << endl;

                  if(calc == stoi(newLine))
                  {
                      cout << "CALC TEST SUCCESSFUL" << endl << endl;
                  }
                  else
                  {
                      cout << "ERROR: CALC TEST UNSUCCESSFUL" << endl << endl;
                  }
                  */

                  lineVec.erase(lineVec.begin() + i);             //erase original line
                  lineVec.insert(lineVec.begin() + i, newLine);   //write new result into line
                  
                  

                  //INCREASE NUMBER OF AUSLIEFERUNG
                  int newOut = stoi(lineVec.at(2)) + 1;
                  string newOutStr = to_string(newOut);
                  lineVec.erase(lineVec.begin() + 2);               //erase original line
                  lineVec.insert(lineVec.begin() + 2, newLine);
            
                  stringstream msg;
                  msg << "Order was placed. " << amount << " pieces of " << product << " have been ordered to " << warehouse << endl;
                  _return = msg.str();
                }
            }
        }

        overwrite.open("stock.txt");    //open file for writing

        if (!overwrite)
        {
            cerr << "Error: file could not be opened for writing" << endl;
        }

        else
        {
            //cout << endl << "file is open" << endl;

            for (size_t i = 3; i < lineVec.size(); i++)
            {
                overwrite << lineVec.at(i) << endl;     //write content of vector into file
            }
            stockChanged = true;
        }
    }
    logfile.close();
    overwrite.close();

    if(stockChanged){
        logfile.open("overview.txt");      //open file for reading

    if (!logfile)
    {
        cerr << "Error: file could not be opened for reading" << endl;
    }
    else{

for (size_t i = 0; i < lineVec.size(); i++)
            {
            
                overwrite << lineVec.at(i) << endl;     //write content of vector into file
            }

    }
    }
  }

};