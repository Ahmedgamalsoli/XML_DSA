#include "Operations.h"
#include "SimpleXMLParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include <zlib.h>
#include <queue>
#include "errorCorrecting.h"
#include "errorChecking.h"
typedef unsigned long long ull;
string Xml_Formatting(const std::string& xml)
{

    SimpleXMLParser parser;
    vector<User> users = parser.parse(xml);

    int i1=0 , i2=0 , i3=0 , i4=0 ;
    string output;
    output.append("<users>\n");

    for(int i=0;i<users.size();i++)
    {
        i4++;
        i1=0 ; i2=0 ; i3=0 ;
        output.append("\t<user>\n");
        output.append("\t\t<id>");output+=to_string(users[i].getID()); output.append("</id>\n");
        output.append("\t\t<name>");output+=users[i].getName(); output.append("</name>\n");

        output.append("\t\t<posts>\n");
        for (const auto& post : users[i].getPosts())
        {
            i2++;
            output.append("\t\t\t<post>\n");
            output.append("\t\t\t\t<body>\n");output.append("\t\t\t\t\t");output+=post.getBody();output.append("\n");output.append("\t\t\t\t</body>\n");
            output.append("\t\t\t\t<topics>\n");
            for (const auto& topic : post.getTopics())
            {
                i1++;
                output.append("\t\t\t\t\t<topic>\n");output.append("\t\t\t\t\t\t"); output+=topic.name;output.append("\n");output.append("\t\t\t\t\t</topic>\n");

            }
            output.append("\t\t\t\t</topics>\n");
            output.append("\t\t\t</post>\n");
        }
        output.append("\t\t</posts>\n");

        output.append("\t\t<followers>\n");
        for (const auto& follower : users[i].getFollowers())
        {
            i3++;
            output.append("\t\t\t<follower>\n");
            output.append("\t\t\t\t<id>");output+=to_string(follower.id);output.append("</id>\n");
            output.append("\t\t\t</follower>\n");

        }
        output.append("\t\t</followers>\n");



        output.append("\t</user>\n");

    }
    output.append("<users>\n");


    return output;
}


string Xml_Minifying(const std::string& xml)
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xml);

    int i1 = 0, i2 = 0, i3 = 0, i4 = 0;
    string output;
    output.append("<users>");

    for (int i = 0; i < users.size(); i++)
    {
        i4++;
        i1 = 0;
        i2 = 0;
        i3 = 0;
        output.append("<user>");
        output.append("<id>");
        output += to_string(users[i].getID());
        output.append("</id>");
        output.append("<name>");
        output += users[i].getName();
        output.append("</name>");

        output.append("<posts>");
        for (const auto &post : users[i].getPosts())
        {
            i2++;
            output.append("<post>");
            output.append("<body>");
            output += post.getBody();
            output.append("</body>");
            output.append("<topics>");
            for (const auto &topic : post.getTopics())
            {
                i1++;
                output.append("<topic>");
                output += topic.name;
                output.append("</topic>");
            }
            output.append("</topics>");
            output.append("</post>");
        }
        output.append("</posts>");

        output.append("<followers>");
        for (const auto &follower : users[i].getFollowers())
        {
            i3++;
            output.append("<follower>");
            output.append("<id>");
            output += to_string(follower.id);
            output.append("</id>");
            output.append("</follower>");
        }
        output.append("</followers>");

        output.append("</user>");
    }
    output.append("</users>");

    return output;
}
std::string Align_Xml(const std::string& unalignedXml) {
    std::stringstream alignedXml;
    int indentationLevel = 0;

    for (char ch : unalignedXml) {
        if (ch == '<') {
            alignedXml << '\n' << std::string(indentationLevel, ' ');
            alignedXml << ch;
            indentationLevel += 2; // Increase the indentation for the next line
        } else if (ch == '>') {
            alignedXml << ch;
            indentationLevel -= 2; // Decrease the indentation for the closing tag
        } else {
            alignedXml << ch;
        }
    }

    return alignedXml.str();
}

std::string compressXml(const std::string& xml) {
    // Remove whitespace, indentation, and newlines
    std::string compressedXml = Xml_Minifying(xml);


    // Shorten tag names using a simple abbreviation map
    std::vector<std::vector<std::string>> abbreviationVector = {
            {"users", "u"},
            //{"</users>", "</u>"},
            {"user", "ur"},
            //{"</user>", "</ur>"},
            {"id", "i"},
            // {"</id>", "</i>"},
            {"name", "n"},
            // {"</name>", "</n>"},
            {"posts", "p"},
            // {"</posts>", "</p>"},
            {"post", "po"},
            //  {"</post>", "</po>"},
            {"body", "b"},
            // {"</body>", "</b>"},
            {"topics", "t"},
            // {"</topics>", "</t>"},
            {"topic", "to"},
            // {"</topic>", "</to>"},
            {"followers", "f"},
            // {"</followers>", "</f>"},
            {"follower", "fo"},
            // {"</follower>", "</fo>"}
    };

    for (const auto& entry : abbreviationVector) {
        size_t pos = 0;
        while ((pos = compressedXml.find(entry[0], pos)) != std::string::npos) {
            compressedXml.replace(pos, entry[0].length(), entry[1]);
        }
    }

    return compressedXml;
}

