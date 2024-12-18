#ifndef NETWORK_H
#define NETWORK_H
#include "user.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector> 
#include <set> 


class Network{ 
public: 
   
    //CONSTRUCTOR 

    //Pre: 
    //Post: Initializes an empty Network with no users 
    Network();

    //Pre: id exists 
    //Post: Takes in an ID and returns a pointer to the corresponding User
    User* getUser(int id);

    //Pre: User* Exists 
    //Post: Add user to the Network database 
    void addUser(User*);

    //Pre: User's exist and Friend Connection doesn't exist
    //Post: Adds a friend connection or returns -1 if either user is invalid and 0 otherwise 
    int addConnection(std::string s1, std::string s2);

    //Pre: User exists and there is a connection 
    //Post: Removes connection between Users 
    int deleteConnection(std::string s1, std::string s2);

    //Pre: User exists
    //Post: Return id of the corresponding user
    int getId(std::string name);

    //Pre: 
    //Post: Return number of Users in the Network
    int numUsers(); 

    //Pre: 
    //Post: Initializes all of the networks information from a file.
    void readUsers(char * fname); 

    //Pre:
    //Post: writes all of the networks information to a file. 
    void writeUsers(char * frname);

    //Pre:
    //Post: Computes the shorted distance between two users. 
    //Return vector containing shortest path of vertices starting at User from and ending at user to 
    std::vector<int> shortestPath(int from, int to);

    //Pre: 
    //Post: Return the vector of IDs from user from to user to that matches the distance provided.
    //If no such user exists, the function  returns empty vector, set "to" to -1
    std::vector<int> distanceUser(int from, int& to, int distance);

    //Pre: 
    //Post: Return a vector of friend ids with the highest score. Score meaning that the friend ids have many friends in common with 
    //User at id who. Return empty vector if no suggestions exist. Set score to 0. 
    std::vector<int> suggestFriends(int who, int& score);

    //Pre:
    //Post: Returns a vector containing the different groups within the network 
    std::vector<std::vector<int> > groups(); 

    //pre:
    //Post: Add new post to the messages vector of the user whose id is ownerId. 
    void addPost(int ownderId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic); 

    //Pre:
    //Post: Call User::getPostsString - return result 
    std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic);

    //Pre:
    //post:: read from a file. Return -1 if file cannot be obtained
    int readPosts(char* fname);

    
    //Pre: 
    //Post: Return true if post1 is less than post2
    static bool compareId(Post* post1, Post* post2);

    //Pre:
    //post: Write to a file. Return -1 if file cannot be opened
    int writePosts(char* fname);

    
private:

    //Class Invariants 
    //1. Vector of User pointers to dynamically allocate User Objects. 
    //users_[i] should always have id i for all i = 0, 1, 2,...,users_.size()-1 
    std::vector<User*> users_; 

    //Pre: 
    //Post: Trim Tab off of the file 
    void trimTab(std::string& str);

    
};

#endif //NETWORK_H