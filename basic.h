#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <conio.h>
#include <vector>
#include <string>
using namespace std;

const string clientsFilePath = "clients.txt";
const string usersFilePath = "users.txt";
const string FileDelim = "#//#";

enum enMainAction
{
    Show = 1,
    Add,
    Delete,
    Update,
    Find,
    Transactions,
    ManageUsers,
    Logout
};

enum enUsersAction
{
    ShowUser = 1,
    AddUser,
    DeleteUser,
    UpdateUser,
    FindUser,
    MainMenu
};

enum enTrans
{
    eDeposit = 1,
    eWithdraw,
    eDisplayBalances,
    eMainMenu
};

enum enPermissionValue
{
    pListClients = 1,
    pAddClients = 2,
    pDeleteClients = 4,
    pUpdateClients = 8,
    pFindClients = 16,
    pTransactions = 32,
    pManageUsers = 64,
};

struct stClient
{
    string accountNumber;
    string PINcode;
    string name;
    string phone;
    double balance;
    bool markForDelete = false;
};

struct stUser
{
    string username;
    string password;
    int permissions;
    bool markForDelete = false;
};

extern stUser currentClient;

template <class T>
T input(string message = "")
{
    cout << message;
    T data;
    cin >> data;
    cin.ignore();
    return data;
}

string readLine(string message = "")
{
    string line;
    cout << message;
    getline(cin, line);
    return line;
}

void clearScreen()
{
    cout << "\033[2J\033[H";
}

void waitToPress()
{
    cout << "\nPress any key to continue... ";
    getch();
}

bool validateAccess(enPermissionValue permission)
{
    if ((permission & currentClient.permissions) == permission)
        return true;
    return false;
}

void showAccessDenied()
{
    cout << "-------------------------\n";
    cout << "      Access Denied      \n";
    cout << "-------------------------\n";
    cout << "You don\'t have access to this feature\n";
    cout << "You can contact your admin\n";
}

vector<string> strSplit(string str, string delim = " ")
{
    vector<string> words;
    string word;
    int delimPos;
    while ((delimPos = str.find(delim)) != string::npos)
    {
        word = str.substr(0, delimPos);
        if (word != "")
            words.push_back(word);

        str.erase(0, delimPos + delim.length());
    }
    if (str != "")
        words.push_back(str);

    return words;
}

stClient convertLineToClient(string line, string delim = FileDelim)
{
    vector<string> vData = strSplit(line, delim);

    stClient client;
    client.accountNumber = vData[0];
    client.PINcode = vData[1];
    client.name = vData[2];
    client.phone = vData[3];
    client.balance = stod(vData[4]);

    return client;
}

stUser convertLineToUser(string line, string delim = FileDelim)
{
    vector<string> vData = strSplit(line, delim);

    stUser user;
    user.username = vData[0];
    user.password = vData[1];
    user.permissions = stoi(vData[2]);
    return user;
}

string clientToLine(stClient &client, string delim = FileDelim)
{
    string line = client.accountNumber + delim;
    line += (client.PINcode + delim);
    line += (client.name + delim);
    line += (client.phone + delim);
    line += to_string(client.balance);
    return line;
}

string userToLine(stUser &user, string delim = FileDelim)
{
    string line = user.username + delim;
    line += (user.password + delim);
    line += to_string(user.permissions);
    return line;
}

void fillClientsVector(vector<stClient> &vClients)
{
    fstream fClients;
    fClients.open(clientsFilePath, ios::in);
    if (fClients.is_open())
    {
        string line;

        while (getline(fClients, line))
            vClients.push_back(convertLineToClient(line));

        fClients.close();
    }
}

void fillUsersVector(vector<stUser> &vUsers)
{
    fstream fUsers;
    fUsers.open(usersFilePath, ios::in);
    if (fUsers.is_open())
    {
        string line;

        while (getline(fUsers, line))
            vUsers.push_back(convertLineToUser(line));

        fUsers.close();
    }
}

void saveClientsToFile(vector<stClient> &vClients)
{
    fstream fClients;
    fClients.open(clientsFilePath, ios::out);
    if (fClients.is_open())
    {
        for (stClient &client : vClients)
            if (!client.markForDelete)
                fClients << clientToLine(client) << endl;

        fClients.close();
    }
}

void saveUsersToFile(vector<stUser> &vUsers)
{
    fstream fUsers;
    fUsers.open(usersFilePath, ios::out);
    if (fUsers.is_open())
    {
        for (stUser &user : vUsers)
            if (!user.markForDelete)
                fUsers << userToLine(user) << endl;

        fUsers.close();
    }
}

void appendLineInFile(string line, string filePath)
{
    fstream file;
    file.open(filePath, ios::app);
    if (file.is_open())
    {
        file << line << endl;
        file.close();
    }
}

void displayClientCard(stClient &client)
{
    cout << left << setw(14) << "Account Number" << ": " << client.accountNumber << endl;
    cout << left << setw(14) << "PIN Code" << ": " << client.PINcode << endl;
    cout << left << setw(14) << "Name" << ": " << client.name << endl;
    cout << left << setw(14) << "Phone" << ": " << client.phone << endl;
    cout << left << setw(14) << "Balance" << ": " << client.balance << endl;
}

void displayUserCard(stUser &user)
{
    cout << left << setw(12) << "Username" << ": " << user.username << endl;
    cout << left << setw(12) << "Password" << ": " << user.password << endl;
    cout << left << setw(12) << "Permissions" << ": " << user.permissions << endl;
}

bool findClient(vector<stClient> &vClients, string accountNumber, stClient &client)
{
    for (stClient &c : vClients)
        if (c.accountNumber == accountNumber)
        {
            client = c;
            return true;
        }
    return false;
}

bool findClient(string accountNumber)
{
    fstream fClients;
    string line;

    fClients.open(clientsFilePath, ios::in);
    if (fClients.is_open())
    {
        while (getline(fClients, line))
        {
            if (convertLineToClient(line).accountNumber == accountNumber)
            {
                fClients.close();
                return true;
            }
        }
        fClients.close();
    }
    return false;
}

bool findClient()
{
    cout << "------------------------------------\n";
    cout << "        Find Client Screen          \n";
    cout << "------------------------------------\n";

    string accountNumber = input<string>("Enter account number: ");
    vector<stClient> vClients;
    fillClientsVector(vClients);

    for (stClient &client : vClients)
    {
        if (client.accountNumber == accountNumber)
        {
            displayClientCard(client);
            return true;
        }
    }
    cout << "Client not found\n";
    return false;
}

bool findUser(stUser &user)
{
    vector<stUser> vUsers;
    fillUsersVector(vUsers);

    for (stUser &u : vUsers)
    {
        if (user.username == u.username && user.password == u.password)
        {
            user = u;
            return true;
        }
    }
    return false;
}

bool findUser(string username)
{
    vector<stUser> vUsers;
    fillUsersVector(vUsers);

    for (stUser &user : vUsers)
        if (user.username == username)
            return true;

    return false;
}

bool findUser()
{
    cout << "------------------------------------\n";
    cout << "         Find User Screen           \n";
    cout << "------------------------------------\n";

    string username = input<string>("Enter username: ");
    vector<stUser> vUsers;
    fillUsersVector(vUsers);

    for (stUser &user : vUsers)
    {
        if (user.username == username)
        {
            displayUserCard(user);
            return true;
        }
    }
    cout << "User not found\n";
    return false;
}
