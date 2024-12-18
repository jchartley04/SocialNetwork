#ifndef USER_H
#define USER_H

#include "post.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector> 
#include <set> 


class User{ 
public: 
    //CONSTRUCTORS

    //pre: 
    //post: creates a user with arbitrary values 
    User(); 

    //pre: 
    //post: Creates a new user with user information 
    User(int id, std::string name, int year, int zip, std::set<int> friends);


    //ACCESSORS 
    //Pre: 
    //Post: Returns user id 
    int getId(); 

    //Pre:
    //Post: Returns user name
    std::string getName(); 

    //Pre: 
    //Post: Returns user birth year
    int getYear(); 

    //Pre:
    //Post: Returns user zip code 
    int getZip(); 

    //Pre: 
    //Post: Returns a reference to user friends 
    std::set<int> & getFriends();

    //MODIFICATION MEMBER

    //Pre: Friend doesnt exist
    //Post: Adds friend friend to friends_
    void addFriend(int id);

    //Pre: Edge exists between the two vertices 
    //Post: Edge / Friendship removed 
    void deleteFriend(int id);

    
    //Pre: 
    //Post: Post added to messages_. Order is stored by message id
    void addPost(Post*); 

    //Pre: messages_ is not empty 
    //Post: return vector of Post pointers 
    std::vector<Post*> getPosts(); 

    //Pre:
    //Post: Returns a string that holds the most recent howMany posts (if they are less than how many)
    std::string getPostsString(int howMany, bool showOnlyPublic);


    //FINAL PROJECT

    //pre:
    //post: returns the bio string
    std::string getBio();


    //pre:
    //post: Set the user bio to the value passed by user
    void setBio(std::string b);

    
private:

    //Class invariants
    //1. id_ stores user id 0,1,2,...
    //2. name_ stores user name as a string 
    //3. year_ stores the birth year of the user 
    //4. Stores the zip code for the user 
    //5. Stores a list of friends for each user through adjacency lists
    //6. messages_ is a vector of Post* pointers.
    //7. Stores user bio.
    int id_; 

    std::string name_; 

    int year_;

    int zip_;

    std::set<int> friends_;

    std::vector<Post*> messages_;


    //ADDED FOR FINAL PROJECT

    std::string bio;

};

#endif //USER_H;
