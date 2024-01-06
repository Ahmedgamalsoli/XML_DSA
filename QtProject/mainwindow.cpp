#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <zlib.h>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <QStack>
#include <stack>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

typedef unsigned long long ull;
using namespace std;

class Topic {
public:
    std::string name;
};
class Post {
private:
    std::string body;
    std::vector<Topic> topics;

public:
    // Constructor
    Post() {}

    // Getter and Setter declarations
    std::string getBody() const {
        return body;
    }

    void setBody(const std::string& newBody) {
        body = newBody;
    }

    std::vector<Topic>& getTopics() const {
        return (std::vector<Topic>&)(topics);
    }

    void setTopics(const std::vector<Topic>& newTopics) {
        topics = newTopics;
    }
};
class Follower {
public:
    int id;
};
class User {
private:
    int id;
    std::string name;
    std::vector<Post> posts;
    std::vector<Follower> followers;

public:

    // Constructor
    User() {}

    // Getter and Setter declarations
    int getID() const {
        return id;
    }

    void setID(int newID) {
        id = newID;
    }

    std::string getName() const {
        return name;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    Post getPosts(int i) const {
        return posts[i];
    }

    std::vector<Post> & getPosts() const {
        return (std::vector<Post> &) (posts);
    }


    void setPosts(const std::vector<Post>& newPosts) {
        posts = newPosts;
    }

    Follower getFollowers(int i) const {
        return followers[i];
    }

    std::vector<Follower>& getFollowers() const {
        return (std::vector<Follower> &) (followers);
    }

    void setFollowers(const std::vector<Follower>& newFollowers) {
        followers = newFollowers;
    }

    // Additional methods
    void addPost(const Post& newPost) {
        posts.push_back(newPost);
    }

    void addFollower(const Follower& newFollower) {
        followers.push_back(newFollower);
    }
};
class SimpleXMLParser {
public:
    vector<User> parse(const string& xmlContent) {
        //        ifstream file(xmlFilePath);
        //        stringstream buffer;
        //        buffer << file.rdbuf();

        vector<User> users;

        //  string xmlContent = buffer.str();
        ull pos = 0;

        while ((pos = xmlContent.find("<user>", pos)) != string::npos) {
            User user;
            user.setID(extractIntField(xmlContent, "id", pos));
            user.setName(extractField(xmlContent, "name", pos));

            // Parse posts
            ull postsStart = xmlContent.find("<posts>", pos);
            ull postsEnd = xmlContent.find("</posts>", postsStart);
            string postsContent = xmlContent.substr(postsStart, postsEnd - postsStart);

            parsePosts(postsContent, user.getPosts());

            // Parse followers
            ull followersStart = xmlContent.find("<followers>", pos);
            ull followersEnd = xmlContent.find("</followers>", followersStart);
            string followersContent = xmlContent.substr(followersStart, followersEnd - followersStart);

            parseFollowers(followersContent, user.getFollowers());

            users.push_back(user);

            // Move to the next position to search for the next user
            pos = followersEnd;
        }

        return users;
    }

private:
    string extractField(const string& content, const string& fieldName, ull startPos = 0) {
        ull start ;
        ull end   ;

        if(fieldName=="topic"){
            startPos+=fieldName.length()+2;
            while(!isalpha(content[startPos]))
            {
                startPos++;
            }
            start = startPos ;
            //            while(isalpha(content[startPos]))
            //            {
            //                startPos++;
            //            }
            while( 1 )
            {
                startPos++;
                if((content[startPos]=='<'))
                    if((content[startPos+1]=='/') )
                        break;
                if((content[startPos]=='\n'))
                    break;
                if((content[startPos]==' '))
                    if((content[startPos+1]==' ') )
                        break;
            }
            end = startPos;
        }
        else if(fieldName=="body"){
            startPos = content.find("<body>", startPos);
            startPos+=fieldName.length()+2;
            while(!isalpha(content[startPos]))
            {
                startPos++;
            }
            start = startPos ;
            while( 1 )
            {
                startPos++;
                if((content[startPos]=='<'))
                    if((content[startPos+1]=='/') )
                        break;
                if((content[startPos]=='\n'))
                    break;
                if((content[startPos]==' '))
                    if((content[startPos+1]==' ') )
                        break;
            }
            end = startPos;
        }
        else{
            startPos = content.find("<" + fieldName + ">", startPos) + fieldName.length() + 2;
            // end = content.find("</" + fieldName + ">", start);
            while(!isalpha(content[startPos]))
            {
                startPos++;
            }
            start = startPos ;
            while( 1 )
            {
                startPos++;
                if((content[startPos]=='<'))
                    if((content[startPos+1]=='/') )
                        break;
                if((content[startPos]=='\n'))
                    break;
                if((content[startPos]==' '))
                    if((content[startPos+1]==' ') )
                        break;
            }
            end = startPos;
        }
        return content.substr(start, end - start);
    }

