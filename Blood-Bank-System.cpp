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

class BloodBankException : public runtime_error {
public:
    explicit BloodBankException(const string& message) : runtime_error(message) {}
};

class ValidationException : public BloodBankException {
public:
    explicit ValidationException(const string& message) : BloodBankException("Validation Error: " + message) {}
};

class AuthenticationException : public BloodBankException {
public:
    explicit AuthenticationException(const string& message) : BloodBankException("Authentication Error: " + message) {}
};

namespace Validation {
    bool isValidString(const std::string& str);
    bool isAlphanumeric(const std::string& str);
    bool isValidInteger(int value);
    bool isValidDate(const std::string& date);
    bool isValidBloodType(const std::string& bloodType);
    bool isValidPassword(const std::string& password);
    void clearInputBuffer();
    int getIntegerInput();
    std::string getStringInput(const std::string& prompt);
    std::string getPasswordInput(const std::string& prompt);
}

class ValidationStrategy {
public:
    virtual ~ValidationStrategy() = default;
    virtual bool validate(const string& value) const = 0;
};

class AlphanumericValidation : public ValidationStrategy {
public:
    bool validate(const string& value) const override {
        if (value.empty()) return false;
        return all_of(value.begin(), value.end(), [](char c) { return isalnum(static_cast<unsigned char>(c)); });
    }
};

class BloodTypeValidation : public ValidationStrategy {
public:
    bool validate(const string& bloodType) const override {
        static const vector<string> validTypes = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        return find(validTypes.begin(), validTypes.end(), bloodType) != validTypes.end();
    }
};

class User {
protected:
    string userID;
    string name;
    string contact;
    string role;
    string password;

public:
    User(const string& id, const string& n, const string& c, const string& r, const string& p);
    virtual ~User() = default;
    string getUserID() const;
    string getName() const;
    string getContact() const;
    string getRole() const;
    bool authenticate(const string& pwd) const;
    bool setUserID(const string& id);
    bool setName(const string& n);
    bool setContact(const string& c);
    bool setRole(const string& r);
    bool setPassword(const string& pwd);
    virtual void displayUserInfo() const = 0; // Make this pure virtual
};

class Donor : public User {
private:
    std::string bloodType;

public:
    Donor(const std::string& id, const std::string& n, const std::string& c, const std::string& bt, const std::string& p);
    std::string getBloodType() const;
    bool setBloodType(const std::string& bt);
    void displayUserInfo() const;
};

class AdminStaff : public User {
public:
    AdminStaff(const std::string& id, const std::string& n, const std::string& c, const std::string& p);
    void displayUserInfo() const;
};

class Requestor : public User {
public:
    Requestor(const std::string& id, const std::string& n, const std::string& c, const std::string& p);
    void displayUserInfo() const;
};

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

class UserManager {
private:
    std::vector<User*> users;

public:
    UserManager();
    ~UserManager();
    bool addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact, const std::string& password, const std::string& bloodType = "");
    User* getUserByID(const std::string& id);
    std::vector<User*> getUsersByRole(const std::string& role) const;
    void displayAllUsers() const;
    bool updateUser(const std::string& id, const std::string& newName, const std::string& newContact);
    bool deleteUser(const std::string& id);
};

class BloodBankSystem {
private:
    UserManager userManager;
    std::vector<BloodInventory> bloodInventory;
    std::vector<BloodRequest> bloodRequests;
    User* loggedInUser;

public:
    BloodBankSystem();
    bool addDonor(const std::string& id, const std::string& name, const std::string& contact, const std::string& bloodType, const std::string& password);
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
    bool addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact, const std::string& password);
    User* getUserByID(const std::string& id);
    void displayAllUsers() const;
    bool updateUser(const std::string& id, const std::string& newName, const std::string& newContact);
    bool deleteUser(const std::string& id);
    bool loginUser(const std::string& id, const std::string& password);
    void logoutUser();
    User* getLoggedInUser() const;
    bool registerRequestor(const std::string& id, const std::string& name, const std::string& contact, const std::string& password);
    bool isBloodAvailable(const std::string& bloodType, int quantity) const;
    void fulfillRequest(const std::string& requestID);
    void removeExpiredBlood();
    std::string getCurrentDate();
};

