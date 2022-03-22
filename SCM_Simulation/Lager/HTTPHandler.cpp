//
// Created by Cheryl on 22.11.2021.
//

#include "HTTPHandler.h"

void HTTPHandler::parseHTTPRequest(const char *buffer) {
    string tmp = "";

    cout << "[HTTP Handler] parsing request now....." << endl;

    bool readAgent = false;
    istringstream iss(buffer);
    vector<string> parsed((istream_iterator<string>(iss)), istream_iterator<string>());
    if (parsed[0] == "GET") {
        int i = 2;
        cout << "This is ok" << endl;
        url = parsed[1];
        cout << url << endl;
        while (parsed[i].compare("Accept:") != 0) {
            if (readAgent) {
                userAgent += parsed[i] + " ";
            }
            if (parsed[i].compare("User-Agent:") == 0) {
                readAgent = true;
            }
            //  cout << readAgent << endl;

            i++;
        }
    }

    //cout << userAgent << endl;

    /*for(int i = 0; i < parsed.size(); i++){
        cout << parsed[i] << endl;
    }*/

}

void HTTPHandler::generateHTTPResponse() {

    printf("In Generate HTTP response \n");

    vector<sensorMessage *> data = fetchSensorData();

    string content;
    int errorCode;

    //generate webpage
    string str = generateStatusPage(data);
    //string str = " Sensor information will be displayed here soon... ";
    content = str;
    errorCode = 200;


    // Write the document back to the client
    std::ostringstream oss;
    oss << "HTTP/1.1 " << errorCode << " OK\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-Type: text/html\r\n";
    oss << "Content-Length: " << content.size() << "\r\n";
    oss << "\r\n";
    oss << content;

    std::string output = oss.str();
    cout << output << endl;

    response = output;

}

void HTTPHandler::processURL() {

    char *line = const_cast<char *>(url.c_str());

    char *p = strtok(line, "/");
    while (p != NULL) {
        urlInfo.push_back(p);
        p = strtok(NULL, "/");
    }

}

bool isNumber(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }

    return true;
}

vector<sensorMessage *> HTTPHandler::fetchSensorData() {

    //printf("In Fetch Sensor Data \n");

    /*
     * search with url through sensor history to find the sensor and then retrieve the vector of sensor statuses
     *  //z.B einzeln: /id/1/index /type/inventory/index alle: /all Historie: /id/1 /type/inventory
     */
    processURL();
    bool searchAll = false, searchByID = false, searchByType = false, searchByProduct = false, singleStatus = false;
    int index = 0;
    string filter;


    /**
     * loginfo saves all the information from the logfile in a vector of sensor messages
     * if all information was requested, no further processing is needed
     * if specific information was requested, only the needed information is copied into sensorlist and returned
     */
    vector<sensorMessage *> logInfo, filteredList, singleList;

    //check that url has something in it
    if (!urlInfo.empty()) {
        /**check first word behind slash
         * valid words: all, id, type
        **/
        if (urlInfo[0] == "all") {
            filter = "";
            searchAll = true;
        } else if (urlInfo[0] == "inventory") {
            showInventory = true;
            filter = "";
            cout << "Inventory search" << endl;
            return logInfo;
        } else if (urlInfo[0] == "id" && urlInfo.size() > 1) {
            filter = urlInfo[1];
            searchByID = true;
        } else if (urlInfo[0] == "type" && urlInfo.size() > 1) {
            filter = urlInfo[1];
            searchByType = true;
        } else if (urlInfo[0] == "product" && urlInfo.size() > 1) {
            filter = urlInfo[1];
            searchByProduct = true;
        } else {
            return logInfo;
        }
        //check if single status wanted or multiple
        if (urlInfo.size() > 2) {
            //get index of status
            if (isNumber(urlInfo[2])) {
                //cout << "[HTTP] Single Status being set to true" << endl;
                singleStatus = true;
                index = stoi(urlInfo[2]) - 1;
                //cout << "Index= " << index << endl;
            }
        }
    } else {
        return logInfo;
    }

    ifstream logData;

    logData.open("sensorLog.txt");

    if (!logData) {
        cerr << "Error: file could not be opened" << endl;
    } else {

        string line, time, id, product, type;
        int i = 1;
        cout << " Log file is open " << endl;
        while (!logData.eof()) {
            getline(logData, line);
            //read data from text file into vector
            switch (i % 5) {
                case 0:
                    if (i > 0) {
                        logInfo.push_back(new sensorMessage{id, "", "", type, time, product});
                    }
                    break;
                case 1:
                    time = line;
                    break;
                case 2:
                    id = line;
                    break;
                case 3:
                    type = line;
                    break;
                case 4:
                    product = line;
                    break;
                default:
                    break;
            }
            //cout << line;
            i++;
        }
        logData.close();
    }

    for (size_t j = 0; j < logInfo.size(); j++) {
        cout << logInfo[j];
    }

    if (searchAll) {
        return logInfo;
    } else {
        string select;
        for (size_t a = 0; a < logInfo.size(); a++) {
            if (searchByProduct) {
                select = logInfo.at(a)->scannedProduct;
            } else if (searchByType) {
                select = logInfo.at(a)->sensorType;
            } else if (searchByID) {
                select = logInfo.at(a)->sensorID;
            }
            if (select == filter) {
                filteredList.push_back(logInfo.at(a));
            }
        }
        if (singleStatus) {
            if ((size_t)index < filteredList.size()) {
                singleList.push_back(filteredList.at(index));
            }
            return singleList;
        }
    }
    return filteredList;

}