    int extractIntField(const string& content, const string& fieldName, ull startPos = 0) {
        ull start = content.find("<" + fieldName + ">", startPos) + fieldName.length() + 2;
        ull end = content.find("</" + fieldName + ">", start);
        return stoi(content.substr(start, end - start));
    }

    void parsePosts(const string& content, vector<Post>& posts) {
        ull pos = 0;
        while ((pos = content.find("<post>", pos)) != string::npos) {
            Post post;
            //pos = content.find("<body>", pos);
            post.setBody(extractField(content, "body", pos));

            ull topicsStart = content.find("<topics>", pos);
            ull topicsEnd = content.find("</topics>", topicsStart);
            string topicsContent = content.substr(topicsStart, topicsEnd - topicsStart);

            parseTopics(topicsContent, post.getTopics());

            posts.push_back(post);
            pos = content.find("</post>", pos) + 7;
        }
    }

    void parseFollowers(const string& content, vector<Follower>& followers) {
        ull pos = 0;
        while ((pos = content.find("<follower>", pos)) != string::npos) {
            Follower follower;
            follower.id = extractIntField(content, "id", pos);
            followers.push_back(follower);
            pos = content.find("</follower>", pos) + 10;
        }
    }

    void parseTopics(const string& content, vector<Topic>& topics) {
        ull pos = 0;
        while ((pos = content.find("<topic>", pos)) != string::npos) {
            Topic topic;
            topic.name = extractField(content, "topic", pos);
            topics.push_back(topic);
            pos = content.find("</topic>", pos) + 8;
        }





    }

};
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
        output.append("    <user>\n");
        output.append("        <id>");output+=to_string(users[i].getID()); output.append("</id>\n");
        output.append("        <name>");output+=users[i].getName(); output.append("</name>\n");

        output.append("        <posts>\n");
        for (const auto& post : users[i].getPosts())
        {
            i2++;
            output.append("            <post>\n");
            output.append("                <body>\n");output.append("                   ");output+=post.getBody();output.append("\n");output.append("                </body>\n");
            output.append("                <topics>\n");
            for (const auto& topic : post.getTopics())
            {
                i1++;
                output.append("                    <topic>\n");output.append("                         "); output+=topic.name;output.append("\n");output.append("                    </topic>\n");

            }
            output.append("                </topics>\n");
            output.append("            </post>\n");
        }
        output.append("        </posts>\n");

        output.append("        <followers>\n");
        for (const auto& follower : users[i].getFollowers())
        {
            i3++;
            output.append("            <follower>\n");
            output.append("                <id>");output+=to_string(follower.id);output.append("</id>\n");
            output.append("            </follower>\n");

        }
        output.append("        </followers>\n");



