#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    char name[50];
    char accountType;
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void modifyAccount();
    void deposit(double);
    void withdraw(double);
    void report() const;
    int getAccountNumber() const;
    double getBalance() const;
    char getAccountType() const;
};

void BankAccount::createAccount() {
    cout << "\nEnter Account Number : ";
    cin >> accountNumber;
    cin.ignore();

    cout << "Enter Account Holder Name : ";
    cin.getline(name, 50);

    cout << "Enter Type of Account (S for Savings / C for Current) : ";
    cin >> accountType;
    accountType = toupper(accountType);

    cout << "Enter Initial Deposit Amount : ";
    cin >> balance;

    cout << "\nAccount Created Successfully...\n";
}

void BankAccount::showAccount() const {
    cout << "\nAccount Number : " << accountNumber;
    cout << "\nAccount Holder Name : " << name;
    cout << "\nType of Account : " << accountType;
    cout << "\nBalance Amount : " << balance << endl;
}

void BankAccount::modifyAccount() {
    cout << "\nModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name, 50);

    cout << "Modify Type of Account : ";
    cin >> accountType;

    accountType = toupper(accountType);

    cout << "Modify Balance Amount : ";
    cin >> balance;

    cout << "\nAccount Updated Successfully...\n";
}

void BankAccount::deposit(double amount) {
    balance += amount;
}

void BankAccount::withdraw(double amount) {
    balance -= amount;
}

void BankAccount::report() const {
    cout << left << setw(15) << accountNumber
         << setw(25) << name
         << setw(10) << accountType
         << setw(15) << balance << endl;
}

int BankAccount::getAccountNumber() const {
    return accountNumber;
}

double BankAccount::getBalance() const {
    return balance;
}

char BankAccount::getAccountType() const {
    return accountType;
}

fstream file;

void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAllAccounts();
void depositWithdraw(int, int);
void introScreen();
void loadingAnimation();
void transactionHistory();
void saveTransaction(string);

void loadingAnimation() {
    cout << "\nLoading";

    for (int i = 0; i < 5; i++) {
        cout << ".";

        for (long j = 0; j < 100000000; j++) {
        }
    }

    cout << "\n";
}

void introScreen() {
    cout << "=======================================================\n";
    cout << "            BANK MANAGEMENT APPLICATION                \n";
    cout << "=======================================================\n";
    cout << "Developed using C++ with OOP and File Handling\n";
    cout << "=======================================================\n\n";
}

void saveTransaction(string transaction) {
    ofstream outFile;
    outFile.open("transactions.txt", ios::app);

    outFile << transaction << endl;

    outFile.close();
}

void transactionHistory() {
    ifstream inFile;
    string line;

    inFile.open("transactions.txt");

    cout << "\n=========== TRANSACTION HISTORY ===========\n";

    while (getline(inFile, line)) {
        cout << line << endl;
    }

    inFile.close();
}

void writeAccount() {
    BankAccount account;

    file.open("account.dat", ios::binary | ios::app);

    account.createAccount();

    file.write(reinterpret_cast<char*>(&account), sizeof(BankAccount));

    file.close();
}

void displayAccount(int number) {
    BankAccount account;
    bool found = false;

    file.open("account.dat", ios::binary | ios::in);

    if (!file) {
        cout << "\nFile could not be opened !!\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&account), sizeof(BankAccount))) {
        if (account.getAccountNumber() == number) {
            account.showAccount();
            found = true;
        }
    }

    file.close();

    if (!found) {
        cout << "\nAccount Number does not exist...\n";
    }
}

void modifyAccount(int number) {
    bool found = false;
    BankAccount account;

    file.open("account.dat", ios::binary | ios::in | ios::out);

    while (file.read(reinterpret_cast<char*>(&account), sizeof(BankAccount)) && !found) {
        if (account.getAccountNumber() == number) {
            account.showAccount();

            cout << "\nEnter New Account Details\n";
            account.modifyAccount();

            int position = (-1) * static_cast<int>(sizeof(BankAccount));
            file.seekp(position, ios::cur);

            file.write(reinterpret_cast<char*>(&account), sizeof(BankAccount));

            cout << "\nRecord Updated Successfully...\n";
            found = true;
        }
    }

    file.close();

    if (!found) {
        cout << "\nRecord Not Found...\n";
    }
}

void deleteAccount(int number) {
    BankAccount account;
    ifstream inFile;
    ofstream outFile;

    inFile.open("account.dat", ios::binary);

    if (!inFile) {
        cout << "\nFile could not be opened !!\n";
        return;
    }

    outFile.open("Temp.dat", ios::binary);

    inFile.seekg(0, ios::beg);

    while (inFile.read(reinterpret_cast<char*>(&account), sizeof(BankAccount))) {
        if (account.getAccountNumber() != number) {
            outFile.write(reinterpret_cast<char*>(&account), sizeof(BankAccount));
        }
    }

    outFile.close();
    inFile.close();

    remove("account.dat");
    rename("Temp.dat", "account.dat");

    cout << "\nRecord Deleted Successfully...\n";
}

