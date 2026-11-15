#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>

#include "Account.h"

class Bank {
private:
    std::vector<Account> accounts;
    std::string dataFile;

    // File handling
    void loadFromFile();
    void saveToFile() const;

    // Menu actions (each one reads input, calls Account/Bank functions, prints result)
    void printMenu() const;
    Account* askForAccount();
    void createAccountMenu();
    void depositMenu();
    void withdrawMenu();
    void displayAllAccounts() const;
    void searchAccountMenu();

public:
    explicit Bank(const std::string& file = "data/accounts.txt");

    void createAccount(int accNo, const std::string& name, double openingBalance);
    Account* findAccount(int accNo);

    void run();  // main menu loop
};

#endif
