#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"

#include <iostream>

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);


    //Read users that are within the network.
    network.readUsers("newUsers.txt");

    //Read posts within the social network
    network.readPosts("posts.txt");

    connect(ui->loginButton, &QPushButton::clicked, this,
            &SocialNetworkWindow::loginButtonClick);

    //Friends Table click
    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::friendTableClick);

    //Suggest Friends Table Click
    connect(ui->suggestTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::suggestTableClick);


    //Back button when on current user page
    connect(ui->backButton, &QPushButton::clicked, this,
            &SocialNetworkWindow::backButtonClick);

    //Add friend button
    connect(ui->addButton, &QPushButton::clicked, this, &SocialNetworkWindow::addButtonClick);

    //Allowing logged in user to click on current friend table
    connect(ui->currentFriendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::currentFriendTableClick);

    //Connect Search Button
    connect(ui->searchButton, &QPushButton::clicked, this,
            &SocialNetworkWindow::searchButtonClick);


    //Connect Home Button
    connect(ui->homeButton, &QPushButton::clicked, this,
            &SocialNetworkWindow::homeButtonClick);


    //Connect Publish Button
    connect(ui->publishButton, &QPushButton::clicked, this,
          &SocialNetworkWindow::publishButtonClick);

    //Call display to show login page
    display();
}

//TRIM TAB FUNCTION
void SocialNetworkWindow::trimTab(std::string& str)
{

    while (!str.empty() && str[0] == '\t')
    {
        //Removes any leading tabs
        str.erase(0, 1);
    }

    // Remove trailing tabs
    while (!str.empty() && str.back() == '\t')
    {
        //This will remove the last character in a string if its a tab.
        str.pop_back();
    }
}

void SocialNetworkWindow::loginButtonClick()
{
    //DEBUGGING
    std::cout << "Login attempt: " << ui->loginText->toPlainText().toStdString() << std::endl;


    clicked = true; //User has clicked.
    //If the user does not input any name into the login text box. Return a message
    if(ui->loginText->toPlainText().isEmpty())
    {
        ui->messageLabel->show();
        return; //End search
    }
    //Checks if a user is found
    bool found = false;

    //DEBUGGING
    //std::cout << "NUMBER OF USERS: " << network.numUsers() << std::endl;

    for(int i = 0; i < network.numUsers(); i++)
    {
        User* user = network.getUser(i); // Get the user pointer


        //****DEBUGGING*****
        //std::cout << "Checking user: " << user->getName() << std::endl;

        if(ui->loginText->toPlainText() == QString::fromStdString(user->getName()))
        {
            //Keep track of logged in user
            loggedInUser = user;
            //User found
            found = true;

            //***DEBUGGING***
            //std::cout << "Match found!" << std::endl;

            //std::cout << loggedInUser->getName() << std::endl;
            //std::cout << "bio: " << loggedInUser->getBio() << std::endl;


            //Adds logged in user to the stack.
            userStack.push(loggedInUser);

            //Call display function
            display();

            break;
        }
    }


    if(found == false)
    {

        //****DEBUGGING*****
        std::cout << "No user found." << std::endl;

        //Call display function to show message. This occurs when loggedInUser is still nullptr
        display();
    }


}

void SocialNetworkWindow::friendTableClick(int row, int col)
{
    ui->searchLabel->hide();
    QTableWidgetItem* item = ui->friendsTable->item(row,col);

    //Get the friends name in the table
    QString friendName = item->text();
    //int friendId;
    bool found = false; //Tracks if a user is found
    for(int i = 0; i < network.numUsers(); i++)
    {
        //get user object
        User* user = network.getUser(i);
        if(QString::fromStdString(user->getName()) == friendName)
        {
            //Set currentUser to user in order to keep track of display
            currentUser = user;

            userStack.push(currentUser);
            //friendId = user->getId();
            found = true;
        }
    }
    //if found is false, there is no user.
    if(found == false)
    {
        return;
    }
    display();

}

