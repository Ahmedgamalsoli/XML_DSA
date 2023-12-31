//
// Created by seif alrahman on 12/30/2023.
//

#ifndef DSA_PROJECT_OPERATIONS_H
#define DSA_PROJECT_OPERATIONS_H
#include <string>
#include<vector>
#include "User.h"
using namespace std;
string CorrectXML(const string& RealxmlFilePath);
vector<vector<int>> generateGraph(vector<User> users);
vector <int> calculateIndegree (vector<vector<int>> NetworkGraph );
int mostInfluencerUser(vector<int> Indegree);
int most_Active_user(vector<User> users);
vector<int> mutualFollowers (int ID1 , int ID2 ,vector<vector<int>> NetworkGraph);
vector<int> suggestUsers(int ID,vector<vector<int>> NetworkGraph);
vector <pair<Post,int>> postSearch (string topic ,vector<User> users);
string extractTagName(const string& tag);
bool isTag(const string& str);
std::string decompressJson(const std::string& Json);
std::string compressJson(const std::vector<User> users);
string Json_Formatting(std::string& json);
string Json_Minifying(const std::vector<User> users);
string Xml_to_Json(const std::vector<User> users);
std::string decompressXml(const std::string& xml);
std::string compressXml(const std::string& xml);
std::string Align_Xml(const std::string& unalignedXml);
string Xml_Minifying(const std::string& xml);
string Xml_Formatting(const std::string& xml);
#endif //DSA_PROJECT_OPERATIONS_H
