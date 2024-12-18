#include "user.h"
#include "post.h"
#include<string>
#include<iostream>

//Default Constructor 
//Create an empty user
User::User()
{
    id_ = -1;  //set default id to NULL
    name_ = " "; //set default name to empty string
    zip_ = -1; //default zip to NULL
    year_ = -1; //birth year set to NULL
}

//Creates user from user input 
User::User(int id, std::string name, int year, int zip, std::set<int> friends)
{
    id_ = id;
    name_ = name; 
    year_ = year;
    zip_ = zip; 
    friends_ = friends; 
}

//Return current user id
int User::getId()
{
    return id_; 
}

//Return user name
std::string User::getName()
{
    return name_; 
}

//Return user birth year
int User::getYear()
{
    return year_;
}

//Return user's zip code
int User::getZip()
{
    return zip_;                               
}



//Return user's list of friend connections
std::set<int> & User::getFriends()
{
    return friends_;
}

void User::addFriend(int id)
{
    //Check if friend exists 
    for(auto e : friends_)
    {
        if(e == id)
            return; //Return if friend exists
    }
    friends_.insert(id); //Inserts friend id into adjacency list for friends
}

void User::deleteFriend(int id)
{
    bool value = false;
    //Checks if friend exists and deletes friend
    for(auto e : friends_)
    {
        if(e == id)
            value = true; //id is within friends_ list. 
    }
    if(value == true)
    {
        friends_.erase(id); //remove id from friends_ 
    }
    return; //Return if id is not inside friends_ 
}


//Pre: 
//Post: Post added to messages_. Order is stored by message id
void User::addPost(Post* post)
{
    //Adds a pointer to the messages_ vector which stores posts
    messages_.push_back(post); 
}

//Pre: messages_ is not empty 
//Post: return vector of Post pointers 
std::vector<Post*> User::getPosts()
{
    //Ensures messages_ has posts 
    if(messages_.size() <= 0)
    {
        return {}; //Return empty vector 
    }
    return messages_;
}

//Pre:
//Post: Returns a string that holds the most recent howMany posts (if they are less than how many)
std::string User::getPostsString(int howMany, bool showOnlyPublic)
{   
    std::string postString; //String will store the result expected to be returned
    //Ensures the number of posts does not exceed howMany 
    int count = 0; 
    //Iterate over the posts. From the most recent to least recent. 
    for(int i = messages_.size()-1; i >= 0; i--)
    {
        if(count >= howMany)
        {
            break; //break for loop if count is greater than how many 
        }

        //Stores the value at messages_[i]
        Post* ptr = messages_[i];  

        //If the post is private and showOnlyPublic is true, then it will continue to the next iteration 
        if(showOnlyPublic && !ptr->getIsPublic())
        {
            continue; //Continue to the next iteration to the for loop. 
        }

        //If the post is public 
        postString = postString + ptr->toString() + "\n\n";

        count++; //Keeps track of how many posts have been added to postString to be returned. Cannot exceed howMany  

        

    }
    return postString; 
}

//getter for user bio
std::string User::getBio()
{
    return bio;
}

void User::setBio(std::string b)
{
    bio = b;
}
