#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// One entry of an account's history: what happened and the balance afterwards.
class Transaction {
private:
    std::string type;  // "DEPOSIT" or "WITHDRAW"
    double amount;
    double balanceAfter;
    std::string remark;

public:
    Transaction() : type(""), amount(0.0), balanceAfter(0.0), remark("") {}
    Transaction(const std::string& t, double amt, double balAfter, const std::string& rem = "")
        : type(t), amount(amt), balanceAfter(balAfter), remark(rem) {}

    std::string getType() const { return type; }
    double getAmount() const { return amount; }
    double getBalanceAfter() const { return balanceAfter; }
    std::string getRemark() const { return remark; }
};

#endif