void SocialNetworkWindow::suggestTableClick(int row, int col)
{

     ui->searchLabel->hide();

    QTableWidgetItem* item = ui->suggestTable->item(row,col);

    //Get the friends name in the table
    QString suggestName = item->text();
    //int suggestId;
    bool found = false; //Tracks if a user is found
    for(int i = 0; i < network.numUsers(); i++)
    {
        //get user object
        User* user = network.getUser(i);
        if(QString::fromStdString(user->getName()) == suggestName)
        {
            //Set currentUser to user in order to keep track of display
            currentUser = user;

            //Add user to the stack
            userStack.push(currentUser);

            //suggestId = user->getId();
            found = true;
        }
    }
    //if found is false, there is no user.
    if(found == false)
    {
        return;
    }
    display();
}

void SocialNetworkWindow::backButtonClick()
{
     ui->searchLabel->hide();

    //Set current user to null in order to go back to the display with currentUser as nullptr and logged in user != nullptr

    //Pop the current user
    userStack.pop();

    //Set current user to the user next in the stack
    currentUser = userStack.top();

    if(userStack.top() == loggedInUser)
    {
        ui->backButton->hide();

        //Reset Display
        currentUser = nullptr;
    }

    display();
}

void SocialNetworkWindow::addButtonClick()
{
    std::set<int> friends = loggedInUser->getFriends();
    bool friendshipFound = false;
    for(auto id : friends)
    {
        //Gets the user at id
        User* userFriend = network.getUser(id);

        if(userFriend->getId() == currentUser->getId())
        {
            //Current user and loggedInUser are friends
            friendshipFound = true;
        }
    }
    //If the current user and loggedInUser are not friends. Add them as friends.
    if(friendshipFound == false)
    {
        loggedInUser->addFriend(currentUser->getId());

        currentUser->addFriend(loggedInUser->getId());

        //Update the network with the new friendship
        network.writeUsers("newUsers.txt");


    }

    display();
}

//Allow user to click on friends' friend's profile
void SocialNetworkWindow::currentFriendTableClick(int row, int col)
{
    ui->searchLabel->hide();
    QTableWidgetItem* item = ui->currentFriendsTable->item(row,col);

    //Get the friends name in the table
    QString friendName = item->text();

    //int friendId;
    bool found = false; //Tracks if a user is found
    for(int i = 0; i < network.numUsers(); i++)
    {
        //get user object
        User* user = network.getUser(i);
        if(QString::fromStdString(user->getName()) == friendName)
        {
            //std::cout <<"DEBUGGING INSIDE LOOP" << std::endl;
            //std::cout << "Searching for user: " << friendName.toStdString() << ", Found user: " << user->getName() << std::endl;

            //Set currentUser to user in order to keep track of display
            currentUser = user;

            userStack.push(currentUser);




            //friendId = user->getId();
            found = true;
        }
    }




    //std::cout<< "**** DEBUGGING OUTSIDE LOOP ****" << std::endl;
    //if found is false, there is no user.
    if(found == false)
    {
        return;
    }
    display();

}


//Search button implementation
void SocialNetworkWindow::searchButtonClick()
{
    //If the user does not input any name into the login text box. Return a message
    if(ui->searchBar->toPlainText().isEmpty())
    {
        ui->searchLabel->show();
    }
    //Checks if a user is found
    bool found = false;
    for(int i = 0; i < network.numUsers(); i++)
    {
        User* user = network.getUser(i); // Get the user pointer
        if(ui->searchBar->toPlainText() == QString::fromStdString(user->getName()))
        {
            ui->searchLabel->hide();
            //Keep track of logged in user
            currentUser = user;

            //User found
            found = true;

            //Adds logged in user to the stack.
            userStack.push(currentUser);

            //Call display function
            display();

            break;
        }
    }

    if(found == false)
    {
        ui->searchLabel->show();
        //Call display function to show message. This occurs when loggedInUser is still nullptr
        display();
    }

}


//Home button implementation
void SocialNetworkWindow::homeButtonClick()
{
    //Set current user to null in order to go back to the display with currentUser as nullptr and logged in user != nullptr
    currentUser = nullptr;

    ui->searchLabel->hide();
    //Clear the user stack to reset.
    while(!userStack.empty())
    {
        userStack.pop();
    }

    //push logged in user back onto the stack. Should always be the first user in the stack. Always at the bottom
    userStack.push(loggedInUser);

    display();
}


