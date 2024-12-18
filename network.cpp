#include "network.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <stack>
#include <vector>

//PRIVATE TRIM TAB FUNCTION
void Network::trimTab(std::string& str)
{
    // Find the first non-space and non-tab character
    size_t pos = str.find_first_not_of(" \t");

    // Erase leading spaces and tabs
    if (pos != std::string::npos)
    {
        str.erase(0, pos);
    }
}

//Default Constructor 
Network::Network()
{
    //users_.push_back(new User); //Initializes users_ as empty
}

//Returns pointer to user inside of vector
User* Network::getUser(int id)
{
    //Iterate through users_ vector
    for(int i = 0; i < users_.size(); i++)
    {
        //If users_[i] is = to id passed, return a pointer to that user 
        if(users_[i]->getId() == id)
            return users_[i];
    }
    return nullptr; //return nullptr if user is not in vector 
}

//Pre: User* Exists 
//Post: Add user to the Network database 
void Network::addUser(User* u)
{
    for(auto e : users_)
        if(e == u)
        {
            return; //user already in users_
        }
    users_.push_back(u);
}

int Network::addConnection(std::string s1, std::string s2)
{
    User* user1;
    User* user2; 

    //Ensures there are no duplicate users
    int numUser1 = 0;
    int numUser2 = 0; 

    //Check whether or not user is within users_ 
    for(auto e : users_)
    {
        if(e->getName() == s1)
        {
            ++numUser1;
            user1 = e; 
        }
        else if(e->getName() == s2)
        {
            ++numUser2;
            user2 = e;
        }
    }
    if(numUser1 > 1  || numUser1 == 0 || numUser2 > 1 || numUser2 == 0)
    {
        return -1; //More than 1 user with a given name or user doesnt exist.
    }  

    user1->addFriend(user2->getId());
    user2->addFriend(user1->getId());
    return 0; //All users are valid 
    


}

int Network::deleteConnection(std::string s1, std::string s2)
{
    User* user1;
    User* user2; 

    //Ensures there are no duplicate users
    int numUser1 = 0;
    int numUser2 = 0; 

    //Check whether or not user is within users_ 
    for(auto e : users_)
    {
        //if value e has the same name as s1 increase numUser1
        if(e->getName() == s1)
        {
            ++numUser1;
            user1 = e; //set user1 to value e 
        }
        else if(e->getName() == s2)
        {
            ++numUser2;
            user2 = e;
        }
    }
    //If numUser1 or 2 is more than 1 there is a duplicate. If user1/2 is 0, the user doesnt exist 
    if(numUser1 > 1  || numUser1 == 0 || numUser2 > 1 || numUser2 == 0)
    {
        return -1; //return -1 for invalid user
    }  

    user1->deleteFriend(user2->getId()); //Removes user2 id in users_[user1.id]
    user2->deleteFriend(user1->getId()); //Removes user2 id in users_[user2.id]
    return 0; //All users are valid 
}

int Network::getId(std::string name)
{
    User* user1;
    
    //Tracks if there is a duplicate user or if user doesnt exist 
    int numUser = 0;
    for(auto e : users_)
    {
        if(e->getName() == name)
        {
            numUser++; 
            user1 = e; 
        }
    }

    if(numUser == 0 || numUser > 1)
    {
        return -1; //Duplicate user or no user exists
    }

    return user1->getId(); 

}

//Returns the number of vertices in adjList 
int Network::numUsers()
{   
    return users_.size(); 

}