void displayAllAccounts() {
    BankAccount account;

    file.open("account.dat", ios::binary | ios::in);

    if (!file) {
        cout << "\nFile could not be opened !!\n";
        return;
    }

    cout << "\n\n================ ALL ACCOUNT HOLDERS ================\n\n";

    cout << left << setw(15) << "Account No"
         << setw(25) << "Name"
         << setw(10) << "Type"
         << setw(15) << "Balance" << endl;

    cout << "======================================================\n";

    while (file.read(reinterpret_cast<char*>(&account), sizeof(BankAccount))) {
        account.report();
    }

    file.close();
}

void depositWithdraw(int number, int option) {
    double amount;
    bool found = false;

    BankAccount account;

    file.open("account.dat", ios::binary | ios::in | ios::out);

    if (!file) {
        cout << "\nFile could not be opened !!\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&account), sizeof(BankAccount)) && !found) {
        if (account.getAccountNumber() == number) {
            account.showAccount();

            if (option == 1) {
                cout << "\nEnter Amount to Deposit : ";
                cin >> amount;

                account.deposit(amount);

                saveTransaction("Deposit: Account No " + to_string(number) +
                                " Amount " + to_string(amount));

                cout << "\nAmount Deposited Successfully...\n";
            }

            if (option == 2) {
                cout << "\nEnter Amount to Withdraw : ";
                cin >> amount;

                int currentBalance = account.getBalance() - amount;

                if (currentBalance < 0) {
                    cout << "\nInsufficient Balance...\n";
                }
                else {
                    account.withdraw(amount);

                    saveTransaction("Withdraw: Account No " + to_string(number) +
                                    " Amount " + to_string(amount));

                    cout << "\nAmount Withdrawn Successfully...\n";
                }
            }

            int position = (-1) * static_cast<int>(sizeof(BankAccount));
            file.seekp(position, ios::cur);

            file.write(reinterpret_cast<char*>(&account), sizeof(BankAccount));

            found = true;
        }
    }

    file.close();

    if (!found) {
        cout << "\nRecord Not Found...\n";
    }
}

void searchByName() {
    BankAccount account;
    char searchName[50];
    bool found = false;

    cin.ignore();

    cout << "\nEnter Account Holder Name to Search : ";
    cin.getline(searchName, 50);

    file.open("account.dat", ios::binary | ios::in);

    while (file.read(reinterpret_cast<char*>(&account), sizeof(BankAccount))) {
        if (strcmp(searchName, searchName) >= 0) {
            account.showAccount();
            found = true;
        }
    }

    file.close();

    if (!found) {
        cout << "\nNo Account Found...\n";
    }
}

void accountStatistics() {
    BankAccount account;
    int totalAccounts = 0;
    double totalBalance = 0;

    file.open("account.dat", ios::binary | ios::in);

    while (file.read(reinterpret_cast<char*>(&account), sizeof(BankAccount))) {
        totalAccounts++;
        totalBalance += account.getBalance();
    }

    file.close();

    cout << "\n=========== ACCOUNT STATISTICS ===========\n";
    cout << "Total Accounts : " << totalAccounts << endl;
    cout << "Total Balance  : " << totalBalance << endl;

    if (totalAccounts > 0) {
        cout << "Average Balance: " << totalBalance / totalAccounts << endl;
    }
}

int main() {
    int choice;
    int number;

    introScreen();
    loadingAnimation();

    do {
        cout << "\n\n=========== MAIN MENU ===========\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Balance Inquiry\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Close an Account\n";
        cout << "7. Modify an Account\n";
        cout << "8. Transaction History\n";
        cout << "9. Account Statistics\n";
        cout << "10. Exit\n";

        cout << "\nSelect Your Option (1-10) : ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeAccount();
            break;

        case 2:
            cout << "\nEnter Account Number : ";
            cin >> number;
            depositWithdraw(number, 1);
            break;

        case 3:
            cout << "\nEnter Account Number : ";
            cin >> number;
            depositWithdraw(number, 2);
            break;

        case 4:
            cout << "\nEnter Account Number : ";
            cin >> number;
            displayAccount(number);
            break;

        case 5:
            displayAllAccounts();
            break;

        case 6:
            cout << "\nEnter Account Number : ";
            cin >> number;
            deleteAccount(number);
            break;

        case 7:
            cout << "\nEnter Account Number : ";
            cin >> number;
            modifyAccount(number);
            break;

        case 8:
            transactionHistory();
            break;

        case 9:
            accountStatistics();
            break;

        case 10:
            cout << "\nThank You for Using Bank Management System...\n";
            break;

        default:
            cout << "\nInvalid Option... Please Try Again...\n";
        }

    } while (choice != 10);

    return 0;
}