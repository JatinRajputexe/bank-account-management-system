#include "Bank.h"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace {

// Splits "a|b||c" into {"a","b","","c"} (keeps empty fields).
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (true) {
        std::size_t pos = line.find(delimiter, start);
        if (pos == std::string::npos) {
            parts.push_back(line.substr(start));
            break;
        }
        parts.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    return parts;
}

}  // namespace

Bank::Bank(const std::string& file) : dataFile(file) {
    loadFromFile();  // records are available again as soon as the program starts
}

// ---------------------------------------------------------------- file handling
// File format (one account block, then its transactions):
//   A|accountNumber|customerName|balance|numberOfTransactions
//   T|type|amount|balanceAfter|remark
void Bank::loadFromFile() {
    std::ifstream in(dataFile);
    if (!in.is_open()) {  // normal on the very first run
        std::cout << "No saved data found - starting with an empty bank.\n";
        return;
    }

    std::string line;
    int lineNo = 0;
    try {
        while (std::getline(in, line)) {
            ++lineNo;
            if (line.empty()) continue;

            std::vector<std::string> f = split(line, '|');
            if (f.size() != 5 || f[0] != "A") throw std::runtime_error("bad account line");
            int accNo = std::stoi(f[1]);
            double balance = std::stod(f[3]);
            int count = std::stoi(f[4]);

            std::vector<Transaction> history;
            for (int i = 0; i < count; ++i) {
                if (!std::getline(in, line)) throw std::runtime_error("missing transaction line");
                ++lineNo;
                std::vector<std::string> t = split(line, '|');
                if (t.size() != 5 || t[0] != "T") throw std::runtime_error("bad transaction line");
                history.emplace_back(t[1], std::stod(t[2]), std::stod(t[3]), t[4]);
            }
            accounts.emplace_back(accNo, f[2], balance, history);
        }
        std::cout << "Loaded " << accounts.size() << " account(s) from " << dataFile << ".\n";
    } catch (const std::exception& e) {
        std::cout << "Warning: data file is damaged near line " << lineNo << " (" << e.what()
                  << "). " << accounts.size() << " account(s) were loaded.\n";
        in.close();
        // keep a copy so nothing is lost when the file is rewritten
        std::ifstream original(dataFile, std::ios::binary);
        std::ofstream backup(dataFile + ".bak", std::ios::binary);
        if (original.is_open() && backup.is_open()) {
            backup << original.rdbuf();
            std::cout << "A backup was saved as " << dataFile << ".bak\n";
        }
    }
}

void Bank::saveToFile() const {
    std::ofstream out(dataFile);
    if (!out.is_open()) {  // e.g. the data/ folder is missing
        std::cout << "Error: could not open " << dataFile
                  << " for writing (does the data folder exist?). Changes NOT saved.\n";
        return;
    }
    out << std::fixed << std::setprecision(2);
    for (const Account& acc : accounts) {
        const std::vector<Transaction>& h = acc.getHistory();
        out << "A|" << acc.getAccountNumber() << '|' << acc.getCustomerName() << '|'
            << acc.getBalance() << '|' << h.size() << '\n';
        for (const Transaction& t : h) {
            out << "T|" << t.getType() << '|' << t.getAmount() << '|' << t.getBalanceAfter()
                << '|' << t.getRemark() << '\n';
        }
    }
    if (!out) std::cout << "Error: writing to " << dataFile << " failed. Changes may be lost.\n";
}

// ------------------------------------------------------------------ core logic
Account* Bank::findAccount(int accNo) {
    for (Account& acc : accounts) {
        if (acc.getAccountNumber() == accNo) return &acc;
    }
    return nullptr;
}

void Bank::createAccount(int accNo, const std::string& name, double openingBalance) {
    if (findAccount(accNo) != nullptr)
        throw std::invalid_argument("Account number " + std::to_string(accNo) + " already exists.");
    accounts.emplace_back(accNo, name, openingBalance);  // constructor validates the data
    saveToFile();
}

// ------------------------------------------------------------------- menu part
void Bank::printMenu() const {
    std::cout << "\n========== BANK ACCOUNT MANAGEMENT SYSTEM ==========\n"
              << " 1. Create account\n"
              << " 2. Deposit money\n"
              << " 3. Withdraw money\n"
              << " 4. Display all accounts\n"
              << " 5. Search account / view transaction history\n"
              << " 0. Exit\n"
              << "====================================================\n";
}

Account* Bank::askForAccount() {
    int accNo = util::readInt("Account number: ");
    Account* acc = findAccount(accNo);
    if (acc == nullptr) std::cout << "Account not found.\n";
    return acc;
}

void Bank::createAccountMenu() {
    int accNo = util::readInt("New account number: ");
    std::string name = util::readLine("Customer name: ");
    double opening = util::readDouble("Opening balance (Rs.): ");
    createAccount(accNo, name, opening);
    std::cout << "Account " << accNo << " created successfully.\n";
}

void Bank::depositMenu() {
    Account* acc = askForAccount();
    if (acc == nullptr) return;
    double amount = util::readDouble("Amount to deposit (Rs.): ");
    acc->deposit(amount);
    saveToFile();
    std::cout << "Deposit successful. New balance: " << util::money(acc->getBalance()) << "\n";
}

void Bank::withdrawMenu() {
    Account* acc = askForAccount();
    if (acc == nullptr) return;
    double amount = util::readDouble("Amount to withdraw (Rs.): ");
    acc->withdraw(amount);  // may throw InsufficientBalanceException
    saveToFile();
    std::cout << "Withdrawal successful. New balance: " << util::money(acc->getBalance()) << "\n";
}

void Bank::displayAllAccounts() const {
    if (accounts.empty()) {
        std::cout << "No accounts available.\n";
        return;
    }
    std::cout << "\nTotal accounts: " << accounts.size() << "\n";
    for (const Account& acc : accounts) {
        std::cout << "----------------------------------------\n";
        acc.display();
    }
    std::cout << "----------------------------------------\n";
}

void Bank::searchAccountMenu() {
    Account* acc = askForAccount();
    if (acc == nullptr) return;
    std::cout << "----------------------------------------\n";
    acc->display(true);
    std::cout << "----------------------------------------\n";
}

void Bank::run() {
    bool running = true;
    while (running) {
        printMenu();
        int choice = util::readInt("Enter your choice: ");
        try {
            switch (choice) {
                case 1: createAccountMenu(); break;
                case 2: depositMenu(); break;
                case 3: withdrawMenu(); break;
                case 4: displayAllAccounts(); break;
                case 5: searchAccountMenu(); break;
                case 0:
                    running = false;
                    std::cout << "Thank you for using the Bank Account Management System.\n";
                    break;
                default: std::cout << "Invalid choice. Please select 0-5.\n";
            }
        } catch (const InsufficientBalanceException& e) {
            std::cout << "Transaction failed: " << e.what() << "\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}