std::string decompressXml(const std::string& xml) {
    // Remove whitespace, indentation, and newlines
    string decompressed_xml=xml;

    // Shorten tag names using a simple abbreviation map
    std::vector<std::vector<std::string>> abbreviationVector = {
            {"u", "users"},
            //{"</u>", "</users>"},
            {"ur", "user"},
            //{"</ur>", "</user>"},
            {"i", "id"},
            //{"</i>", "</id>"},
            {"n", "name"},
            //{"</n>", "</name>"},
            {"p", "posts"},
            //{"</p>", "</posts>"},
            {"po", "post"},
            //{"</po>", "</post>"},
            {"b", "body"},
            //{"</b>", "</body>"},
            {"t", "topics"},
            //{"</t>", "</topics>"},
            {"to", "topic"},
            //{"</to>", "</topic>"},
            {"f", "followers"},
            //{"</f>", "</followers>"},
            {"fo", "follower"},
            //{"</fo>", "</follower>"}
    };

    for (const auto& entry : abbreviationVector) {
        size_t pos = 0;
        while ((pos = decompressed_xml.find(entry[0], pos)) != std::string::npos) {
            decompressed_xml.replace(pos, entry[0].length(), entry[1]);
        }
    }
//    string prefinal_xml=Align_Xml(decompressed_xml);
//    SimpleXMLParser parser;
//    vector<User> users = parser.parse(prefinal_xml);
//
//    string finalxml=Xml_Formatting(users);
    string aa =Align_Xml(decompressed_xml);
//    SimpleXMLParser parser;
//    vector<User> users = parser.parse(aa);

    string zz=Xml_Formatting(aa);
    return zz;
}


// Function to convert XML to JSON
string Xml_to_Json(const std::vector<User> users)
{

    int i1=0 , i2=0 , i3=0 , i4=0 ;
    string output;
    output.append("{\n");
    output.append("\t\"users\": [\n");

    for(int i=0;i<users.size();i++)
    {
        i4++;
        i1=0 ; i2=0 ; i3=0 ;
        output.append("\t\t{\n");
        output.append("\t\t\t\"id\": \"");output+=to_string(users[i].getID()); output.append("\",\n");
        output.append("\t\t\t\"name\": \"");output+=users[i].getName(); output.append("\",\n");

        output.append("\t\t\t\"posts\": [\n");
        for (const auto& post : users[i].getPosts())
        {
            i2++;
            output.append("\t\t\t\t{\n");
            output.append("\t\t\t\t\t\"body\": \"");output+=post.getBody();output.append("\",\n");
            output.append("\t\t\t\t\t\"topics\": [\n");
            for (const auto& topic : post.getTopics())
            {
                i1++;
                output.append("\t\t\t\t\t\t\""); output+=topic.name;
                if( !(i1==post.getTopics().size()) )
                {
                    output.append("\",\n");
                }
                else{
                    output.append("\"\n");
                    output.append("\t\t\t\t\t\t]\n");
                }
            }
            if( !(i2==users[i].getPosts().size()) )
            {
                output.append("\t\t\t\t},\n");
            }
            else{
                output.append("\t\t\t\t}\n");
            }
        }
        output.append("\t\t\t],\n");

        output.append("\t\t\t\"followers\": [\n");
        for (const auto& follower : users[i].getFollowers())
        {
            i3++;
            output.append("\t\t\t\t{\n");
            output.append("\t\t\t\t\t\"id\": \"");output+=to_string(follower.id);output.append("\"\n");
            if( !(i3==users[i].getFollowers().size()) )
            {
                output.append("\t\t\t\t},\n");
            }
            else{
                output.append("\t\t\t\t}\n");
            }
        }
        output.append("\t\t\t]\n");


        if( !(i4==users.size()) )
        {
            output.append("\t\t},\n");
        }
        else{
            output.append("\t\t}\n");
        }

    }
    output.append("\t]\n");
    output.append("}\n");


    return output;
}

string Json_Minifying(const std::vector<User> users)
{

    int i1=0 , i2=0 , i3=0 , i4=0 ;
    string output;
    output.append("{");
    output.append("\"users\": [");

    for(int i=0;i<users.size();i++)
    {
        i4++;
        i1=0 ; i2=0 ; i3=0 ;
        output.append("{");
        output.append("\"id\": \"");output+=to_string(users[i].getID()); output.append("\",");
        output.append("\"name\": \"");output+=users[i].getName(); output.append("\",");

        output.append("\"posts\": [");
        for (const auto& post : users[i].getPosts())
        {
            i2++;
            output.append("{");
            output.append("\"body\": \"");output+=post.getBody();output.append("\",");
            output.append("\"topics\": [");
            for (const auto& topic : post.getTopics())
            {
                i1++;
                output.append("\""); output+=topic.name;
                if( !(i1==post.getTopics().size()) )
                {
                    output.append("\",");
                }
                else{
                    output.append("\"");
                    output.append("]");
                }
            }
            if( !(i2==users[i].getPosts().size()) )
            {
                output.append("},");
            }
            else{
                output.append("}");
            }
        }
        output.append("],");

        output.append("\"followers\": [");
        for (const auto& follower : users[i].getFollowers())
        {
            i3++;
            output.append("{");
            output.append("\"id\": \"");output+=to_string(follower.id);output.append("\"");
            if( !(i3==users[i].getFollowers().size()) )
            {
                output.append("},");
            }
            else{
                output.append("}");
            }
        }
        output.append("]");


        if( !(i4==users.size()) )
        {
            output.append("},");
        }
        else{
            output.append("}");
        }

    }
    output.append("]");
    output.append("}");

    return output;
}

string Json_Formatting(std::string& json) {
    std::string formattedJson;
    int indentation = 0;

    for (char c : json) {
        if (c == '{' || c == '[') {
            formattedJson += c;
            formattedJson += '\n' + std::string(++indentation, '\t');
        } else if (c == '}' || c == ']') {
            formattedJson += '\n' + std::string(--indentation, '\t') + c;
        } else if (c == ',') {
            formattedJson += c;
            formattedJson += '\n' + std::string(indentation, '\t');
        } else {
            formattedJson += c;
        }
    }

    return formattedJson ;
}