//Connect publish bio button
void SocialNetworkWindow::publishButtonClick()
{
    std::cout << "publishButtonClicked!!!!!" << std::endl;
    // If the user does not input any name into the bio text box, show an error message
    if (ui->bioTextEdit->toPlainText().isEmpty())
    {
        ui->bioErrorLabel->setText("Please enter a bio."); // Update the error label
        ui->bioErrorLabel->show(); // Show the error message
    }
    else
    {
        // Set user bio to the bio entered by user
        QString userBio = ui->bioTextEdit->toPlainText();

        // Convert QString to std::string for trimming
        std::string userBioStr = userBio.toStdString();

        // Trim tab character if necessary
        trimTab(userBioStr);

        // CHECK IF TRIM TAB WORKED.
        std::cout << "IF TRIM TAB WORKS, NO TAB SHOULD BE AFTER ARROW ->" << userBioStr << std::endl;

        // Set the trimmed bio back to the user
        loggedInUser->setBio(userBioStr);

        // Debug output
        std::cout << "Updated bio: " << loggedInUser->getBio() << std::endl;

        ui->bioErrorLabel->hide(); // Hide any previous error messages

        // Write the updated user info to a file
        network.writeUsers("newUsers.txt");
    }

    display(); // Update the display as needed
}





/*




                        DISPLAY FUNCTION BELOW






*/

