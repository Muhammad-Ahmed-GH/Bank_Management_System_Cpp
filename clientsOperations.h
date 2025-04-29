#pragma once
#include "basic.h"
#include "usersOperations.h"
extern stUser currentClient;

namespace display
{
    void displayClient(stClient &client)
    {
        cout << "| " << left << setw(15) << client.accountNumber;
        cout << "| " << left << setw(11) << client.PINcode;
        cout << "| " << left << setw(23) << client.name;
        cout << "| " << left << setw(14) << client.phone;
        cout << "| " << left << setw(14) << client.balance;
        cout << "|";
    }

    void showClients()
    {
        if (!validateAccess(enPermissionValue::pListClients))
        {
            showAccessDenied();
            return;
        }
        vector<stClient> vClients;
        fillClientsVector(vClients);

        cout << setw(37) << right << '(' << vClients.size() << ") client(s)\n";
        cout << " ______________________________________________________________________________________" << endl;
        cout << "| Account Number | PIN Code   | Name                   | Phone         | Balance       |" << endl;
        cout << " ______________________________________________________________________________________" << endl;

        for (stClient &client : vClients)
        {
            displayClient(client);
            cout << endl;
        }

        cout << " ______________________________________________________________________________________" << endl;
    }
}

namespace add
{
    void enterClientData(stClient &client)
    {
        client.accountNumber = input<string>("Account number: ");

        while (findClient(client.accountNumber))
        {
            cout << "The client already exists!\n";
            client.accountNumber = input<string>("Enter a valid account number: ");
        }

        client.PINcode = input<string>("PIN code: ");
        client.name = readLine("Name: ");
        client.phone = input<string>("Phone: ");
        client.balance = input<double>("Balance: ");
    }

    void enterClientToFile()
    {
        stClient client;
        enterClientData(client);
        appendLineInFile(clientToLine(client), clientsFilePath);
    }

    void enterClientsToFile()
    {
        if (!validateAccess(enPermissionValue::pAddClients))
        {
            showAccessDenied();
            return;
        }

        cout << "------------------------------------\n";
        cout << "           Add New Client           \n";
        cout << "------------------------------------\n";

        char again = 'Y';
        while (again == 'Y' || again == 'y')
        {
            enterClientToFile();
            cout << "Client was added successfully.\n";
            again = input<char>("Any more clients? (y/n)\n");
        }
    }
}

namespace del
{
    bool deleteClientRecord(string accountNumber)
    {
        vector<stClient> vClients;
        fillClientsVector(vClients);

        for (stClient &client : vClients)
        {
            if (client.accountNumber == accountNumber)
            {
                displayClientCard(client);

                char choice = input<char>("Are you sure you want to delete this client? y/n\n");
                if (choice == 'y' || choice == 'Y')
                {
                    client.markForDelete = true;
                    saveClientsToFile(vClients);
                    cout << "Client deleted successfully\n";

                    vClients.clear();
                    fillClientsVector(vClients);
                    return true;
                }
                else
                    return false;
            }
        }
        cout << "Client not found...\n";
        return false;
    }

    void deleteClient()
    {
        if (!validateAccess(enPermissionValue::pDeleteClients))
        {
            showAccessDenied();
            return;
        }

        cout << "------------------------------------\n";
        cout << "       Delete Client Screen         \n";
        cout << "------------------------------------\n";

        string accountNumber = input<string>("Enter the account number: ");
        deleteClientRecord(accountNumber);
    }
}

namespace update
{
    void updateClientRecord(stClient &client)
    {
        client.PINcode = input<string>("PIN code: ");
        client.name = readLine("Name: ");
        client.phone = input<string>("Phone: ");
    }

    bool updateClientRecord(string accountNumber)
    {
        vector<stClient> vClients;
        fillClientsVector(vClients);

        for (stClient &client : vClients)
        {
            if (client.accountNumber == accountNumber)
            {
                displayClientCard(client);

                char choice = input<char>("Are you sure you want to update this client? y/n\n");
                if (choice == 'y' || choice == 'Y')
                {
                    updateClientRecord(client);
                    saveClientsToFile(vClients);
                    cout << "Client updated successfully\n";
                    return true;
                }
                else
                    return false;
            }
        }
        cout << "Client not found...\n";
        return false;
    }

    void updateClientRecord()
    {
        if (!validateAccess(enPermissionValue::pUpdateClients))
        {
            showAccessDenied();
            return;
        }

        cout << "------------------------------------\n";
        cout << "       Update Client Screen         \n";
        cout << "------------------------------------\n";

        string accountNumber = input<string>("Enter the account number: ");
        updateClientRecord(accountNumber);
    }
}

namespace transact
{
    void transactions();

    void addToBalance(vector<stClient> &vClients, string accountNumber, double amount)
    {
        for (stClient &client : vClients)
        {
            if (client.accountNumber == accountNumber)
            {
                client.balance += amount;
                saveClientsToFile(vClients);
                cout << "Transaction done successfully\n";
                break;
            }
        }
    }