int main() {
    try {
        BloodBankSystem bbs;
        string userID, name, contact, bloodType, requestID, status, requestDate, password;
        int quantity, choice;

        bool running = true;
        bool mainMenuShown = false; 

        while (running) {
            bbs.removeExpiredBlood();

            if (bbs.getLoggedInUser() == nullptr) {
                if (mainMenuShown) break; // Exit after showing main menu once
                mainMenuShown = true;     

                cout << "\n--- Blood Bank Management System ---\n";
                cout << "1. Login\n";
                cout << "2. Register as Requestor\n";
                cout << "3. Register as Donor\n";
                cout << "0. Exit\n";
                cout << "Enter your choice: ";
                
                try {
                    choice = Validation::getIntegerInput();

                    switch (choice) {
                        case 1: {
                            userID = Validation::getStringInput("Enter User ID: ");
                            if (!Validation::isAlphanumeric(userID)) {
                                throw ValidationException("Invalid User ID format - must be alphanumeric");
                            }
                            password = Validation::getPasswordInput("Enter Password: ");
                            
                            if (bbs.loginUser(userID, password)) {
                                cout << "Logged in successfully!\n";
                            } else {
                                throw AuthenticationException("Invalid credentials");
                            }
                            break;
                        }
                        case 2: { // Register Requestor
                            userID = Validation::getStringInput("Enter Requestor ID: ");
                            if (!Validation::isAlphanumeric(userID)) {
                                throw ValidationException("Invalid Requestor ID format - must be alphanumeric");
                            }
                            name = Validation::getStringInput("Enter Name: ");
                            if (!Validation::isValidString(name)) {
                                throw ValidationException("Invalid name");
                            }
                            contact = Validation::getStringInput("Enter Contact: ");
                            if (!Validation::isValidString(contact)) {
                                throw ValidationException("Invalid contact");
                            }
                            password = Validation::getPasswordInput("Enter Password (min 6 chars, must include uppercase, lowercase and digit): ");
                            if (!Validation::isValidPassword(password)) {
                                throw ValidationException("Invalid password format");
                            }
                            
                            if (bbs.registerRequestor(userID, name, contact, password)) {
                                cout << "Requestor registered successfully!\n";
                            } else {
                                throw ValidationException("Registration failed - ID may already exist");
                            }
                            break;
                        }
                        case 3: { // Register Donor
                            userID = Validation::getStringInput("Enter Donor ID: ");
                            if (!Validation::isAlphanumeric(userID)) {
                                throw ValidationException("Invalid Donor ID format - must be alphanumeric");
                            }
                            name = Validation::getStringInput("Enter Name: ");
                            if (!Validation::isValidString(name)) {
                                throw ValidationException("Invalid name");
                            }
                            contact = Validation::getStringInput("Enter Contact: ");
                            if (!Validation::isValidString(contact)) {
                                throw ValidationException("Invalid contact");
                            }
                            bloodType = Validation::getStringInput("Enter Blood Type (A+/A-/B+/B-/AB+/AB-/O+/O-): ");
                            if (!Validation::isValidBloodType(bloodType)) {
                                throw ValidationException("Invalid blood type");
                            }
                            password = Validation::getPasswordInput("Enter Password (min 6 chars, must include uppercase, lowercase and digit): ");
                            if (!Validation::isValidPassword(password)) {
                                throw ValidationException("Invalid password format");
                            }
                            
                            if (bbs.addDonor(userID, name, contact, bloodType, password)) {
                                cout << "Donor registered successfully!\n";
                            } else {
                                throw ValidationException("Registration failed - ID may already exist");
                            }
                            break;
                        }
                        case 0:
                            running = false;
                            cout << "Thank you for using Blood Bank System!\n";
                            break;
                        default:
                            throw ValidationException("Invalid choice");
                    }
                }
                catch (const ValidationException& e) {
                    cerr << "Validation Error: " << e.what() << endl;
                }
                catch (const AuthenticationException& e) {
                    cerr << "Authentication Error: " << e.what() << endl;
                }
                catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            } else {
                string role = bbs.getLoggedInUser()->getRole();
                
                if (role == "Admin") {
                    // ... Admin menu implementation (your existing code) ...
                }
                else if (role == "Donor") {
                    try {
                        cout << "\n--- Donor Menu ---\n";
                        cout << "1. View My Information\n";
                        cout << "2. Update My Information\n";
                        cout << "3. Donate Blood\n";
                        cout << "4. View My Donations\n";
                        cout << "5. Logout\n";
                        cout << "Enter your choice: ";
                        
                        choice = Validation::getIntegerInput();
                        
                        switch (choice) {
                            case 1: {
                                bbs.getLoggedInUser()->displayUserInfo();
                                break;
                            }
                            case 2: {
                                string newName = Validation::getStringInput("Enter new name: ");
                                string newContact = Validation::getStringInput("Enter new contact: ");
                                string newBloodType = Validation::getStringInput("Enter new blood type (A+/A-/B+/B-/AB+/AB-/O+/O-): ");
                                
                                if (!Validation::isValidBloodType(newBloodType)) {
                                    throw ValidationException("Invalid blood type");
                                }
                                
                                if (bbs.updateDonor(bbs.getLoggedInUser()->getUserID(), newName, newContact, newBloodType)) {
                                    cout << "Information updated successfully!\n";
                                } else {
                                    throw ValidationException("Failed to update information");
                                }
                                break;
                            }
                            case 3: {
                                string bloodType = Validation::getStringInput("Enter blood type to donate (A+/A-/B+/B-/AB+/AB-/O+/O-): ");
                                if (!Validation::isValidBloodType(bloodType)) {
                                    throw ValidationException("Invalid blood type");
                                }
                                
                                std::cout << "Enter quantity (in units): ";
                                int quantity = Validation::getIntegerInput();
                                if (!Validation::isValidInteger(quantity) || quantity <= 0) {
                                    throw ValidationException("Invalid quantity");
                                }
                                
                                string donationDate = bbs.getCurrentDate();
                                
                                if (bbs.addBlood(bloodType, quantity, donationDate)) {
                                    cout << "Blood donation recorded successfully!\n";
                                } else {
                                    throw ValidationException("Failed to record blood donation");
                                }
                                break;
                            }
                            case 4: {
                                cout << "--- My Donations ---\n";
                                bbs.viewBloodInventory(); // You might want to create a method to view only specific donor's donations
                                break;
                            }
                            case 5: {
                                bbs.logoutUser();
                                cout << "Logged out successfully!\n";
                                break;
                            }
                            default:
                                throw ValidationException("Invalid choice");
                        }
                    }
                    catch (const ValidationException& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                    break;
                }
                else if (role == "Requestor") {
                    try {
                        cout << "\n--- Requestor Menu ---\n";
                        cout << "1. View My Information\n";
                        cout << "2. Update My Information\n";
                        cout << "3. Make Blood Request\n";
                        cout << "4. View My Requests\n";
                        cout << "5. Logout\n";
                        cout << "Enter your choice: ";
                        
                        choice = Validation::getIntegerInput();
                        
                        switch (choice) {
                            case 1: {
                                bbs.getLoggedInUser()->displayUserInfo();
                                break;
                            }
                            case 2: {
                                string newName = Validation::getStringInput("Enter new name: ");
                                string newContact = Validation::getStringInput("Enter new contact: ");
                                
                                if (bbs.updateUser(bbs.getLoggedInUser()->getUserID(), newName, newContact)) {
                                    cout << "Information updated successfully!\n";
                                } else {
                                    throw ValidationException("Failed to update information");
                                }
                                break;
                            }
                            case 3: {
                                string bloodType = Validation::getStringInput("Enter required blood type (A+/A-/B+/B-/AB+/AB-/O+/O-): ");
                                if (!Validation::isValidBloodType(bloodType)) {
                                    throw ValidationException("Invalid blood type");
                                }
                                
                                std::cout << "Enter quantity needed (in units): ";
                                int quantity = Validation::getIntegerInput();
                                if (!Validation::isValidInteger(quantity) || quantity <= 0) {
                                    throw ValidationException("Invalid quantity");
                                }
                                
                                string requestDate = bbs.getCurrentDate();
                                
                                if (bbs.addRequest(bbs.getLoggedInUser()->getUserID(), bloodType, quantity, requestDate)) {
                                    cout << "Blood request submitted successfully!\n";
                                } else {
                                    throw ValidationException("Failed to submit blood request");
                                }
                                break;
                            }
                            case 4: {
                                cout << "--- My Requests ---\n";
                                bbs.viewRequestsByRequestor(bbs.getLoggedInUser()->getUserID());
                                break;
                            }
                            case 5: {
                                bbs.logoutUser();
                                cout << "Logged out successfully!\n";
                                break;
                            }
                            default:
                                throw ValidationException("Invalid choice");
                        }
                    }
                    catch (const ValidationException& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            }
        }
    }
    catch (const exception& e) {
        cerr << "Fatal Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

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

    bool isValidPassword(const std::string& password) {
        if (password.length() < 6) return false;
        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;
        for (char c : password) {
            if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
            if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
            if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        }
        return hasUpper && hasLower && hasDigit;
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
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (!input.empty()) break;
            std::cout << "Input cannot be empty. Please try again.\n";
        }
        return input;
    }

    std::string getPasswordInput(const std::string& prompt) {
        std::string password;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, password);
            if (isValidPassword(password)) break;
            std::cout << "Password must be at least 6 characters with at least one uppercase, one lowercase letter and one digit.\n";
        }
        return password;
    }
}

User::User(const std::string& id, const std::string& n, const std::string& c, const std::string& r, const std::string& p)
    : userID(id), name(n), contact(c), role(r), password(p) {}

std::string User::getUserID() const { return userID; }
std::string User::getName() const { return name; }
std::string User::getContact() const { return contact; }
std::string User::getRole() const { return role; }

bool User::authenticate(const std::string& pwd) const {
    return password == pwd;
}

bool User::setUserID(const std::string& id) {
    if (Validation::isAlphanumeric(id)) {
        userID = id;
        return true;
    }
    return false;
}

bool User::setName(const std::string& n) {
    if (Validation::isValidString(n)) {
        name = n;
        return true;
    }
    return false;
}

bool User::setContact(const std::string& c) {
    if (Validation::isValidString(c)) {
        contact = c;
        return true;
    }
    return false;
}

bool User::setRole(const std::string& r) {
    if (Validation::isValidString(r)) {
        role = r;
        return true;
    }
    return false;
}

bool User::setPassword(const std::string& pwd) {
    if (Validation::isValidPassword(pwd)) {
        password = pwd;
        return true;
    }
    return false;
}

Donor::Donor(const std::string& id, const std::string& n, const std::string& c, const std::string& bt, const std::string& p)
    : User(id, n, c, "Donor", p), bloodType(bt) {}

std::string Donor::getBloodType() const { return bloodType; }

bool Donor::setBloodType(const std::string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    return false;
}

void Donor::displayUserInfo() const {
    std::cout << "User ID: " << userID << ", Name: " << name << ", Contact: " << contact << ", Role: " << role << std::endl;
    std::cout << "Blood Type: " << bloodType << std::endl;
}

AdminStaff::AdminStaff(const std::string& id, const std::string& n, const std::string& c, const std::string& p)
    : User(id, n, c, "Admin", p) {}

void AdminStaff::displayUserInfo() const {
    std::cout << "User ID: " << userID << ", Name: " << name << ", Contact: " << contact << ", Role: " << role << std::endl;
}

Requestor::Requestor(const std::string& id, const std::string& n, const std::string& c, const std::string& p)
    : User(id, n, c, "Requestor", p) {}

void Requestor::displayUserInfo() const {
    std::cout << "User ID: " << userID << ", Name: " << name << ", Contact: " << contact << ", Role: " << role << std::endl;
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
    return false;
}

bool BloodInventory::setQuantity(int qty) {
    if (Validation::isValidInteger(qty)) {
        quantity = qty;
        return true;
    }
    return false;
}

bool BloodInventory::setDonationDate(const std::string& dd) {
    if (Validation::isValidDate(dd)) {
        donationDate = dd;
        return true;
    }
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
            quantity = 0;
        }
    }
}

