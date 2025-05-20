#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <ctime>

// --- Helper Functions for Input Validation ---
namespace Validation {
    bool isValidString(const std::string& str);
    bool isAlphanumeric(const std::string& str);
    bool isValidInteger(int value);
    bool isValidDate(const std::string& date);
    bool isValidBloodType(const std::string& bloodType);
    void clearInputBuffer();
    int getIntegerInput();
    std::string getStringInput(const std::string& prompt);
}

// --- Base Class: User ---
class User {
protected:
    std::string userID;
    std::string name;
    std::string contact;
    std::string role;

public:
    User(const std::string& id, const std::string& n, const std::string& c, const std::string& r);
    virtual ~User() = default;
    std::string getUserID() const;
    std::string getName() const;
    std::string getContact() const;
    std::string getRole() const;
    bool setUserID(const std::string& id);
    bool setName(const std::string& n);
    bool setContact(const std::string& c);
    bool setRole(const std::string& r);
    virtual void displayUserInfo() const;
};

// --- Derived Class: Donor ---
class Donor : public User {
private:
    std::string bloodType;

public:
    Donor(const std::string& id, const std::string& n, const std::string& c, const std::string& bt);
    std::string getBloodType() const;
    bool setBloodType(const std::string& bt);
    void displayUserInfo() const ;
};

// --- Derived Class: AdminStaff ---
class AdminStaff : public User {
public:
    AdminStaff(const std::string& id, const std::string& n, const std::string& c);
    void displayUserInfo() const ;
};

// --- Derived Class: Requestor ---
class Requestor : public User {
public:
    Requestor(const std::string& id, const std::string& n, const std::string& c);
    void displayUserInfo() const ;
};

// --- Class: BloodInventory ---
class BloodInventory {
private:
    std::string bloodType;
    int quantity;
    std::string donationDate;

public:
    BloodInventory(const std::string& bt, int qty, const std::string& dd);
    std::string getBloodType() const;
    int getQuantity() const;
    std::string getDonationDate() const;
    bool setBloodType(const std::string& bt);
    bool setQuantity(int qty);
    bool setDonationDate(const std::string& dd);
    void increaseQuantity(int qty);
    void decreaseQuantity(int qty);
    void displayBloodInfo() const;
    bool isExpired(const std::string& currentDate) const;
};

// --- Class: BloodRequest ---
class BloodRequest {
private:
    std::string requestID;
    std::string requestorID;
    std::string bloodType;
    int quantity;
    std::string status;
    std::string requestDate;

public:
    BloodRequest(const std::string& id, const std::string& reqID, const std::string& bt, int qty, const std::string& rd);
    std::string getRequestID() const;
    std::string getRequestorID() const;
    std::string getBloodType() const;
    int getQuantity() const;
    std::string getStatus() const;
    std::string getRequestDate() const;
    bool setStatus(const std::string& st);
    bool setBloodType(const std::string& bt);
    bool setQuantity(int qty);
    bool setRequestDate(const std::string& rd);
    void displayRequestInfo() const;
};

// --- Class: UserManager ---
class UserManager {
private:
    std::vector<User*> users;

public:
    UserManager();
    ~UserManager();
    bool addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact, const std::string& bloodType = "");
    User* getUserByID(const std::string& id);
    std::vector<User*> getUsersByRole(const std::string& role) const;
    void displayAllUsers() const;
    bool updateUser(const std::string& id, const std::string& newName, const std::string& newContact);
    bool deleteUser(const std::string& id);
};

