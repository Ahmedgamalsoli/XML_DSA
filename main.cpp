//  Created by Ahmed Khalifa on 25/12/2023.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <zlib.h>
#include <stack>

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
                if((content[startPos+1]=='\n'))
                        break;
                if((content[startPos+1]==' '))
                    if((content[startPos+2]==' ') )
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
                if((content[startPos+1]=='\n'))
                        break;
                if((content[startPos+1]==' '))
                    if((content[startPos+2]==' ') )
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
                if((content[startPos+1]==' '))
                    if((content[startPos+2]==' ') )
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

string Json_Formatting(const std::string& json) {
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


int main() {
    // Example XML string
    std::string xmlFilePath = "/Users/ahmedkhalifa/Desktop/DS_project/sample.xml";
    
    // Read XML content from the file
    std::ifstream xmlFile(xmlFilePath);
    if (!xmlFile.is_open()) {
        std::cerr << "Error: Unable to open XML file." << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << xmlFile.rdbuf();
    std::string xmlData = buffer.str();
    
    
//    std::cout << "Minified Data:\n" << std::endl;
//    cout<<Xml_Minifying(xmlData)<< std::endl;
//    std::cout << "Formatted Data:\n" << std::endl;
//    cout<<Xml_Formatting(xmlData)<< std::endl;
//    
//    
//    string x=compressXml(xmlData);
//    string y=decompressXml(x);
//
//    
//    std::cout << "Compressed Data:\n" << x<< std::endl;
//    std::cout << "Decompressed Data:\n" << y<< std::endl;
    SimpleXMLParser parser;
    vector<User> users = parser.parse(xmlData);
    cout<<endl<<endl;
    string json =Xml_to_Json(users);
    std::cout << "JSON Data:\n" <<  std::endl;
    cout<<json<<endl;

    string jsonx =Json_Minifying(users);
    std::cout << "JSON Data:\n" <<  std::endl;
    cout<<jsonx<<endl;
    
    
     jsonx =compressJson(users);
    std::cout << "Compressed JSON Data:\n" <<  std::endl;
    cout<<jsonx<<endl;
    
    
    std::cout << "deCompressed JSON Data:\n" <<  std::endl;
    cout<<decompressJson(jsonx)<<endl;
    
    return 0;
}