        output.append("    </user>\n");

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
        {"<users>", "<u>"},
        {"</users>", "</u>"},
        {"<user>", "<ur>"},
        {"</user>", "</ur>"},
        {"<id>", "<i>"},
        {"</id>", "</i>"},
        {"<name>", "<n>"},
        {"</name>", "</n>"},
        {"<posts>", "<p>"},
        {"</posts>", "</p>"},
        {"<post>", "<po>"},
        {"</post>", "</po>"},
        {"<body>", "<b>"},
        {"</body>", "</b>"},
        {"<topics>", "<t>"},
        {"</topics>", "</t>"},
        {"<topic>", "<to>"},
        {"</topic>", "</to>"},
        {"<followers>", "<f>"},
        {"</followers>", "</f>"},
        {"<follower>", "<fo>"},
        {"</follower>", "</fo>"}
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
        {"<u>", "<users>"},
        {"</u>", "</users>"},
        {"<ur>", "<user>"},
        {"</ur>", "</user>"},
        {"<i>", "<id>"},
        {"</i>", "</id>"},
        {"<n>", "<name>"},
        {"</n>", "</name>"},
        {"<p>", "<posts>"},
        {"</p>", "</posts>"},
        {"<po>", "<post>"},
        {"</po>", "</post>"},
        {"<b>", "<body>"},
        {"</b>", "</body>"},
        {"<t>", "<topics>"},
        {"</t>", "</topics>"},
        {"<to>", "<topic>"},
        {"</to>", "</topic>"},
        {"<f>", "<followers>"},
        {"</f>", "</followers>"},
        {"<fo>", "<follower>"},
        {"</fo>", "</follower>"}
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
string Xml_to_Json(vector<User> users)
{
    int i1=0 , i2=0 , i3=0 , i4=0 ;
    string output;
    output.append("{\n");
    output.append("    \"users\": [\n");

    for(int i=0;i<users.size();i++)
    {
        i4++;
        i1=0 ; i2=0 ; i3=0 ;
        output.append("        {\n");
        output.append("            \"id\": \"");output+=to_string(users[i].getID()); output.append("\",\n");
        output.append("            \"name\": \"");output+=users[i].getName(); output.append("\",\n");

        output.append("            \"posts\": [\n");
        for (const auto& post : users[i].getPosts())
        {
            i2++;
            output.append("                {\n");
            output.append("                \"body\": \"");output+=post.getBody();output.append("\",\n");
            output.append("                \"topics\": [\n");
            for (const auto& topic : post.getTopics())
            {
                i1++;
                output.append("            \""); output+=topic.name;
                if( !(i1==post.getTopics().size()) )
                {
                    output.append("\",\n");
                }
                else{
                    output.append("\"\n");
                    output.append("                ]\n");
                }
            }
            if( !(i2==users[i].getPosts().size()) )
            {
                output.append("                },\n");
            }
            else{
                output.append("                }\n");
            }
        }
        output.append("            ],\n");

        output.append("            \"followers\": [\n");
        for (const auto& follower : users[i].getFollowers())
        {
            i3++;
            output.append("                {\n");
            output.append("                    \"id\": \"");output+=to_string(follower.id);output.append("\"\n");
            if( !(i3==users[i].getFollowers().size()) )
            {
                output.append("                },\n");
            }
            else{
                output.append("                }\n");
            }
        }
        output.append("            ]\n");


        if( !(i4==users.size()) )
        {
            output.append("        },\n");
        }
        else{
            output.append("        }\n");
        }

    }
    output.append("    ]\n");
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
string Json_Formatting(const std::string& json) {
    std::string formattedJson;
    int indentation = 0;

    for (char c : json) {
        if (c == '{' || c == '[') {
            formattedJson += c;
            formattedJson += std::string(++indentation, '    ');
        } else if (c == '}' || c == ']') {
            formattedJson += std::string(--indentation, '    ') + c;
        } else if (c == ',') {
            formattedJson += c;
            formattedJson +=  std::string(indentation, '    ');
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
void generateDotFile(const std::vector<User>& users, const std::string& fileName) {
    std::ofstream dotFile(fileName);

    if (!dotFile.is_open()) {
        std::cerr << "Error: Unable to open DOT file for writing.\n";
        return;
    }

    dotFile << "digraph UserGraph {\n";

    for (const User& user : users) {
        dotFile << "  " << user.getID() << " [label=\""<< user.getName() << "\"];\n";

        for (const Follower& follower : user.getFollowers()) {
            dotFile << "  " << user.getID() << " -> " << follower.id << ";\n";
        }
    }

    dotFile << "}\n";
    dotFile.close();

    //std::cout << "DOT file generated successfully: " << fileName << "\n";
}
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
                if(!tagStack.empty()){
                    errorMessages+=tagStack.top() ;
                    tagStack.pop() ;
                    errorMessages+=tagStack.top() ;
                    errorMessages+="</users>"+ to_string(lineNumber) ;
                    tagStack.pop() ;}
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
    if(lines[0]!="<users>" ){
        lines[0]="<users>" ;
        file.close();
        file.open(xmlFilePath, std::ios::out | std::ios::trunc);
        while (lines.back()==" "){
            lines.pop_back() ;
        }

        for (const auto &modifiedLine : lines) {

            file << modifiedLine<<endl ;

        }
        return lines ;}
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

    //    lines[beginline]="                    <topic***********************>" ;
    //    file.close();
    //    file.open(xmlFilePath, std::ios::out | std::ios::trunc);
    //    for (const auto &modifiedLine : lines) {
    //        file << modifiedLine<<endl ;
    //    }
    if((begin=="<topics>" && beginminus=="<post>" && (lines[beginline-2].find("</body>",0)!=string::npos))
        ||(beginminus=="<topics>"&& begin!="<topic>" &&end=="</topic>" )){

        if((beginminus=="<topics>"&& begin!="<topic>" &&end=="</topic>" )){lines[beginline-1]="                    <topic>";
            file.close();
            file.open(xmlFilePath, std::ios::out | std::ios::trunc);
            while (lines.back()==" "){
                lines.pop_back() ;
            }
            for (const auto &modifiedLine : lines) {

                file << modifiedLine<<endl ;
            }
            return lines ;}
        lines[beginline]="                    <topic>" ;
        lines[beginline+2]="                    </topic>" ;
        beginline+=3;
        while(beginline<=endline){
            if(lines[beginline].find("</topics>",0)!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                break;
            }else{
                lines[beginline]="                    <topic>" ;
                lines[beginline+2]="                    </topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="                </topics>";
        lines[beginline+1]="            </post>";

    }



    else if (begin=="<topic>"){
        lines[beginline+1]="                    </topic>" ;
    }



    else if(begin=="<body>"&&beginminus=="<post>"){
        lines[beginline+1]="                </body>" ;
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
            lines[beginline-1]="                <id>"+id+"</id>";
        else
            lines[beginline-1]="        <id>"+id+"</id>";
    }



    else if (begin=="<name>" && beginminus=="<user>" && lines[beginline-3]=="    <user>"){
        ull start = lines[beginline-1].find("<name>")+6 ;
        string name ;
        if(lines[beginline-1].find("</")==string::npos){
            name=lines[beginline-1].substr(start) ;
        }else{
            ull end =lines[beginline-1].find("</") ;
            name=lines[beginline-1].substr(start,end-start);

        }
        lines[beginline-1]="        <name>"+name+"</name>";
    }




    else if (( begin=="<post>"  &&  beginminus=="<posts>" ) ||
             (beginminus=="<post>"&& begin!="<body>" &&end=="</body>" )
             || (beginminus=="<post>"&& begin!="<topics>" &&end=="</topics>" )  ){
        if((beginminus=="<post>"&& begin!="<body>" &&end=="</body>" )){beginline--;}
        if((beginminus=="<post>"&& begin!="<topics>" &&end=="</topics>" ) ){beginline-=4;}
        lines[beginline]="                <body>" ;
        beginline+=2;
        lines[beginline]="                </body>" ;
        beginline++;
        lines[beginline]="                <topics>" ;
        beginline++;
        while(1){
            if( lines[beginline].find("</topics>")!=string::npos || lines[beginline+1].find("</post>")!=string::npos ){
                break;
            }else{
                lines[beginline]="                    <topic>" ;
                lines[beginline+2]="                    </topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="                </topics>";
        lines[beginline+1]="            </post>";


    }


    else if ((begin=="<posts>" &&  ( (lines[beginline-2].find("</name>",0)!=string::npos)||(lines[beginline-3].find("</id>",0)!=string::npos)||(lines[beginline-4].find("<user>",0)!=string::npos)))
             ||(beginminus=="<posts>" && begin!="<post>" && end=="</post>")){
        if((beginminus=="<posts>" && begin!="<post>" && end=="</post>")){beginline--;}
        lines[beginline]="            <post>" ;
        beginline++;
        lines[beginline]="                <body>" ;
        beginline+=2;
        lines[beginline]="                </body>" ;
        beginline++;
        lines[beginline]="                <topics>" ;
        beginline++;
        while(1){
            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos|| lines[beginline].find("</post>")!=string::npos ){
                break;
            }else{
                lines[beginline]="                    <topic>" ;
                lines[beginline+2]="                    </topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="                </topics>";
        lines[beginline+1]="            </post>";
        beginline+=2 ;
        while(1){
            if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos ){
                break ;
            }else{
                lines[beginline]="            <post>" ;
                beginline++;
                lines[beginline]="                <body>" ;
                beginline+=2;
                lines[beginline]="                </body>" ;
                beginline++;
                lines[beginline]="                <topics>" ;
                beginline++;
                while(1){
                    if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                        break;
                    }
                    else{
                        lines[beginline]="                    <topic>" ;
                        lines[beginline+2]="                    </topic>" ;
                        beginline+=3;
                    }
                }
                lines[beginline]="                </topics>";
                lines[beginline+1]="            </post>";
                beginline+=2;
            }
        }
        lines[beginline]="        </posts>";


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

        lines[beginline]="                <id>"+id+"</id>";
        lines[beginline+1]="            </follower>";

    }


    else if((begin=="<followers>" && beginminus=="<user>" &&  (lines[beginline-2].find("</posts>",0)!=string::npos) )
             || (beginminus=="<followers>" && begin!="<follower>"  && end=="</follower>" )){

        if((beginminus=="<followers>" && begin!="<follower>"  && end=="</follower>" )){beginline--;}
        lines[beginline]="            <follower>";
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

        lines[beginline]="                <id>"+id+"</id>";
        //********************************************************************
        beginline++;
        lines[beginline]="            </follower>";
        beginline++;
        while(1){
            if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos) {
                break;
            }else{
                lines[beginline]="            <follower>";
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

                lines[beginline]="                <id>"+id+"</id>";
                //********************************************************************
                beginline++;
                lines[beginline]="            </follower>";
                beginline++;
            }
        }
        lines[beginline]="        </followers>" ;
        beginline++;
        lines[beginline]="    </user>" ;


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
        lines[beginline]="    <user>" ;
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

        lines[beginline]="        <id>"+id+"</id>";
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

        lines[beginline]="        <name>"+nn+"</name>";

        beginline++;

        //**************************POSTS******************************************
        lines[beginline]="        <posts>" ;
        beginline++;
        lines[beginline]="            <post>" ;
        beginline++;
        lines[beginline]="                <body>" ;
        beginline+=2;
        lines[beginline]="                </body>" ;
        beginline++;
        lines[beginline]="                <topics>" ;
        beginline++;
        while(1){
            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                break;
            }else{
                lines[beginline]="                    <topic>" ;
                lines[beginline+2]="                    </topic>" ;
                beginline+=3;
            }
        }
        lines[beginline]="                </topics>";
        lines[beginline+1]="            </post>";
        beginline+=2 ;
        while(1){
            if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos){
                break ;
            }else{
                lines[beginline]="            <post>" ;
                beginline++;
                lines[beginline]="                <body>" ;
                beginline+=2;
                lines[beginline]="                </body>" ;
                beginline++;
                lines[beginline]="                <topics>" ;
                beginline++;
                lines[beginline]="                    <topic>" ;//////////////
                lines[beginline+2]="                    </topic>" ;////////////////////////
                beginline+=3;//////////////////////
                while(1){
                    if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                        break;
                    }else{
                        lines[beginline]="                    <topic>" ;
                        lines[beginline+2]="                    </topic>" ;
                        beginline+=3;
                    }
                }
                lines[beginline]="                </topics>";
                lines[beginline+1]="            </post>";
                beginline+=2;
            }
        }
        lines[beginline]="        </posts>";
        beginline++;
        //***************************************************
        //FOLLOWERS******************************************
        lines[beginline]="        <followers>" ;
        beginline++;
        lines[beginline]="            <follower>";
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

        lines[beginline]="                <id>"+idd+"</id>";
        //**********************FOLLOWER*********************************
        beginline++;
        lines[beginline]="            </follower>";
        beginline++;
        while(1){
            if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos  ) {
                break;
            }else{
                lines[beginline]="            <follower>";
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

                lines[beginline]="                <id>"+id+"</id>";
                //********************************************************************
                beginline++;
                lines[beginline]="            </follower>";
                beginline++;
            }
        }
        lines[beginline]="        </followers>" ;
        beginline++;
        lines[beginline]="    </user>" ;
    }


    else if ((begin=="<users>")
             || (beginminus =="<users>" && begin!="<user>" && end=="</user>" )
             ||( begin=="<users>"&& end=="</user>") ){
        if((beginminus =="<users>" && begin!="<user>" && end=="</user>")){
            lines[beginline-1]="    <user>" ;
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
                lines[beginline]="    <user>" ;
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

                lines[beginline]="        <id>"+id+"</id>";
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

                lines[beginline]="        <name>"+nn+"</name>";

                beginline++;

                //**************************POSTS******************************************
                lines[beginline]="        <posts>" ;
                beginline++;
                lines[beginline]="            <post>" ;
                beginline++;
                lines[beginline]="                <body>" ;
                beginline+=2;
                lines[beginline]="                </body>" ;
                beginline++;
                lines[beginline]="                <topics>" ;
                beginline++;
                while(1){
                    if(lines[beginline].find("</topics>")!=string::npos ||lines[beginline+1].find("</post>")!=string::npos||lines[beginline].find("</post>")!=string::npos){
                        break;
                    }else{
                        lines[beginline]="                    <topic>" ;
                        lines[beginline+2]="                    </topic>" ;
                        beginline+=3;
                    }
                }
                lines[beginline]="                </topics>";
                lines[beginline+1]="            </post>";
                beginline+=2 ;
                while(1){
                    if(lines[beginline].find("</posts>")!=string::npos||lines[beginline+1].find("<followers>")!=string::npos){
                        break ;
                    }else{
                        lines[beginline]="            <post>" ;
                        beginline++;
                        lines[beginline]="                <body>" ;
                        beginline+=2;
                        lines[beginline]="                </body>" ;
                        beginline++;
                        lines[beginline]="                <topics>" ;
                        beginline++;
                        lines[beginline]="                    <topic>" ;//////////////
                        lines[beginline+2]="                    </topic>" ;////////////////////////
                        beginline+=3;//////////////////////
                        while(1){
                            if(lines[beginline].find("</topics>")!=string::npos||lines[beginline+1].find("</post>")!=string::npos){
                                break;
                            }else{
                                lines[beginline]="                    <topic>"    ;
                                lines[beginline+2]="                    </topic>" ;
                                beginline+=3;
                            }
                        }
                        lines[beginline]="                </topics>";
                        lines[beginline+1]="            </post>";
                        beginline+=2;
                    }
                }
                lines[beginline]="        </posts>";
                beginline++;
                //***************************************************
                //FOLLOWERS******************************************
                lines[beginline]="        <followers>" ;
                beginline++;
                lines[beginline]="            <follower>";
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

                lines[beginline]="                <id>"+idd+"</id>";
                //**********************FOLLOWER*********************************
                beginline++;
                lines[beginline]="            </follower>";
                beginline++;
                while(1){
                    if(lines[beginline].find("</followers>")!=string::npos||lines[beginline+1].find("</user>")!=string::npos) {
                        break;
                    }else{
                        lines[beginline]="            <follower>";
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

                        lines[beginline]="                <id>"+id+"</id>";
                        //********************************************************************
                        beginline++;
                        lines[beginline]="            </follower>";
                        beginline++;
                    }
                }
                lines[beginline]="        </followers>" ;
                beginline++;
                lines[beginline]="    </user>" ;
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
struct Operation {
    QString data;
    int index;

    Operation(const QString& d , int i) : data(d),index(i) {}
};

vector<vector<int>> generateGraph(vector<User> users){

    vector<vector<int>> NetworkGraph (10000, vector<int>(10000));;

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
    int id ;
    for(int i=0 ; i<users.size() ;i++)
    {
        if(users[i].getFollowers().size() > max){
            max = users[i].getFollowers().size();
            id = users[i].getID();
        }
    }
    return id;
}

string mutualFollowers (int ID1 , int ID2 ,vector<vector<int>> NetworkGraph){
    vector <int> mutual ;
    string res;
    for(int i =0 ; i< NetworkGraph.size() ;i++){
        if(NetworkGraph[ID1][i]==1 && NetworkGraph[ID2][i]==1){
            mutual.push_back(i) ;
            res+= "ID:" + to_string(i) + "  ";
        }
    }
    return res ;
}

string suggestUsers(int ID,vector<vector<int>> NetworkGraph){
    vector <int> suggested ;
    string res;
    for (int i =0 ; i<NetworkGraph.size() ; i++){
        if(NetworkGraph[ID][i]==1){
            for (int j =0 ; j< NetworkGraph.size() ;j++){
                if(NetworkGraph[i][j]==1 && NetworkGraph[ID][j]!=1 && (ID != j)){
                    suggested.push_back(j) ;
                    res+= "ID:" + to_string(j) + "  ";
                }
            }
        }
    }
    return res ;
}

string postSearch (string topic ,vector<User> users){
    vector <pair<Post,int>> result ;
    string res;
    int i =0;
    for(auto user : users ){
        for(auto post : user.getPosts()){
            i++;
            for (auto t :post.getTopics()){
                if(t.name==topic ){
                    result.push_back({post,user.getID()});
                    res += "ID:" + to_string(user.getID()) + "   Post:"+ to_string(i) + "      ";
                }
            }
        }
        i = 0;
    }

    return res ;
}



std::string xmlData;
std::string xml_compressed;
std::string Json_compressed;
std::string JsonData;
int I = 0;
QStack<Operation> undoStack;
QStack<Operation> redoStack;
int flagjsonC = 0;
int flagxmlC = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Compress->setEnabled(false);
    ui->Decompress->setEnabled(false);
    ui->Minify->setEnabled(false);
    ui->Format->setEnabled(false);
    ui->Graph->setEnabled(false);
    ui->undo->setEnabled(false);
    ui->redo->setEnabled(false);
    Operation operation("",3);
    undoStack.push(operation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_insert_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/path/to/file/");
    ui->Path->setText(fileName);
    std::string xmlFilePath = ui->Path->text().toStdString() ;
    // Read XML content from the file
    std::ifstream xmlFile(xmlFilePath);
    if (!xmlFile.is_open()) {
        std::cerr << "Error: Unable to open XML file." << std::endl;
    }

    std::stringstream buffer;
    buffer << xmlFile.rdbuf();
    xmlData = buffer.str();
    QString temp = QtPrivate::convertToQString(xmlData);
    ui->input_text->clear();
    ui->input_text->insertPlainText(temp);
    ui->Compress->setEnabled(true);
    ui->Minify->setEnabled(true);
    ui->Format->setEnabled(true);
    ui->Graph->setEnabled(true);

    Operation operation(temp,0);
    undoStack.push(operation);
    ui->undo->setEnabled(true);
    flagxmlC = 0;
    flagjsonC = 0;
    ui->Decompress->setEnabled(false);
}


void MainWindow::on_Minify_clicked()
{
    if(I==0){
        QString temp = QtPrivate::convertToQString(Xml_Minifying(xmlData));
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,0);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
    }
    else if(I==1){
        SimpleXMLParser parser;
        vector<User> users = parser.parse(xmlData);
        QString temp = QtPrivate::convertToQString(Json_Minifying(users));
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,1);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
    }
}


void MainWindow::on_Compress_clicked()
{
    if(I==0){
        xml_compressed=compressXml(xmlData);
        QString temp = QtPrivate::convertToQString(xml_compressed);
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        flagxmlC = 1;
        Operation operation(temp,0);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
        ui->Decompress->setEnabled(true);
    }
    else if(I==1){
        SimpleXMLParser parser;
        vector<User> users = parser.parse(xmlData);
        Json_compressed=compressJson(users);
        QString temp = QtPrivate::convertToQString(Json_compressed);
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        flagjsonC = 1;
        Operation operation(temp,1);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
        ui->Decompress->setEnabled(true);
    }

}


void MainWindow::on_Decompress_clicked()
{
    if(I==0){
        QString temp = QtPrivate::convertToQString(decompressXml(xml_compressed));
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,0);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
    }
    else if(I==1){
        QString temp = QtPrivate::convertToQString(decompressJson(Json_compressed));
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,1);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
    }
}


void MainWindow::on_Format_clicked()
{
    if(I==0){
        QString temp = QtPrivate::convertToQString(Xml_Formatting(xmlData));
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,0);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
    }
    else if(I==1){
        QString temp = QtPrivate::convertToQString(Json_Formatting(JsonData));
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,1);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
    }
}