void SocialNetworkWindow::display()
{
    //Beginning of the program. Hide all profile buttons and error messages
    if(loggedInUser == nullptr && currentUser == nullptr && clicked == false)
    {


        ui->messageLabel->hide();
        ui->profileLabel->hide();
        ui->friendsTable->hide();
        ui->suggestTable->hide();
        ui->postLabel->hide();
        ui->backButton->hide();
        ui->addButton->hide();
        ui->suggestLabel->hide();
        ui->friendsLabel->hide();
        ui->postTitle->hide();
        ui->currentFriendsTable->hide();
        ui->currentFriendLabel->hide();

        //Final Project Additions.
        ui->homeButton->hide();
        ui->publishButton->hide();
        ui->searchBar->hide();
        ui->searchButton->hide();
        ui->bioText->hide();
        ui->bioTextEdit->hide();
        ui->bioLabel->hide();
        ui->searchLabel->hide();
        ui->bioErrorLabel->hide();

        //Set the error message but dont display it
        ui->messageLabel->setText("MUST ENTER A USER NAME THAT IS WITHIN THE NETWORK");

    }
    //SHOW ERROR MESSAGE. THE USER ENTERED IS INVALID. loggedInUser remained nullptr.
    else if(loggedInUser == nullptr && clicked == true)
    {
        ui->messageLabel->show();
    }

    //SHOW LOGGED IN USER PAGE HERE
    else if(loggedInUser != nullptr && currentUser == nullptr && clicked == true)
    {
        //ui->searchLabel->hide();
        ui->loginButton->hide();
        ui->messageLabel->hide();
        ui->loginText->hide();
        ui->loginLabel->hide();
        ui->backButton->hide();
        ui->addButton->hide();
        ui->currentFriendLabel->hide();
        ui->currentFriendsTable->hide();

        //Final Project Additions.
        ui->homeButton->hide();
        ui->publishButton->show();
        ui->searchBar->show();
        ui->searchButton->show();
        ui->bioText->show();
        ui->bioTextEdit->show();
        ui->bioLabel->hide();




        //Need to show profile page buttons and tables here !!!! **********
        ui->profileLabel->setText("Your Page");
        ui->profileLabel->show();
        ui->postLabel->show();
        ui->suggestTable->show();
        ui->friendsTable->show();
        ui->suggestLabel->show();
        ui->friendsLabel->show();
        ui->postTitle->show();

        //IMPLEMENTING FRIENDS TABLE
        std::set<int> friends = loggedInUser->getFriends();
        ui->friendsTable->setColumnCount(1);
        ui->friendsTable->setRowCount(friends.size());

        //Keep track of which row to add friend
        int row = 0;
        for(auto id : friends)
        {
            //Gets the user at id
            User* userFriend = network.getUser(id);

            //Store the friends name
            QString friendName = QString::fromStdString(userFriend->getName());

            //Creates an item for the table
            QTableWidgetItem* item = new QTableWidgetItem(friendName);

            ui->friendsTable->setItem(row, 0, item);

            row++;
        }

        //IMPLEMENTING POSTS for Logged In User.
        int id = loggedInUser->getId();
        //std::cout << "***BEFORE POSTS***" << std::endl;
        //Output the message
        ui->postLabel->setText(QString::fromStdString(network.getPostsString(id, 5, false)));


        //IMPLEMENTING SUGGESTED FRIENDS



        //Iterating over all of the suggested friends to be added to the table
        int score;
        std::vector<int> suggestedFriends = network.suggestFriends(id, score);
        //std::cout << "Suggested Friends Count:" << suggestedFriends.size() << std::endl;
        int size = suggestedFriends.size();

        //Suggested friends table.
        ui->suggestTable->setColumnCount(1);
        ui->suggestTable->setRowCount(size);
        for(int i = 0; i < size; i++)
        {
            //std::cout<< "***SUGGESTED FRIEND ID***. -->   " << suggestedFriends[i] << std::endl;
            User* suggestedFriend = network.getUser(suggestedFriends[i]);
            //Store each suggested friends name
            QString suggestName = QString::fromStdString(suggestedFriend->getName());
            //std::cout << "Suggested Friend Name: " << suggestName.toStdString() << std::endl; // Debug line
            //Creates an item for the table that stores the suggested friends name
            QTableWidgetItem* item = new QTableWidgetItem(suggestName);

            ui->suggestTable->setItem(i, 0, item); //Updates each row in the table
        }

        // Implementing bio for logged in user
        std::string bio = loggedInUser->getBio();
        trimTab(bio);
        if (!bio.empty())
        {
            //DEBUGGING:
            //std::cout << "IF TRIMMED->" << bio << std::endl;
            ui->bioTextEdit->setText(QString::fromStdString(bio));
        }
        else
        {
            ui->bioTextEdit->setText("Bio not posted. Click here to add a bio.");
        }


    }
    else if(currentUser != nullptr && clicked == true)
    {

        //Need to show profile page buttons and tables here !!!! **********
        QString name = QString::fromStdString(currentUser->getName());
        ui->profileLabel->setText(name + "'s Page");
        ui->profileLabel->show();
        ui->postLabel->show();
        ui->suggestTable->hide();
        ui->friendsTable->hide();
        ui->suggestLabel->hide();
        ui->friendsLabel->hide();
        ui->currentFriendLabel->show();
        ui->postTitle->show();
        ui->backButton->show();
        ui->addButton->show();
        ui->currentFriendsTable->show();


        //Final Project Additions.
        ui->homeButton->show();
        ui->publishButton->show();
        ui->searchBar->show();
        ui->searchButton->show();
        ui->bioText->show();
        ui->bioTextEdit->hide();
        ui->publishButton->hide();
        //ui->searchLabel->hide();
        ui->bioErrorLabel->hide();

        //CHECK IF THE USER HAS A LABEL OR NOT!!! If not, set label to Bio in progress... OR No Bio Posted.
        ui->bioLabel->show();

        //Show user's bio
        std::string bio = currentUser->getBio();
        trimTab(bio);
        if (!bio.empty())
        {
            std::cout << "IF TRIMMED->" << bio << std::endl;

            ui->bioLabel->setText(QString::fromStdString(bio));
        }
        else
        {
            ui->bioLabel->setText("No Bio Posted");
        }




        //IMPLEMENTING currentUser friend TABLE
        std::set<int> friends = currentUser->getFriends();
        ui->currentFriendsTable->setColumnCount(1);
        ui->currentFriendsTable->setRowCount(friends.size());

        //Keep track of which row to add friend
        int row = 0;
        for(auto id : friends)
        {
            //Gets the user at id
            User* userFriend = network.getUser(id);

            //Store the friends name
            QString friendName = QString::fromStdString(userFriend->getName());

            //Creates an item for the table
            QTableWidgetItem* item = new QTableWidgetItem(friendName);

            ui->currentFriendsTable->setItem(row, 0, item);

            row++;
        }

        //Showing posts for current User.

        //IMPLEMENTING POSTS For Current User Page. NOT logged in uger.
        int id = currentUser->getId();
        //std::cout << "***BEFORE POSTS***" << std::endl;
        //Output the message
        ui->postLabel->setText(QString::fromStdString(network.getPostsString(id, 5, true)));

    }
}


SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}