// --- Class: BloodBankSystem ---
class BloodBankSystem {
private:
    UserManager userManager;
    std::vector<BloodInventory> bloodInventory;
    std::vector<BloodRequest> bloodRequests;
    User* loggedInUser;

public:
    BloodBankSystem();
    bool addDonor(const std::string& id, const std::string& name, const std::string& contact, const std::string& bloodType);
    void viewDonors() const;
    void searchDonorsByBloodType(const std::string& bloodType) const;
    void searchDonorsByName(const std::string& name) const;
    bool updateDonor(const std::string& id, const std::string& newName, const std::string& newContact, const std::string& newBloodType);
    bool deleteDonor(const std::string& id);
    bool addBlood(const std::string& bloodType, int quantity, const std::string& donationDate);
    void viewBloodInventory() const;
    void viewBloodAvailability(const std::string& bloodType) const;
    bool updateBloodStock(const std::string& bloodType, int quantityChange);
    bool deleteBlood(const std::string& bloodType, const std::string& donationDate);
    bool addRequest(const std::string& requestorID, const std::string& bloodType, int quantity, const std::string& requestDate);
    void viewRequests() const;
    void viewRequestsByStatus(const std::string& status) const;
    void viewRequestsByRequestor(const std::string& requestorId) const;
    bool updateRequestStatus(const std::string& requestID, const std::string& newStatus);
    bool updateRequest(const std::string& requestID, const std::string& newBloodType, int newQuantity, const std::string& newRequestDate);
    bool deleteRequest(const std::string& requestID);
    bool addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact);
    User* getUserByID(const std::string& id);
    void displayAllUsers() const;
    bool updateUser(const std::string& id, const std::string& newName, const std::string& newContact);
    bool deleteUser(const std::string& id);
    bool loginUser(const std::string& id);
    void logoutUser();
    User* getLoggedInUser() const;
    bool registerRequestor(const std::string& id, const std::string& name, const std::string& contact);
    bool isBloodAvailable(const std::string& bloodType, int quantity) const;
    void fulfillRequest(const std::string& requestID);
    void removeExpiredBlood();
    std::string getCurrentDate();
};

int main();

// --- Function Definitions ---

namespace Validation {
    bool isValidString(const std::string& str) {
        return !str.empty();
    }