void BloodInventory::displayBloodInfo() const {
    std::cout << "Blood Type: " << bloodType << ", Quantity: " << quantity << ", Donation Date: " << donationDate << std::endl;
}

bool BloodInventory::isExpired(const std::string& currentDate) const {
    if (!Validation::isValidDate(currentDate) || !Validation::isValidDate(donationDate))
        return false;

    int donationYear = std::stoi(donationDate.substr(0, 4));
    int donationMonth = std::stoi(donationDate.substr(5, 2));
    int donationDay = std::stoi(donationDate.substr(8, 2));

    int currentYear = std::stoi(currentDate.substr(0, 4));
    int currentMonth = std::stoi(currentDate.substr(5, 2));
    int currentDay = std::stoi(currentDate.substr(8, 2));

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
    return false;
}

bool BloodRequest::setBloodType(const std::string& bt) {
    if (Validation::isValidBloodType(bt)) {
        bloodType = bt;
        return true;
    }
    return false;
}

bool BloodRequest::setQuantity(int qty) {
    if (Validation::isValidInteger(qty)) {
        quantity = qty;
        return true;
    }
    return false;
}

bool BloodRequest::setRequestDate(const std::string& rd) {
    if (Validation::isValidDate(rd)) {
        requestDate = rd;
        return true;
    }
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

bool UserManager::addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact, const std::string& password, const std::string& bloodType) {
    if (getUserByID(id) != nullptr) {
        return false;
    }
    if (role == "Donor") {
        Donor* newDonor = new Donor(id, name, contact, bloodType, password);
        if (!newDonor->setUserID(id) || !newDonor->setName(name) || !newDonor->setContact(contact) || !newDonor->setBloodType(bloodType) || !newDonor->setPassword(password)) {
            delete newDonor;
            return false;
        }
        users.push_back(newDonor);
        return true;
    } else if (role == "Admin") {
        AdminStaff* newAdmin = new AdminStaff(id, name, contact, password);
        if (!newAdmin->setUserID(id) || !newAdmin->setName(name) || !newAdmin->setContact(contact) || !newAdmin->setPassword(password)) {
            delete newAdmin;
            return false;
        }
        users.push_back(newAdmin);
        return true;
    } else if (role == "Requestor") {
        Requestor* newRequestor = new Requestor(id, name, contact, password);
        if (!newRequestor->setUserID(id) || !newRequestor->setName(name) || !newRequestor->setContact(contact) || !newRequestor->setPassword(password)) {
            delete newRequestor;
            return false;
        }
        users.push_back(newRequestor);
        return true;
    }
    else {
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
    if (users.empty()) {
        std::cout << "No users to display." << std::endl;
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

bool BloodBankSystem::addDonor(const std::string& id, const std::string& name, const std::string& contact, const std::string& bloodType, const std::string& password) {
    return userManager.addUser("Donor", id, name, contact, password, bloodType);
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
        std::cout << "Invalid blood type.\n";
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
        std::cout << "Invalid name.\n";
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
        if (donor) {
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
        std::cout << "Invalid blood type.\n";
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
        request.displayRequestInfo();
    }
}

void BloodBankSystem::viewRequestsByStatus(const std::string& status) const {
    if (status != "Pending" && status != "Fulfilled" && status != "Cancelled") {
        std::cout << "Invalid status.\n";
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
        std::cout << "Invalid Requestor ID.\n";
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

bool BloodBankSystem::addUser(const std::string& role, const std::string& id, const std::string& name, const std::string& contact, const std::string& password) {
    return userManager.addUser(role, id, name, contact, password);
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

bool BloodBankSystem::loginUser(const std::string& id, const std::string& password) {
    User* user = userManager.getUserByID(id);
    if (user && user->authenticate(password)) {
        loggedInUser = user;
        return true;
    }
    return false;
}

void BloodBankSystem::logoutUser() {
    loggedInUser = nullptr;
}

User* BloodBankSystem::getLoggedInUser() const {
    return loggedInUser;
}

bool BloodBankSystem::registerRequestor(const std::string& id, const std::string& name, const std::string& contact, const std::string& password) {
    return userManager.addUser("Requestor", id, name, contact, password);
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
            it = bloodInventory.erase(it);
        } else {
            ++it;
        }
    }
}

std::string BloodBankSystem::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);

    if (month.length() == 1) month = "0" + month;
    if (day.length() == 1) day = "0" + day;

    return year + "-" + month + "-" + day;
}