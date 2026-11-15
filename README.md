# Bank Account Management System

Mini Project 3 | Object-Oriented Programming using C++ (CIC-211)

Authors: Vishnu Prasad(18914803125) & Jatin(19014803125) | Maharaja Agrasen Institute of Technology

A console, menu-driven C++17 application that simulates basic banking: create accounts, deposit, withdraw
(never beyond the balance), display accounts, search an account and view its transaction history.
Records are saved to a file and reloaded on the next run.

## Repository structure

```
bank-account-management-system/
├── include/
│   ├── Transaction.h    # one history entry (type, amount, balance after, remark)
│   ├── Account.h        # Account class + InsufficientBalanceException
│   ├── Bank.h           # Bank class: account list, menu, file I/O
│   └── Utils.h          # money formatting + safe console input helpers
├── src/
│   ├── main.cpp         # short main(): creates Bank and runs the menu
│   ├── Account.cpp
│   └── Bank.cpp
├── data/                # accounts.txt is created here at runtime
├── tests/
│   ├── sample_input.txt # scripted input for the test cases
│   └── test_cases.md
├── docs/                # report PDF and screenshots
├── Makefile
├── .gitignore
└── README.md
```

## Build and run

```
g++ -std=c++17 -Wall -Iinclude src/main.cpp src/Account.cpp src/Bank.cpp -o bank
./bank              # Windows: bank.exe
```

Or use `make run`. Needs GCC 9+ (for `std::filesystem`); on older GCC add `-lstdc++fs`.
Run it from the repository root so `data/accounts.txt` is found.

## OOP concepts and where they are used

| Concept | Where |
|---|---|
| Classes and objects, encapsulation | `Account` keeps all data private; `Transaction`, `Bank` |
| Constructors | `Account()`, `Account(no, name, opening)`, `Account(no, name, balance, history)` |
| Function overloading | `Account::deposit(amount)` / `deposit(amount, remark)`, `display()` / `display(bool)` |
| Exception handling | `InsufficientBalanceException`, `std::invalid_argument`, try-catch in `Bank::run()` |
| File handling | `Bank::loadFromFile()`, `Bank::saveToFile()` (`ifstream`/`ofstream`) |
| Vectors | `vector<Account>` in `Bank`, `vector<Transaction>` in `Account` |

## Data file format (`data/accounts.txt`)

```
A|accountNumber|customerName|balance|numberOfTransactions
T|type|amount|balanceAfter|remark
```

## Mapped SDG

SDG 8 (Decent Work and Economic Growth),
SDG 9 (Industry, Innovation and Infrastructure)&
SDG 16 (Strong Institutions).