    bool isAlphanumeric(const std::string& str) {
        if (str.empty()) return false;
        for (char c : str) {
            if (!std::isalnum(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
    }

    bool isValidInteger(int value) {
        return value >= 0;
    }

    bool isValidDate(const std::string& date) {
        if (date.length() != 10) return false;
        if (date[4] != '-' || date[7] != '-') return false;
        for (int i = 0; i < 4; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        for (int i = 5; i < 7; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        for (int i = 8; i < 10; ++i) {
            if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        return true;
    }

    bool isValidBloodType(const std::string& bloodType) {
        if (bloodType.length() < 2 || bloodType.length() > 3) return false;
        std::string upperBloodType = bloodType;
        for (char &c : upperBloodType) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return (upperBloodType == "A+" || upperBloodType == "A-" ||
                upperBloodType == "B+" || upperBloodType == "B-" ||
                upperBloodType == "AB+" || upperBloodType == "AB-" ||
                upperBloodType == "O+" || upperBloodType == "O-");
    }

    void clearInputBuffer() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
    }

    int getIntegerInput() {
        int value;
        while (!(std::cin >> value)) {
            std::cout << "Invalid input. Please enter an integer: ";
            clearInputBuffer();
        }
        clearInputBuffer();
        return value;
    }

    std::string getStringInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }
}

User::User(const std::string& id, const std::string& n, const std::string& c, const std::string& r)
    : userID(id), name(n), contact(c), role(r) {}

std::string User::getUserID() const { return userID; }
std::string User::getName() const { return name; }
std::string User::getContact() const { return contact; }
std::string User::getRole() const { return role; }

bool User::setUserID(const std::string& id) {
    if (Validation::isAlphanumeric(id)) {
        userID = id;
        return true;
    }
    std::cerr << "Error: User ID must be alphanumeric.\n";
    return false;
}

bool User::setName(const std::string& n) {
    if (Validation::isValidString(n)) {
        name = n;
        return true;
    }
    std::cerr << "Error: Name cannot be empty.\n";
    return false;
}

bool User::setContact(const std::string& c) {
    if (Validation::isValidString(c)) {
        contact = c;
        return true;
    }
    std::cerr << "Error: Contact cannot be empty.\n";
    return false;
}

bool User::setRole(const std::string& r) {
    if (Validation::isValidString(r)) {
        role = r;
        return true;
    }
    std::cerr << "Error: Role cannot be empty.\n";
    return false;
}

void User::displayUserInfo() const {
    std::cout << "User ID: " << userID << ", Name: " << name << ", Contact: " << contact << ", Role: " << role << std::endl;
}

Donor::Donor(const std::string& id, const std::string& n, const std::string& c, const std::string& bt)
    : User(id, n, c, "Donor"), bloodType(bt) {}

std::string Donor::getBloodType() const { return bloodType; }

bool Donor::setBloodType(const std::string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    std::cerr << "Error: Invalid blood type.\n";
    return false;
}

void Donor::displayUserInfo() const{
    User::displayUserInfo();
    std::cout << "Blood Type: " << bloodType << std::endl;
}

AdminStaff::AdminStaff(const std::string& id, const std::string& n, const std::string& c)
    : User(id, n, c, "Admin") {}

void AdminStaff::displayUserInfo() const {
    User::displayUserInfo();
}

Requestor::Requestor(const std::string& id, const std::string& n, const std::string& c)
    : User(id, n, c, "Requestor") {}

void Requestor::displayUserInfo() const {
    User::displayUserInfo();
}

BloodInventory::BloodInventory(const std::string& bt, int qty, const std::string& dd)
    : bloodType(bt), quantity(qty), donationDate(dd) {}

std::string BloodInventory::getBloodType() const { return bloodType; }
int BloodInventory::getQuantity() const { return quantity; }
std::string BloodInventory::getDonationDate() const { return donationDate; }

bool BloodInventory::setBloodType(const std::string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    std::cerr << "Error: Invalid blood type.\n";
    return false;
}

bool BloodInventory::setQuantity(int qty) {
    if (Validation::isValidInteger(qty)) {
        quantity = qty;
        return true;
    }
    std::cerr << "Error: Invalid quantity.\n";
    return false;
}

bool BloodInventory::setDonationDate(const std::string& dd) {
    if (Validation::isValidDate(dd)) {
        donationDate = dd;
        return true;
    }
    std::cerr << "Error: Invalid donation date.\n";
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
             std::cout << "Not enough blood of type " << bloodType << " to decrease by " << qty << " units.\n";
            quantity = 0;
        }
    }
}

void BloodInventory::displayBloodInfo() const {
    std::cout << "Blood Type: " << bloodType << ", Quantity: " << quantity << ", Donation Date: " << donationDate << std::endl;
}

bool BloodInventory::isExpired(const std::string& currentDate) const {
    // Simplified expiration check: Blood expires after 42 days (6 weeks)
    // In a real-world scenario, this would be more complex.
    if (!Validation::isValidDate(currentDate) || !Validation::isValidDate(donationDate))
        return false;

    int donationYear = std::stoi(donationDate.substr(0, 4));
    int donationMonth = std::stoi(donationDate.substr(5, 2));
    int donationDay = std::stoi(donationDate.substr(8, 2));

    int currentYear = std::stoi(currentDate.substr(0, 4));
    int currentMonth = std::stoi(currentDate.substr(5, 2));
    int currentDay = std::stoi(currentDate.substr(8, 2));

    // Calculate days since donation (very simplified)
    int daysSinceDonation = (currentYear - donationYear) * 365 +
                            (currentMonth - donationMonth) * 30 +
                            (currentDay - donationDay);

    return daysSinceDonation > 42;
}

BloodRequest::BloodRequest(const std::string& id, const std::string& reqID, const std::string& bt, int qty, const std::string& rd)
    : requestID(id), requestorID(reqID), bloodType(bt), quantity(qty), status("Pending"), requestDate(rd) {}

std::string BloodRequest::getRequestID() const { return requestID; }
std::string BloodRequest::getRequestorID() const { return requestorID; }
std::string BloodRequest::getBloodType() const { return bloodType; }
int BloodRequest::getQuantity() const { return quantity; }
std::string BloodRequest::getStatus() const { return status; }
std::string BloodRequest::getRequestDate() const { return requestDate; }

bool BloodRequest::setStatus(const std::string& st) {
    if (st == "Pending" || st == "Fulfilled" || st == "Cancelled") {
        status = st;
        return true;
    }
    std::cerr << "Error: Invalid status.\n";
    return false;
}

bool BloodRequest::setBloodType(const std::string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    std::cerr << "Error: Invalid blood type.\n";
    return false;
}

bool BloodRequest::setQuantity(int qty) {
    if (Validation::isValidInteger(qty)) {
        quantity = qty;
        return true;
    }
    std::cerr << "Error: Invalid quantity.\n";
    return false;
}

bool BloodRequest::setRequestDate(const std::string& rd) {
    if (Validation::isValidDate(rd)) {
        requestDate = rd;
        return true;
    }
    std::cerr << "Error: Invalid request date.\n";
    return false;
}

void BloodRequest::displayRequestInfo() const {
    std::cout << "Request ID: " << requestID
              << ", Requestor ID: " << requestorID
              << ", Blood Type: " << bloodType
              << ", Quantity: " << quantity
              << ", Status: " << status
              << ", Request Date: " << requestDate
              << std::endl;
}

UserManager::UserManager() : users() {}

UserManager::~UserManager() {
    for (User* user : users) {
        delete user;
    }
    users.clear();
}

bool UserManager::addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact, const std::string& bloodType) {
    if (getUserByID(id) != nullptr) {
        std::cerr << "Error: User ID already exists.\n";
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
        std::cerr << "Error: Invalid role.\n";
        return false;
    }
}

User* UserManager::getUserByID(const std::string& id) {
    for (User* user : users) {
        if (user->getUserID() == id) {
            return user;
        }
    }
    return nullptr;
}

std::vector<User*> UserManager::getUsersByRole(const std::string& role) const {
    std::vector<User*> result;
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
        std::cout<< "No users to display." << std::endl;
        return;
    }
    for (const auto& user : users) {
        user->displayUserInfo();
    }
}