void MainWindow::on_comboBox_activated(int index)
{
    I = index;
    QString temp;
    if(index==0){
        temp = QtPrivate::convertToQString(xmlData);
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,0);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
        if(flagxmlC){
            ui->Decompress->setEnabled(true);
        }
        else{
            ui->Decompress->setEnabled(false);
        }
    }
    else if(index == 1){
        SimpleXMLParser parser;
        vector<User> users = parser.parse(xmlData);
        JsonData = Xml_to_Json(users);
        temp = QtPrivate::convertToQString(JsonData);
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        Operation operation(temp,1);
        undoStack.push(operation);
        ui->undo->setEnabled(true);
        if(flagjsonC){
            ui->Decompress->setEnabled(true);
        }
        else{
            ui->Decompress->setEnabled(false);
        }
    }

}


void MainWindow::on_Graph_clicked()
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);

    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir parentDir = QDir(appDirPath);
    //parentDir.cdUp();
    QString relativeFilePath = "graph.dot";
    QString picPath = "graph.dot.png";
    QString fullFilePath = parentDir.filePath(relativeFilePath);
    QString picFilePath = parentDir.filePath(picPath);
    std::string fullFilePathString = fullFilePath.toStdString();

    generateDotFile(users,fullFilePathString);

    QString command = "dot -Tpng -O " + fullFilePath;
    command.replace("/", "\\");
    std::string commandSTR = command.toStdString();
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << command);
    process.waitForFinished();

    QPixmap pic (picFilePath);
    //ui->graphpic->autoFillBackground();
    ui->graphpic->setPixmap(pic);
    ui->graphpic->setScaledContents(true);

}


