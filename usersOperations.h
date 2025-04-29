#pragma once
#include "basic.h"
extern stUser currentClient;

namespace display
{
    void displayUser(stUser &user)
    {
        cout << "| " << left << setw(17) << user.username;
        cout << "| " << left << setw(17) << user.password;
        cout << "| " << left << setw(13) << user.permissions;
        cout << "|";
    }

    void showUsers()
    {
        vector<stUser> vUsers;
        fillUsersVector(vUsers);

        cout << setw(20) << right << '(' << vUsers.size() << ") user(s)\n";
        cout << " ____________________________________________________\n";
        cout << "| Username         | Password         | Permissions  |\n";
        cout << " ____________________________________________________\n";

        for (stUser &user : vUsers)
        {
            displayUser(user);
            cout << endl;
        }

        cout << " ____________________________________________________" << endl;
    }
}

namespace add
{
    short readPermissions()
    {
        char choice = input<char>("Do you want the user to have full access? y/n\n");
        if (choice == 'y' || choice == 'Y')
            return -1;

        short permissions = 0;
        choice = input<char>("Do you want the user to to be able to show clients\' data? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pListClients;

        choice = input<char>("Do you want the user to to be able to add clients? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pAddClients;

        choice = input<char>("Do you want the user to to be able to delete clients? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pDeleteClients;

        choice = input<char>("Do you want the user to to be able to update clients? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pUpdateClients;

        choice = input<char>("Do you want the user to to be able to find clients? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pFindClients;

        choice = input<char>("Do you want the user to to be able to do transactions? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pTransactions;

        choice = input<char>("Do you want the user to to be able to manage users? y/n\n");
        if (choice == 'y' || choice == 'Y')
            permissions += enPermissionValue::pManageUsers;

        return permissions;
    }

    void enterUserData(stUser &user)
    {
        user.username = input<string>("Username: ");

        while (findUser(user.username))
        {
            cout << "The user already exists!\n";
            user.username = input<string>("Enter a valid account number: ");
        }

        user.password = input<string>("Password: ");
        user.permissions = readPermissions();
    }

    void enterUserToFile()
    {
        stUser user;
        enterUserData(user);
        appendLineInFile(userToLine(user), usersFilePath);
    }

    void enterUsersToFile()
    {
        cout << "------------------------------------\n";
        cout << "            Add New User            \n";
        cout << "------------------------------------\n";

        char again = 'Y';
        while (again == 'Y' || again == 'y')
        {
            enterUserToFile();
            cout << "User was added successfully.\n";
            again = input<char>("Any more users? (y/n)\n");
        }
    }
}

namespace del
{
    bool deleteUserRecord(string username)
    {
        if (username == "Admin")
        {
            cout << "You can\'t delete this user\n";
            return false;
        }

        vector<stUser> vUsers;
        fillUsersVector(vUsers);

        for (stUser &user : vUsers)
        {
            if (user.username == username)
            {
                displayUserCard(user);

                char choice = input<char>("Are you sure you want to delete this user? y/n\n");
                if (choice == 'y' || choice == 'Y')
                {
                    user.markForDelete = true;
                    saveUsersToFile(vUsers);
                    cout << "User deleted successfully\n";

                    vUsers.clear();
                    fillUsersVector(vUsers);
                    return true;
                }
                else
                    return false;
            }
        }
        cout << "User not found...\n";
        return false;
    }

    void deleteUser()
    {
        cout << "------------------------------------\n";
        cout << "        Delete User Screen          \n";
        cout << "------------------------------------\n";

        string username = input<string>("Enter the username: ");
        deleteUserRecord(username);
    }
}

namespace update
{
    void updateUserRecord(stUser &user)
    {
        user.password = input<string>("Password: ");
        user.permissions = add::readPermissions();
    }

    bool updateUserRecord(string username)
    {
        vector<stUser> vUsers;
        fillUsersVector(vUsers);

        for (stUser &user : vUsers)
        {
            if (user.username == username)
            {
                displayUserCard(user);

                char choice = input<char>("Are you sure you want to update this user? y/n\n");
                if (choice == 'y' || choice == 'Y')
                {
                    updateUserRecord(user);
                    saveUsersToFile(vUsers);
                    cout << "User updated successfully\n";
                    return true;
                }
                else
                    return false;
            }
        }
        cout << "User not found...\n";
        return false;
    }

    void updateUserRecord()
    {
        cout << "------------------------------------\n";
        cout << "        Update User Screen          \n";
        cout << "------------------------------------\n";

        string username = input<string>("Enter the username: ");
        updateUserRecord(username);
    }
}