//Pre: 
//Post: Initializes all of the networks information from a file.
void Network::readUsers(char * fname)
{   
    
    std::ifstream file(fname);

    if(!file)
    {
        //std::cerr << "Error: Could not open file " << fname << std::endl;
        return; //File reading error 
    }

    std::string line; //line takes in each line in file fname
    getline(file,line); //set line to the first line in fname
    std::stringstream ss; 
    ss << line; 
    int numUsers; 
    ss >> numUsers; //Convert string number to int number 

    
    
    for(int i = 0; i < numUsers; i++)
    {
        //Get Id
        std::string stringId; 
        getline(file,stringId);
        std::stringstream sId(stringId);
        int intId; 
        sId >> intId;

        //Get name 
        std::string string; 
        std::string temp;
        getline(file, string); 
        std::stringstream sName(string);
        std::string name;
        while(sName >> temp)
        {
            name = name + temp + " "; //Sets name equal to First _ Last
        }

        //Checks if there is any white space at the end of name
        if(name[name.size()-1] == ' ')
        {
            name = name.substr(0,name.size()-1); //Removes last ending space in name. 
        }
        

        //Get birth year 
        std::string year; 
        getline(file,year); 
        std::stringstream sYear(year);
        int intYear;
        sYear >> intYear; 

        //Get zip code 
        std::string zip; 
        getline(file,zip); 
        std::stringstream sZip(zip); 
        int intZip; 
        sZip >> intZip; 

        std::string fList; //Tracks each friend id string
        std::set<int> connections;
        getline(file, fList); //Take in the entire line of friends 
        std::stringstream fss; //friend stringstream
        fss << fList; 
        int friendId; 
        while(fss >> friendId) //Reads number by number (stops every white space or line break)
        {
            connections.insert(friendId);  //Insert friendIdinto connection set (necessary for adding new User)

        }


        //GET USER'S Bio
        std::string bioString;
        getline(file, bioString);

        trimTab(bioString);

        std::cout << "trimmed inside network.cpp ->" << bioString << std::endl;

        User* newUser = new User(intId, name, intYear, intZip, connections); //Create User in Network from file.txt
        addUser(newUser);

        //Set the user bio to bioString
        newUser->setBio(bioString);
    }
   

    file.close();  //Close file 


}

//Pre:
//Post: writes all of the networks information to a file. 
void Network::writeUsers(char * frname)
{
    std::ofstream outfile(frname);
    if(!outfile.is_open())
    {
        return; //No file opened 
    }

    outfile << users_.size() << "\n"; //Add number of users to top of txt file 
    
    for(int i = 0; i < users_.size(); i++) //Iterate over adjList and output each Users information into file 
    {
        outfile << users_[i]->getId() << "\n"; 

        outfile << "\t" << users_[i]->getName() << "\n";

        outfile << "\t" << users_[i]->getYear() << "\n";

        outfile << "\t" << users_[i]->getZip() << "\n";

        outfile << "\t"; 
       
        std::set<int> friendSet; //Create friendSet to store users_[i] set of friend connections
        friendSet = users_[i]->getFriends(); 
        std::vector<int> fList(friendSet.begin(), friendSet.end());  //Use a vector to store the friends in the set 
        
        //Iterate over vector that stores friendSet 
        for(int j = 0; j < fList.size(); j++)
        {
            outfile << fList[j] << " ";
        }

        //FINAL PROJECT ADDITION. USER BIO.

        outfile << "\n";

        outfile << "\t" << users_[i]->getBio() << "\n";


    }
}
std::vector<int> Network::shortestPath(int from, int to)    
{
    //Ensure that from and to are value users 
    if(to < 0 || to > users_.size()-1)
    {
        return{}; //Return empty vector because the id at to is larger than the list of users
    }

    if(from < 0 || from > users_.size()-1)
    {
        return{}; //Return empty vector. from must be greater than 0 and less than users.size()-1
    }
    std::queue<int> q;
    std::vector<bool> visited(users_.size() , 0); //Checks whether user has been visited or not 
    std::vector<int> dist (users_.size(),-1); //Tracks the distance from user 'from'
    std::vector<int> prev(users_.size(), -1); //Track previous positions 

    dist[from] = 0; //Initialize user id of user 'from' to zero because it is the source itself
    visited[from] = true; //Initialize id of user from to true (already visited)
    q.push(from);

    while (q.size() > 0)
    {
        int cur = q.front(); 
        q.pop(); 


        //If the cur user is equal to user to. End BFS. 
        if (cur == to)
        {   
            break;
        }

        //Iterate over user cur's friends. Check if id = to 
        for(int neighbor : users_[cur]->getFriends())
        {
            if(!visited[neighbor])
            {
                dist[neighbor] = dist[cur] + 1;
                visited[neighbor] = true; 
                prev[neighbor] = cur;
                q.push(neighbor);
            }
        }
    }

    std::vector<int> shortestPath; 
    int value = to; //Tracks the value of prev[value]

    if(prev[to] == from)
    {
        shortestPath.push_back(from); //Return the shortest path. from and to are direct neighbors.
        shortestPath.push_back(to);
        return shortestPath;
    } 
    else if(dist[to] == -1)
    {
        shortestPath.push_back(dist[to]);
        return shortestPath; //No connection from user 'from' to user 'to'
    }

    //Add value of to the front of the temporary front of the path
    shortestPath.push_back(value);
    //While value is not equal to from. Continue to iterate over prev list 
    while(value != from)
    {
        shortestPath.push_back(prev[value]);
        value = prev[value];
    }

    //Reverse the shortest path to show from -> to 
    reverse(shortestPath.begin(), shortestPath.end()); 
    return shortestPath;

}   

