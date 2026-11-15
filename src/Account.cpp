#include "Account.h"

#include <iomanip>
#include <iostream>

Account::Account() : accountNumber(0), customerName(""), balance(0.0) {}

Account::Account(int accNo, const std::string& name, double openingBalance)
    : accountNumber(accNo), customerName(util::trim(util::sanitize(name))), balance(0.0) {
    if (accountNumber <= 0)
        throw std::invalid_argument("Account number must be a positive number.");
    if (customerName.empty())
        throw std::invalid_argument("Customer name cannot be empty.");
    openingBalance = util::round2(openingBalance);
    if (openingBalance < 0)
        throw std::invalid_argument("Opening balance cannot be negative.");
    if (openingBalance > 0)
        deposit(openingBalance, "Opening balance");
}

Account::Account(int accNo, const std::string& name, double currentBalance,
                 const std::vector<Transaction>& savedHistory)
    : accountNumber(accNo), customerName(name), balance(currentBalance), history(savedHistory) {}

int Account::getAccountNumber() const { return accountNumber; }
std::string Account::getCustomerName() const { return customerName; }
double Account::getBalance() const { return balance; }
const std::vector<Transaction>& Account::getHistory() const { return history; }

void Account::deposit(double amount) {
    deposit(amount, "Cash deposit");
}

void Account::deposit(double amount, const std::string& remark) {
    amount = util::round2(amount);
    if (amount <= 0)
        throw std::invalid_argument("Deposit amount must be greater than zero.");
    balance = util::round2(balance + amount);
    history.emplace_back("DEPOSIT", amount, balance, util::sanitize(remark));
}

void Account::withdraw(double amount) {
    amount = util::round2(amount);
    if (amount <= 0)
        throw std::invalid_argument("Withdrawal amount must be greater than zero.");
    if (amount > balance)  // never allow a withdrawal greater than the balance
        throw InsufficientBalanceException(balance);
    balance = util::round2(balance - amount);
    history.emplace_back("WITHDRAW", amount, balance, "Cash withdrawal");
}

void Account::display() const {
    std::cout << "  Account Number : " << accountNumber << "\n"
              << "  Customer Name  : " << customerName << "\n"
              << "  Balance        : " << util::money(balance) << "\n";
}

void Account::display(bool withHistory) const {
    display();
    if (withHistory) showHistory();
}

void Account::showHistory() const {
    std::cout << "  Transaction history:\n";
    if (history.empty()) {
        std::cout << "    (no transactions yet)\n";
        return;
    }
    std::cout << "    " << std::left << std::setw(5) << "No." << std::setw(10) << "Type"
              << std::setw(16) << "Amount" << std::setw(16) << "Balance" << "Remark\n";
    int n = 1;
    for (const Transaction& t : history) {
        std::cout << "    " << std::left << std::setw(5) << n++ << std::setw(10) << t.getType()
                  << std::setw(16) << util::money(t.getAmount()) << std::setw(16)
                  << util::money(t.getBalanceAfter()) << t.getRemark() << "\n";
    }
    std::cout << std::right;
}
