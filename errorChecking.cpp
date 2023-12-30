#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include "errorChecking.h"
#include <cctype>
using namespace  std ;
typedef unsigned long long ull;


bool errorChecking(const string& xmlFilePath,string & errorMessages,string &beginminus) {
    ifstream inputFile(xmlFilePath);

    if (!inputFile.is_open()) {
        return 0;
    }

    stack<string> tagStack;
    string line;
    string correctedXml;
    ull pos =0;
    ull lineNumber =0;

    while (getline(inputFile, line)) {
        lineNumber++;
        pos =0;
        if((pos = line.find("<users>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<users>") ;
        }else if((pos = line.find("<user>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<user>") ;
        }else if((pos = line.find("<posts>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<posts>") ;
        }else if((pos = line.find("<post>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<post>") ;
        }else if((pos = line.find("<topics>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<topics>") ;
        }else if((pos = line.find("<topic>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<topic>") ;
        }else if((pos = line.find("<followers>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<followers>") ;
        }else if((pos = line.find("<follower>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<follower>") ;
        }else if((pos = line.find("<body>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<body>") ;
        }else if((pos = line.find("<id>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<id>") ;
        }else if((pos = line.find("<name>", 0)) != string::npos){
            tagStack.push(to_string(lineNumber));
            tagStack.push("<name>") ;
        }
//***********************************************************************
//****************************CLOSING TAGS*******************************
        if((pos = line.find("</users>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<users>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;


            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</users>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;

                return false ;
            }
        }


        else if((pos = line.find("</user>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<user>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</user>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }


        else if((pos = line.find("</posts>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<posts>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</posts>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }



        else if((pos = line.find("</post>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<post>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</post>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }

        }


        else if((pos = line.find("</topics>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<topics>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</topics>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }


        else if((pos = line.find("</topic>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<topic>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</topic>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }



        else if((pos = line.find("</followers>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<followers>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;


            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</followers>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }


        else if((pos = line.find("</follower>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<follower>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</follower>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }



        else if((pos = line.find("</body>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<body>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</body>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }


        else if((pos = line.find("</id>", 0)) != string::npos){
            if(!tagStack.empty() && tagStack.top()=="<id>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</id>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }
        }



        else if((pos = line.find("</name>", 0)) != string::npos){

            if(!tagStack.empty() && tagStack.top()=="<name>"){
                tagStack.pop() ;//NO ERROR
                tagStack.pop() ;

            }else{
                errorMessages+=tagStack.top() ;
                tagStack.pop() ;
                errorMessages+=tagStack.top() ;
                errorMessages+="</name>"+ to_string(lineNumber) ;
                tagStack.pop() ;
                if(!tagStack.empty())
                    beginminus=tagStack.top() ;
                return false ;
            }


        }




    }

    if(tagStack.empty())
        return true;
    else{
        while(!tagStack.empty()){
            errorMessages+=tagStack.top();
            tagStack.pop();
            errorMessages+=tagStack.top() ;
            tagStack.pop() ;
        }
        return false ;
    }

}