void MainWindow::on_save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::homePath(), "XML Files (*.xml);;JSON Files (*.json);;All Files (*)");


    if (filePath.isNull()) {
        qDebug() << "User canceled the file dialog.";

    }


    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream out(&file);

        out << ui->input_text->toPlainText() ;

        file.close();

        qDebug() << "File saved successfully.";
    } else {

        qDebug() << "Error: Couldn't open the file for writing.";
    }
}


void MainWindow::on_enter_clicked()
{
    QString temp = ui->input_text->toPlainText();
    ui->input_text->clear();
    ui->input_text->insertPlainText(temp);
    xmlData = Xml_Formatting(temp.toStdString());
    ui->Compress->setEnabled(true);
    ui->Minify->setEnabled(true);
    ui->Format->setEnabled(true);
    ui->Graph->setEnabled(true);
    Operation operation(temp,0);
    undoStack.push(operation);
    ui->undo->setEnabled(true);
    flagxmlC = 0;
    flagjsonC = 0;
    ui->Decompress->setEnabled(false);
}





void MainWindow::on_undo_clicked()
{
    redoStack.push(undoStack.pop());

    Operation lastOperation = undoStack.top();
    if(lastOperation.index == 0){
        xmlData = Xml_Formatting(lastOperation.data.toStdString());
    }
    else{
        JsonData = Json_Formatting(lastOperation.data.toStdString());
    }
    ui->input_text->clear();
    ui->input_text->insertPlainText(lastOperation.data);
    ui->redo->setEnabled(true);
    if (undoStack.top().index == 3) {
        ui->undo->setEnabled(false);
    }
}



