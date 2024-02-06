#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include "myMethod.h"
#include "conio.h"

using namespace std;
using namespace MyMethod;

string const fileName = "Client.txt";

struct ClientData {
    string AccountNumber;
    string FullName;
    string PinCode;
    string Phone;
    double AccountBalance{};
    bool isMarkForDelete = false;
};

enum enMainOption {
    enPrintAllClients = 1,
    enAddClient = 2,
    enUpdateClient = 3,
    enDeletrClient = 4,
    enFindClient = 5,
    enTransactionMenue = 6,
    enExit = 7
};

enum enTransaction {
    enDeposite = 1,
    enWithdraw = 2,
    enPrint = 3,
    enExitToMainMenu = 4
};

vector<ClientData> loadClientDataFromFile(const string &fileName);

void showMainMenu();

void bankSystem();

void transaction();

void showTransactionMenu();

bool isClientExist(const string &accountNumber) {
    vector<ClientData> vClient = loadClientDataFromFile(fileName);
    for (ClientData &cData: vClient) {
        if (cData.AccountNumber == accountNumber)
            return true;
    }
    return false;
}

ClientData readDateFromUser() {
    ClientData data;
    cout << "Enter Acount Number ?";
    getline(cin >> ws, data.AccountNumber);
    while (isClientExist(data.AccountNumber)) {
        cout << "\nClient with [" << data.AccountNumber <<
                "] is already exists, Enter another Account Number?";
        getline(cin >> ws, data.AccountNumber);
    }
    cout << "Enter Full Name ?";
    getline(cin >> ws, data.FullName);
    cout << "Enter PinCode ?";
    getline(cin, data.PinCode);
    cout << "Enter Phone ?";
    getline(cin, data.Phone);
    cout << "Enter Acount Balance ?";
    cin >> data.AccountBalance;
    return data;
}

string clintDataAsString(const ClientData &data, const string &sep) {
    return data.FullName + sep +
           data.AccountNumber + sep +
           to_string(data.AccountBalance) + sep +
           data.PinCode + sep +
           data.Phone;
}

ClientData readDataFromString(string txt) {
    ClientData data;
    const vector<string> vText = split(std::move(txt), "#//#");
    data.FullName = vText.at(0);
    data.AccountNumber = vText.at(1);
    data.AccountBalance = stod(vText.at(2));
    data.PinCode = vText.at(3);
    data.Phone = vText.at(4);
    return data;
}

void saveDateInFile(const string &fileName, const string &stStringLine) {
    fstream myFile;
    myFile.open(fileName, ios::out | ios::app);
    if (myFile.is_open()) {
        myFile << stStringLine << endl;
        myFile.close();
    }
}

void addNewClient() {
    const ClientData client = readDateFromUser();
    saveDateInFile(fileName, clintDataAsString(client, "#//#"));
}

void addClients() {
    char enterAgain = 'Y';
    do {
        system("cls");
        cout << "Adding New Client:\n";
        addNewClient();
        cout << "\nClient Added Successfully, do u want to add more Clients?";
        cin >> enterAgain;
    } while (toupper(enterAgain) == 'Y');
}

vector<ClientData> loadClientDataFromFile(const string &fileName) {
    vector<ClientData> vText;
    fstream myFile;
    myFile.open(fileName, ios::in);

    if (myFile.is_open()) {
        string line;
        ClientData data;

        while (getline(myFile, line)) {
            data = readDataFromString(line);
            vText.push_back(data);
        }
        myFile.close();
    }
    return vText;
}

void PrintClientRecord(const ClientData &Client, bool isMain = true) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(20) << left << Client.FullName;
    cout << "| " << setw(12) << left << Client.AccountBalance;
    if (isMain) {
        cout << "| " << setw(10) << left << Client.PinCode;
        cout << "| " << setw(12) << left << Client.Phone;
    }
}

void PrintClientRecords(const ClientData &Client) {
    cout << "\n************************\n";
    cout << "Acount Number :" << Client.AccountNumber;
    cout << "\nPin code :" << left << Client.PinCode;
    cout << "\nName :" << Client.FullName;
    cout << "\nPhone :" << left << Client.Phone;
    cout << "\nAcount Balance :" << Client.AccountBalance;
    cout << "\n************************\n";
}

double totalBalance(const vector<ClientData> &vClients) {
    double sum = 0.0;
    for (ClientData client: vClients) {
        sum += client.AccountBalance;
    }
    return sum;
}

