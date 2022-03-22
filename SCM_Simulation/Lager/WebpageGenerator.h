//
// Created by Cheryl on 22.11.2021.
//

#ifndef LAGER_WEBPAGEGENERATOR_H
#define LAGER_WEBPAGEGENERATOR_H

#include <string>
#include <cstdio>
#include <vector>
#include "utils.h"
#include <sstream>

using namespace std;

class WebpageGenerator {
public:
    WebpageGenerator() {}

    ~WebpageGenerator() = default;

    string generateHeader();

    string generateFooter();

    string writeParagraph(string s);

    string startTable();

    string endTable();

    string writeRow(string one, string two, string three, string four);


};


#endif //LAGER_WEBPAGEGENERATOR_H