std::string compressJson(const std::vector<User> users) {
    // Remove whitespace, indentation, and newlines
    std::string compressedJson = Json_Minifying(users);


    // Shorten tag names using a simple abbreviation map
    std::vector<std::vector<std::string>> abbreviationVector = {
            {"users", "u"},
            //{"</users>", "</u>"},
            {"user", "ur"},
            //{"</user>", "</ur>"},
            {"id", "i"},
            // {"</id>", "</i>"},
            {"name", "n"},
            // {"</name>", "</n>"},
            {"posts", "p"},
            // {"</posts>", "</p>"},
            {"post", "po"},
            //  {"</post>", "</po>"},
            {"body", "b"},
            // {"</body>", "</b>"},
            {"topics", "t"},
            // {"</topics>", "</t>"},
            {"topic", "to"},
            // {"</topic>", "</to>"},
            {"followers", "f"},
            // {"</followers>", "</f>"},
            {"follower", "fo"},
            // {"</follower>", "</fo>"}
    };

    for (const auto& entry : abbreviationVector) {
        size_t pos = 0;
        while ((pos = compressedJson.find(entry[0], pos)) != std::string::npos) {
            compressedJson.replace(pos, entry[0].length(), entry[1]);
        }
    }

    return compressedJson;
}

std::string decompressJson(const std::string& Json) {
    // Remove whitespace, indentation, and newlines
    string decompressed_Json=Json;

    // Shorten tag names using a simple abbreviation map
    std::vector<std::vector<std::string>> abbreviationVector = {
            {"\"u\"", "\"users\""},
            //{"</u>", "</users>"},
            {"\"ur\"", "\"user\""},
            //{"</ur>", "</user>"},
            {"\"i\"", "\"id\""},
            //{"</i>", "</id>"},
            {"\"n\"", "\"name\""},
            //{"</n>", "</name>"},
            {"\"p\"", "\"posts\""},
            //{"</p>", "</posts>"},
            {"\"po\"", "\"post\""},
            //{"</po>", "</post>"},
            {"\"b\"", "\"body\""},
            //{"</b>", "</body>"},
            {"\"t\"", "\"topics\""},
            //{"</t>", "</topics>"},
            {"\"to\"", "\"topic\""},
            //{"</to>", "</topic>"},
            {"\"f\"", "\"followers\""},
            //{"</f>", "</followers>"},
            {"\"fo\"", "\"follower\""},
            //{"</fo>", "</follower>"}
    };

    for (const auto& entry : abbreviationVector) {
        size_t pos = 0;
        while ((pos = decompressed_Json.find(entry[0], pos)) != std::string::npos) {
            decompressed_Json.replace(pos, entry[0].length(), entry[1]);
        }
    }

    string finaljson =Json_Formatting(decompressed_Json);

    return finaljson;
}


bool isTag(const string& str) {
    return !str.empty() && str[0] == '<' && str.back() == '>';
}

