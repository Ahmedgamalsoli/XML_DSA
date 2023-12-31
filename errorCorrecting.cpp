//
// Created by seif alrahman on 12/30/2023.
//
#include "errorCorrecting.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
using namespace  std;
typedef unsigned long long ull;

vector<string> errorCorrecting (const string& xmlFilePath,string &errorMessages,string &beginminus){
    fstream file(xmlFilePath, std::ios::in | std::ios::out);
    vector<string> lines;
    string linee;
    if (!file.is_open()) {
        return lines  ;
    }
    file.seekg(std::ios::beg);
    while (getline(file, linee)) {
        lines.push_back(linee);
    }



    stack<string> tagStack;
    string line;
    string correctedXml;
    ull pos =0;
    ull lineNumber =0;
    string begin ;
    int beginline=0;
    string end ;
    int endline ;
    lines.push_back(" ");
    lines[0]="<users>" ;
    if((errorMessages.find("<users>", 0)) != string::npos) {
        begin ="<users>";
        beginline=stoi(errorMessages.substr(7,errorMessages.find("</",0)-7 )) ;
    }
    if((errorMessages.find("<user>", 0)) != string::npos) {
        begin ="<user>";
        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
    }
    if((errorMessages.find("<topics>", 0)) != string::npos) {
        begin ="<topics>";
        beginline=stoi(errorMessages.substr(8,errorMessages.find("</",0)-8 )) ;
    }
    if((errorMessages.find("<topic>", 0)) != string::npos) {
        begin ="<topic>";
        beginline=stoi(errorMessages.substr(7,errorMessages.find("</",0)-7 )) ;
    }
    if((errorMessages.find("<body>", 0)) != string::npos) {
        begin ="<body>";
        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
    }
    if((errorMessages.find("<followers>", 0)) != string::npos) {
        begin ="<followers>";
        beginline=stoi(errorMessages.substr(11,errorMessages.find("</",0)-11 )) ;
    }
    if((errorMessages.find("<follower>", 0)) != string::npos) {
        begin ="<follower>";
        beginline=stoi(errorMessages.substr(10,errorMessages.find("</",0)-10 )) ;
    }
    if((errorMessages.find("<posts>", 0)) != string::npos) {
        begin ="<posts>";
        beginline=stoi(errorMessages.substr(7,errorMessages.find("</",0)-7 )) ;
    }
    if((errorMessages.find("<post>", 0)) != string::npos) {
        begin ="<post>";
        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
    }
    if((errorMessages.find("<id>", 0)) != string::npos) {
        begin ="<id>";
        beginline=stoi(errorMessages.substr(4,errorMessages.find("</",0)-4 )) ;
    }
    if((errorMessages.find("<name>", 0)) != string::npos) {
        begin ="<name>";
        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
    }
    //*************************************************************************************************************************************************
    //*************************************************************************************************************************************************
    if((errorMessages.find("</users>", 0)) != string::npos) {
        end ="</users>";
        endline=stoi(errorMessages.substr((errorMessages.find("</users>", 0))+8)) ;
    }
    if((errorMessages.find("</user>", 0)) != string::npos) {
        end ="</user>";
        endline=stoi(errorMessages.substr((errorMessages.find("</user>", 0))+7)) ;
    }
    if((errorMessages.find("</posts>", 0)) != string::npos) {
        end ="</posts>";
        endline=stoi(errorMessages.substr((errorMessages.find("</posts>", 0))+8)) ;
    }
    if((errorMessages.find("</post>", 0)) != string::npos) {
        end ="</post>";
        endline=stoi(errorMessages.substr((errorMessages.find("</post>", 0))+7)) ;
    }
    if((errorMessages.find("</id>", 0)) != string::npos) {
        end ="</id>";
        endline=stoi(errorMessages.substr((errorMessages.find("</id>", 0))+5)) ;
    }
    if((errorMessages.find("</topics>", 0)) != string::npos) {
        end ="</topics>";
        endline=stoi(errorMessages.substr((errorMessages.find("</topics>", 0))+9)) ;
    }
    if((errorMessages.find("</topic>", 0)) != string::npos) {
        end ="</topic>";
        endline=stoi(errorMessages.substr((errorMessages.find("</topic>", 0))+8)) ;
    }if((errorMessages.find("</name>", 0)) != string::npos) {
        end ="</name>";
        endline=stoi(errorMessages.substr((errorMessages.find("</name>", 0))+7)) ;
    }if((errorMessages.find("</followers>", 0)) != string::npos) {
        end ="</followers>";
        endline=stoi(errorMessages.substr((errorMessages.find("</followers>", 0))+12)) ;
    }
    if((errorMessages.find("</follower>", 0)) != string::npos) {
        end ="</follower>";
        endline=stoi(errorMessages.substr((errorMessages.find("</follower>", 0))+11)) ;
    }
    if((errorMessages.find("</body>", 0)) != string::npos) {
        end ="</body>";
        endline=stoi(errorMessages.substr((errorMessages.find("</body>", 0))+7)) ;
    }

//    lines[beginline]="\t\t\t\t\t<topic***********************>" ;
//    file.close();
//    file.open(xmlFilePath, std::ios::out | std::ios::trunc);
//    for (const auto &modifiedLine : lines) {
//        file << modifiedLine<<endl ;
//    }
    if((begin=="<topics>" && beginminus=="<post>" && (lines[beginline-2].find("</body>",0)!=string::npos))
       ||(beginminus=="<topics>"&& begin!="<topic>" &&end=="</topic>" )){

        if((beginminus=="<topics>"&& begin!="<topic>" &&end=="</topic>" )){lines[beginline-1]="\t\t\t\t\t<topic>";
            file.close();
            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
            while (lines.back()==" "){
                lines.pop_back() ;
            }
            for (const auto &modifiedLine : lines) {

                file << modifiedLine<<endl ;
            }
            return lines ;}
        lines[beginline]="\t\t\t\t\t<topic>" ;
        lines[beginline+2]="\t\t\t\t\t</topic>" ;
        beginline+=3;
        while(beginline<=endline){
            if(lines[beginline].find("</topics>",0)!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                break;
            }else{
                lines[beginline]="\t\t\t\t\t<topic>" ;
                lines[beginline+2]="\t\t\t\t\t</topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="\t\t\t\t</topics>";
        lines[beginline+1]="\t\t\t</post>";

    }



    else if (begin=="<topic>"&&beginminus=="<topics>"){
        lines[beginline+1]="\t\t\t\t\t</topic>" ;
    }



    else if(begin=="<body>"&&beginminus=="<post>"){
        lines[beginline+1]="\t\t\t\t</body>" ;
    }


    else if (begin=="<id>"&& ( beginminus=="<user>" || beginminus=="<follower>" ) &&(lines[beginline-2]=="    <user>" || lines[beginline-2]=="            <follower>")){
        ull start = lines[beginline-1].find("<id>")+4 ;
        string id ;
        if(lines[beginline-1].find("</")==string::npos){
            id=lines[beginline-1].substr(start) ;
        }else{
            ull end =lines[beginline-1].find("</") ;
            id=lines[beginline-1].substr(start,end-start);
        }
        if(beginminus=="<follower>")
            lines[beginline-1]="\t\t\t\t<id>"+id+"</id>";
        else
            lines[beginline-1]="\t\t<id>"+id+"</id>";
    }



    else if (begin=="<name>" && beginminus=="<user>" && lines[beginline-3]=="\t<user>"){
        ull start = lines[beginline-1].find("<name>")+6 ;
        string name ;
        if(lines[beginline-1].find("</")==string::npos){
            name=lines[beginline-1].substr(start) ;
        }else{
            ull end =lines[beginline-1].find("</") ;
            name=lines[beginline-1].substr(start,end-start);

        }
        lines[beginline-1]="\t\t<name>"+name+"</name>";
    }




    else if (( begin=="<post>"  &&  beginminus=="<posts>" ) ||
             (beginminus=="<post>"&& begin!="<body>" &&end=="</body>" )
             || (beginminus=="<post>"&& begin!="<topics>" &&end=="</topics>" )  ){
        if((beginminus=="<post>"&& begin!="<body>" &&end=="</body>" )){beginline--;}
        if((beginminus=="<post>"&& begin!="<topics>" &&end=="</topics>" ) ){beginline-=4;}
        lines[beginline]="\t\t\t\t<body>" ;
        beginline+=2;
        lines[beginline]="\t\t\t\t</body>" ;
        beginline++;
        lines[beginline]="\t\t\t\t<topics>" ;
        beginline++;
        while(1){
            if( lines[beginline].find("</topics>")!=string::npos || lines[beginline+1].find("</post>")!=string::npos ){
                break;
            }else{
                lines[beginline]="\t\t\t\t\t<topic>" ;
                lines[beginline+2]="\t\t\t\t\t</topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="\t\t\t\t</topics>";
        lines[beginline+1]="\t\t\t</post>";


    }


    else if ((begin=="<posts>" && beginminus=="<user>" &&  (lines[beginline-2].find("</name>",0)!=string::npos))
             ||(beginminus=="<posts>" && begin!="<post>" && end=="</post>")){
        if((beginminus=="<posts>" && begin!="<post>" && end=="</post>")){beginline--;}
        lines[beginline]="\t\t\t<post>" ;
        beginline++;
        lines[beginline]="\t\t\t\t<body>" ;
        beginline+=2;
        lines[beginline]="\t\t\t\t</body>" ;
        beginline++;
        lines[beginline]="\t\t\t\t<topics>" ;
        beginline++;
        while(1){
            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos|| lines[beginline].find("</post>")!=string::npos ){
                break;
            }else{
                lines[beginline]="\t\t\t\t\t<topic>" ;
                lines[beginline+2]="\t\t\t\t\t</topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="\t\t\t\t</topics>";
        lines[beginline+1]="\t\t\t</post>";
        beginline+=2 ;
        while(1){
            if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos ){
                break ;
            }else{
                lines[beginline]="\t\t\t<post>" ;
                beginline++;
                lines[beginline]="\t\t\t\t<body>" ;
                beginline+=2;
                lines[beginline]="\t\t\t\t</body>" ;
                beginline++;
                lines[beginline]="\t\t\t\t<topics>" ;
                beginline++;
                while(1){
                    if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                        break;
                    }
                    else{
                        lines[beginline]="\t\t\t\t\t<topic>" ;
                        lines[beginline+2]="\t\t\t\t\t</topic>" ;
                        beginline+=3;
                    }
                }
                lines[beginline]="\t\t\t\t</topics>";
                lines[beginline+1]="\t\t\t</post>";
                beginline+=2;
            }
        }
        lines[beginline]="\t\t</posts>";


    }


    else if ((begin=="<follower>" && beginminus=="<followers>")
             ||(beginminus=="<follower>"&&begin!="<id>" &&end=="</id>" )){

        if((beginminus=="<follower>"&&begin!="<id>" &&end=="</id>")){beginline--;}

        char c =lines[beginline][4] ;
        int start=0;
        if('1'<=c&&c<='9'){
            start=4;
        }else{
            start=lines[beginline].find(">")+1 ;

        }


        string id ;
        if(lines[beginline].find("</")==string::npos){
            id=lines[beginline].substr(start) ;
        }else{
            ull end =lines[beginline].find("</") ;
            id=lines[beginline].substr(start,end-start);
        }

        lines[beginline]="\t\t\t\t<id>"+id+"</id>";
        lines[beginline+1]="\t\t\t</follower>";

    }


    else if((begin=="<followers>" && beginminus=="<user>" &&  (lines[beginline-2].find("</posts>",0)!=string::npos) )
            || (beginminus=="<followers>" && begin!="<follower>"  && end=="</follower>" )){

        if((beginminus=="<followers>" && begin!="<follower>"  && end=="</follower>" )){beginline--;}
        lines[beginline]="\t\t\t<follower>";
        beginline++;
        //********************WATCHOUT copied code **************************
        char c =lines[beginline][4] ;
        int start=0;
        if('1'<=c&&c<='9'){
            start=4;
        }else{
            start=lines[beginline].find(">")+1 ;

        }


        string id ;
        if(lines[beginline].find("</")==string::npos){
            id=lines[beginline].substr(start) ;
        }else{
            ull end =lines[beginline].find("</") ;
            id=lines[beginline].substr(start,end-start);
        }

        lines[beginline]="\t\t\t\t<id>"+id+"</id>";
        //********************************************************************
        beginline++;
        lines[beginline]="\t\t\t</follower>";
        beginline++;
        while(1){
            if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos) {
                break;
            }else{
                lines[beginline]="\t\t\t<follower>";
                beginline++;
                //********************WATCHOUT copied code **************************
                char c =lines[beginline][4] ;
                int start=0;
                if('1'<=c&&c<='9'){
                    start=4;
                }else{
                    start=lines[beginline].find(">")+1 ;

                }


                string id ;
                if(lines[beginline].find("</")==string::npos){
                    id=lines[beginline].substr(start) ;
                }else{
                    ull end =lines[beginline].find("</") ;
                    id=lines[beginline].substr(start,end-start);
                }

                lines[beginline]="\t\t\t\t<id>"+id+"</id>";
                //********************************************************************
                beginline++;
                lines[beginline]="\t\t\t</follower>";
                beginline++;
            }
        }
        lines[beginline]="\t\t</followers>" ;
        beginline++;
        lines[beginline]="\t</user>" ;


    }


    else if ((begin=="<user>" && beginminus=="<users>" )
             || (beginminus=="<user>" && begin!="<id>"  && end=="</id>" )
             || (beginminus=="<user>" && begin!="<name>"  && end=="</name>")
             || (beginminus=="<user>" && begin!="<posts>"  && end=="</posts>")
             || ((beginminus=="<user>" && begin!="<followers>"  && end=="</followers>")) ){

        if((beginminus=="<user>" && begin!="<id>"  && end=="</id>" )){beginline--;}
        if((beginminus=="<user>" && begin!="<name>"  && end=="</name>")){beginline-=2;}
        if( (beginminus=="<user>" && begin!="<posts>"  && end=="</posts>")){beginline-=3;}
        if((beginminus=="<user>" && begin!="<followers>"  && end=="</followers>")){
            lines[beginline-1]="        <followers>" ;
            file.close();
            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
            while (lines.back()==" "){
                lines.pop_back() ;
            }
            for (const auto &modifiedLine : lines) {

                file << modifiedLine<<endl ;
            }
            return  lines;
        }
        beginline--;
        lines[beginline]="\t<user>" ;
        beginline++;

        //********************WATCHOUT copied code **************************
        char c =lines[beginline][2] ;
        int start=0;
        if('1'<=c&&c<='9'){
            start=2;
        }else{
            start=lines[beginline].find(">")+1 ;

        }


        string id ;
        if(lines[beginline].find("</")==string::npos){
            id=lines[beginline].substr(start) ;
        }else{
            ull end =lines[beginline].find("</") ;
            id=lines[beginline].substr(start,end-start);
        }

        lines[beginline]="\t\t<id>"+id+"</id>";
        beginline++;
        //**********************NAME******************************
        char b =lines[beginline][2] ;
        int startt=0;
        if(b!='<'){
            startt=2;
        }else{
            startt=lines[beginline].find(">")+1 ;

        }

        string nn ;
        if(lines[beginline].find("</")==string::npos){
            nn=lines[beginline].substr(startt) ;
        }else{
            ull end =lines[beginline].find("</") ;
            nn=lines[beginline].substr(startt,end-startt);
        }

        lines[beginline]="\t\t<name>"+nn+"</name>";

        beginline++;

        //**************************POSTS******************************************
        lines[beginline]="\t\t<posts>" ;
        beginline++;
        lines[beginline]="\t\t\t<post>" ;
        beginline++;
        lines[beginline]="\t\t\t\t<body>" ;
        beginline+=2;
        lines[beginline]="\t\t\t\t</body>" ;
        beginline++;
        lines[beginline]="\t\t\t\t<topics>" ;
        beginline++;
        while(1){
            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                break;
            }else{
                lines[beginline]="\t\t\t\t\t<topic>" ;
                lines[beginline+2]="\t\t\t\t\t</topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="\t\t\t\t</topics>";
        lines[beginline+1]="\t\t\t</post>";
        beginline+=2 ;
        while(1){
            if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos){
                break ;
            }else{
                lines[beginline]="\t\t\t<post>" ;
                beginline++;
                lines[beginline]="\t\t\t\t<body>" ;
                beginline+=2;
                lines[beginline]="\t\t\t\t</body>" ;
                beginline++;
                lines[beginline]="\t\t\t\t<topics>" ;
                beginline++;
                lines[beginline]="\t\t\t\t\t<topic>" ;//////////////
                lines[beginline+2]="\t\t\t\t\t</topic>" ;////////////////////////
                beginline+=3;//////////////////////
                while(1){
                    if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                        break;
                    }else{
                        lines[beginline]="\t\t\t\t\t<topic>" ;
                        lines[beginline+2]="\t\t\t\t\t</topic>" ;
                        beginline+=3;
                    }
                }
                lines[beginline]="\t\t\t\t</topics>";
                lines[beginline+1]="\t\t\t</post>";
                beginline+=2;
            }
        }
        lines[beginline]="\t\t</posts>";
        beginline++;
        //***************************************************
        //FOLLOWERS******************************************
        lines[beginline]="\t\t<followers>" ;
        beginline++;
        lines[beginline]="\t\t\t<follower>";
        beginline++;
        //********************WATCHOUT copied code(ID inside followers) **************************
        char cc =lines[beginline][4] ;
        int starttt=0;
        if('1'<=cc&&cc<='9'){
            starttt=4;
        }else{
            starttt=lines[beginline].find(">")+1;
        }

        string idd ;
        if(lines[beginline].find("</")==string::npos){
            idd=lines[beginline].substr(starttt) ;
        }else{
            ull end =lines[beginline].find("</") ;
            idd=lines[beginline].substr(starttt,end-starttt);
        }

        lines[beginline]="\t\t\t\t<id>"+idd+"</id>";
//**********************FOLLOWER*********************************
        beginline++;
        lines[beginline]="\t\t\t</follower>";
        beginline++;
        while(1){
            if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos  ) {
                break;
            }else{
                lines[beginline]="\t\t\t<follower>";
                beginline++;
                //********************WATCHOUT copied code **************************
                char c =lines[beginline][4] ;
                int start=0;
                if('1'<=c&&c<='9'){
                    start=4;
                }else{
                    start=lines[beginline].find(">")+1 ;
                }

                string id ;
                if(lines[beginline].find("</")==string::npos){
                    id=lines[beginline].substr(start) ;
                }else{
                    ull end =lines[beginline].find("</") ;
                    id=lines[beginline].substr(start,end-start);
                }

                lines[beginline]="\t\t\t\t<id>"+id+"</id>";
                //********************************************************************
                beginline++;
                lines[beginline]="\t\t\t</follower>";
                beginline++;
            }
        }
        lines[beginline]="\t\t</followers>" ;
        beginline++;
        lines[beginline]="\t</user>" ;
    }


    else if ((begin=="<users>")
             || (beginminus =="<users>" && begin!="<user>" && end=="</user>" )
             ||( begin=="<users>"&& end=="</user>") ){
        if((beginminus =="<users>" && begin!="<user>" && end=="</user>")){
            lines[beginline-1]="\t<user>" ;
            file.close();
            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
            while (lines.back()==" "){
                lines.pop_back() ;
            }
            for (const auto &modifiedLine : lines) {

                file << modifiedLine<<endl ;
            }
            return lines;
        }
        while(1){
            if(beginline>(lines.size()-3))
                break;
            else{
                lines[beginline]="\t<user>" ;
                beginline++;

                //********************WATCHOUT copied code **************************
                char c =lines[beginline][2] ;
                int start=0;
                if('1'<=c&&c<='9'){
                    start=2;
                }else{
                    start=lines[beginline].find(">")+1 ;

                }


                string id ;
                if(lines[beginline].find("</")==string::npos){
                    id=lines[beginline].substr(start) ;
                }else{
                    ull end =lines[beginline].find("</") ;
                    id=lines[beginline].substr(start,end-start);
                }

                lines[beginline]="\t\t<id>"+id+"</id>";
                beginline++;
                //**********************NAME******************************
                char b =lines[beginline][2] ;
                int startt=0;
                if(c!='<'){
                    startt=2;
                }else{
                    startt=lines[beginline].find(">")+1 ;

                }

                string nn ;
                if(lines[beginline].find("</")==string::npos){
                    nn=lines[beginline].substr(startt) ;
                }else{
                    ull end =lines[beginline].find("</") ;
                    nn=lines[beginline].substr(startt,end-startt);
                }

                lines[beginline]="\t\t<name>"+nn+"</name>";

                beginline++;

                //**************************POSTS******************************************
                lines[beginline]="\t\t<posts>" ;
                beginline++;
                lines[beginline]="\t\t\t<post>" ;
                beginline++;
                lines[beginline]="\t\t\t\t<body>" ;
                beginline+=2;
                lines[beginline]="\t\t\t\t</body>" ;
                beginline++;
                lines[beginline]="\t\t\t\t<topics>" ;
                beginline++;
                while(1){
                    if(lines[beginline].find("</topics>")!=string::npos ||lines[beginline+1].find("</post>")!=string::npos||lines[beginline].find("</post>")!=string::npos){
                        break;
                    }else{
                        lines[beginline]="\t\t\t\t\t<topic>" ;
                        lines[beginline+2]="\t\t\t\t\t</topic>" ;
                        beginline+=3;
                    }
                }
                lines[beginline]="\t\t\t\t</topics>";
                lines[beginline+1]="\t\t\t</post>";
                beginline+=2 ;
                while(1){
                    if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos){
                        break ;
                    }else{
                        lines[beginline]="\t\t\t<post>" ;
                        beginline++;
                        lines[beginline]="\t\t\t\t<body>" ;
                        beginline+=2;
                        lines[beginline]="\t\t\t\t</body>" ;
                        beginline++;
                        lines[beginline]="\t\t\t\t<topics>" ;
                        beginline++;
                        lines[beginline]="\t\t\t\t\t<topic>" ;//////////////
                        lines[beginline+2]="\t\t\t\t\t</topic>" ;////////////////////////
                        beginline+=3;//////////////////////
                        while(1){
                            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                                break;
                            }else{
                                lines[beginline]="\t\t\t\t\t<topic>"    ;
                                lines[beginline+2]="\t\t\t\t\t</topic>" ;
                                beginline+=3;
                            }
                        }
                        lines[beginline]="\t\t\t\t</topics>";
                        lines[beginline+1]="\t\t\t</post>";
                        beginline+=2;
                    }
                }
                lines[beginline]="\t\t</posts>";
                beginline++;
                //***************************************************
                //FOLLOWERS******************************************
                lines[beginline]="\t\t<followers>" ;
                beginline++;
                lines[beginline]="\t\t\t<follower>";
                beginline++;
                //********************WATCHOUT copied code(ID inside followers) **************************
                char cc =lines[beginline][4] ;
                int starttt=0;
                if('1'<=cc&&cc<='9'){
                    starttt=4;
                }else{
                    starttt=lines[beginline].find(">")+1;
                }

                string idd ;
                if(lines[beginline].find("</")==string::npos){
                    idd=lines[beginline].substr(starttt) ;
                }else{
                    ull end =lines[beginline].find("</") ;
                    idd=lines[beginline].substr(starttt,end-starttt);
                }

                lines[beginline]="\t\t\t\t<id>"+idd+"</id>";
//**********************FOLLOWER*********************************
                beginline++;
                lines[beginline]="\t\t\t</follower>";
                beginline++;
                while(1){
                    if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos) {
                        break;
                    }else{
                        lines[beginline]="\t\t\t<follower>";
                        beginline++;
                        //********************WATCHOUT copied code **************************
                        char c =lines[beginline][4] ;
                        int start=0;
                        if('1'<=c&&c<='9'){
                            start=4;
                        }else{
                            start=lines[beginline].find(">")+1 ;
                        }

                        string id ;
                        if(lines[beginline].find("</")==string::npos){
                            id=lines[beginline].substr(start) ;
                        }else{
                            ull end =lines[beginline].find("</") ;
                            id=lines[beginline].substr(start,end-start);
                        }

                        lines[beginline]="\t\t\t\t<id>"+id+"</id>";
                        //********************************************************************
                        beginline++;
                        lines[beginline]="\t\t\t</follower>";
                        beginline++;
                    }
                }
                lines[beginline]="\t\t</followers>" ;
                beginline++;
                lines[beginline]="\t</user>" ;
                beginline++;
            }


        }

        lines[beginline]="</users>";



    }



    file.close();
    file.open(xmlFilePath, std::ios::out | std::ios::trunc);
    while (lines.back()==" "){
        lines.pop_back() ;
    }

    for (const auto &modifiedLine : lines) {

        file << modifiedLine<<endl ;

    }
    return lines ;
}
