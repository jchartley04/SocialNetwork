#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include <stack>
#include "post.h"
#include "network.h"
#include "user.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    //Trim leading tab
    void trimTab(std::string& str);

    //pre:
    //post: login page either passes or throws an error
    void loginButtonClick();

    //pre:
    //post: decides the state of the displayed window
    void display();

    //pre:
    //post: Show friend profile
    void friendTableClick(int row, int col);

    //pre:
    //post: show suggested friend profile
    void suggestTableClick(int row, int col);

    //pre:
    //post: return to logged in user page
    void backButtonClick();


    //pre: loggedInUser & CurrentUser are not friends
    //post: Add loggedInUser and CurrentUser as friends
    void addButtonClick();

    //pre:
    //post: Go to current users friends page
    void currentFriendTableClick(int row, int col);


    //pre:
    //post: Search button clicked will go to found user or show error message. Push onto userstack
    void searchButtonClick();


    //pre:
    //post: Go back to logged in user page
    void homeButtonClick();

    //pre:
    //post: Add a bio to your user page. Write this bio to Users.txt
    void publishButtonClick();


private:
    Ui::SocialNetworkWindow *ui;

    //Class Invarients
    //1. network object to store network of users
    //2. loggedUser tracks the user who is logged in
    //3. currentUser tracks which user page we are on
    //4. Tracks whether user has attempted to sign in

    Network network;
    User* loggedInUser = nullptr;
    User* currentUser = nullptr;
    bool clicked = false;

    //Keeps track of all profiles until back to home page
    std::stack<User*> userStack;

};
#endif // SOCIALNETWORKWINDOW_H
