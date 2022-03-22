//
// Created by Cheryl on 22.11.2021.
//

#include "WebpageGenerator.h"

string WebpageGenerator::generateHeader() {
    // printf("Header is fine \n");
    stringstream header;

char* clientID = getenv ("LAGERID");
        if (clientID==nullptr) {
            printf ("No client ID");
            throw runtime_error ("MQTT Pub Error: Client ID missing");
        }

    header << "<!DOCTYPE html>\n" << "<html lang=\"de\">\n"
           << "<head>\n"
           << "    <meta charset=\"UTF-8\"/>\n"
           << "    <title>Sensor Status</title>\n"
           << "</head>\n"
           /*template taken from https://www.w3schools.com/css/tryit.asp?filename=trycss_table_fancy*/
           << "<style>\n"
           << "#sensors {\n"
              "    font-family: Arial, Helvetica, sans-serif;\n"
              "    border-collapse: collapse;\n"
              "    width: 100%;\n"
              "}\n"
              "\n"
              "#sensors td, #sensors th {\n"
              "    border: 1px solid #ddd;\n"
              "    padding: 8px;\n"
              "}\n"
              "\n"
              "#sensors tr:nth-child(even){background-color: #f2f2f2;}\n"
              "\n"
              "#sensors tr:hover {background-color: #ddd;}\n"
              "\n"
              "#sensors th {\n"
              "    padding-top: 12px;\n"
              "    padding-bottom: 12px;\n"
              "    text-align: left;\n"
              "    background-color: #000080;\n"
              "    color: white;\n"
              "}</style>\n"
           << "<body>\n"
           << "<h1 style=\"text-align: center\">SCM Sensor Information - " << clientID << "</h1>\n"
           << "<p style=\"text-align: center\"> <a href=\"/\">Home</a>\n"
           << "<a href=\"/all\">All</a> \n"
           << "<a href=\"/type/Inventarisierung\">Inventarisierung</a>\n"
           << "<a href=\"/type/Einlieferung\">Einlieferung</a>\n"
           << "<a href=\"/type/Auslieferung\">Auslieferung</a>\n"
           << "<a href=\"/inventory\">Inventar</a> </p>\n";

    return header.str();
}

string WebpageGenerator::generateFooter() {
    //  printf("Footer is fine \n");
    stringstream footer;
    footer << "</body>\n"
           << "</html>";
    return footer.str();
}

string WebpageGenerator::writeParagraph(string s) {
    // printf("Read data: ");
    stringstream p;
    p << "<p>" << s << "</p>\n";

    return p.str();
}

string WebpageGenerator::startTable() {
    stringstream p;
    p << "<table id=\"sensors\" style=\"width:100%\">\n"
         "  <tr>\n"
         "    <th style=\"text-align: left\">Timestamp</th>\n"
         "    <th style=\"text-align: left\">Product</th>\n"
         "    <th style=\"text-align: left\">Sensor Type</th>\n"
         "    <th style=\"text-align: left\">Sensor ID</th>\n"
         "  </tr>\n";

    return p.str();
}

string WebpageGenerator::endTable() {

    stringstream p;
    p << "</table>";

    return p.str();

}

string WebpageGenerator::writeRow(string one, string two, string three, string four) {

    stringstream p;
    p << "  <tr>\n<td>" << one << "</td>\n<td>" << two << "</td>\n<td>" << three << "</td>\n<td>" << four
      << "</td>\n</tr>";

    return p.str();

}