string extractTagName(const string& tag) {
    return tag.substr(1, tag.length() - 2);
}
//
//bool errorChecking(const string& xmlFilePath,string & errorMessages,string &beginminus) {
//    ifstream inputFile(xmlFilePath);
//
//    if (!inputFile.is_open()) {
//        return 0;
//    }
//
//    stack<string> tagStack;
//    string line;
//    string correctedXml;
//    ull pos =0;
//    ull lineNumber =0;
//
//    while (getline(inputFile, line)) {
//        lineNumber++;
//        pos =0;
//        if((pos = line.find("<users>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<users>") ;
//        }else if((pos = line.find("<user>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<user>") ;
//        }else if((pos = line.find("<posts>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<posts>") ;
//        }else if((pos = line.find("<post>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<post>") ;
//        }else if((pos = line.find("<topics>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<topics>") ;
//        }else if((pos = line.find("<topic>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<topic>") ;
//        }else if((pos = line.find("<followers>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<followers>") ;
//        }else if((pos = line.find("<follower>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<follower>") ;
//        }else if((pos = line.find("<body>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<body>") ;
//        }else if((pos = line.find("<id>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<id>") ;
//        }else if((pos = line.find("<name>", 0)) != string::npos){
//            tagStack.push(to_string(lineNumber));
//            tagStack.push("<name>") ;
//        }
////***********************************************************************
////****************************CLOSING TAGS*******************************
//        if((pos = line.find("</users>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<users>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</users>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//
//                return false ;
//            }
//        }
//
//
//        else if((pos = line.find("</user>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<user>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</user>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//        else if((pos = line.find("</posts>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<posts>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</posts>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//
//        else if((pos = line.find("</post>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<post>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</post>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//
//        }
//
//
//        else if((pos = line.find("</topics>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<topics>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</topics>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//        else if((pos = line.find("</topic>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<topic>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</topic>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//
//        else if((pos = line.find("</followers>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<followers>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</followers>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//        else if((pos = line.find("</follower>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<follower>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</follower>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//
//        else if((pos = line.find("</body>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<body>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</body>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//        else if((pos = line.find("</id>", 0)) != string::npos){
//            if(!tagStack.empty() && tagStack.top()=="<id>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</id>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//        }
//
//
//
//        else if((pos = line.find("</name>", 0)) != string::npos){
//
//            if(!tagStack.empty() && tagStack.top()=="<name>"){
//                tagStack.pop() ;//NO ERROR
//                tagStack.pop() ;
//
//            }else{
//                errorMessages+=tagStack.top() ;
//                tagStack.pop() ;
//                errorMessages+=tagStack.top() ;
//                errorMessages+="</name>"+ to_string(lineNumber) ;
//                tagStack.pop() ;
//                if(!tagStack.empty())
//                    beginminus=tagStack.top() ;
//                return false ;
//            }
//
//
//        }
//
//
//
//
//    }
//
//    if(tagStack.empty())
//        return true;
//    else{
//        while(!tagStack.empty()){
//            errorMessages+=tagStack.top();
//            tagStack.pop();
//            errorMessages+=tagStack.top() ;
//            tagStack.pop() ;
//        }
//        return false ;
//    }
//
//}
//
//vector<string> errorCorrecting (const string& xmlFilePath,string &errorMessages,string &beginminus){
//    fstream file(xmlFilePath, std::ios::in | std::ios::out);
//    vector<string> lines;
//    string linee;
//    if (!file.is_open()) {
//        return lines  ;
//    }
//    file.seekg(std::ios::beg);
//    while (getline(file, linee)) {
//        lines.push_back(linee);
//    }
//
//
//
//    stack<string> tagStack;
//    string line;
//    string correctedXml;
//    ull pos =0;
//    ull lineNumber =0;
//    string begin ;
//    int beginline=0;
//    string end ;
//    int endline ;
//    lines.push_back(" ");
//    lines[0]="<users>" ;
//    if((errorMessages.find("<users>", 0)) != string::npos) {
//        begin ="<users>";
//        beginline=stoi(errorMessages.substr(7,errorMessages.find("</",0)-7 )) ;
//    }
//    if((errorMessages.find("<user>", 0)) != string::npos) {
//        begin ="<user>";
//        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
//    }
//    if((errorMessages.find("<topics>", 0)) != string::npos) {
//        begin ="<topics>";
//        beginline=stoi(errorMessages.substr(8,errorMessages.find("</",0)-8 )) ;
//    }
//    if((errorMessages.find("<topic>", 0)) != string::npos) {
//        begin ="<topic>";
//        beginline=stoi(errorMessages.substr(7,errorMessages.find("</",0)-7 )) ;
//    }
//    if((errorMessages.find("<body>", 0)) != string::npos) {
//        begin ="<body>";
//        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
//    }
//    if((errorMessages.find("<followers>", 0)) != string::npos) {
//        begin ="<followers>";
//        beginline=stoi(errorMessages.substr(11,errorMessages.find("</",0)-11 )) ;
//    }
//    if((errorMessages.find("<follower>", 0)) != string::npos) {
//        begin ="<follower>";
//        beginline=stoi(errorMessages.substr(10,errorMessages.find("</",0)-10 )) ;
//    }
//    if((errorMessages.find("<posts>", 0)) != string::npos) {
//        begin ="<posts>";
//        beginline=stoi(errorMessages.substr(7,errorMessages.find("</",0)-7 )) ;
//    }
//    if((errorMessages.find("<post>", 0)) != string::npos) {
//        begin ="<post>";
//        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
//    }
//    if((errorMessages.find("<id>", 0)) != string::npos) {
//        begin ="<id>";
//        beginline=stoi(errorMessages.substr(4,errorMessages.find("</",0)-4 )) ;
//    }
//    if((errorMessages.find("<name>", 0)) != string::npos) {
//        begin ="<name>";
//        beginline=stoi(errorMessages.substr(6,errorMessages.find("</",0)-6 )) ;
//    }
//    //*************************************************************************************************************************************************
//    //*************************************************************************************************************************************************
//    if((errorMessages.find("</users>", 0)) != string::npos) {
//        end ="</users>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</users>", 0))+8)) ;
//    }
//    if((errorMessages.find("</user>", 0)) != string::npos) {
//        end ="</user>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</user>", 0))+7)) ;
//    }
//    if((errorMessages.find("</posts>", 0)) != string::npos) {
//        end ="</posts>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</posts>", 0))+8)) ;
//    }
//    if((errorMessages.find("</post>", 0)) != string::npos) {
//        end ="</post>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</post>", 0))+7)) ;
//    }
//    if((errorMessages.find("</id>", 0)) != string::npos) {
//        end ="</id>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</id>", 0))+5)) ;
//    }
//    if((errorMessages.find("</topics>", 0)) != string::npos) {
//        end ="</topics>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</topics>", 0))+9)) ;
//    }
//    if((errorMessages.find("</topic>", 0)) != string::npos) {
//        end ="</topic>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</topic>", 0))+8)) ;
//    }if((errorMessages.find("</name>", 0)) != string::npos) {
//        end ="</name>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</name>", 0))+7)) ;
//    }if((errorMessages.find("</followers>", 0)) != string::npos) {
//        end ="</followers>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</followers>", 0))+12)) ;
//    }
//    if((errorMessages.find("</follower>", 0)) != string::npos) {
//        end ="</follower>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</follower>", 0))+11)) ;
//    }
//    if((errorMessages.find("</body>", 0)) != string::npos) {
//        end ="</body>";
//        endline=stoi(errorMessages.substr((errorMessages.find("</body>", 0))+7)) ;
//    }
//
////    lines[beginline]="\t\t\t\t\t<topic***********************>" ;
////    file.close();
////    file.open(xmlFilePath, std::ios::out | std::ios::trunc);
////    for (const auto &modifiedLine : lines) {
////        file << modifiedLine<<endl ;
////    }
//    if((begin=="<topics>" && beginminus=="<post>" && (lines[beginline-2].find("</body>",0)!=string::npos))
//       ||(beginminus=="<topics>"&& begin!="<topic>" &&end=="</topic>" )){
//
//        if((beginminus=="<topics>"&& begin!="<topic>" &&end=="</topic>" )){lines[beginline-1]="\t\t\t\t\t<topic>";
//            file.close();
//            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
//            while (lines.back()==" "){
//                lines.pop_back() ;
//            }
//            for (const auto &modifiedLine : lines) {
//
//                file << modifiedLine<<endl ;
//            }
//            return lines ;}
//        lines[beginline]="\t\t\t\t\t<topic>" ;
//        lines[beginline+2]="\t\t\t\t\t</topic>" ;
//        beginline+=3;
//        while(beginline<=endline){
//            if(lines[beginline].find("</topics>",0)!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
//                break;
//            }else{
//                lines[beginline]="\t\t\t\t\t<topic>" ;
//                lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                beginline+=3;
//            }
//        }
//        lines[beginline]="\t\t\t\t</topics>";
//        lines[beginline+1]="\t\t\t</post>";
//
//    }
//
//
//
//    else if (begin=="<topic>"&&beginminus=="<topics>"){
//        lines[beginline+1]="\t\t\t\t\t</topic>" ;
//    }
//
//
//
//    else if(begin=="<body>"&&beginminus=="<post>"){
//        lines[beginline+1]="\t\t\t\t</body>" ;
//    }
//
//
//    else if (begin=="<id>"&& ( beginminus=="<user>" || beginminus=="<follower>" ) &&(lines[beginline-2]=="    <user>" || lines[beginline-2]=="            <follower>")){
//        ull start = lines[beginline-1].find("<id>")+4 ;
//        string id ;
//        if(lines[beginline-1].find("</")==string::npos){
//            id=lines[beginline-1].substr(start) ;
//        }else{
//            ull end =lines[beginline-1].find("</") ;
//            id=lines[beginline-1].substr(start,end-start);
//        }
//        if(beginminus=="<follower>")
//            lines[beginline-1]="\t\t\t\t<id>"+id+"</id>";
//        else
//            lines[beginline-1]="\t\t<id>"+id+"</id>";
//    }
//
//
//
//    else if (begin=="<name>" && beginminus=="<user>" && lines[beginline-3]=="\t<user>"){
//        ull start = lines[beginline-1].find("<name>")+6 ;
//        string name ;
//        if(lines[beginline-1].find("</")==string::npos){
//            name=lines[beginline-1].substr(start) ;
//        }else{
//            ull end =lines[beginline-1].find("</") ;
//            name=lines[beginline-1].substr(start,end-start);
//
//        }
//        lines[beginline-1]="\t\t<name>"+name+"</name>";
//    }
//
//
//
//
//    else if (( begin=="<post>"  &&  beginminus=="<posts>" ) ||
//             (beginminus=="<post>"&& begin!="<body>" &&end=="</body>" )
//             || (beginminus=="<post>"&& begin!="<topics>" &&end=="</topics>" )  ){
//        if((beginminus=="<post>"&& begin!="<body>" &&end=="</body>" )){beginline--;}
//        if((beginminus=="<post>"&& begin!="<topics>" &&end=="</topics>" ) ){beginline-=4;}
//        lines[beginline]="\t\t\t\t<body>" ;
//        beginline+=2;
//        lines[beginline]="\t\t\t\t</body>" ;
//        beginline++;
//        lines[beginline]="\t\t\t\t<topics>" ;
//        beginline++;
//        while(1){
//            if( lines[beginline].find("</topics>")!=string::npos || lines[beginline+1].find("</post>")!=string::npos ){
//                break;
//            }else{
//                lines[beginline]="\t\t\t\t\t<topic>" ;
//                lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                beginline+=3;
//            }
//        }
//        lines[beginline]="\t\t\t\t</topics>";
//        lines[beginline+1]="\t\t\t</post>";
//
//
//    }
//
//
//    else if ((begin=="<posts>" && beginminus=="<user>" &&  (lines[beginline-2].find("</name>",0)!=string::npos))
//             ||(beginminus=="<posts>" && begin!="<post>" && end=="</post>")){
//        if((beginminus=="<posts>" && begin!="<post>" && end=="</post>")){beginline--;}
//        lines[beginline]="\t\t\t<post>" ;
//        beginline++;
//        lines[beginline]="\t\t\t\t<body>" ;
//        beginline+=2;
//        lines[beginline]="\t\t\t\t</body>" ;
//        beginline++;
//        lines[beginline]="\t\t\t\t<topics>" ;
//        beginline++;
//        while(1){
//            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos|| lines[beginline].find("</post>")!=string::npos ){
//                break;
//            }else{
//                lines[beginline]="\t\t\t\t\t<topic>" ;
//                lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                beginline+=3;
//            }
//        }
//        lines[beginline]="\t\t\t\t</topics>";
//        lines[beginline+1]="\t\t\t</post>";
//        beginline+=2 ;
//        while(1){
//            if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos ){
//                break ;
//            }else{
//                lines[beginline]="\t\t\t<post>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t<body>" ;
//                beginline+=2;
//                lines[beginline]="\t\t\t\t</body>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t<topics>" ;
//                beginline++;
//                while(1){
//                    if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
//                        break;
//                    }
//                    else{
//                        lines[beginline]="\t\t\t\t\t<topic>" ;
//                        lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                        beginline+=3;
//                    }
//                }
//                lines[beginline]="\t\t\t\t</topics>";
//                lines[beginline+1]="\t\t\t</post>";
//                beginline+=2;
//            }
//        }
//        lines[beginline]="\t\t</posts>";
//
//
//    }
//
//
//    else if ((begin=="<follower>" && beginminus=="<followers>")
//             ||(beginminus=="<follower>"&&begin!="<id>" &&end=="</id>" )){
//
//        if((beginminus=="<follower>"&&begin!="<id>" &&end=="</id>")){beginline--;}
//
//        char c =lines[beginline][4] ;
//        int start=0;
//        if('1'<=c&&c<='9'){
//            start=4;
//        }else{
//            start=lines[beginline].find(">")+1 ;
//
//        }
//
//
//        string id ;
//        if(lines[beginline].find("</")==string::npos){
//            id=lines[beginline].substr(start) ;
//        }else{
//            ull end =lines[beginline].find("</") ;
//            id=lines[beginline].substr(start,end-start);
//        }
//
//        lines[beginline]="\t\t\t\t<id>"+id+"</id>";
//        lines[beginline+1]="\t\t\t</follower>";
//
//    }
//
//
//    else if((begin=="<followers>" && beginminus=="<user>" &&  (lines[beginline-2].find("</posts>",0)!=string::npos) )
//            || (beginminus=="<followers>" && begin!="<follower>"  && end=="</follower>" )){
//
//        if((beginminus=="<followers>" && begin!="<follower>"  && end=="</follower>" )){beginline--;}
//        lines[beginline]="\t\t\t<follower>";
//        beginline++;
//        //********************WATCHOUT copied code **************************
//        char c =lines[beginline][4] ;
//        int start=0;
//        if('1'<=c&&c<='9'){
//            start=4;
//        }else{
//            start=lines[beginline].find(">")+1 ;
//
//        }
//
//
//        string id ;
//        if(lines[beginline].find("</")==string::npos){
//            id=lines[beginline].substr(start) ;
//        }else{
//            ull end =lines[beginline].find("</") ;
//            id=lines[beginline].substr(start,end-start);
//        }
//
//        lines[beginline]="\t\t\t\t<id>"+id+"</id>";
//        //********************************************************************
//        beginline++;
//        lines[beginline]="\t\t\t</follower>";
//        beginline++;
//        while(1){
//            if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos) {
//                break;
//            }else{
//                lines[beginline]="\t\t\t<follower>";
//                beginline++;
//                //********************WATCHOUT copied code **************************
//                char c =lines[beginline][4] ;
//                int start=0;
//                if('1'<=c&&c<='9'){
//                    start=4;
//                }else{
//                    start=lines[beginline].find(">")+1 ;
//
//                }
//
//
//                string id ;
//                if(lines[beginline].find("</")==string::npos){
//                    id=lines[beginline].substr(start) ;
//                }else{
//                    ull end =lines[beginline].find("</") ;
//                    id=lines[beginline].substr(start,end-start);
//                }
//
//                lines[beginline]="\t\t\t\t<id>"+id+"</id>";
//                //********************************************************************
//                beginline++;
//                lines[beginline]="\t\t\t</follower>";
//                beginline++;
//            }
//        }
//        lines[beginline]="\t\t</followers>" ;
//        beginline++;
//        lines[beginline]="\t</user>" ;
//
//
//    }
//
//
//    else if ((begin=="<user>" && beginminus=="<users>" )
//             || (beginminus=="<user>" && begin!="<id>"  && end=="</id>" )
//             || (beginminus=="<user>" && begin!="<name>"  && end=="</name>")
//             || (beginminus=="<user>" && begin!="<posts>"  && end=="</posts>")
//             || ((beginminus=="<user>" && begin!="<followers>"  && end=="</followers>")) ){
//
//        if((beginminus=="<user>" && begin!="<id>"  && end=="</id>" )){beginline--;}
//        if((beginminus=="<user>" && begin!="<name>"  && end=="</name>")){beginline-=2;}
//        if( (beginminus=="<user>" && begin!="<posts>"  && end=="</posts>")){beginline-=3;}
//        if((beginminus=="<user>" && begin!="<followers>"  && end=="</followers>")){
//            lines[beginline-1]="        <followers>" ;
//            file.close();
//            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
//            while (lines.back()==" "){
//                lines.pop_back() ;
//            }
//            for (const auto &modifiedLine : lines) {
//
//                file << modifiedLine<<endl ;
//            }
//            return  lines;
//        }
//        beginline--;
//        lines[beginline]="\t<user>" ;
//        beginline++;
//
//        //********************WATCHOUT copied code **************************
//        char c =lines[beginline][2] ;
//        int start=0;
//        if('1'<=c&&c<='9'){
//            start=2;
//        }else{
//            start=lines[beginline].find(">")+1 ;
//
//        }
//
//
//        string id ;
//        if(lines[beginline].find("</")==string::npos){
//            id=lines[beginline].substr(start) ;
//        }else{
//            ull end =lines[beginline].find("</") ;
//            id=lines[beginline].substr(start,end-start);
//        }
//
//        lines[beginline]="\t\t<id>"+id+"</id>";
//        beginline++;
//        //**********************NAME******************************
//        char b =lines[beginline][2] ;
//        int startt=0;
//        if(b!='<'){
//            startt=2;
//        }else{
//            startt=lines[beginline].find(">")+1 ;
//
//        }
//
//        string nn ;
//        if(lines[beginline].find("</")==string::npos){
//            nn=lines[beginline].substr(startt) ;
//        }else{
//            ull end =lines[beginline].find("</") ;
//            nn=lines[beginline].substr(startt,end-startt);
//        }
//
//        lines[beginline]="\t\t<name>"+nn+"</name>";
//
//        beginline++;
//
//        //**************************POSTS******************************************
//        lines[beginline]="\t\t<posts>" ;
//        beginline++;
//        lines[beginline]="\t\t\t<post>" ;
//        beginline++;
//        lines[beginline]="\t\t\t\t<body>" ;
//        beginline+=2;
//        lines[beginline]="\t\t\t\t</body>" ;
//        beginline++;
//        lines[beginline]="\t\t\t\t<topics>" ;
//        beginline++;
//        while(1){
//            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
//                break;
//            }else{
//                lines[beginline]="\t\t\t\t\t<topic>" ;
//                lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                beginline+=3;
//            }
//        }
//        lines[beginline]="\t\t\t\t</topics>";
//        lines[beginline+1]="\t\t\t</post>";
//        beginline+=2 ;
//        while(1){
//            if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos){
//                break ;
//            }else{
//                lines[beginline]="\t\t\t<post>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t<body>" ;
//                beginline+=2;
//                lines[beginline]="\t\t\t\t</body>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t<topics>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t\t<topic>" ;//////////////
//                lines[beginline+2]="\t\t\t\t\t</topic>" ;////////////////////////
//                beginline+=3;//////////////////////
//                while(1){
//                    if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
//                        break;
//                    }else{
//                        lines[beginline]="\t\t\t\t\t<topic>" ;
//                        lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                        beginline+=3;
//                    }
//                }
//                lines[beginline]="\t\t\t\t</topics>";
//                lines[beginline+1]="\t\t\t</post>";
//                beginline+=2;
//            }
//        }
//        lines[beginline]="\t\t</posts>";
//        beginline++;
//        //***************************************************
//        //FOLLOWERS******************************************
//        lines[beginline]="\t\t<followers>" ;
//        beginline++;
//        lines[beginline]="\t\t\t<follower>";
//        beginline++;
//        //********************WATCHOUT copied code(ID inside followers) **************************
//        char cc =lines[beginline][4] ;
//        int starttt=0;
//        if('1'<=cc&&cc<='9'){
//            starttt=4;
//        }else{
//            starttt=lines[beginline].find(">")+1;
//        }
//
//        string idd ;
//        if(lines[beginline].find("</")==string::npos){
//            idd=lines[beginline].substr(starttt) ;
//        }else{
//            ull end =lines[beginline].find("</") ;
//            idd=lines[beginline].substr(starttt,end-starttt);
//        }
//
//        lines[beginline]="\t\t\t\t<id>"+idd+"</id>";
////**********************FOLLOWER*********************************
//        beginline++;
//        lines[beginline]="\t\t\t</follower>";
//        beginline++;
//        while(1){
//            if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos  ) {
//                break;
//            }else{
//                lines[beginline]="\t\t\t<follower>";
//                beginline++;
//                //********************WATCHOUT copied code **************************
//                char c =lines[beginline][4] ;
//                int start=0;
//                if('1'<=c&&c<='9'){
//                    start=4;
//                }else{
//                    start=lines[beginline].find(">")+1 ;
//                }
//
//                string id ;
//                if(lines[beginline].find("</")==string::npos){
//                    id=lines[beginline].substr(start) ;
//                }else{
//                    ull end =lines[beginline].find("</") ;
//                    id=lines[beginline].substr(start,end-start);
//                }
//
//                lines[beginline]="\t\t\t\t<id>"+id+"</id>";
//                //********************************************************************
//                beginline++;
//                lines[beginline]="\t\t\t</follower>";
//                beginline++;
//            }
//        }
//        lines[beginline]="\t\t</followers>" ;
//        beginline++;
//        lines[beginline]="\t</user>" ;
//    }
//
//
//    else if ((begin=="<users>")
//             || (beginminus =="<users>" && begin!="<user>" && end=="</user>" )
//             ||( begin=="<users>"&& end=="</user>") ){
//        if((beginminus =="<users>" && begin!="<user>" && end=="</user>")){
//            lines[beginline-1]="\t<user>" ;
//            file.close();
//            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
//            while (lines.back()==" "){
//                lines.pop_back() ;
//            }
//            for (const auto &modifiedLine : lines) {
//
//                file << modifiedLine<<endl ;
//            }
//            return lines;
//        }
//        while(1){
//            if(beginline>(lines.size()-3))
//                break;
//            else{
//                lines[beginline]="\t<user>" ;
//                beginline++;
//
//                //********************WATCHOUT copied code **************************
//                char c =lines[beginline][2] ;
//                int start=0;
//                if('1'<=c&&c<='9'){
//                    start=2;
//                }else{
//                    start=lines[beginline].find(">")+1 ;
//
//                }
//
//
//                string id ;
//                if(lines[beginline].find("</")==string::npos){
//                    id=lines[beginline].substr(start) ;
//                }else{
//                    ull end =lines[beginline].find("</") ;
//                    id=lines[beginline].substr(start,end-start);
//                }
//
//                lines[beginline]="\t\t<id>"+id+"</id>";
//                beginline++;
//                //**********************NAME******************************
//                char b =lines[beginline][2] ;
//                int startt=0;
//                if(c!='<'){
//                    startt=2;
//                }else{
//                    startt=lines[beginline].find(">")+1 ;
//
//                }
//
//                string nn ;
//                if(lines[beginline].find("</")==string::npos){
//                    nn=lines[beginline].substr(startt) ;
//                }else{
//                    ull end =lines[beginline].find("</") ;
//                    nn=lines[beginline].substr(startt,end-startt);
//                }
//
//                lines[beginline]="\t\t<name>"+nn+"</name>";
//
//                beginline++;
//
//                //**************************POSTS******************************************
//                lines[beginline]="\t\t<posts>" ;
//                beginline++;
//                lines[beginline]="\t\t\t<post>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t<body>" ;
//                beginline+=2;
//                lines[beginline]="\t\t\t\t</body>" ;
//                beginline++;
//                lines[beginline]="\t\t\t\t<topics>" ;
//                beginline++;
//                while(1){
//                    if(lines[beginline].find("</topics>")!=string::npos ||lines[beginline+1].find("</post>")!=string::npos||lines[beginline].find("</post>")!=string::npos){
//                        break;
//                    }else{
//                        lines[beginline]="\t\t\t\t\t<topic>" ;
//                        lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                        beginline+=3;
//                    }
//                }
//                lines[beginline]="\t\t\t\t</topics>";
//                lines[beginline+1]="\t\t\t</post>";
//                beginline+=2 ;
//                while(1){
//                    if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos){
//                        break ;
//                    }else{
//                        lines[beginline]="\t\t\t<post>" ;
//                        beginline++;
//                        lines[beginline]="\t\t\t\t<body>" ;
//                        beginline+=2;
//                        lines[beginline]="\t\t\t\t</body>" ;
//                        beginline++;
//                        lines[beginline]="\t\t\t\t<topics>" ;
//                        beginline++;
//                        lines[beginline]="\t\t\t\t\t<topic>" ;//////////////
//                        lines[beginline+2]="\t\t\t\t\t</topic>" ;////////////////////////
//                        beginline+=3;//////////////////////
//                        while(1){
//                            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
//                                break;
//                            }else{
//                                lines[beginline]="\t\t\t\t\t<topic>"    ;
//                                lines[beginline+2]="\t\t\t\t\t</topic>" ;
//                                beginline+=3;
//                            }
//                        }
//                        lines[beginline]="\t\t\t\t</topics>";
//                        lines[beginline+1]="\t\t\t</post>";
//                        beginline+=2;
//                    }
//                }
//                lines[beginline]="\t\t</posts>";
//                beginline++;
//                //***************************************************
//                //FOLLOWERS******************************************
//                lines[beginline]="\t\t<followers>" ;
//                beginline++;
//                lines[beginline]="\t\t\t<follower>";
//                beginline++;
//                //********************WATCHOUT copied code(ID inside followers) **************************
//                char cc =lines[beginline][4] ;
//                int starttt=0;
//                if('1'<=cc&&cc<='9'){
//                    starttt=4;
//                }else{
//                    starttt=lines[beginline].find(">")+1;
//                }
//
//                string idd ;
//                if(lines[beginline].find("</")==string::npos){
//                    idd=lines[beginline].substr(starttt) ;
//                }else{
//                    ull end =lines[beginline].find("</") ;
//                    idd=lines[beginline].substr(starttt,end-starttt);
//                }
//
//                lines[beginline]="\t\t\t\t<id>"+idd+"</id>";
////**********************FOLLOWER*********************************
//                beginline++;
//                lines[beginline]="\t\t\t</follower>";
//                beginline++;
//                while(1){
//                    if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos) {
//                        break;
//                    }else{
//                        lines[beginline]="\t\t\t<follower>";
//                        beginline++;
//                        //********************WATCHOUT copied code **************************
//                        char c =lines[beginline][4] ;
//                        int start=0;
//                        if('1'<=c&&c<='9'){
//                            start=4;
//                        }else{
//                            start=lines[beginline].find(">")+1 ;
//                        }
//
//                        string id ;
//                        if(lines[beginline].find("</")==string::npos){
//                            id=lines[beginline].substr(start) ;
//                        }else{
//                            ull end =lines[beginline].find("</") ;
//                            id=lines[beginline].substr(start,end-start);
//                        }
//
//                        lines[beginline]="\t\t\t\t<id>"+id+"</id>";
//                        //********************************************************************
//                        beginline++;
//                        lines[beginline]="\t\t\t</follower>";
//                        beginline++;
//                    }
//                }
//                lines[beginline]="\t\t</followers>" ;
//                beginline++;
//                lines[beginline]="\t</user>" ;
//                beginline++;
//            }
//
//
//        }
//
//        lines[beginline]="</users>";
//
//
//
//    }
//
//
//
//    file.close();
//    file.open(xmlFilePath, std::ios::out | std::ios::trunc);
//    while (lines.back()==" "){
//        lines.pop_back() ;
//    }
//
//    for (const auto &modifiedLine : lines) {
//
//        file << modifiedLine<<endl ;
//
//    }
//    return lines ;
//}

