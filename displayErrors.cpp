#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include "errorChecking.h"
#include "errorCorrecting.h"
#include "displayErrors.h"
using namespace  std ;
typedef unsigned long long ull;


vector<int> displayErrors(const string& aux_xmlFilePath ,const string& RealxmlFilePath ){
    string errorMessages ;
    string beginminus ;
    vector<int> linesOfErrors ;
    vector <string> lines ;
    bool IsCorrect=errorChecking(aux_xmlFilePath,errorMessages,beginminus);
    while(IsCorrect!=true){
        lines =errorCorrecting(aux_xmlFilePath,errorMessages,beginminus) ;
        IsCorrect=errorChecking(aux_xmlFilePath,errorMessages,beginminus);
    }
    std::ifstream f1(aux_xmlFilePath);
    std::ifstream f2(RealxmlFilePath);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return linesOfErrors;
    }

    std::string line1, line2;
    int num =1 ;
    while (std::getline(f1, line1) && std::getline(f2, line2)) {

        if (removeSpaces(line1) != removeSpaces(line2)) {
            linesOfErrors.push_back(num) ;
        }
        num++;
    }
    return linesOfErrors ;


}