bool UserManager::updateUser(const std::string& id, const std::string& newName, const std::string& newContact) {
    User* user = getUserByID(id);
    if (user) {
        bool validName = user->setName(newName);
        bool validContact = user->setContact(newContact);
        return validName && validContact;
    }
    return false;
}

bool UserManager::deleteUser(const std::string& id) {
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

bool BloodBankSystem::addDonor(const std::string& id, const std::string& name, const std::string& contact, const std::string& bloodType) {
    return userManager.addUser("Donor", id, name, contact, bloodType);
}

void BloodBankSystem::viewDonors() const {
    std::vector<User*> donors = userManager.getUsersByRole("Donor");
    if (donors.empty()) {
        std::cout << "No donors found.\n";
        return;
    }
    std::cout << "--- Donor List ---\n";
    for (const auto& donor : donors) {
        donor->displayUserInfo();
    }
}

void BloodBankSystem::searchDonorsByBloodType(const std::string& bloodType) const {
    if (!Validation::isValidBloodType(bloodType)) {
        std::cerr << "Error: Invalid blood type.\n";
        return;
    }
    std::cout << "--- Donors with Blood Type " << bloodType << " ---\n";
    bool found = false;
    std::vector<User*> donors = userManager.getUsersByRole("Donor");
    for (const auto& donor : donors) {
        Donor* d = dynamic_cast<Donor*>(donor);
        if (d && d->getBloodType() == bloodType) {
            d->displayUserInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No donors found with blood type " << bloodType << ".\n";
    }
}

void BloodBankSystem::searchDonorsByName(const std::string& name) const {
    if (!Validation::isValidString(name)) {
        std::cerr << "Error: Invalid name.\n";
        return;
    }
    std::cout << "--- Donors with Name containing " << name << " ---\n";
    bool found = false;
     std::vector<User*> donors = userManager.getUsersByRole("Donor");
    for (const auto& donor : donors) {
        if (donor->getName().find(name) != std::string::npos) {
            donor->displayUserInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No donors found with name containing " << name << ".\n";
    }
}

bool BloodBankSystem::updateDonor(const std::string& id, const std::string& newName, const std::string& newContact, const std::string& newBloodType) {
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

bool BloodBankSystem::deleteDonor(const std::string& id) {
    return userManager.deleteUser(id);
}

bool BloodBankSystem::addBlood(const std::string& bloodType, int quantity, const std::string& donationDate) {
    if (!Validation::isValidBloodType(bloodType) || !Validation::isValidInteger(quantity) || !Validation::isValidDate(donationDate)) {
        return false;
    }
    bloodInventory.emplace_back(bloodType, quantity, donationDate);
    return true;
}

void BloodBankSystem::viewBloodInventory() const {
    if (bloodInventory.empty()) {
        std::cout << "Blood inventory is empty.\n";
        return;
    }
    std::cout << "--- Blood Inventory ---\n";
    for (const auto& blood : bloodInventory) {
        blood.displayBloodInfo();
    }
}

void BloodBankSystem::viewBloodAvailability(const std::string& bloodType) const {
      if (!Validation::isValidBloodType(bloodType)) {
        std::cerr << "Error: Invalid blood type.\n";
        return;
    }
    std::cout << "--- Blood Availability for " << bloodType << " ---\n";
    bool found = false;
    for (const auto& blood : bloodInventory) {
        if (blood.getBloodType() == bloodType) {
            std::cout << "Quantity: " << blood.getQuantity() << " units\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "Blood type " << bloodType << " not found in inventory.\n";
    }
}

bool BloodBankSystem::updateBloodStock(const std::string& bloodType, int quantityChange) {
    if (!Validation::isValidBloodType(bloodType) || !Validation::isValidInteger(quantityChange)) {
        return false;
    }
    for (auto& blood : bloodInventory) {
        if (blood.getBloodType() == bloodType) {
            if (quantityChange > 0)
                blood.increaseQuantity(quantityChange);
            else
                blood.decreaseQuantity(std::abs(quantityChange));
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::deleteBlood(const std::string& bloodType, const std::string& donationDate) {
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

bool BloodBankSystem::addRequest(const std::string& requestorID, const std::string& bloodType, int quantity, const std::string& requestDate) {
    if (!Validation::isValidBloodType(bloodType) || !Validation::isValidInteger(quantity) || !Validation::isValidDate(requestDate)) {
        return false;
    }
    if (!isBloodAvailable(bloodType, quantity)) {
        std::cout << "Insufficient blood stock to fulfill this request.\n";
        return false;
    }
    std::string requestID = "REQ-" + std::to_string(bloodRequests.size() + 1);
    bloodRequests.emplace_back(requestID, requestorID, bloodType, quantity, requestDate);
    return true;
}

void BloodBankSystem::viewRequests() const {
    if (bloodRequests.empty()) {
        std::cout << "No blood requests found.\n";
        return;
    }
    std::cout << "--- Blood Requests ---\n";
    for (const auto& request : bloodRequests) {
        request.displayRequestInfo();}
}

void BloodBankSystem::viewRequestsByStatus(const std::string& status) const {
    if (status != "Pending" && status != "Fulfilled" && status != "Cancelled") {
        std::cerr << "Error: Invalid status.\n";
        return;
    }
    std::cout << "--- Blood Requests with Status: " << status << " ---\n";
    bool found = false;
    for (const auto& request : bloodRequests) {
        if (request.getStatus() == status) {
            request.displayRequestInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No requests found with status: " << status << ".\n";
    }
}

void BloodBankSystem::viewRequestsByRequestor(const std::string& requestorId) const {
    if (!Validation::isAlphanumeric(requestorId)) {
        std::cerr << "Error: Invalid Requestor ID.\n";
        return;
    }
    std::cout << "--- Blood Requests for Requestor: " << requestorId << " ---\n";
    bool found = false;
    for (const auto& request : bloodRequests) {
        if (request.getRequestorID() == requestorId) {
            request.displayRequestInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No requests found for requestor: " << requestorId << ".\n";
    }
}

bool BloodBankSystem::updateRequestStatus(const std::string& requestID, const std::string& newStatus) {
    if (newStatus != "Pending" && newStatus != "Fulfilled" && newStatus != "Cancelled") {
        std::cerr << "Error: Invalid status.\n";
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

bool BloodBankSystem::updateRequest(const std::string& requestID, const std::string& newBloodType, int newQuantity, const std::string& newRequestDate) {
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

bool BloodBankSystem::deleteRequest(const std::string& requestID) {
    for (auto it = bloodRequests.begin(); it != bloodRequests.end(); ++it) {
        if (it->getRequestID() == requestID) {
            bloodRequests.erase(it);
            return true;
        }
    }
    return false;
}

bool BloodBankSystem::addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact) {
    return userManager.addUser(role, id, name, contact);
}

User* BloodBankSystem::getUserByID(const std::string& id) {
    return userManager.getUserByID(id);
}

void BloodBankSystem::displayAllUsers() const {
    userManager.displayAllUsers();
}

bool BloodBankSystem::updateUser(const std::string& id, const std::string& newName, const std::string& newContact) {
    return userManager.updateUser(id, newName, newContact);
}

bool BloodBankSystem::deleteUser(const std::string& id) {
    return userManager.deleteUser(id);
}

bool BloodBankSystem::loginUser(const std::string& id) {
    loggedInUser = userManager.getUserByID(id);
    return loggedInUser != nullptr;
}

void BloodBankSystem::logoutUser() {
    loggedInUser = nullptr;
}

User* BloodBankSystem::getLoggedInUser() const{
    return loggedInUser;
}

bool BloodBankSystem::registerRequestor(const std::string& id, const std::string& name, const std::string& contact) {
    return userManager.addUser("Requestor", id, name, contact);
}

bool BloodBankSystem::isBloodAvailable(const std::string& bloodType, int quantity) const {
    for (const auto& blood : bloodInventory) {
        if (blood.getBloodType() == bloodType && blood.getQuantity() >= quantity) {
            return true;
        }
    }
    return false;
}

void BloodBankSystem::fulfillRequest(const std::string& requestID) {
    for (auto& request : bloodRequests) {
        if (request.getRequestID() == requestID) {
            if (request.getStatus() != "Pending") {
                std::cout << "Request is not pending.\n";
                return;
            }
            if (isBloodAvailable(request.getBloodType(), request.getQuantity())) {
                for (auto& blood : bloodInventory) {
                    if (blood.getBloodType() == request.getBloodType()) {
                        blood.decreaseQuantity(request.getQuantity());
                        request.setStatus("Fulfilled");
                        std::cout << "Request fulfilled successfully.\n";
                        return;
                    }
                }
            } else {
                std::cout << "Insufficient blood stock to fulfill this request.\n";
                return;
            }
        }
    }
    std::cout << "Request not found.\n";
}

void BloodBankSystem::removeExpiredBlood() {
    std::string currentDate = getCurrentDate();
    for (auto it = bloodInventory.begin(); it != bloodInventory.end();) {
        if (it->isExpired(currentDate)) {
            std::cout << "Expired blood of type " << it->getBloodType()
                      << " donated on " << it->getDonationDate()
                      << " removed from inventory.\n";
            it = bloodInventory.erase(it);
        } else {
            ++it;
        }
    }
}

std::string BloodBankSystem::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    // Format: YYYY-MM-DD
    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);

    if (month.length() == 1) month = "0" + month;
    if (day.length() == 1) day = "0" + day;

    return year + "-" + month + "-" + day;
}

int main() {
    BloodBankSystem bbs;
    std::string userID, name, contact, bloodType, requestID, status, requestDate;
    int quantity, choice;

    // Add some initial data
    bbs.addUser("Admin", "admin1", "Admin User", "123-456-7890");
    bbs.addDonor("donor1", "Donor One", "987-654-3210", "A+");
    bbs.addDonor("donor2", "Donor Two", "555-123-4567", "O-");
    bbs.addBlood("A+", 10, "2024-01-15");
    bbs.addBlood("O-", 5, "2024-01-20");
    bbs.addRequest("donor1", "A+", 2, "2024-02-01");

    bool running = true;
    while (running) {
        bbs.removeExpiredBlood();

        std::cout << "\n--- Blood Bank Management System ---\n";
        if (bbs.getLoggedInUser() == nullptr)
        {
            std::cout << "1. Login\n";
            std::cout << "2. Register as Requestor\n";
            std::cout << "3. View All Users\n";
            std::cout << "4. View All Donors\n";
            std::cout << "5. View Blood Inventory\n";
            std::cout << "6. View Blood Requests\n";
            std::cout << "0. Exit\n";
            std::cout << "Enter your choice: ";
            choice = Validation::getIntegerInput();

            switch (choice) {
                case 1: {
                    userID = Validation::getStringInput("Enter User ID to login: ");
                    if (bbs.loginUser(userID)) {
                         std::cout << "Logged in successfully!\n";
                    } else {
                        std::cout << "Login failed.\n";
                    }
                    break;
                }
                case 2: {
                    userID = Validation::getStringInput("Enter Requestor ID: ");
                    name = Validation::getStringInput("Enter Requestor Name: ");
                    contact = Validation::getStringInput("Enter Requestor Contact: ");
                    if (bbs.registerRequestor(userID, name, contact)) {
                        std::cout << "Requestor registered successfully. You can now login.\n";
                    } else {
                        std::cout << "Failed to register as requestor.\n";
                    }
                    break;
                }
                case 3:
                    bbs.displayAllUsers();
                    break;
                case 4:
                    bbs.viewDonors();
                    break;
                case 5:
                    bbs.viewBloodInventory();
                    break;
                case 6:
                    bbs.viewRequests();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
        else
        {
            if (bbs.getLoggedInUser()->getRole() == "Admin") {
                std::cout << "\n--- Admin Menu ---\n";
                std::cout << "1. Add Donor\n";
                std::cout << "2. View Donors\n";
                std::cout << "3. Search Donors by Blood Type\n";
                std::cout << "4. Search Donors by Name\n";
                std::cout << "5. Update Donor\n";
                std::cout << "6. Delete Donor\n";
                std::cout << "7. Add Blood\n";
                std::cout << "8. View Blood Inventory\n";
                std::cout << "9. View Blood Availability\n";
                std::cout << "10. Update Blood Stock\n";
                std::cout << "11. Delete Blood\n";
                std::cout << "12. Add Request\n";
                std::cout << "13. View Requests\n";
                std::cout << "14. View Requests by Status\n";
                std::cout << "15. View Requests by Requestor\n";
                std::cout << "16. Update Request Status\n";
                std::cout << "17. Update Request\n";
                std::cout << "18. Delete Request\n";
                std::cout << "19. View All Users\n";
                std::cout << "20. Update User\n";
                std::cout << "21. Delete User\n";
                std::cout << "22. Logout\n";
                std::cout << "23. Fulfill Request\n";
                std::cout << "Enter your choice: ";
                choice = Validation::getIntegerInput();

                switch (choice) {
                    case 1: {
                        userID = Validation::getStringInput("Enter Donor ID: ");
                        name = Validation::getStringInput("Enter Donor Name: ");
                        contact = Validation::getStringInput("Enter Donor Contact: ");
                        bloodType = Validation::getStringInput("Enter Donor Blood Type: ");
                        if (bbs.addDonor(userID, name, contact, bloodType)) {
                            std::cout << "Donor added successfully.\n";
                        } else {
                            std::cout << "Failed to add donor.\n";
                        }
                        break;
                    }
                    case 2:
                        bbs.viewDonors();
                        break;
                    case 3:
                        bloodType = Validation::getStringInput("Enter Blood Type to search: ");
                        bbs.searchDonorsByBloodType(bloodType);
                        break;
                    case 4:
                        name = Validation::getStringInput("Enter Donor Name to search: ");
                        bbs.searchDonorsByName(name);
                        break;
                    case 5: {
                        userID = Validation::getStringInput("Enter Donor ID to update: ");
                        name = Validation::getStringInput("Enter New Name: ");
                        contact = Validation::getStringInput("Enter New Contact: ");
                        bloodType = Validation::getStringInput("Enter New Blood Type: ");
                        if (bbs.updateDonor(userID, name, contact, bloodType)) {
                            std::cout << "Donor information updated successfully.\n";
                        } else {
                            std::cout << "Failed to update donor information.\n";
                        }
                        break;
                    }
                    case 6:
                        userID = Validation::getStringInput("Enter Donor ID to delete: ");
                        if (bbs.deleteDonor(userID)) {
                            std::cout << "Donor deleted successfully.\n";
                        } else {
                            std::cout << "Failed to delete donor.\n";
                        }
                        break;
                    case 7: {
                        bloodType = Validation::getStringInput("Enter Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        std::string donationDate = Validation::getStringInput("Enter Donation Date (YYYY-MM-DD): ");
                        if (bbs.addBlood(bloodType, quantity, donationDate)) {
                            std::cout << "Blood added successfully.\n";
                        } else {
                            std::cout << "Failed to add blood.\n";
                        }
                        break;
                    }
                    case 8:
                        bbs.viewBloodInventory();
                        break;
                    case 9:
                        bloodType = Validation::getStringInput("Enter Blood Type to check availability: ");
                        bbs.viewBloodAvailability(bloodType);
                        break;
                    case 10: {
                        bloodType = Validation::getStringInput("Enter Blood Type to update stock: ");
                        std::cout << "Enter Quantity Change (positive to add, negative to remove): ";
                        quantity = Validation::getIntegerInput();
                        if (bbs.updateBloodStock(bloodType, quantity)) {
                            std::cout << "Blood stock updated successfully.\n";
                        } else {
                            std::cout << "Failed to update blood stock.\n";
                        }
                        break;
                    }
                    case 11: {
                        bloodType = Validation::getStringInput("Enter Blood Type to delete: ");
                        std::string donationDate = Validation::getStringInput("Enter Donation Date of the blood to delete: ");
                        if (bbs.deleteBlood(bloodType, donationDate)) {
                            std::cout << "Blood deleted successfully.\n";
                        } else {
                            std::cout << "Failed to delete blood.\n";
                        }
                        break;
                    }
                    case 12: {
                        userID = Validation::getStringInput("Enter Requestor ID: ");
                        bloodType = Validation::getStringInput("Enter Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        requestDate = Validation::getStringInput("Enter Request Date (YYYY-MM-DD): ");
                        if (bbs.addRequest(userID, bloodType, quantity, requestDate)) {
                            std::cout << "Blood request added successfully.\n";
                        } else {
                            std::cout << "Failed to add blood request.\n";
                        }
                        break;
                    }
                    case 13:
                        bbs.viewRequests();
                        break;
                    case 14:
                        status = Validation::getStringInput("Enter Status to view requests (Pending, Fulfilled, Cancelled): ");
                        bbs.viewRequestsByStatus(status);
                        break;
                    case 15:
                        userID = Validation::getStringInput("Enter Requestor ID to view requests: ");
                        bbs.viewRequestsByRequestor(userID);
                        break;
                    case 16: {
                        requestID = Validation::getStringInput("Enter Request ID to update status: ");
                        status = Validation::getStringInput("Enter New Status (Pending, Fulfilled, Cancelled): ");
                        if (bbs.updateRequestStatus(requestID, status)) {
                            std::cout << "Request status updated successfully.\n";
                        } else {
                            std::cout << "Failed to update request status.\n";
                        }
                        break;
                    }
                    case 17: {
                        requestID = Validation::getStringInput("Enter Request ID to update: ");
                        bloodType = Validation::getStringInput("Enter New Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        requestDate = Validation::getStringInput("Enter New Request Date (YYYY-MM-DD): ");
                        if (bbs.updateRequest(requestID, bloodType, quantity, requestDate)) {
                            std::cout << "Request updated successfully.\n";
                        } else {
                            std::cout << "Failed to update request.\n";
                        }
                        break;
                    }
                    case 18:
                        requestID = Validation::getStringInput("Enter Request ID to delete: ");
                        if (bbs.deleteRequest(requestID)) {
                            std::cout << "Request deleted successfully.\n";
                        } else {
                            std::cout << "Failed to delete request.\n";
                        }
                        break;
                    case 19:
                        bbs.displayAllUsers();
                        break;
                    case 20:
                        userID = Validation::getStringInput("Enter User ID to update: ");
                        name = Validation::getStringInput("Enter New Name: ");
                        contact = Validation::getStringInput("Enter New Contact: ");
                        if (bbs.updateUser(userID, name, contact)) {
                            std::cout << "User information updated successfully.\n";
                        } else {
                            std::cout << "Failed to update user information.\n";
                        }
                        break;
                    case 21:
                        userID = Validation::getStringInput("Enter User ID to delete: ");
                        if (bbs.deleteUser(userID)) {
                            std::cout << "User deleted successfully.\n";
                        } else {
                            std::cout << "Failed to delete user.\n";
                        }
                        break;
                    case 22:
                        bbs.logoutUser();
                        std::cout << "Logged out.\n";
                        break;
                    case 23: {
                        requestID = Validation::getStringInput("Enter Request ID to fulfill: ");
                        bbs.fulfillRequest(requestID);
                        break;
                    }
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            }
            else if (bbs.getLoggedInUser()->getRole() == "Donor") {
                std::cout << "\n--- Donor Menu ---\n";
                std::cout << "1. View My Profile\n";
                std::cout << "2. Inquire to Give Blood\n";
                std::cout << "3. Logout\n";
                std::cout << "Enter your choice: ";
                choice = Validation::getIntegerInput();

                switch (choice) {
                    case 1:
                        bbs.getLoggedInUser()->displayUserInfo();
                        break;
                    case 2: {
                        bloodType = Validation::getStringInput("Enter your Blood Type: ");
                        std::cout << "Thank you for your interest. We will contact you soon.\n";
                        break;
                    }
                    case 3:
                        bbs.logoutUser();
                        std::cout << "Logged out.\n";
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            }
             else if (bbs.getLoggedInUser()->getRole() == "Requestor") {
                std::cout << "\n--- Requestor Menu ---\n";
                std::cout << "1. View My Profile\n";
                std::cout << "2. Add Blood Request\n";
                std::cout << "3. View My Blood Requests\n";
                std::cout << "4. Update My Blood Request\n";
                std::cout << "5. Cancel My Blood Request\n";
                std::cout << "6. Logout\n";
                std::cout << "Enter your choice: ";
                choice = Validation::getIntegerInput();

                switch (choice) {
                    case 1:
                        bbs.getLoggedInUser()->displayUserInfo();
                        break;
                    case 2: {
                        std::string requestorId = bbs.getLoggedInUser()->getUserID();
                        bloodType = Validation::getStringInput("Enter Blood Type: ");
                        quantity = Validation::getIntegerInput();
                        requestDate = Validation::getStringInput("Enter Request Date (YYYY-MM-DD): ");
                        if (bbs.addRequest(requestorId, bloodType, quantity, requestDate)) {
                            std::cout << "Blood request added successfully.\n";
                        } else {
                            std::cout << "Failed to add blood request.\n";
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
                            std::cout << "Request updated successfully.\n";
                        } else {
                            std::cout << "Failed to update request.\n";
                        }
                        break;
                    }
                    case 5: {
                        requestID = Validation::getStringInput("Enter Request ID to cancel:");
                         if (bbs.updateRequestStatus(requestID, "Cancelled")) {
                            std::cout << "Request cancelled successfully.\n";
                        } else {
                            std::cout << "Failed to cancel request.\n";
                        }
                        break;
                    }
                    case 6:
                        bbs.logoutUser();
                        std::cout << "Logged out.\n";
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }

 return 0;
;}