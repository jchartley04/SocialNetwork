#include "post.h"
#include "user.h"
#include "network.h"
#include <string> 
#include <sstream>
//Default construtor. Initialize values to 0 
Post::Post()
{
    messageId_ = 0;
    ownerId_ = 0; 
    message_ = "";
    likes_ = 0; 
}

//Constructor with paramters. Initialize all values with the values passed by user 
Post::Post(int messageId, int ownerId, std::string message, int likes)
{
    messageId_ = messageId; 
    ownerId_ = ownerId; 
    message_ = message; 
    likes_ = likes;
}

//Return a string '[message_] liked by [likes_] people 
std::string Post::toString()
{
    return message_ + " Liked by " + std::to_string(likes_) + " people.";
}

//Getter for messageId 
int Post::getMessageId()
{
    return messageId_; 
}

//Getter for owner id 
int Post::getOwnerId()
{
    return ownerId_;
}

//Getter for getting the message / post 
std::string Post::getMessage()
{
    return message_; 
}

//Getter for the number of likes 
int Post::getLikes()
{
    return likes_; 
}

//Virtual functions are useful for overriding 
//Virtual function to getting the author. 
std::string Post::getAuthor()
{
    //base case 
    return "";
}

//Virtual function to check if the post is public or not. 
bool Post::getIsPublic()
{
    //unless otherwise specified, the post is public 
    return true; 
}

//Incoming post class inherits from post class 

IncomingPost::IncomingPost() : Post()
{
    author_ = "";
    isPublic_ = true;
}

IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author) : Post(messageId, ownerId, message, likes)
{
    author_ = author; 
    isPublic_ = isPublic; 
}

//Print countents as string 
std::string IncomingPost::toString()
{
    //Use ostringstream to handle string
    //std::cout << "***INSIDE INCOMING POST TO STRING****" << std::endl;
    std::ostringstream oss;
    

    //If it is not public. Include private. 
    if (getIsPublic()) {
        //std::cout << "Inside true if statement" << std::endl;
        oss << getAuthor() + " wrote: " << Post::toString();
    }
    else
    {
       //std::cout << "Inside else statement" << std::endl;
       oss << getAuthor() + " wrote (private): " << Post::toString();
    }
    
    
    return oss.str();
}

//Getter for author 
std::string IncomingPost::getAuthor() 
{
    return author_; 
}

//Getter for post privacy. Public or Private post. 
bool IncomingPost::getIsPublic()
{
    return isPublic_;
}