void PrintAllClientsData(const vector<ClientData> &vClients, bool isMain = true) {
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(20) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    if (isMain) {
        cout << "| " << left << setw(10) << "Pin Code";
        cout << "| " << left << setw(12) << "Phone";
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (const ClientData &Client: vClients) {
        PrintClientRecord(Client, isMain);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    if (!isMain) {
        cout << left << setw(41) << "" << totalBalance(vClients) << endl;
    }
}

bool findClientByAccountNumber(const vector<ClientData> &vData, ClientData &data, const string &accountNumber) {
    for (const ClientData &client: vData) {
        if (client.AccountNumber == accountNumber) {
            data = client;
            return true;
        }
    }
    return false;
}

string readClientAccountNumber() {
    cout << "Enter Account Number?\n";
    string accountNummber;
    getline(cin >> ws, accountNummber);
    return accountNummber;
}

void saveVectortoFileAfterUpdateclient(const string &fileName, const vector<ClientData> &vClient) {
    fstream myFile;
    myFile.open(fileName, ios::out);
    if (myFile.is_open()) {
        for (const ClientData &client: vClient) {
            if (client.isMarkForDelete == false) {
                myFile << clintDataAsString(client, "#//#") << endl;
            }
        }
        myFile.close();
    }
}

bool markClientToDeletIt(const string &accountNumber, vector<ClientData> &vClient) {
    for (ClientData &client: vClient) {
        if (client.AccountNumber == accountNumber) {
            client.isMarkForDelete = true;
            return true;
        }
    }
    return false;
}

ClientData updateRecordData(const ClientData &clientData) {
    ClientData data;
    data.AccountNumber = clientData.AccountNumber;
    cout << "Enter Full Name ?";
    getline(cin >> ws, data.FullName);
    cout << "Enter PinCode ?";
    getline(cin, data.PinCode);
    cout << "Enter Phone ?";
    getline(cin, data.Phone);
    cout << "Enter Acount Balance ?";
    cin >> data.AccountBalance;
    return data;
}

ClientData transRecordData(const ClientData &clientData, int newAmount, enTransaction transaction) {
    ClientData data;
    data.AccountNumber = clientData.AccountNumber;
    data.Phone = clientData.Phone;
    data.FullName = clientData.FullName;
    data.PinCode = clientData.PinCode;
    if (transaction == enWithdraw) {
        data.AccountBalance = clientData.AccountBalance - newAmount;
    } else {
        data.AccountBalance = clientData.AccountBalance + newAmount;
    }
    return data;
}

double showBalance(vector<ClientData> &vClient, const string &accountN) {
    double acccout = 0.0;
    for (ClientData &cData: vClient) {
        if (cData.AccountNumber == accountN) {
            acccout = cData.AccountBalance;
            break;
        }
    }
    return acccout;
}

bool deleteClientByAccountNumber(const string &accountNumber, vector<ClientData> &vClient) {
    ClientData stClient;
    if (findClientByAccountNumber(vClient, stClient, accountNumber)) {
        char deleteOrNot = 'n';
        PrintClientRecords(stClient);
        cout << "\nAre You sure u want delete this record y/n ?";
        cin >> deleteOrNot;
        if (tolower(deleteOrNot) == 'y') {
            markClientToDeletIt(accountNumber, vClient);
            saveVectortoFileAfterUpdateclient(fileName, vClient);
            cout << "\nClient Delete\n";
            return true;
        }
    } else {
        system("color 4F");
        cout << "Accout Number [" << accountNumber << "] is Not Found\n";
        system("color 0F");
        return false;
    }
}

bool updateClientByAccountNumber(const string &accountNumber, vector<ClientData> &vClient) {
    ClientData stClient;
    if (findClientByAccountNumber(vClient, stClient, accountNumber)) {
        char updateOrNot = 'n';
        PrintClientRecords(stClient);
        cout << "\nAre You sure u want update this record y/n ?";
        cin >> updateOrNot;
        if (tolower(updateOrNot) == 'y') {
            for (ClientData &data: vClient) {
                if (data.AccountNumber == accountNumber) {
                    data = updateRecordData(data);
                    break;
                }
            }
            saveVectortoFileAfterUpdateclient(fileName, vClient);
            cout << "\nUpdate Done\n";
            return true;
        }
    } else {
        system("color 4F");
        cout << "Accout Number [" << accountNumber << "] is Not Found\n";
        system("color 0F");
        return false;
    }
}

bool transactionOperations(const string &accountNumber, vector<ClientData> &vClient, double amount) {
    char updateOrNot = 'n';
    cout << "\nAre You sure u want to update Account Balance of this record y/n ?";
    cin >> updateOrNot;

    if (tolower(updateOrNot) == 'y') {
        for (ClientData &data: vClient) {
            if (data.AccountNumber == accountNumber) {
                data.AccountBalance += amount;
                saveVectortoFileAfterUpdateclient(fileName, vClient);
                cout << "\nTransaction Done." << "Your Balance = " << showBalance(vClient, accountNumber) << endl;

                return true;
            }
        }
    }
}

void transactionMenuEcho() {
    system("echo Press any key to back to Transaction Menue...");
    system("pause > nul");
    showTransactionMenu();
}

string convertEnumToString(enTransaction transaction) {
    string txt = "";
    transaction == enDeposite ? txt = "Deposite" : txt = "WithDraw";
    return txt;
}

void drawAndDepositeOperation(vector<ClientData> vClient, enTransaction transaction) {
    cout << "\n*******************\n";
    cout << "------" << convertEnumToString(transaction) << "-----\n";
    cout << "*******************\n";

    ClientData stClient;
    string accountNumber = readClientAccountNumber();

    while (!findClientByAccountNumber(vClient, stClient, accountNumber)) {
        cout << "\nClient with [" << accountNumber << "] does not found!!\n";
        accountNumber = readClientAccountNumber();
    }
    PrintClientRecords(stClient);

    double amount = 0.0;
    cout << "\nEnter " << convertEnumToString(transaction) << " Amount? ";
    cin >> amount;
    if (transaction == enDeposite) {
        transactionOperations(accountNumber, vClient, amount);
    } else {
        while (amount > stClient.AccountBalance) {
            cout << "\nNo Enought money in Account to withdraw!! \nYour current Balance is = " << stClient.
                    AccountBalance;
            cout << "\nEnter another amount : ";
            cin >> amount;
        }
        transactionOperations(accountNumber, vClient, amount * -1);
    }
}

void transaction(enTransaction transaction) {
    vector<ClientData> vClient = loadClientDataFromFile(fileName);
    switch (transaction) {
        case enWithdraw: {
            drawAndDepositeOperation(vClient, enWithdraw);
            transactionMenuEcho();
            break;
        }
        case enDeposite: {
            drawAndDepositeOperation(vClient, enDeposite);
            transactionMenuEcho();
            break;
        }
        case enPrint: {
            PrintAllClientsData(vClient, false);
            transactionMenuEcho();
            break;
        }
        case enExitToMainMenu: {
            showMainMenu();
            break;
        }
    }
}

void showMainMenu() {
    cout << "---------------------\n";
    cout <<
            "[1]:Print all Record\n[2]:Add Client\n[3]:Update Client\n[4]:Delete Client\n[5]:Find Client\n[6]:Transictions\n[7]:Exit\n";
    cout << "---------------------\n";
    bankSystem();
}

void showTransactionMenu() {
    cout << "---------------------\n";
    cout << "[1]:Deposit\n[2]:WithDraw\n[3]:PrintRecords\n[4]:Back to Main Menu\n";
    cout << "---------------------\n";
    transaction(enTransaction(readNumber(1, 4)));
}

void usingEcho() {
    system("echo Press any key to back to Main Menue...");
    system("pause > nul");
    showMainMenu();
}

void bankSystem() {
    vector<ClientData> vClient = loadClientDataFromFile(fileName);
    switch (static_cast<enMainOption>(readNumber(1, 7))) {
        case enPrintAllClients: {
            PrintAllClientsData(vClient);
            usingEcho();
            break;
        }
        case enAddClient: {
            addClients();
            usingEcho();
            break;
        }
        case enUpdateClient: {
            updateClientByAccountNumber(readClientAccountNumber(), vClient);
            usingEcho();
            break;
        }
        case enDeletrClient: {
            deleteClientByAccountNumber(readClientAccountNumber(), vClient);
            usingEcho();
            break;
        }
        case enFindClient: {
            ClientData stClient; //empty client
            findClientByAccountNumber(vClient, stClient, readClientAccountNumber());
            PrintClientRecords(stClient);
            usingEcho();
            break;
        }
        case enTransactionMenue: {
            showTransactionMenu();
            break;
        }
        case enExit: {
            cout << "\nThank u to Use Our Services Good Bay :)";
            break;
        }
    }
}

int main() {
    showMainMenu();
    return 0;
}
