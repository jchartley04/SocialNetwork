#ifndef POST_H
#define POST_H
#include <string> 

//Class Post represents the different aspects of a social media post 
class Post
{
public:

    //Default constructor, empty post
    Post();
    
    //Constuctor with parameters. 
    Post(int messageId, int ownerId, std::string message, int likes); 

    //Return a string '[message_] liked by [likes_] people 
    virtual std::string toString();

    //Getter for messageId 
    int getMessageId(); 

    //Getter for owner id 
    int getOwnerId(); 

    //Getter for getting the message / post 
    std::string getMessage(); 

    //Getter for the number of likes 
    int getLikes(); 

    //Virtual functions are useful for overriding 
    //Virtual function to getting the author. 
    virtual std::string getAuthor(); 

    //Virtual function to check if the post is public or not. 
    virtual bool getIsPublic(); 

private:
    //Class Invariants 
    //1. messageId_ is the id of the message 
    //2. ownerId_ is the id of the owner of the post/author 
    //3. string message_ is the content of the post 
    //4. likes_ counts the number of users who have liked the post 
    int messageId_; 
    int ownerId_; 
    std::string message_; 
    int likes_; 

};

//IncomingPost class inheriting from Post 
class IncomingPost : public Post
{
public: 

    //CONSTRUCTORS 

    //Default Constructor 
    IncomingPost(); 

    //Constructor with parameters 
    IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author);

    std::string toString(); 


    //GETTER FUNCTIONS

    //Pre:
    //Post: Returns author name 
    std::string getAuthor();

    //Pre:
    //Post: Returns whether post is public or private 
    bool getIsPublic(); 

private:
    //Class Invariants 
    //1. author is the owner of the post 
    //2. isPublic stores whether or not the post is a public post or a private post 
    std::string author_; 
    bool isPublic_; 


};

#endif