std::vector<int> Network::distanceUser(int from, int& to, int distance)
{
    if(from < 0 || from > users_.size()-1)
    {
        to = -1; //Not possible if from is not a valid user 
        return{}; //Return empty vector. from must be greater than 0 and less than users.size()-1
        
    }
    std::queue<int> q;
    std::vector<bool> visited(users_.size() , 0); //Checks whether user has been visited or not 
    std::vector<int> dist (users_.size(),-1); //Tracks the distance from user 'from'
    std::vector<int> prev(users_.size(), -1); //Track previous positions 

    bool found = false; //This bool var tracks whether or not a user at distance 'distance' is found 

    dist[from] = 0; //Initialize user id of user 'from' to zero because it is the source itself
    visited[from] = true; //Initialize id of user from to true (already visited)
    q.push(from);

    while (q.size() > 0)
    {
        int cur = q.front(); 
        q.pop(); 

        //Iterate over user cur's friends. Check if id = to 
        for(int neighbor : users_[cur]->getFriends())
        {
            if(!visited[neighbor])
            {
                dist[neighbor] = dist[cur] + 1;
                visited[neighbor] = true; 
                prev[neighbor] = cur;
                q.push(neighbor);

                if(dist[neighbor] == distance)
                {
                    to = neighbor;
                    found = true;
                    break; //Break inner for loop 
                
                }
            }
        }
        if(found == true)
        {
            break; //Break out of while loop 
        }
    }

    //Check if found is false. If fale, no user found. Set to = -1 and return empty vector 
    if(found == false)
    {
        to = -1; 
        return{}; 
    }
    
    //Return the shortest path 
    std::vector<int> shortestPath; 
    int value = to; //Tracks the value of prev[value]

    //If user to is a direct neighbor to user 'from'
    if(prev[to] == from)
    {
        shortestPath.push_back(from); //Return the shortest path. from and to are direct neighbors.
        shortestPath.push_back(to);
        return shortestPath;
    } 
   

    //Add value of to the front of the temporary front of the path
    shortestPath.push_back(value);
    //While value is not equal to from. Continue to iterate over prev list 
    while(value != from)
    {
        shortestPath.push_back(prev[value]);
        value = prev[value];
    }

    //Reverse the shortest path from user 'from' to a user 'to' at the provided distance 
    reverse(shortestPath.begin(), shortestPath.end()); 
    return shortestPath;
}

