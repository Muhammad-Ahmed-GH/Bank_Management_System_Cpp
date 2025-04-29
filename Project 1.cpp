#include "clientsOperations.h"
using namespace display;
using namespace add;
using namespace del;
using namespace update;
using namespace transact;
using namespace manageUsers;

void showMainMenu();
void Login();

stUser currentUser;

void performAction(enMainAction choice)
{
    clearScreen();

    switch (choice)
    {
    case enMainAction::Show:
        showClients();
        waitToPress();
        showMainMenu();
        break;

    case enMainAction::Add:
        enterClientsToFile();
        waitToPress();
        showMainMenu();
        break;

    case enMainAction::Delete:
        deleteClient();
        waitToPress();
        showMainMenu();
        break;

    case enMainAction::Update:
        updateClientRecord();
        waitToPress();
        showMainMenu();
        break;

    case enMainAction::Find:
        findClient();
        waitToPress();
        showMainMenu();

    case enMainAction::Transactions:
        transactions();
        showMainMenu();
        break;

    case enMainAction::ManageUsers:
        manageUsersScreen();
        showMainMenu();
        break;

    case enMainAction::Logout:
        Login();
        break;

    default:
        cout << "Invalid input!\n";
        waitToPress();
        showMainMenu();
        break;
    }
}

void showMainMenu()
{
    clearScreen();
    cout << "==========================================\n";
    cout << "             Main Menu Screen             \n";
    cout << "==========================================\n";
    cout << "      [1] Show Client List\n";
    cout << "      [2] Add New Client\n";
    cout << "      [3] Delete Client\n";
    cout << "      [4] Update Client Info\n";
    cout << "      [5] Find Client\n";
    cout << "      [6] Transactions\n";
    cout << "      [7] Manage Users\n";
    cout << "      [8] Logout\n";
    cout << "==========================================\n";
    cout << "What do you want to do?\n";

    performAction((enMainAction)input<short>());
}

void Login()
{
    bool loginFail = false;
    do
    {
        clearScreen();
        cout << "---------------------\n";
        cout << "        Login        \n";
        cout << "---------------------\n";
        if(loginFail)
            cout << "Invalid username/password\n";

        currentUser.username = input<string>("Username: ");
        currentUser.password = input<string>("Password: ");

        loginFail = !findUser(currentUser);

    } while (loginFail);
    showMainMenu();
}

int main()
{
    Login();
    return 0;
}