string removeSpaces(const std::string& str) {
    std::string result;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(result), ::isspace);
    return result;
}

bool areLinesIdentical(const std::string& line1, const std::string& line2) {
    return removeSpaces(line1) == removeSpaces(line2);
}
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

string CorrectXML(const string& RealxmlFilePath){
    string errorMessages ;
    string beginminus ;
    vector <string>lines;
    bool result=errorChecking(RealxmlFilePath,errorMessages,beginminus);
    while(result!=true ){
        lines=errorCorrecting(RealxmlFilePath,errorMessages,beginminus);
        errorMessages="";
        result = errorChecking(RealxmlFilePath,errorMessages,beginminus);
    }
    string ans  ;
    for( auto s : lines){
        ans+=s ;
    }

    return ans ;

}


vector<vector<int>> generateGraph(vector<User> users){

    vector<vector<int>> NetworkGraph(10000) ;

    for ( auto user : users ) {
        for (auto follower : user.getFollowers()){
            NetworkGraph[user.getID()][follower.id] =1 ;
        }
    }
    return NetworkGraph ;

}

vector <int> calculateIndegree (vector<vector<int>> NetworkGraph ){
    int size =NetworkGraph.size();
    vector<int> Indegree (size , 0) ;
    for (auto node : NetworkGraph){
        for ( int i =0 ; i<node.size() ; i++){
            if(node[i]==1){
                Indegree[i]++;
            }
        }
    }
    return Indegree ;
}
int mostInfluencerUser(vector<int> Indegree){
    int maxUser =0 ;
    int maxIn =0 ;
    for (int i =0 ; i< Indegree.size() ; i++){
        if(Indegree[i]>=maxIn){
            maxIn=Indegree[i];
            maxUser=i ;
        }
    }
    return maxUser;
}