std::vector<int> Network::suggestFriends(int who, int& score)
{
    //Set score equal to 0. If there are friend suggestions, score will increment. If not then score will remain zero for return 
    score = 0; 

    //User home points to user at index who 
    User* userWho = getUser(who); 

    //If there is no user at index who. Return empty vector.
    if(userWho == nullptr)
    {
        return{};
    }
    //Get friends of user at id 'who' and their friend list. 
    std::vector<int> friends(userWho->getFriends().begin(), userWho->getFriends().end());

    //Store whos' friends' friends. Unless f is who themselves. Or if f is within friends. 
    //vector[0] stores Friend ID. Vector[1] contains UserId's score
    std::vector<std::pair<int, int> > connections; 

    //Go over friends in friends vector
    for(int f : friends)
    {
        //Get user at with id f 
        User* findMutual = getUser(f);

        //Store findMutual user's friends in mutualList 
        std::vector<int> mutualList(findMutual->getFriends().begin(), findMutual->getFriends().end());

        //Iterate over the user ids in mutualList to check if 
        for(int i = 0; i < mutualList.size(); i++)
        {
            //Ensure that mutualList[i] is not who user themselves. (Dont add who as a suggested friend)
            if(mutualList[i] != who)
            {
                //Tracks whether or not mutual friend is within whos' friends
                bool isDirectFriend = false; 
                for(int j = 0; j < friends.size(); j++)
                {
                    //If mutualList[i] is also inside friends. Then the user at mList[i] is a direct friend
                    if(mutualList[i] == friends[j])
                    {
                        isDirectFriend = true;
                    }
                }
                //If mutualList[i] i not a direct friend. Add it to connections vector pair 
                if(isDirectFriend == false)
                {
                    for(int k = 0; k < connections.size(); k++)
                    {
                        //If the id of mList[i] is already in connections. Increment score value of pair. 
                        if(connections[k].first == mutualList[i])
                        {
                            connections[k].second++; //Increment score 
                        }
                    }
                    //Checks if new mutual Friend isnt in connections. 
                    bool contains = false; 
                    for(int g = 0; g < connections.size(); g++)
                    {
                        if(connections[g].first == mutualList[i])
                        {
                            contains = true;
                        }
                    }

                    //If connections doesnt already contain mutualList[i] then add it to connections. Set the score for that index to 1
                    if(contains == false)
                    {
                        //Add id mList[i] and set score value to 1. 
                        connections.push_back(std::make_pair(mutualList[i], 1));
                    }
                    
                }
            }
        }
    }

    //Calculating the score. 

    //If connections pair is empty there are no friends to suggest. 
    if(connections.empty())
    {
        score = 0; 
        return{};
    }

    //finalSuggestions stores the friend suggestions with the maxScore 
    std::vector<int> finalSuggestions = {}; 
    int maxScore = 0; 
    for(int c = 0; c < connections.size(); c++)
    {
        //If connections score is greater than the max. Set final suggestions to empty. Add connections id to finalSuggestions 
        if(connections[c].second > maxScore)
        {
            //Reset finalSuggestions to only score new user with maxScore
            finalSuggestions = {};
            //Reset score to be the maxScore 
            maxScore = connections[c].second;
            finalSuggestions.push_back(connections[c].first);
        } 

        //If connections[c] score is the same as maxScore then they can be added to finalSuggestions 
        else if (connections[c].second == maxScore)
        {
            finalSuggestions.push_back(connections[c].first);
        }
    }

    //Set score equal to max score within connections 
    score = maxScore; 

    //Return the final friend suggestions for user 'who' 
    //std::cout << "------" << std::endl;
    //std::cout << (finalSuggestions.size()) << std::endl;
    return finalSuggestions; 

}