vector<string> HTTPHandler::getStock() {
    ifstream logfile;
    vector<string> data;
    logfile.open("stock.txt");
    if (!logfile) {
        cerr << "Error: file could not be opened" << endl;
    } else {
        string line;
        while (!logfile.eof()) {
            getline(logfile, line);
            cout << line;
            data.push_back(line);
        }
    }
    return data;
}

string HTTPHandler::generateStatusPage(vector<sensorMessage *> s) {

    printf("In Generate Status Page \n");
    string page = "";
    WebpageGenerator generator;
    page = generator.generateHeader();
    if (s.empty()) {

        if (showInventory) {
            cout << "Showing inventory..." << endl;
            vector<string> data = getStock();
            size_t i = 0;
            page += generator.writeParagraph("Current stock");
            string line;
            while (i < data.size()) {
                line = " ";
                switch (i) {
                    case 0:
                        line = "Schuhe: ";
                        break;
                    case 1:
                        line = "Hose: ";
                        break;
                    case 2:
                        line = "Jeans: ";
                        break;
                    case 3:
                        line = "Shirt: ";
                        break;
                    case 4:
                        line = "Pullover: ";
                        break;
                    case 5:
                        line = "Hemd: ";
                        break;
                    case 6:
                        line = "Kleid: ";
                        break;
                    case 7:
                        line = "Rock: ";
                        break;
                    case 8:
                        line = "Jacke: ";
                        break;
                    case 9:
                        line = "Mantel: ";
                        break;
                    default:
                        break;
                }
                cout << "Data " << data.at(i) << endl;
                page += generator.writeParagraph(line + data.at(i));
                i++;
            }
        } else {

            page += generator.writeParagraph("Please click or enter one of the following urls to retrieve data.");
            page += generator.writeParagraph("/all - for data of all sensors.");
            page += generator.writeParagraph(
                    "/id/n - for data history of an existing sensor with the id 'n', eg. id/2");
            page += generator.writeParagraph(
                    "/id/n/m - for entry number 'm' of an existing sensor with the id 'n', eg. /id/1/3");
            page += generator.writeParagraph("/type/Inventarisierung - for data history of all inventory scans");
            page += generator.writeParagraph("/type/Inventarisierung/n - for for entry number 'n' of inventory scans");
            page += generator.writeParagraph("/type/Einlieferung - for data history of product check ins");
            page += generator.writeParagraph("/type/Einlieferung/n - for for entry number 'n' of product check ins");
            page += generator.writeParagraph("/type/Auslieferung - for data history of product check outs");
            page += generator.writeParagraph("/type/Auslieferung/n - for for entry number 'n' check outs");
            page += generator.writeParagraph(
                    "/product/productName - for data history of all existing records of product with the name 'productName'");


        }


    } else {
        page += generator.startTable();
        for (sensorMessage *status: s) {
            page += generator.writeRow(status->scannedTime, status->scannedProduct, status->sensorType, status->sensorID
            );
        }
        page += generator.endTable();
    }
    page += generator.generateFooter();

    cout << page << endl;
    return page;

}