#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <ctime>

using namespace std;
// --- Helper Functions for Input Validation ---
namespace Validation {
    bool isValidString(const string& str);
    bool isAlphanumeric(const string& str);
    bool isValidInteger(int value);
    bool isValidDate(const string& date);
    bool isValidBloodType(const string& bloodType);
    void clearInputBuffer();
    int getIntegerInput();
    string getStringInput(const string& prompt);
}

// --- Base Class: User ---
class User {
protected:
    string userID;
    string name;
    string contact;
    string role;

public:
    User(const string& id, const string& n, const string& c, const string& r);
    virtual ~User() = default;
    string getUserID() const;
    string getName() const;
    string getContact() const;
    string getRole() const;
    bool setUserID(const string& id);
    bool setName(const string& n);
    bool setContact(const string& c);
    bool setRole(const string& r);
    virtual void displayUserInfo() const;
};

// --- Derived Class: Donor ---
class Donor : public User {
private:
    string bloodType;

public:
    Donor(const string& id, const string& n, const string& c, const string& bt);
    string getBloodType() const;
    bool setBloodType(const string& bt);
    void displayUserInfo() const ;
};

// --- Derived Class: AdminStaff ---
class AdminStaff : public User {
public:
    AdminStaff(const string& id, const string& n, const string& c);
    void displayUserInfo() const ;
};

// --- Derived Class: Requestor ---
class Requestor : public User {
public:
    Requestor(const string& id, const string& n, const string& c);
    void displayUserInfo() const ;
};

// --- Class: BloodInventory ---
class BloodInventory {
private:
    string bloodType;
    int quantity;
    string donationDate;

public:
    BloodInventory(const string& bt, int qty, const string& dd);
    string getBloodType() const;
    int getQuantity() const;
    string getDonationDate() const;
    bool setBloodType(const string& bt);
    bool setQuantity(int qty);
    bool setDonationDate(const string& dd);
    void increaseQuantity(int qty);
    void decreaseQuantity(int qty);
    void displayBloodInfo() const;
    bool isExpired(const string& currentDate) const;
};

// --- Class: BloodRequest ---
class BloodRequest {
private:
    string requestID;
    string requestorID;
    string bloodType;
    int quantity;
    string status;
    string requestDate;

public:
    BloodRequest(const string& id, const string& reqID, const string& bt, int qty, const string& rd);
    string getRequestID() const;
    string getRequestorID() const;
    string getBloodType() const;
    int getQuantity() const;
    string getStatus() const;
    string getRequestDate() const;
    bool setStatus(const string& st);
    bool setBloodType(const string& bt);
    bool setQuantity(int qty);
    bool setRequestDate(const string& rd);
    void displayRequestInfo() const;
};

// --- Class: UserManager ---
class UserManager {
private:
    vector<User*> users;

public:
    UserManager();
    ~UserManager();
    bool addUser(const string& role, const string& id, const string& name, const string& contact, const string& bloodType = "");
    User* getUserByID(const string& id);
    vector<User*> getUsersByRole(const string& role) const;
    void displayAllUsers() const;
    bool updateUser(const string& id, const string& newName, const string& newContact);
    bool deleteUser(const string& id);
};

// --- Class: BloodBankSystem ---
class BloodBankSystem {
private:
    UserManager userManager;
    vector<BloodInventory> bloodInventory;
    vector<BloodRequest> bloodRequests;
    User* loggedInUser;

public:
    BloodBankSystem();
    bool addDonor(const string& id, const string& name, const string& contact, const string& bloodType);
    void viewDonors() const;
    void searchDonorsByBloodType(const string& bloodType) const;
    void searchDonorsByName(const string& name) const;
    bool updateDonor(const string& id, const string& newName, const string& newContact, const string& newBloodType);
    bool deleteDonor(const string& id);
    bool addBlood(const string& bloodType, int quantity, const string& donationDate);
    void viewBloodInventory() const;
    void viewBloodAvailability(const string& bloodType) const;
    bool updateBloodStock(const string& bloodType, int quantityChange);
    bool deleteBlood(const string& bloodType, const string& donationDate);
    bool addRequest(const string& requestorID, const string& bloodType, int quantity, const string& requestDate);
    void viewRequests() const;
    void viewRequestsByStatus(const string& status) const;
    void viewRequestsByRequestor(const string& requestorId) const;
    bool updateRequestStatus(const string& requestID, const string& newStatus);
    bool updateRequest(const string& requestID, const string& newBloodType, int newQuantity, const string& newRequestDate);
    bool deleteRequest(const string& requestID);
    bool addUser(const string& role, const string& id, const string& name, const string& contact);
    User* getUserByID(const string& id);
    void displayAllUsers() const;
    bool updateUser(const string& id, const string& newName, const string& newContact);
    bool deleteUser(const string& id);
    bool loginUser(const string& id);
    void logoutUser();
    User* getLoggedInUser() const;
    bool registerRequestor(const string& id, const string& name, const string& contact);
    bool isBloodAvailable(const string& bloodType, int quantity) const;
    void fulfillRequest(const string& requestID);
    void removeExpiredBlood();
    string getCurrentDate();
};

