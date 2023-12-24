// User.h
#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include "Post.h" // Include the header file for the Post class

using namespace std;

class User {
private:
    int ID;
    string name;
    vector<Post> Posts;
    vector<User> Followers; // the users I follow

public:
    // Constructor
    User(int ID);



    // Getter and Setter declarations
    int getID() const;
    void setID(int newID);
    string getName() const;
    void setName(const string& newName);
    vector<Post> getPosts() const;
    void setPosts(const vector<Post>& newPosts);
    vector<User> getFollowers() const;
    void setFollowers(const vector<User>& newFollowers);

    // Additional methods
    void addPost(Post newPost);
    void addFollower(User newUser);
};

#endif // USER_H
