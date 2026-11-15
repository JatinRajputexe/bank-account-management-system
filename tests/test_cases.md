# Test cases

Run: `./bank < tests/sample_input.txt` (delete `data/accounts.txt` first for a fresh start).

| # | Input | Expected output |
|---|-------|-----------------|
| 1 | Create account 1001, "Ravi", opening balance 5000 | Account 1001 created successfully |
| 2 | Deposit 2000 into 1001 | New balance: Rs. 7000.00 |
| 3 | Withdraw 3000 from 1001 | New balance: Rs. 4000.00 |
| 4 | Withdraw 10000 from 1001 (boundary / invalid) | Transaction failed: Insufficient balance (available: Rs. 4000.00); balance unchanged |
| 5a | Deposit -500 | Error: Deposit amount must be greater than zero. |
| 5b | Create account 1001 again | Error: Account number 1001 already exists. |
| 6a | Search account 9999 | Account not found. |
| 6b | Restart program, choose 4 (Display all) | "Loaded 1 account(s)" and Ravi's balance Rs. 4000.00 is shown |
| 7 | Search account 1001 | Details + history: 3 rows (opening, deposit, withdrawal) |