int main();

// --- Function Definitions ---

namespace Validation {
    bool isValidString(const string& str) {
        return !str.empty();
    }

    bool isAlphanumeric(const string& str) {
        if (str.empty()) return false;
        for (char c : str) {
            if (!isalnum(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
    }

    bool isValidInteger(int value) {
        return value >= 0;
    }

    bool isValidDate(const string& date) {
        if (date.length() != 10) return false;
        if (date[4] != '-' || date[7] != '-') return false;
        for (int i = 0; i < 4; ++i) {
            if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        for (int i = 5; i < 7; ++i) {
            if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        for (int i = 8; i < 10; ++i) {
            if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        return true;
    }

    bool isValidBloodType(const string& bloodType) {
        if (bloodType.length() < 2 || bloodType.length() > 3) return false;
        string upperBloodType = bloodType;
        for (char &c : upperBloodType) {
            c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }
        return (upperBloodType == "A+" || upperBloodType == "A-" ||
                upperBloodType == "B+" || upperBloodType == "B-" ||
                upperBloodType == "AB+" || upperBloodType == "AB-" ||
                upperBloodType == "O+" || upperBloodType == "O-");
    }

    void clearInputBuffer() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();
    }

    int getIntegerInput() {
        int value;
        while (!(cin >> value)) {
            cout << "Invalid input. Please enter an integer: ";
            clearInputBuffer();
        }
        clearInputBuffer();
        return value;
    }

    string getStringInput(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }
}

User::User(const string& id, const string& n, const string& c, const string& r)
    : userID(id), name(n), contact(c), role(r) {}

string User::getUserID() const { return userID; }
string User::getName() const { return name; }
string User::getContact() const { return contact; }
string User::getRole() const { return role; }

bool User::setUserID(const string& id) {
    if (Validation::isAlphanumeric(id)) {
        userID = id;
        return true;
    }
    cerr << "Error: User ID must be alphanumeric.\n";
    return false;
}

bool User::setName(const string& n) {
    if (Validation::isValidString(n)) {
        name = n;
        return true;
    }
    cerr << "Error: Name cannot be empty.\n";
    return false;
}

bool User::setContact(const string& c) {
    if (Validation::isValidString(c)) {
        contact = c;
        return true;
    }
    cerr << "Error: Contact cannot be empty.\n";
    return false;
}

bool User::setRole(const string& r) {
    if (Validation::isValidString(r)) {
        role = r;
        return true;
    }
    cerr << "Error: Role cannot be empty.\n";
    return false;
}

void User::displayUserInfo() const {
    cout << "User ID: " << userID << ", Name: " << name << ", Contact: " << contact << ", Role: " << role << endl;
}

Donor::Donor(const string& id, const string& n, const string& c, const string& bt)
    : User(id, n, c, "Donor"), bloodType(bt) {}

string Donor::getBloodType() const { return bloodType; }

bool Donor::setBloodType(const string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    cerr << "Error: Invalid blood type.\n";
    return false;
}

void Donor::displayUserInfo() const{
    User::displayUserInfo();
    cout << "Blood Type: " << bloodType << endl;
}

AdminStaff::AdminStaff(const string& id, const string& n, const string& c)
    : User(id, n, c, "Admin") {}

void AdminStaff::displayUserInfo() const {
    User::displayUserInfo();
}

Requestor::Requestor(const string& id, const string& n, const string& c)
    : User(id, n, c, "Requestor") {}

void Requestor::displayUserInfo() const {
    User::displayUserInfo();
}

BloodInventory::BloodInventory(const string& bt, int qty, const string& dd)
    : bloodType(bt), quantity(qty), donationDate(dd) {}

string BloodInventory::getBloodType() const { return bloodType; }
int BloodInventory::getQuantity() const { return quantity; }
string BloodInventory::getDonationDate() const { return donationDate; }

bool BloodInventory::setBloodType(const string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    cerr << "Error: Invalid blood type.\n";
    return false;
}

bool BloodInventory::setQuantity(int qty) {
    if (Validation::isValidInteger(qty)) {
        quantity = qty;
        return true;
    }
    cerr << "Error: Invalid quantity.\n";
    return false;
}

bool BloodInventory::setDonationDate(const string& dd) {
    if (Validation::isValidDate(dd)) {
        donationDate = dd;
        return true;
    }
    cerr << "Error: Invalid donation date.\n";
    return false;
}

void BloodInventory::increaseQuantity(int qty) {
    if(Validation::isValidInteger(qty) && qty > 0)
        quantity += qty;
}

void BloodInventory::decreaseQuantity(int qty) {
     if (Validation::isValidInteger(qty) && qty > 0) {
        if (quantity >= qty) {
            quantity -= qty;
        } else {
             cout << "Not enough blood of type " << bloodType << " to decrease by " << qty << " units.\n";
            quantity = 0;
        }
    }
}

void BloodInventory::displayBloodInfo() const {
    cout << "Blood Type: " << bloodType << ", Quantity: " << quantity << ", Donation Date: " << donationDate << endl;
}

bool BloodInventory::isExpired(const string& currentDate) const {
    // Simplified expiration check: Blood expires after 42 days (6 weeks)
    // In a real-world scenario, this would be more complex.
    if (!Validation::isValidDate(currentDate) || !Validation::isValidDate(donationDate))
        return false;

    int donationYear = stoi(donationDate.substr(0, 4));
    int donationMonth = stoi(donationDate.substr(5, 2));
    int donationDay = stoi(donationDate.substr(8, 2));

    int currentYear = stoi(currentDate.substr(0, 4));
    int currentMonth = stoi(currentDate.substr(5, 2));
    int currentDay = stoi(currentDate.substr(8, 2));

    // Calculate days since donation (very simplified)
    int daysSinceDonation = (currentYear - donationYear) * 365 +
                            (currentMonth - donationMonth) * 30 +
                            (currentDay - donationDay);

    return daysSinceDonation > 42;
}

BloodRequest::BloodRequest(const string& id, const string& reqID, const string& bt, int qty, const string& rd)
    : requestID(id), requestorID(reqID), bloodType(bt), quantity(qty), status("Pending"), requestDate(rd) {}

string BloodRequest::getRequestID() const { return requestID; }
string BloodRequest::getRequestorID() const { return requestorID; }
string BloodRequest::getBloodType() const { return bloodType; }
int BloodRequest::getQuantity() const { return quantity; }
string BloodRequest::getStatus() const { return status; }
string BloodRequest::getRequestDate() const { return requestDate; }

bool BloodRequest::setStatus(const string& st) {
    if (st == "Pending" || st == "Fulfilled" || st == "Cancelled") {
        status = st;
        return true;
    }
    cerr << "Error: Invalid status.\n";
    return false;
}

bool BloodRequest::setBloodType(const string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    cerr << "Error: Invalid blood type.\n";
    return false;
}

bool BloodRequest::setQuantity(int qty) {
    if (Validation::isValidInteger(qty)) {
        quantity = qty;
        return true;
    }
    cerr << "Error: Invalid quantity.\n";
    return false;
}

bool BloodRequest::setRequestDate(const string& rd) {
    if (Validation::isValidDate(rd)) {
        requestDate = rd;
        return true;
    }
    cerr << "Error: Invalid request date.\n";
    return false;
}

void BloodRequest::displayRequestInfo() const {
    cout << "Request ID: " << requestID
              << ", Requestor ID: " << requestorID
              << ", Blood Type: " << bloodType
              << ", Quantity: " << quantity
              << ", Status: " << status
              << ", Request Date: " << requestDate
              << endl;
}

UserManager::UserManager() : users() {}

UserManager::~UserManager() {
    for (User* user : users) {
        delete user;
    }
    users.clear();
}

bool UserManager::addUser(const string& role, const string& id, const string& name, const string& contact, const string& bloodType) {
    if (getUserByID(id) != nullptr) {
        cerr << "Error: User ID already exists.\n";
        return false;
    }
    if (role == "Donor") {
        Donor* newDonor = new Donor(id, name, contact, bloodType);
         if (!newDonor->setUserID(id) || !newDonor->setName(name) || !newDonor->setContact(contact) || !newDonor->setBloodType(bloodType))
         {
             delete newDonor;
             return false;
         }
        users.push_back(newDonor);
        return true;
    } else if (role == "Admin") {
        AdminStaff* newAdmin = new AdminStaff(id, name, contact);
        if (!newAdmin->setUserID(id) || !newAdmin->setName(name) || !newAdmin->setContact(contact))
         {
             delete newAdmin;
             return false;
         }
        users.push_back(newAdmin);
        return true;
    } else if (role == "Requestor") {
        Requestor* newRequestor = new Requestor(id, name, contact);
        if (!newRequestor->setUserID(id) || !newRequestor->setName(name) || !newRequestor->setContact(contact))
         {
             delete newRequestor;
             return false;
         }
        users.push_back(newRequestor);
        return true;
    }
    else {
        cerr << "Error: Invalid role.\n";
        return false;
    }
}

User* UserManager::getUserByID(const string& id) {
    for (User* user : users) {
        if (user->getUserID() == id) {
            return user;
        }
    }
    return nullptr;
}

vector<User*> UserManager::getUsersByRole(const string& role) const {
    vector<User*> result;
    for (User* user : users) {
        if (user->getRole() == role) {
            result.push_back(user);
        }
    }
    return result;
}

void UserManager::displayAllUsers() const {
    if (users.empty())
    {
        cout<< "No users to display." << endl;
        return;
    }
    for (const auto& user : users) {
        user->displayUserInfo();
    }
}

bool UserManager::updateUser(const string& id, const string& newName, const string& newContact) {
    User* user = getUserByID(id);
    if (user) {
        bool validName = user->setName(newName);
        bool validContact = user->setContact(newContact);
        return validName && validContact;
    }
    return false;
}

bool UserManager::deleteUser(const string& id) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getUserID() == id) {
            delete *it;
            users.erase(it);
            return true;
        }
    }
    return false;
}

BloodBankSystem::BloodBankSystem() : loggedInUser(nullptr) {}

bool BloodBankSystem::addDonor(const string& id, const string& name, const string& contact, const string& bloodType) {
    return userManager.addUser("Donor", id, name, contact, bloodType);
}

void BloodBankSystem::viewDonors() const {
    vector<User*> donors = userManager.getUsersByRole("Donor");
    if (donors.empty()) {
        cout << "No donors found.\n";
        return;
    }
    cout << "--- Donor List ---\n";
    for (const auto& donor : donors) {
        donor->displayUserInfo();
    }
}

void BloodBankSystem::searchDonorsByBloodType(const string& bloodType) const {
    if (!Validation::isValidBloodType(bloodType)) {
        cerr << "Error: Invalid blood type.\n";
        return;
    }
    cout << "--- Donors with Blood Type " << bloodType << " ---\n";
    bool found = false;
    vector<User*> donors = userManager.getUsersByRole("Donor");
    for (const auto& donor : donors) {
        Donor* d = dynamic_cast<Donor*>(donor);
        if (d && d->getBloodType() == bloodType) {
            d->displayUserInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "No donors found with blood type " << bloodType << ".\n";
    }
}

void BloodBankSystem::searchDonorsByName(const string& name) const {
    if (!Validation::isValidString(name)) {
        cerr << "Error: Invalid name.\n";
        return;
    }
    cout << "--- Donors with Name containing " << name << " ---\n";
    bool found = false;
     vector<User*> donors = userManager.getUsersByRole("Donor");
    for (const auto& donor : donors) {
        if (donor->getName().find(name) != string::npos) {
            donor->displayUserInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "No donors found with name containing " << name << ".\n";
    }
}

bool BloodBankSystem::updateDonor(const string& id, const string& newName, const string& newContact, const string& newBloodType) {
    User* user = userManager.getUserByID(id);
    if (user && user->getRole() == "Donor") {
        Donor* donor = dynamic_cast<Donor*>(user);
        if (donor)
        {
            bool validName = donor->setName(newName);
            bool validContact = donor->setContact(newContact);
            bool validBloodType = donor->setBloodType(newBloodType);
            return validName && validContact && validBloodType;
        }
}
    return false;
}

bool BloodBankSystem::deleteDonor(const string& id) {
    return userManager.deleteUser(id);
}

bool BloodBankSystem::addBlood(const string& bloodType, int quantity, const string& donationDate) {
    if (!Validation::isValidBloodType(bloodType) || !Validation::isValidInteger(quantity) || !Validation::isValidDate(donationDate)) {
        return false;
    }
    bloodInventory.emplace_back(bloodType, quantity, donationDate);
    return true;
}

void BloodBankSystem::viewBloodInventory() const {
    if (bloodInventory.empty()) {
        cout << "Blood inventory is empty.\n";
        return;
    }
    cout << "--- Blood Inventory ---\n";
    for (const auto& blood : bloodInventory) {
        blood.displayBloodInfo();
    }
}

void BloodBankSystem::viewBloodAvailability(const string& bloodType) const {
      if (!Validation::isValidBloodType(bloodType)) {
        cerr << "Error: Invalid blood type.\n";
        return;
    }
    cout << "--- Blood Availability for " << bloodType << " ---\n";
    bool found = false;
    for (const auto& blood : bloodInventory) {
        if (blood.getBloodType() == bloodType) {
            cout << "Quantity: " << blood.getQuantity() << " units\n";
            found = true;
        }
    }
    if (!found) {
        cout << "Blood type " << bloodType << " not found in inventory.\n";
    }
}

bool BloodBankSystem::updateBloodStock(const string& bloodType, int quantityChange) {
    if (!Validation::isValidBloodType(bloodType) || !Validation::isValidInteger(quantityChange)) {
        return false;
    }
    for (auto& blood : bloodInventory) {
        if (blood.getBloodType() == bloodType) {
            if (quantityChange > 0)
                blood.increaseQuantity(quantityChange);
            else
                blood.decreaseQuantity(abs(quantityChange));
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::deleteBlood(const string& bloodType, const string& donationDate) {
     if (!Validation::isValidBloodType(bloodType) || !Validation::isValidDate(donationDate)) {
        return false;
    }
    for (auto it = bloodInventory.begin(); it != bloodInventory.end(); ++it) {
        if (it->getBloodType() == bloodType && it->getDonationDate() == donationDate) {
            bloodInventory.erase(it);
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::addRequest(const string& requestorID, const string& bloodType, int quantity, const string& requestDate) {
    if (!Validation::isValidBloodType(bloodType) || !Validation::isValidInteger(quantity) || !Validation::isValidDate(requestDate)) {
        return false;
    }
    if (!isBloodAvailable(bloodType, quantity)) {
        cout << "Insufficient blood stock to fulfill this request.\n";
        return false;
    }
    string requestID = "REQ-" + to_string(bloodRequests.size() + 1);
    bloodRequests.emplace_back(requestID, requestorID, bloodType, quantity, requestDate);
    return true;
}

void BloodBankSystem::viewRequests() const {
    if (bloodRequests.empty()) {
        cout << "No blood requests found.\n";
        return;
    }
    cout << "--- Blood Requests ---\n";
    for (const auto& request : bloodRequests) {
        request.displayRequestInfo();}
}

void BloodBankSystem::viewRequestsByStatus(const string& status) const {
    if (status != "Pending" && status != "Fulfilled" && status != "Cancelled") {
        cerr << "Error: Invalid status.\n";
        return;
    }
    cout << "--- Blood Requests with Status: " << status << " ---\n";
    bool found = false;
    for (const auto& request : bloodRequests) {
        if (request.getStatus() == status) {
            request.displayRequestInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "No requests found with status: " << status << ".\n";
    }
}

void BloodBankSystem::viewRequestsByRequestor(const string& requestorId) const {
    if (!Validation::isAlphanumeric(requestorId)) {
        cerr << "Error: Invalid Requestor ID.\n";
        return;
    }
    cout << "--- Blood Requests for Requestor: " << requestorId << " ---\n";
    bool found = false;
    for (const auto& request : bloodRequests) {
        if (request.getRequestorID() == requestorId) {
            request.displayRequestInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "No requests found for requestor: " << requestorId << ".\n";
    }
}

bool BloodBankSystem::updateRequestStatus(const string& requestID, const string& newStatus) {
    if (newStatus != "Pending" && newStatus != "Fulfilled" && newStatus != "Cancelled") {
        cerr << "Error: Invalid status.\n";
        return false;
    }
    for (auto& request : bloodRequests) {
        if (request.getRequestID() == requestID) {
            request.setStatus(newStatus);
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::updateRequest(const string& requestID, const string& newBloodType, int newQuantity, const string& newRequestDate) {
    if (!Validation::isValidBloodType(newBloodType) || !Validation::isValidInteger(newQuantity) || !Validation::isValidDate(newRequestDate)) {
        return false;
    }
    for (auto& request : bloodRequests) {
        if (request.getRequestID() == requestID) {
            request.setBloodType(newBloodType);
            request.setQuantity(newQuantity);
            request.setRequestDate(newRequestDate);
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::deleteRequest(const string& requestID) {
    for (auto it = bloodRequests.begin(); it != bloodRequests.end(); ++it) {
        if (it->getRequestID() == requestID) {
            bloodRequests.erase(it);
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::addUser(const string& role, const string& id, const string& name, const string& contact) {
    return userManager.addUser(role, id, name, contact);
}

User* BloodBankSystem::getUserByID(const string& id) {
    return userManager.getUserByID(id);
}

void BloodBankSystem::displayAllUsers() const {
    userManager.displayAllUsers();
}

bool BloodBankSystem::updateUser(const string& id, const string& newName, const string& newContact) {
    return userManager.updateUser(id, newName, newContact);
}

bool BloodBankSystem::deleteUser(const string& id) {
    return userManager.deleteUser(id);
}

bool BloodBankSystem::loginUser(const string& id) {
    loggedInUser = userManager.getUserByID(id);
    return loggedInUser != nullptr;
}

void BloodBankSystem::logoutUser() {
    loggedInUser = nullptr;
}

User* BloodBankSystem::getLoggedInUser() const{
    return loggedInUser;
}

bool BloodBankSystem::registerRequestor(const string& id, const string& name, const string& contact) {
    return userManager.addUser("Requestor", id, name, contact);
}

bool BloodBankSystem::isBloodAvailable(const string& bloodType, int quantity) const {
    for (const auto& blood : bloodInventory) {
        if (blood.getBloodType() == bloodType && blood.getQuantity() >= quantity) {
            return true;
        }
    }
    return false;
}

void BloodBankSystem::fulfillRequest(const string& requestID) {
    for (auto& request : bloodRequests) {
        if (request.getRequestID() == requestID) {
            if (request.getStatus() != "Pending") {
                cout << "Request is not pending.\n";
                return;
            }
            if (isBloodAvailable(request.getBloodType(), request.getQuantity())) {
                for (auto& blood : bloodInventory) {
                    if (blood.getBloodType() == request.getBloodType()) {
                        blood.decreaseQuantity(request.getQuantity());
                        request.setStatus("Fulfilled");
                        cout << "Request fulfilled successfully.\n";
                        return;
                    }
                }
            } else {
                cout << "Insufficient blood stock to fulfill this request.\n";
                return;
            }
        }
    }
    cout << "Request not found.\n";
}

void BloodBankSystem::removeExpiredBlood() {
    string currentDate = getCurrentDate();
    for (auto it = bloodInventory.begin(); it != bloodInventory.end();) {
        if (it->isExpired(currentDate)) {
            cout << "Expired blood of type " << it->getBloodType()
                      << " donated on " << it->getDonationDate()
                      << " removed from inventory.\n";
            it = bloodInventory.erase(it);
        } else {
            ++it;
        }
    }
}

string BloodBankSystem::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    // Format: YYYY-MM-DD
    string year = to_string(1900 + ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon);
    string day = to_string(ltm->tm_mday);

    if (month.length() == 1) month = "0" + month;
    if (day.length() == 1) day = "0" + day;

    return year + "-" + month + "-" + day;
}

int main() {
    BloodBankSystem bbs;
    string userID, name, contact, bloodType, requestID, status, requestDate;
    int quantity, choice;

    // Add some initial data
    bbs.addUser("Admin", "admin1", "Admin User", "123-456-7890");
    bbs.addDonor("", "Donor One", "987-654-3210", "A+");
    bbs.addDonor("donor2", "Donor Two", "555-123-4567", "O-");
    bbs.addBlood("A+", 10, "2024-01-15");
    bbs.addBlood("O-", 5, "2024-01-20");
    bbs.addRequest("", "A+", 2, "2024-02-01");

    bool running = true;
    while (running) {
        bbs.removeExpiredBlood();

        cout << "\n--- Blood Bank Management System ---\n";
        if (bbs.getLoggedInUser() == nullptr) {
            cout << "1. Login\n";
            cout << "2. Register as Requestor\n";
            cout << "3. Register as Donor\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            choice = Validation::getIntegerInput();

            switch (choice) {
                case 1: {
                    userID = Validation::getStringInput("Enter User ID to login: ");
                    if (bbs.loginUser(userID)) {
                        cout << "Logged in successfully!\n";
                    } else {
                        cout << "Login failed.\n";
                    }
                    break;
                }
                case 2: {
                    userID = Validation::getStringInput("Enter Requestor ID: ");
                    name = Validation::getStringInput("Enter Requestor Name: ");
                    contact = Validation::getStringInput("Enter Requestor Contact: ");
                    if (bbs.registerRequestor(userID, name, contact)) {
                        cout << "Requestor registered successfully. You can now login.\n";
                    } else {
                        cout << "Failed to register as requestor.\n";
                    }
                    break;
                }
                case 3: {
                    userID = Validation::getStringInput("Enter Donor ID: ");
                    name = Validation::getStringInput("Enter Donor Name: ");
                    contact = Validation::getStringInput("Enter Donor Contact: ");
                    bloodType = Validation::getStringInput("Enter Donor Blood Type: ");
                    if (bbs.addDonor(userID, name, contact, bloodType)) {
                        cout << "Donor registered successfully. You can now login.\n";
                    } else {
                        cout << "Failed to register as donor.\n";
                    }
                    break;
                }
                case 0:
                    running = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } else {
            string role = bbs.getLoggedInUser()->getRole();
            if (role == "Admin") {
                cout << "\n--- Admin Menu ---\n";
                cout << "1. View All Users\n";
                cout << "2. View All Donors\n";
                cout << "3. View Blood Inventory\n";
                cout << "4. View Blood Requests\n";
                cout << "5. Add Request\n";
                cout << "6. Logout\n";
                cout << "0. Exit\n";
                cout << "Enter your choice: ";
                choice = Validation::getIntegerInput();

                switch (choice) {
                    case 1: { // View All Users
                        bbs.displayAllUsers();
                        cout << "\n--- User Management ---\n";
                        cout << "1. Update User\n";
                        cout << "2. Delete User\n";
                        cout << "0. Back\n";
                        cout << "Enter your choice: ";
                        int userChoice = Validation::getIntegerInput();
                        switch (userChoice) {
                            case 1:
                                userID = Validation::getStringInput("Enter User ID to update: ");
                                name = Validation::getStringInput("Enter New Name: ");
                                contact = Validation::getStringInput("Enter New Contact: ");
                                if (bbs.updateUser(userID, name, contact)) {
                                    cout << "User information updated successfully.\n";
                                } else {
                                    cout << "Failed to update user information.\n";
                                }
                                break;
                            case 2:
                                userID = Validation::getStringInput("Enter User ID to delete: ");
                                if (bbs.deleteUser(userID)) {
                                    cout << "User deleted successfully.\n";
                                } else {
                                    cout << "Failed to delete user.\n";
                                }
                                break;
                            case 0:
                                break;
                            default:
                                cout << "Invalid choice.\n";
                        }
                        break;
                    }
                    case 2: { // View All Donors
                        bbs.viewDonors();
                        cout << "\n--- Donor Management ---\n";
                        cout << "1. Add Donor\n";
                        cout << "2. Search Donors by Blood Type\n";
                        cout << "3. Search Donors by Name\n";
                        cout << "4. Update Donor\n";
                        cout << "5. Delete Donor\n";
                        cout << "0. Back\n";
                        cout << "Enter your choice: ";
                        int donorChoice = Validation::getIntegerInput();
                        switch (donorChoice) {
                            case 1:
                                userID = Validation::getStringInput("Enter Donor ID: ");
                                name = Validation::getStringInput("Enter Donor Name: ");
                                contact = Validation::getStringInput("Enter Donor Contact: ");
                                bloodType = Validation::getStringInput("Enter Donor Blood Type: ");
                                if (bbs.addDonor(userID, name, contact, bloodType)) {
                                    cout << "Donor added successfully.\n";
                                } else {
                                    cout << "Failed to add donor.\n";
                                }
                                break;
                            case 2:
                                bloodType = Validation::getStringInput("Enter Blood Type to search: ");
                                bbs.searchDonorsByBloodType(bloodType);
                                break;
                            case 3:
                                name = Validation::getStringInput("Enter Donor Name to search: ");
                                bbs.searchDonorsByName(name);
                                break;
                            case 4:
                                userID = Validation::getStringInput("Enter Donor ID to update: ");
                                name = Validation::getStringInput("Enter New Name: ");
                                contact = Validation::getStringInput("Enter New Contact: ");
                                bloodType = Validation::getStringInput("Enter New Blood Type: ");
                                if (bbs.updateDonor(userID, name, contact, bloodType)) {
                                    cout << "Donor information updated successfully.\n";
                                } else {
                                    cout << "Failed to update donor information.\n";
                                }
                                break;
                            case 5:
                                userID = Validation::getStringInput("Enter Donor ID to delete: ");
                                if (bbs.deleteDonor(userID)) {
                                    cout << "Donor deleted successfully.\n";
                                } else {
                                    cout << "Failed to delete donor.\n";
                                }
                                break;
                            case 0:
                                break;
                            default:
                                cout << "Invalid choice.\n";
                        }
                        break;
                    }
                    case 3: { // View Blood Inventory
                        bbs.viewBloodInventory();
                        cout << "\n--- Blood Inventory Management ---\n";
                        cout << "1. Add Blood\n";
                        cout << "2. View Blood Availability\n";
                        cout << "3. Update Blood Stock\n";
                        cout << "4. Delete Blood\n";
                        cout << "0. Back\n";
                        cout << "Enter your choice: ";
                        int bloodChoice = Validation::getIntegerInput();
                        switch (bloodChoice) {
                            case 1:
                                bloodType = Validation::getStringInput("Enter Blood Type: ");
                                quantity = Validation::getIntegerInput();
                                requestDate = Validation::getStringInput("Enter Donation Date (YYYY-MM-DD): ");
                                if (bbs.addBlood(bloodType, quantity, requestDate)) {
                                    cout << "Blood added successfully.\n";
                                } else {
                                    cout << "Failed to add blood.\n";
                                }
                                break;
                            case 2:
                                bloodType = Validation::getStringInput("Enter Blood Type to check availability: ");
                                bbs.viewBloodAvailability(bloodType);
                                break;
                            case 3:
                                bloodType = Validation::getStringInput("Enter Blood Type to update stock: ");
                                cout << "Enter Quantity Change (positive to add, negative to remove): ";
                                quantity = Validation::getIntegerInput();
                                if (bbs.updateBloodStock(bloodType, quantity)) {
                                    cout << "Blood stock updated successfully.\n";
                                } else {
                                    cout << "Failed to update blood stock.\n";
                                }
                                break;
                            case 4:
                                bloodType = Validation::getStringInput("Enter Blood Type to delete: ");
                                requestDate = Validation::getStringInput("Enter Donation Date of the blood to delete: ");
                                if (bbs.deleteBlood(bloodType, requestDate)) {
                                    cout << "Blood deleted successfully.\n";
                                } else {
                                    cout << "Failed to delete blood.\n";
                                }
                                break;
                            case 0:
                                break;
                            default:
                                cout << "Invalid choice.\n";
                        }
                        break;
                    }
                    case 4: { // View Blood Requests
                        bbs.viewRequests();
                        cout << "\n--- Blood Request Management ---\n";
                        cout << "1. Fulfill Request\n";
                        cout << "2. Delete Request\n";
                        cout << "0. Back\n";
                        cout << "Enter your choice: ";
                        int reqChoice = Validation::getIntegerInput();
                        switch (reqChoice) {
                            case 1:
                                requestID = Validation::getStringInput("Enter Request ID to fulfill: ");
                                bbs.fulfillRequest(requestID);
                                break;
                            case 2:
                                requestID = Validation::getStringInput("Enter Request ID to delete: ");
                                if (bbs.deleteRequest(requestID)) {
                                    cout << "Request deleted successfully.\n";
                                } else {
                                    cout << "Failed to delete request.\n";
                                }
                                break;
                            case 0:
                                break;
                            default:
                                cout << "Invalid choice.\n";
                        }
                        break;
                    }
                    case 5: { // Add Request
                        userID = Validation::getStringInput("Enter Donor ID: ");
                        bloodType = Validation::getStringInput("Enter Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        requestDate = Validation::getStringInput("Enter Request Date (YYYY-MM-DD): ");
                        if (bbs.addRequest(userID, bloodType, quantity, requestDate)) {
                            cout << "Blood request added successfully.\n";
                        } else {
                            cout << "Failed to add blood request.\n";
                        }
                        break;
                    }
                    case 6:
                        bbs.logoutUser();
                        cout << "Logged out.\n";
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } else if (role == "Donor") {
                cout << "\n--- Donor Menu ---\n";
                cout << "1. View My Profile\n";
                cout << "2. Inquire to Give Blood\n";
                cout << "3. Logout\n";
                cout << "Enter your choice: ";
                choice = Validation::getIntegerInput();

                switch (choice) {
                    case 1:
                        bbs.getLoggedInUser()->displayUserInfo();
                        break;
                    case 2: {
                        bloodType = Validation::getStringInput("Enter your Blood Type: ");
                        cout << "Thank you for your interest. We will contact you soon.\n";
                        break;
                    }
                    case 3:
                        bbs.logoutUser();
                        cout << "Logged out.\n";
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } else if (role == "Requestor") {
                cout << "\n--- Requestor Menu ---\n";
                cout << "1. View My Profile\n";
                cout << "2. Add Blood Request\n";
                cout << "3. View My Blood Requests\n";
                cout << "4. Update My Blood Request\n";
                cout << "5. Cancel My Blood Request\n";
                cout << "6. Logout\n";
                cout << "Enter your choice: ";
                choice = Validation::getIntegerInput();

                switch (choice) {
                    case 1:
                        bbs.getLoggedInUser()->displayUserInfo();
                        break;
                    case 2: {
                        string requestorId = bbs.getLoggedInUser()->getUserID();
                        bloodType = Validation::getStringInput("Enter Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        requestDate = Validation::getStringInput("Enter Request Date (YYYY-MM-DD): ");
                        if (bbs.addRequest(requestorId, bloodType, quantity, requestDate)) {
                            cout << "Blood request added successfully.\n";
                        } else {
                            cout << "Failed to add blood request.\n";
                        }
                        break;
                    }
                    case 3:
                        bbs.viewRequestsByRequestor(bbs.getLoggedInUser()->getUserID());
                        break;
                    case 4: {
                        requestID = Validation::getStringInput("Enter Request ID to update: ");
                        bloodType = Validation::getStringInput("Enter New Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        requestDate = Validation::getStringInput("Enter New Request Date (YYYY-MM-DD): ");
                        if (bbs.updateRequest(requestID, bloodType, quantity, requestDate)) {
                            cout << "Request updated successfully.\n";
                        } else {
                            cout << "Failed to update request.\n";
                        }
                        break;
                    }
                    case 5: {
                        requestID = Validation::getStringInput("Enter Request ID to cancel:");
                         if (bbs.updateRequestStatus(requestID, "Cancelled")) {
                            cout << "Request cancelled successfully.\n";
                        } else {
                            cout << "Failed to cancel request.\n";
                        }
                        break;
                    }
                    case 6:
                        bbs.logoutUser();
                        cout << "Logged out.\n";
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }

    return 0;
}