    void deposit()
    {
        cout << "-----------------------------\n";
        cout << "           Deposit           \n";
        cout << "-----------------------------\n";

        stClient client;
        vector<stClient> vClients;
        fillClientsVector(vClients);

        string accountNumber = input<string>("Enter account number: ");

        while (!findClient(vClients, accountNumber, client))
        {
            cout << "The client doesn\'t exist!\n";
            accountNumber = input<string>("Enter another account number: ");
        }
        displayClientCard(client);

        double amount = input<double>("How much money? ;-)\n");
        addToBalance(vClients, accountNumber, amount);
    }

    void withdraw()
    {
        cout << "-----------------------------\n";
        cout << "          Withdraw           \n";
        cout << "-----------------------------\n";

        stClient client;
        vector<stClient> vClients;
        fillClientsVector(vClients);

        string accountNumber = input<string>("Enter account number: ");

        while (!findClient(vClients, accountNumber, client))
        {
            cout << "The client doesn\'t exist!\n";
            accountNumber = input<string>("Enter another account number: ");
        }
        displayClientCard(client);

        double amount = input<double>("How much money? ;-)\n");
        while (amount > client.balance)
        {
            cout << "The amount exceeded your balance!\n";
            amount = input<double>("How much money?\n");
        }
        addToBalance(vClients, accountNumber, -1 * amount);
    }

    void displayBalanceRecord(stClient &client)
    {
        cout << "| " << left << setw(15) << client.accountNumber;
        cout << "| " << left << setw(23) << client.name;
        cout << "| " << left << setw(14) << client.balance;
        cout << "|";
    }

    void diplayBalances()
    {
        vector<stClient> vClients;
        fillClientsVector(vClients);
        double totalBalances = 0;

        cout << setw(23) << right << '(' << vClients.size() << ") client(s)\n";
        cout << " _________________________________________________________\n";
        cout << "| Account Number | Name                   | Balance       |\n";
        cout << " _________________________________________________________\n";

        for (stClient &client : vClients)
        {
            displayBalanceRecord(client);
            cout << '\n';
            totalBalances += client.balance;
        }

        cout << " _________________________________________________________" << endl;
        cout << setw(32) << right << "Total balances = " << totalBalances << '\n';
    }

    void performTransaction(enTrans trans)
    {
        clearScreen();
        switch (trans)
        {
        case enTrans::eDeposit:
            deposit();
            waitToPress();
            transactions();
            break;
        case enTrans::eWithdraw:
            withdraw();
            waitToPress();
            transactions();
            break;
        case enTrans::eDisplayBalances:
            diplayBalances();
            waitToPress();
            transactions();
            break;
        case enTrans::eMainMenu:
            break;

        default:
            cout << "Invalid input!\n";
            waitToPress();
            transactions();
            break;
        }
    }

    void transactions()
    {
        if (!validateAccess(enPermissionValue::pTransactions))
        {
            showAccessDenied();
            waitToPress();
            return;
        }

        clearScreen();
        cout << "------------------------------------\n";
        cout << "            Transactions            \n";
        cout << "------------------------------------\n";
        cout << "[1] Deposit\n";
        cout << "[2] Withdraw\n";
        cout << "[3] Display Balances\n";
        cout << "[4] Return to main menu\n";
        enTrans trans = (enTrans)input<short>("What do you want to do? [1 - 4]\n");
        performTransaction(trans);
    }
}

namespace manageUsers
{
    void manageUsersScreen();

    void performAction(enUsersAction action)
    {
        clearScreen();
        switch (action)
        {
        case enUsersAction::ShowUser:
            display::showUsers();
            break;
        case enUsersAction::AddUser:
            add::enterUsersToFile();
            break;
        case enUsersAction::DeleteUser:
            del::deleteUser();
            break;
        case enUsersAction::UpdateUser:
            update::updateUserRecord();
            break;
        case enUsersAction::FindUser:
            findUser();
            break;
        case enUsersAction::MainMenu:
            return;
        default:
            cout << "Invalid input!\n";
            break;
        }
        waitToPress();
        manageUsersScreen();
    }

    void manageUsersScreen()
    {
        if (!validateAccess(enPermissionValue::pManageUsers))
        {
            showAccessDenied();
            waitToPress();
            return;
        }

        clearScreen();
        cout << "===============================\n";
        cout << "      Manage Users Screen      \n";
        cout << "===============================\n";
        cout << "      [1] List Users\n";
        cout << "      [2] Add Users\n";
        cout << "      [3] Delete Users\n";
        cout << "      [4] Update Users\n";
        cout << "      [5] Find Users\n";
        cout << "      [6] Main Menu\n";

        enUsersAction action = (enUsersAction)input<short>("What do you want to do? [1 - 6]\n");
        performAction(action);
    }
}