//Find the different groups within the network of Users 
std::vector<std::vector<int> > Network::groups()
{
    
    //Tracks which users have been visited 
    std::vector<bool> visited (users_.size(), 0); 

    //No user exists 
    if(users_.size() <= 0)
    {
        return {{}}; //Return empty vector of vetor 
    }


    //STORE VECTOR OF VECTORS FOR RETURN
    std::vector<std::vector<int> > finalGroups; 

    //Loops through all Users in users_ 
    for(int i = 0; i < users_.size(); i++)
    {
        //If visited[i] is false (0). (NOT VISITED YET)
        if(!visited[i])
        {
            //Each DFS search will be stored in group vector. Will be pushed to finalGroups after DFS 
            std::vector<int>group;

            //Stack manages DFS. 
            std::stack<int> stck; 

            //Start stack with the first index in users_
            stck.push(i); 

            //Value at visited[i] = true. (1)
            visited[i] = true; 
        
            //Iterates over all of the connected users 
            while(stck.size() > 0)
            {
                //Set cur to the value at the top of the stack 
                int cur = stck.top();
                //Pop stack, remove cur user 
                stck.pop(); 
                //Push user on top of the stack into group vector. 
                group.push_back(cur); 

                //Go over cur users friends. Each id stored as neighbor
                for(int neighbor : users_[cur]->getFriends())
                {
                    //If neighbor hasnt been visited, proceed 
                    if(!visited[neighbor])
                    {
                        //set visited[neighbor] to 1 
                        visited[neighbor] = true; 
                        //Push neighbor onto stack to continue DFS search until end of connected users 
                        stck.push(neighbor);
                    }
                }
            }
            //Once all of the 
            finalGroups.push_back(group);
        }
    }

    //Return the vector of vector of the different groups in the network. 
    return finalGroups; 

}

//pre:
//Post: Add new post to the messages vector of the user whose id is ownerId. 
void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic)
{
    //Store the user_ at ownerid in order to addPost to that user 
    User* user = getUser(ownerId);
    //Finding message id
    int messageId = 0; 
    for(int i = 0; i < users_.size(); i++)
    {
        User* temp = getUser(i); 
        messageId = messageId + temp->getPosts().size(); //Tracks how many posts there are in the social network 
    }
  
    if(isIncoming)
    {
        IncomingPost* newPost = new IncomingPost(messageId, ownerId, message, likes, isPublic, authorName);
        user->addPost(newPost); //Add incoming post 
    }
    else
    {
        Post* newPost = new Post(messageId, ownerId, message, likes);
        user->addPost(newPost); //Add normal post 
    }
        
    
}

//Pre:
//Post: Call User::getPostsString - return result 
std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic)
{
    //Get the user at the ownerId
    User* user = getUser(ownerId);
    //std::cout << "INSIDE network post string -- user getName --> " << user->getName() << std::endl;

    //Return getPostsString. 
    return user->getPostsString(howMany, showOnlyPublic);
}


//Pre:
//post:: read from a file. Return -1 if file cannot be obtained
int Network::readPosts(char* fname)
{
    std::ifstream file(fname);

    //Return -1 if file cannot be opened 
    if(!file)
    {
        return -1; //File reading error
    }
    int numPosts; 
    std::string line; 
    getline(file, line);
    std::stringstream ss; 
    ss << line; 
    ss >> numPosts; //Stores the number of posts given by the first line 

    for(int i = 0; i < numPosts; i++)
    {
        //Get messageId 
        int messageId;
        std::string mId; 
        getline(file,mId);
        std::stringstream smId; //messageId string stream 
        smId << mId; 
        smId >> messageId;

        //Get Message Text 
        std::string message; 
        std::getline(file, message); 
        //Trim off the tab in the front 
        trimTab(message); //This will call the private func trimTab which trims any leading tab 

        //Recieve ownerId 
        int ownerId; 
        std::string oId; 
        getline(file,oId); 
        trimTab(oId); //Remove leading tab 
        std::stringstream ssOwner; 
        ssOwner << oId; 
        ssOwner >> ownerId; //Stores int value of owner id 

        //Recieve number of likes 
        int likes; 
        std::string stringLike; 
        getline(file, stringLike); 
        trimTab(stringLike); 
        std::stringstream ssLikes; 
        ssLikes << stringLike; 
        ssLikes >> likes; //Stores int value of likes 

        //Recieve state of the post. Owner post if line is empty. OR the string "public" or "private" if message is an incoming post 
        //Also checks if post is private or public 
        bool privacy = true; //tracks whether the post is private or public 
        std::string typeOfPost; 
        getline(file, typeOfPost); 
        trimTab(typeOfPost); 
        if(typeOfPost.empty())
        {
            typeOfPost = "Owner Post";
        }
        else if(typeOfPost == "public")
        {
            typeOfPost = "Incoming";
            privacy = true;
        }
        else if(typeOfPost == "private")
        {
            typeOfPost = "Incoming";
            privacy = false;
        }


        //Recieve state of the author 
        std::string authorState; //Tracks the state of the author. Empty if its owner post. Name if it is incoming 
        std::string authorName;
        std::string typePost; //Stores the type of the post 
        getline(file, authorState);
        trimTab(authorState);
        if(authorState.empty())
        {
            typePost = "Own";
            authorName = "Owner";
        }
        else
        {
            typePost = "Incoming"; 
            authorName = authorState;
        }

        //Create a regular post. Add it to messages_ 
        if(typePost == "Own")
        {
            Post* newPost = new Post(messageId, ownerId, message, likes);
            //Add post to User based on their owner Id 
            for(auto user : users_)
            {
                if (user->getId() == ownerId)
                {
                    user->addPost(newPost); 
                    break; 
                }
            }
        }
        //Add incoming posts to messages_ 
        else if(typePost == "Incoming")
        {
            IncomingPost* newPost = new IncomingPost(messageId, ownerId, message, likes, privacy, authorName);
            for(auto user : users_)
            {
                if(user->getId() == ownerId)
                {
                    user->addPost(newPost); 
                }
            }
        }

    }
    return 0; 
}