int most_Active_user(vector<User> users)
{
    int max=0;
    for(int i=0 ; i<users.size() ;i++)
    {
        if(users[i].getFollowers().size() > max)
            max = users[i].getFollowers().size();
    }
    return max;
}

vector<int> mutualFollowers (int ID1 , int ID2 ,vector<vector<int>> NetworkGraph){
    vector <int> mutual ;
    for(int i =0 ; i< NetworkGraph.size() ;i++){
        if(NetworkGraph[ID1][i]==1 && NetworkGraph[ID2][i]==1){
            mutual.push_back(i) ;
        }
    }
    return mutual ;
}

vector<int> suggestUsers(int ID,vector<vector<int>> NetworkGraph){
    vector <int> suggested ;
    for (int i =0 ; i<NetworkGraph.size() ; i++){
        if(NetworkGraph[ID][i]==1){
            for (int j =0 ; j< NetworkGraph.size() ;j++){
                if(NetworkGraph[i][j]==1 && NetworkGraph[ID][j]!=1){
                    suggested.push_back(j) ;
                }
            }
        }
    }
    return suggested ;
}

vector <pair<Post,int>> postSearch (string topic ,vector<User> users){
    vector <pair<Post,int>> result ;
    for(auto user : users ){
        for(auto post : user.getPosts()){
            for (auto t :post.getTopics()){
                if(t.name==topic ){
                    result.push_back({post,user.getID()});
                }
            }
        }
    }

    return result ;
}
