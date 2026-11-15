#include <exception>
#include <iostream>

#include "Bank.h"

int main() {
    try {
        Bank bank;  // loads data/accounts.txt if it exists
        bank.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