//SORT POST BY MESSAGE ID 
bool Network::compareId(Post* post1, Post* post2)
{   
    //If post1 messageId is less than post2 MessageId then reuturn true 
    if(post1->getMessageId() < post2->getMessageId())
    {
        return true; 
    }
    return false; //If post2 messageId is less than post 1 messageId return false
}

//Pre:
//post: Write to a file. Return -1 if file cannot be opened
int Network::writePosts(char* fname)
{
    std::ofstream file(fname);

    //Return -1 if file cannot be opened 
    if(!file)
    {
        return -1; //File reading error
    }
    //Iterate over the users to get messages_ size 
    int getNumPost = 0; 
    for(int i = 0; i < users_.size(); i++)
    {
        User* temp = getUser(i); 
        getNumPost = getNumPost + temp->getPosts().size(); //Tracks how many posts there are in the social network 
    }
    file << getNumPost << "\n"; //Output the number of posts on the top of the txt file 
    //Collect all posts 
    std::vector<Post*> storePosts; //Stores all of the posts from every single user 
    for(User* user : users_)
    {   
        std::vector<Post*> userPost = user->getPosts(); 
        for(int i = 0; i < userPost.size(); i++)
        {
            //Store posts will store each user's posts 
            storePosts.push_back(userPost[i]);
        }
    }

    std::sort(storePosts.begin(), storePosts.end(), compareId);

    for(Post* post : storePosts)
    {
        //If the post is not incoming. Implement. 
        if(post->getAuthor() == "")
        {
            file << post->getMessageId() << "\n";

            file << "\t" << post->getMessage() << "\n";

            file << "\t" << post->getOwnerId() << "\n";

            file << "\t" << post->getLikes() << "\n";

            //Leave line blank if its an owner post 
            file << "\t" << "\n";

            file << "\t" << "\n";

        } 
        //For incoming posts 
        else 
        {
            file << post->getMessageId() << "\n";

            file << "\t" << post->getMessage() << "\n";

            file << "\t" << post->getOwnerId() << "\n";

            file << "\t" << post->getLikes() << "\n";

            //Leave line blank if its an owner post 
            if(post->getIsPublic() == true)
            {
                file << "\t" << "public" << "\n";
            }
            else
            {
                file << "\t" << "private" << "\n";
            }
            

            file << "\t" << post->getAuthor() << "\n";
        }
    }
    return 0;


};





    
        

   