void MainWindow::on_redo_clicked()
{
    if (!redoStack.isEmpty()) {

        Operation lastOperation = redoStack.pop();
        undoStack.push(lastOperation);
        ui->undo->setEnabled(true);
        if(lastOperation.index == 0){
            xmlData = Xml_Formatting(lastOperation.data.toStdString());
        }
        else{
            JsonData = Json_Formatting(lastOperation.data.toStdString());
        }
        ui->input_text->clear();
        ui->input_text->insertPlainText(lastOperation.data);
    }
    else{
        ui->redo->setEnabled(false);
    }
}

int flag = 1;

void MainWindow::on_correct_clicked()
{
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir parentDir = QDir(appDirPath);
    QString data = "data.xml";
    QString temp = "temp.xml";
    QString fulldataPath = parentDir.filePath(data);
    QString fulltempPath = parentDir.filePath(temp);
    std::string fulldataString = fulldataPath.toStdString();
    std::string fulltempString = fulltempPath.toStdString();

    QFile file(fulldataPath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream out(&file);

        out << ui->input_text->toPlainText() ;

        file.close();

        //qDebug() << " successfull";
    } else {

        qDebug() << "Error: Couldn't open the file for writing.";
    }

    QFile fileTemp(fulltempPath);
    if (fileTemp.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream out(&fileTemp);

        out << ui->input_text->toPlainText() ;

        fileTemp.close();

        //qDebug() << " successfull";
    } else {

        qDebug() << "Error: Couldn't open the file for writing.";
    }

    string errorMessages ;
    string beginminus ;
    bool IsCorrect=errorChecking(fulldataString,errorMessages,beginminus);
    while(IsCorrect!=true){
        ui->input_text->clear();
        ui->input_text->insertPlainText("NO");
        errorCorrecting(fulldataString,errorMessages,beginminus);
        //qDebug() << errorMessages ;
        errorMessages="";
        std::ifstream xmlFile(fulldataString);
        if (!xmlFile.is_open()) {
            std::cerr << "Error: Unable to open XML file." << std::endl;
        }
        std::stringstream buffer;
        buffer << xmlFile.rdbuf();
        string xml = buffer.str();
        QString temp = QtPrivate::convertToQString(xml);
        ui->input_text->clear();
        ui->input_text->insertPlainText(temp);
        IsCorrect=errorChecking(fulldataString,errorMessages,beginminus);
    }
    std::ifstream xmlFile(fulldataString);
    if (!xmlFile.is_open()) {
        std::cerr << "Error: Unable to open XML file." << std::endl;
    }
    std::stringstream buffer;
    buffer << xmlFile.rdbuf();
    xmlData = buffer.str();

    std::ifstream f1(fulldataString);
    std::ifstream f2(fulltempString);

    std::string line1, line2;
    int num =1 ;
    vector<int> linesOfErrors ;
    while (std::getline(f1, line1) && std::getline(f2, line2)) {

        if (removeSpaces(line1) != removeSpaces(line2)) {
            linesOfErrors.push_back(num) ;
        }
        num++;
    }

    QTextCursor cursor = ui->input_text->textCursor();
    cursor.movePosition(QTextCursor::Start);
    QTextCharFormat textFormat;
    textFormat.setForeground(Qt::red);

    for(int j =0 ; j < linesOfErrors.size();j++){
        //qDebug() << linesOfErrors[j] ;
        for (int i = 1; i < linesOfErrors[j] ; ++i) {
            cursor.movePosition(QTextCursor::NextBlock);
            cursor.movePosition(QTextCursor::EndOfBlock);
        }
        cursor.setCharFormat(textFormat);
        cursor.insertText("                           <=== ERROR Corrected ");
        ui->input_text->setTextCursor(cursor);
        cursor.movePosition(QTextCursor::Start);
    }
    textFormat.setForeground(Qt::black);
    cursor.setCharFormat(textFormat);
    cursor.insertText(" ");
    ui->input_text->setTextCursor(cursor);

}


void MainWindow::on_most_influncer_clicked()
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);
    int res = mostInfluencerUser(calculateIndegree(generateGraph(users)));
    ui -> res->setText(QString::number(res));
}


void MainWindow::on_Most_active_clicked()
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);
    int r = most_Active_user(users);
    ui -> res->setText(QString::number(r));
}


void MainWindow::on_Search_post_clicked()
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);
    string topic = ui->post_search->text().toStdString();
    string r =  postSearch (topic ,users);
    ui -> res->setText(QtPrivate::convertToQString(r));
}


void MainWindow::on_suggest_clicked()
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);
    int ID = ui->suggest_id->text().toInt();
    string r = suggestUsers(ID,generateGraph(users));
    ui -> res->setText(QtPrivate::convertToQString(r));
}


void MainWindow::on_search_mutual_clicked()
{
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);
    int ID1 = ui->mutual_1->text().toInt();
    int ID2 = ui->mutual_2->text().toInt();
    string r = mutualFollowers ( ID1 , ID2 ,generateGraph(users));
    ui -> res->setText(QtPrivate::convertToQString(r));
}

