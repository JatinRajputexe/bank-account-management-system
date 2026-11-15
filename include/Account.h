#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdexcept>
#include <string>
#include <vector>

#include "Transaction.h"
#include "Utils.h"

// Custom exception: thrown when a withdrawal is larger than the available balance.
class InsufficientBalanceException : public std::runtime_error {
public:
    explicit InsufficientBalanceException(double available)
        : std::runtime_error("Insufficient balance (available: " + util::money(available) + ")") {}
};

class Account {
private:  // encapsulation: data can only change through the member functions below
    int accountNumber;
    std::string customerName;
    double balance;
    std::vector<Transaction> history;

public:
    // Constructor overloading
    Account();
    Account(int accNo, const std::string& name, double openingBalance);  // brand-new account
    Account(int accNo, const std::string& name, double currentBalance,
            const std::vector<Transaction>& savedHistory);               // restored from file

    // Getters
    int getAccountNumber() const;
    std::string getCustomerName() const;
    double getBalance() const;
    const std::vector<Transaction>& getHistory() const;

    // Function overloading: same name, different parameter lists
    void deposit(double amount);
    void deposit(double amount, const std::string& remark);
    void withdraw(double amount);

    void display() const;                // details + balance
    void display(bool withHistory) const;  // details (+ history if true)
    void showHistory() const;
};

#endif
