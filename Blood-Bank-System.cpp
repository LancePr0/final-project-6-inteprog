#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <cctype>
#include <map>

using namespace std;


const string USERS_FILE = "users.txt";
const string BLOOD_FILE = "blood_inventory.txt";
const string REQUESTS_FILE = "blood_requests.txt";
const string ACTIVITY_LOG_FILE = "activity_log.txt";

const vector<string> VALID_BLOOD_TYPES = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
const vector<string> VALID_ROLES = {"Admin", "Donor", "Requestor"};


class Utility {
public:
    
    static string toUpper(const string& s) {
        string result = s;
        for (char& c : result) c = toupper(c);
        return result;
    }

    
    static string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    
    static bool isValidDate(const string& date) {
        if (date.size() != 10) return false;
        if (date[4] != '-' || date[7] != '-') return false;
        string yearStr = date.substr(0,4);
        string monthStr = date.substr(5,2);
        string dayStr = date.substr(8,2);

        if (!all_of(yearStr.begin(), yearStr.end(), ::isdigit)) return false;
        if (!all_of(monthStr.begin(), monthStr.end(), ::isdigit)) return false;
        if (!all_of(dayStr.begin(), dayStr.end(), ::isdigit)) return false;

        int year = stoi(yearStr);
        int month = stoi(monthStr);
        int day = stoi(dayStr);

        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;

        
        int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if ((year%4==0 && year%100!=0) || (year%400==0)) daysInMonth[1] = 29;

        if (day < 1 || day > daysInMonth[month-1]) return false;
        return true;
    }

   
    static void pause() {
        cout << "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    
    static bool isNumeric(const string& s) {
        return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
    }

    
    static bool isValidBloodType(const string& bt) {
        string upperBT = toUpper(bt);
        return find(VALID_BLOOD_TYPES.begin(), VALID_BLOOD_TYPES.end(), upperBT) != VALID_BLOOD_TYPES.end();
    }

    
    static bool isValidRole(const string& role) {
        return find(VALID_ROLES.begin(), VALID_ROLES.end(), role) != VALID_ROLES.end();
    }

    
    static string getCurrentDate() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buf[11];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", now->tm_year+1900, now->tm_mon+1, now->tm_mday);
        return string(buf);
    }

    
    static vector<string> split(const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};


class Logger {
private:
    ofstream logFile;

public:
    Logger() {
        logFile.open(ACTIVITY_LOG_FILE, ios::app);
        if (!logFile.is_open()) {
            cout << "WARNING: Could not open activity log file.\n";
        }
    }

    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    void log(const string& message) {
        if (!logFile.is_open()) return;
        string timestamp = Utility::getCurrentDate();
        logFile << "[" << timestamp << "] " << message << endl;
    }

    
    void displayLogs() const {
        ifstream file(ACTIVITY_LOG_FILE);
        if (!file.is_open()) {
            cout << "No activity log found.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};


class User {
protected:
    string userID;
    string name;
    string contact;
    string password;
    string role;

public:
    User(const string& id, const string& name, const string& contact, const string& password, const string& role)
        : userID(id), name(name), contact(contact), password(password), role(role) {}

    virtual ~User() {}

    string getUserID() const { return userID; }
    string getName() const { return name; }
    string getContact() const { return contact; }
    string getPassword() const { return password; }
    string getRole() const { return role; }

    void setName(const string& n) { name = n; }
    void setContact(const string& c) { contact = c; }
    void setPassword(const string& p) { password = p; }

    virtual void displayUserInfo() const {
        cout << "UserID: " << userID << "\nName: " << name << "\nContact: " << contact << "\nRole: " << role << "\n";
    }

    bool authenticate(const string& pass) const {
        return password == pass;
    }
};


class Donor : public User {
private:
    string bloodType;

public:
    Donor(const string& id, const string& name, const string& contact, const string& password, const string& bloodType)
        : User(id, name, contact, password, "Donor"), bloodType(bloodType) {}

    string getBloodType() const { return bloodType; }
    void setBloodType(const string& bt) { bloodType = bt; }

    void displayUserInfo() const override {
        User::displayUserInfo();
        cout << "Blood Type: " << bloodType << "\n";
    }
};


class BloodUnit {
private:
    string bloodType;
    int quantity;
    string donationDate;
    string donorName; 

public:
    BloodUnit() : bloodType(""), quantity(0), donationDate(""), donorName("") {}
    BloodUnit(const string& bt, int qty, const string& date, const string& donor)
        : bloodType(bt), quantity(qty), donationDate(date), donorName(donor) {}

    string getBloodType() const { return bloodType; }
    int getQuantity() const { return quantity; }
    string getDonationDate() const { return donationDate; }
    string getDonorName() const { return donorName; } 

    void setBloodType(const string& bt) { bloodType = bt; }
    void setQuantity(int qty) { quantity = qty; }
    void setDonationDate(const string& date) { donationDate = date; }
    void setDonorName(const string& donor) { donorName = donor; } 

    void displayBloodInfo() const {
        cout << "Blood Type: " << bloodType << "\nQuantity: " << quantity
             << "\nDonation Date: " << donationDate
             << "\nDonor Name: " << donorName << "\n"; 
    }
};


class BloodRequest {
private:
    string requestID;
    string requestorID;
    string bloodType;
    int quantity;
    string requestDate;
    string status; 

public:
    BloodRequest() : requestID(""), requestorID(""), bloodType(""), quantity(0), requestDate(""), status("Pending") {}
    BloodRequest(const string& reqID, const string& reqorID, const string& bt, int qty, const string& date, const string& stat = "Pending")
        : requestID(reqID), requestorID(reqorID), bloodType(bt), quantity(qty), requestDate(date), status(stat) {}

    string getRequestID() const { return requestID; }
    string getRequestorID() const { return requestorID; }
    string getBloodType() const { return bloodType; }
    int getQuantity() const { return quantity; }
    string getRequestDate() const { return requestDate; }
    string getStatus() const { return status; }

    void setStatus(const string& s) { status = s; }
    void setQuantity(int q) { quantity = q; }

    void displayRequestInfo() const {
        cout << "Request ID: " << requestID << "\nRequestor ID: " << requestorID << "\nBlood Type: " << bloodType
             << "\nQuantity: " << quantity << "\nRequest Date: " << requestDate << "\nStatus: " << status << "\n";
    }
};


class BloodBankSystem {
private:
    vector<User*> users;
    vector<BloodUnit> bloodInventory;
    vector<BloodRequest> bloodRequests;

    User* currentUser;
    Logger logger;

public:
    BloodBankSystem() : currentUser(nullptr) {
        loadUsers();
        loadBloodInventory();
        loadBloodRequests();
    }

    ~BloodBankSystem() {
        saveAllData();
        for (User* user : users) delete user;
        users.clear();
    }

    void run() {
        while (true) {
            cout << "\n--- Blood Bank Management System ---\n";
            cout << "1. Login\n2. Register\n3. Exit\n";
            int choice = getValidatedChoice(1,3);

            if (choice == 1) {
                if (login()) {
                    logger.log("User " + currentUser->getUserID() + " logged in.");
                    userMenu();
                    logger.log("User " + currentUser->getUserID() + " logged out.");
                    currentUser = nullptr;
                }
            } else if (choice == 2) {
                registerUser();
            } else {
                cout << "Thank you for using the system. Goodbye!\n";
                break;
            }
        }
    }

private:
    
    bool login() {
        string id, pass;
        cout << "Enter UserID: ";
        getline(cin, id);
        cout << "Enter Password: ";
        getline(cin, pass);

        for (User* user : users) {
            if (user->getUserID() == id && user->authenticate(pass)) {
                currentUser = user;
                cout << "Login successful! Welcome, " << currentUser->getName() << " (" << currentUser->getRole() << ").\n";
                return true;
            }
        }
        cout << "Login failed. Invalid UserID or Password.\n";
        return false;
    }

    void registerUser() {
        cout << "--- User Registration ---\n";
        string id;
        while (true) {
            cout << "Enter UserID (no spaces): ";
            getline(cin, id);
            id = Utility::trim(id);
            if (id.empty()) {
                cout << "UserID cannot be empty.\n";
                continue;
            }
            if (find_if(users.begin(), users.end(), [&](User* u){return u->getUserID() == id;}) != users.end()) {
                cout << "UserID already exists. Try another.\n";
                continue;
            }
            break;
        }

        string name;
        while (true) {
            cout << "Enter Full Name: ";
            getline(cin, name);
            name = Utility::trim(name);
            if (!name.empty()) break;
            cout << "Name cannot be empty.\n";
        }

        string contact;
        while (true) {
            cout << "Enter Contact Number: ";
            getline(cin, contact);
            contact = Utility::trim(contact);
            if (!contact.empty() && Utility::isNumeric(contact)) break;
            cout << "Contact must be numeric and cannot be empty.\n";
        }
        string pass1, pass2;
        while (true) {
            cout << "Enter Password: ";
            getline(cin, pass1);
            cout << "Confirm Password: ";
            getline(cin, pass2);
            if (pass1 == pass2 && !pass1.empty()) break;
            cout << "Passwords do not match or are empty. Try again.\n";
        }

        cout << "Choose Role:\n1. Admin\n2. Donor\n3. Requestor\n";
        int roleChoice = getValidatedChoice(1,3);
        string role = VALID_ROLES[roleChoice - 1];

        if (role == "Donor") {
            string bloodType;
            while (true) {
                cout << "Enter Blood Type (A+, A-, B+, B-, AB+, AB-, O+, O-): ";
                getline(cin, bloodType);
                bloodType = Utility::toUpper(Utility::trim(bloodType));
                if (Utility::isValidBloodType(bloodType)) break;
                cout << "Invalid blood type. Try again.\n";
            }
            users.push_back(new Donor(id, name, contact, pass1, bloodType));
        } else {
            users.push_back(new User(id, name, contact, pass1, role));
        }
        cout << "User registered successfully!\n";
        logger.log("New user registered: " + id + " Role: " + role);
        saveUsers();
    }

   
    void userMenu() {
        if (!currentUser) return;

        if (currentUser->getRole() == "Admin") {
            adminMenu();
        } else if (currentUser->getRole() == "Donor") {
            donorMenu();
        } else if (currentUser->getRole() == "Requestor") {
            requestorMenu();
        } else {
            cout << "Unknown role. Logging out.\n";
        }
    }

    
    void adminMenu() {
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Manage Users\n2. Manage Blood Inventory\n3. Manage Blood Requests\n4. View Reports\n5. Logout\n";
            int choice = getValidatedChoice(1,5);

            switch (choice) {
                case 1: manageUsers(); break;
                case 2: manageBloodInventory(); break;
                case 3: manageBloodRequests(); break;
                case 4: viewReports(); break;
                case 5: cout << "Logging out Admin...\n"; return;
                default: cout << "Invalid choice.\n";
            }
        }
    }

    void manageUsers() {
        while (true) {
            cout << "\n--- Manage Users ---\n";
            cout << "1. View All Users\n2. Add User\n3. Update User\n4. Delete User\n5. Back\n";
            int choice = getValidatedChoice(1,5);

            if (choice == 1) {
                if (users.empty()) {
                    cout << "No users found.\n";
                } else {
                    for (User* user : users) {
                        user->displayUserInfo();
                        cout << "------------------\n";
                    }
                }
                Utility::pause();
            } else if (choice == 2) {
                registerUser();
            } else if (choice == 3) {
                cout << "Enter UserID to update: ";
                string id; getline(cin, id);
                User* user = findUserByID(id);
                if (!user) {
                    cout << "User not found.\n";
                    Utility::pause();
                    continue;
                }
                updateUser(user);
            } else if (choice == 4) {
                cout << "Enter UserID to delete: ";
                string id; getline(cin, id);
                if (deleteUser(id)) {
                    cout << "User deleted.\n";
                    logger.log("User deleted: " + id);
                } else {
                    cout << "User not found.\n";
                }
                Utility::pause();
            } else {
                break;
            }
        }
    }

    User* findUserByID(const string& id) {
        for (User* u : users) {
            if (u->getUserID() == id) return u;
        }
        return nullptr;
    }

    void updateUser(User* user) {
        cout << "Updating user " << user->getUserID() << "\n";
        cout << "Leave input blank to keep current value.\n";

        cout << "Current Name: " << user->getName() << "\nNew Name: ";
        string input; getline(cin, input);
        if (!input.empty()) user->setName(input);

        cout << "Current Contact: " << user->getContact() << "\nNew Contact: ";
        getline(cin, input);
        input = Utility::trim(input);
        if (!input.empty()) {
            if (Utility::isNumeric(input)) {
                user->setContact(input);
            } else {
                cout << "Contact must be numeric. Keeping previous.\n";
            }
        }

        
        if (user->getRole() == "Donor") {
            Donor* donor = dynamic_cast<Donor*>(user);
            if (donor) {
                cout << "Current Blood Type: " << donor->getBloodType() << "\nNew Blood Type: ";
                getline(cin, input);
                input = Utility::toUpper(Utility::trim(input));
                if (!input.empty()) {
                    if (Utility::isValidBloodType(input)) {
                        donor->setBloodType(input);
                    } else {
                        cout << "Invalid blood type entered. Keeping previous.\n";
                    }
                }
            }
        }
        cout << "User updated successfully.\n";
        logger.log("User updated: " + user->getUserID());
        saveUsers();
        Utility::pause();
    }

    bool deleteUser(const string& id) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if ((*it)->getUserID() == id) {
                delete *it;
                users.erase(it);
                saveUsers();
                return true;
            }
        }
        return false;
    }

    void manageBloodInventory() {
        while (true) {
            cout << "\n--- Manage Blood Inventory ---\n";
            cout << "1. View Blood Inventory\n2. Add Blood Unit\n3. Update Blood Unit\n4. Delete Blood Unit\n5. Back\n";
            int choice = getValidatedChoice(1,5);

            if (choice == 1) {
                if (bloodInventory.empty()) {
                    cout << "Blood inventory is empty.\n";
                } else {
                    for (size_t i = 0; i < bloodInventory.size(); ++i) {
                        cout << "Record #" << (i+1) << "\n";
                        bloodInventory[i].displayBloodInfo();
                        cout << "------------------\n";
                    }
                }
                Utility::pause();
            } else if (choice == 2) {
                addBloodUnit();
            } else if (choice == 3) {
                updateBloodUnit();
            } else if (choice == 4) {
                deleteBloodUnit();
            } else {
                break;
            }
        }
    }

    void addBloodUnit() {
        cout << "Add Blood Unit\n";
        string bloodType;
        while (true) {
            cout << "Enter Blood Type (A+, A-, B+, B-, AB+, AB-, O+, O-): ";
            getline(cin, bloodType);
            bloodType = Utility::toUpper(Utility::trim(bloodType));
            if (Utility::isValidBloodType(bloodType)) break;
            cout << "Invalid blood type. Try again.\n";
        }

        int quantity;
        while (true) {
            cout << "Enter Quantity (ml): ";
            string qtyStr; getline(cin, qtyStr);
            if (Utility::isNumeric(qtyStr)) {
                quantity = stoi(qtyStr);
                if (quantity > 0) break;
            }
            cout << "Invalid quantity. Must be positive integer.\n";
        }

        string date;
        while (true) {
            cout << "Enter Donation Date (YYYY-MM-DD): ";
            getline(cin, date);
            if (Utility::isValidDate(date)) break;
            cout << "Invalid date format or value. Try again.\n";
        }

        string donorName;
        cout << "Enter Donor Name: ";
        getline(cin, donorName);

        bloodInventory.emplace_back(bloodType, quantity, date, donorName);
        cout << "Blood unit added successfully.\n";
        logger.log("Blood unit added: " + bloodType + " Qty: " + to_string(quantity) + " Donor: " + donorName);
        saveBloodInventory();
        Utility::pause();
    }

    void updateBloodUnit() {
        if (bloodInventory.empty()) {
            cout << "No blood units to update.\n";
            Utility::pause();
            return;
        }
        cout << "Enter record number to update (1 to " << bloodInventory.size() << "): ";
        int rec = getValidatedChoice(1, bloodInventory.size());
        BloodUnit& unit = bloodInventory[rec - 1];
        cout << "Updating blood unit #" << rec << "\n";

        cout << "Current Blood Type: " << unit.getBloodType() << "\nNew Blood Type: ";
        string input; getline(cin, input);
        input = Utility::toUpper(Utility::trim(input));
        if (!input.empty() && Utility::isValidBloodType(input)) {
            unit.setBloodType(input);
        }

        cout << "Current Quantity: " << unit.getQuantity() << "\nNew Quantity: ";
        getline(cin, input);
        if (!input.empty() && Utility::isNumeric(input)) {
            int q = stoi(input);
            if (q > 0) unit.setQuantity(q);
        }

        cout << "Current Donation Date: " << unit.getDonationDate() << "\nNew Donation Date: ";
        getline(cin, input);
        if (!input.empty() && Utility::isValidDate(input)) {
            unit.setDonationDate(input);
        }

        cout << "Current Donor Name: " << unit.getDonorName() << "\nNew Donor Name: ";
        getline(cin, input);
        if (!input.empty()) {
            unit.setDonorName(input);
        }

        cout << "Blood unit updated.\n";
        logger.log("Blood unit updated: Record #" + to_string(rec));
        saveBloodInventory();
        Utility::pause();
    }

    void deleteBloodUnit() {
        if (bloodInventory.empty()) {
            cout << "No blood units to delete.\n";
            Utility::pause();
            return;
        }
        cout << "Enter record number to delete (1 to " << bloodInventory.size() << "): ";
        int rec = getValidatedChoice(1, bloodInventory.size());
        bloodInventory.erase(bloodInventory.begin() + (rec - 1));
        cout << "Blood unit deleted.\n";
        logger.log("Blood unit deleted: Record #" + to_string(rec));
        saveBloodInventory();
        Utility::pause();
    }

    void manageBloodRequests() {
        while (true) {
            cout << "\n--- Manage Blood Requests ---\n";
            cout << "1. View All Requests\n2. Approve Request\n3. Reject Request\n4. Back\n";
            int choice = getValidatedChoice(1,4);

            if (choice == 1) {
                if (bloodRequests.empty()) {
                    cout << "No blood requests found.\n";
                } else {
                    for (const BloodRequest& req : bloodRequests) {
                        req.displayRequestInfo();
                        cout << "------------------\n";
                    }
                }
                Utility::pause();
            } else if (choice == 2) {
                approveRequest();
            } else if (choice == 3) {
                rejectRequest();
            } else {
                break;
            }
        }
    }

    void approveRequest() {
        if (bloodRequests.empty()) {
            cout << "No requests to approve.\n";
            Utility::pause();
            return;
        }
        cout << "Enter Request ID to approve: ";
        string reqID; getline(cin, reqID);
        BloodRequest* req = findRequestByID(reqID);
        if (!req) {
            cout << "Request not found.\n";
            Utility::pause();
            return;
        }
        if (req->getStatus() != "Pending") {
            cout << "Request is already " << req->getStatus() << ".\n";
            Utility::pause();
            return;
        }

        int totalQty = 0;
        for (const BloodUnit& unit : bloodInventory) {
            if (unit.getBloodType() == req->getBloodType()) {
                totalQty += unit.getQuantity();
            }
        }
        if (totalQty < req->getQuantity()) {
            cout << "Insufficient blood quantity in inventory.\n";
            Utility::pause();
            return;
        }

        int qtyToDeduct = req->getQuantity();
        for (BloodUnit& unit : bloodInventory) {
            if (unit.getBloodType() == req->getBloodType()) {
                if (unit.getQuantity() >= qtyToDeduct) {
                    unit.setQuantity(unit.getQuantity() - qtyToDeduct);
                    qtyToDeduct = 0;
                    break;
                } else {
                    qtyToDeduct -= unit.getQuantity();
                    unit.setQuantity(0);
                }
            }
        }

        req->setStatus("Approved");
        cout << "Request approved.\n";
        logger.log("Request approved: " + reqID);
        saveBloodInventory();
        saveBloodRequests();
        Utility::pause();
    }

    void rejectRequest() {
        if (bloodRequests.empty()) {
            cout << "No requests to reject.\n";
            Utility::pause();
            return;
        }
        cout << "Enter Request ID to reject: ";
        string reqID; getline(cin, reqID);
        BloodRequest* req = findRequestByID(reqID);
        if (!req) {
            cout << "Request not found.\n";
            Utility::pause();
            return;
        }
        if (req->getStatus() != "Pending") {
            cout << "Request is already " << req->getStatus() << ".\n";
            Utility::pause();
            return;
        }
        req->setStatus("Rejected");
        cout << "Request rejected.\n";
        logger.log("Request rejected: " + reqID);
        saveBloodRequests();
        Utility::pause();
    }

    BloodRequest* findRequestByID(const string& reqID) {
        for (BloodRequest& req : bloodRequests) {
            if (req.getRequestID() == reqID) return &req;
        }
        return nullptr;
    }

    void viewReports() {
        while (true) {
            cout << "\n--- Reports ---\n";
            cout << "1. Blood Inventory Summary\n2. User Summary\n3. Requests Summary\n4. Activity Log\n5. Back\n";
            int choice = getValidatedChoice(1,5);

            if (choice == 1) {
                bloodInventorySummary();
            } else if (choice == 2) {
                userSummary();
            } else if (choice == 3) {
                requestsSummary();
            } else if (choice == 4) {
                viewActivityLog();
            } else {
                break;
            }
        }
    }

    void bloodInventorySummary() {
        cout << "\n--- Blood Inventory Summary ---\n";
        map<string, int> bloodCount;
        for (const string& bt : VALID_BLOOD_TYPES) bloodCount[bt] = 0;
        for (const BloodUnit& unit : bloodInventory) {
            bloodCount[unit.getBloodType()] += unit.getQuantity();
        }
        for (const auto& pair : bloodCount) {
            cout << pair.first << ": " << pair.second << " ml\n";
        }
        Utility::pause();
    }

    void userSummary() {
        cout << "\n--- User Summary ---\n";
        map<string, int> roleCount;
        for (const string& role : VALID_ROLES) roleCount[role] = 0;
        for (const User* user : users) {
            roleCount[user->getRole()]++;
        }
        for (const auto& pair : roleCount) {
            cout << pair.first << "s: " << pair.second << "\n";
        }
        Utility::pause();
    }

    void requestsSummary() {
        cout << "\n--- Requests Summary ---\n";
        map<string, int> statusCount;
        statusCount["Pending"] = 0;
        statusCount["Approved"] = 0;
        statusCount["Rejected"] = 0;
        for (const BloodRequest& req : bloodRequests) {
            statusCount[req.getStatus()]++;
        }
        for (const auto& pair : statusCount) {
            cout << pair.first << ": " << pair.second << "\n";
        }
        Utility::pause();
    }

    void viewActivityLog() {
        cout << "\n--- Activity Log ---\n";
        logger.displayLogs();
        Utility::pause();
    }


    void donorMenu() {
        while (true) {
            cout << "\n--- Donor Menu ---\n";
            cout << "1. View My Profile\n2. Update Profile\n3. View Blood Inventory\n4. Donate Blood\n5. Logout\n";
            int choice = getValidatedChoice(1,5);
            if (choice == 1) {
                currentUser->displayUserInfo();
                Utility::pause();
            } else if (choice == 2) {
                updateUser(currentUser);
            } else if (choice == 3) {
                viewBloodInventory();
            } else if (choice == 4) {
                donateBlood();
            } else {
                cout << "Logging out Donor...\n";
                break;
            }
        }
    }

    void donateBlood() {
        cout << "--- Donate Blood ---\n";
        Donor* donor = dynamic_cast<Donor*>(currentUser);
        if (!donor) {
            cout << "Error: Only donors can donate blood.\n";
            Utility::pause();
            return;
        }
        string bloodType = donor->getBloodType();
        cout << "Your Blood Type: " << bloodType << endl;

        int quantity;
        while (true) {
            cout << "Enter Quantity to Donate (ml): ";
            string qtyStr; getline(cin, qtyStr);
            if (Utility::isNumeric(qtyStr)) {
                quantity = stoi(qtyStr);
                if (quantity > 0) break;
            }
            cout << "Invalid quantity. Must be positive integer.\n";
        }

        string date = Utility::getCurrentDate();
        string donorName = donor->getName(); 
        bloodInventory.emplace_back(bloodType, quantity, date, donorName); 
        cout << "Thank you for your donation!\n";
        logger.log("Donor " + donor->getUserID() + " donated " + to_string(quantity) + "ml of " + bloodType);
        saveBloodInventory();
        Utility::pause();
    }

    void viewBloodInventory() {
        if (bloodInventory.empty()) {
            cout << "Blood inventory is empty.\n";
        } else {
            for (const BloodUnit& unit : bloodInventory) {
                unit.displayBloodInfo();
                cout << "------------------\n";
            }
        }
        Utility::pause();
    }

    
    void requestorMenu() {
        while (true) {
            cout << "\n--- Requestor Menu ---\n";
            cout << "1. View My Profile\n2. Update Profile\n3. Make Blood Request\n4. View My Requests\n5. Logout\n";
            int choice = getValidatedChoice(1,5);
            if (choice == 1) {
                currentUser->displayUserInfo();
                Utility::pause();
            } else if (choice == 2) {
                updateUser(currentUser);
            } else if (choice == 3) {
                makeBloodRequest();
            } else if (choice == 4) {
                viewMyRequests();
            } else {
                cout << "Logging out Requestor...\n";
                break;
            }
        }
    }

    void makeBloodRequest() {
        cout << "--- Make Blood Request ---\n";
        string bloodType;
        while (true) {
            cout << "Enter Blood Type (A+, A-, B+, B-, AB+, AB-, O+, O-): ";
            getline(cin, bloodType);
            bloodType = Utility::toUpper(Utility::trim(bloodType));
            if (Utility::isValidBloodType(bloodType)) break;
            cout << "Invalid blood type. Try again.\n";
        }

        int quantity;
        while (true) {
            cout << "Enter Quantity (ml): ";
            string qtyStr; getline(cin, qtyStr);
            if (Utility::isNumeric(qtyStr)) {
                quantity = stoi(qtyStr);
                if (quantity > 0) break;
            }
            cout << "Invalid quantity. Must be positive integer.\n";
        }

        string date;
        while (true) {
            cout << "Enter Request Date (YYYY-MM-DD): ";
            getline(cin, date);
            if (Utility::isValidDate(date)) break;
            cout << "Invalid date format or value. Try again.\n";
        }

        string reqID = generateRequestID();
        bloodRequests.emplace_back(reqID, currentUser->getUserID(), bloodType, quantity, date);
        cout << "Blood request submitted. Request ID: " << reqID << "\n";
        logger.log("New blood request: " + reqID + " by " + currentUser->getUserID());
        saveBloodRequests();
        Utility::pause();
    }

    void viewMyRequests() {
        cout << "--- My Blood Requests ---\n";
        bool found = false;
        for (const BloodRequest& req : bloodRequests) {
            if (req.getRequestorID() == currentUser->getUserID()) {
                req.displayRequestInfo();
                cout << "------------------\n";
                found = true;
            }
        }
        if (!found) cout << "You have no blood requests.\n";
        Utility::pause();
    }

    
    int getValidatedChoice(int min, int max) {
        while (true) {
            cout << "Enter choice (" << min << "-" << max << "): ";
            string input; getline(cin, input);
            if (Utility::isNumeric(input)) {
                int choice = stoi(input);
                if (choice >= min && choice <= max) return choice;
            }
            cout << "Invalid choice. Try again.\n";
        }
    }

    string generateRequestID() {
        static int counter = 1000;
        return "REQ" + to_string(counter++);
    }

    void loadUsers() {
        ifstream file(USERS_FILE);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            
            vector<string> tokens = Utility::split(line, '|');
            if (tokens.size() < 5) continue;
            string id = tokens[0];
            string name = tokens[1];
            string contact = tokens[2];
            string pass = tokens[3];
            string role = tokens[4];
            if (role == "Donor" && tokens.size() == 6) {
                string bloodType = tokens[5];
                users.push_back(new Donor(id, name, contact, pass, bloodType));
            } else {
                users.push_back(new User(id, name, contact, pass, role));
            }
        }
        file.close();
    }

    void saveUsers() {
        ofstream file(USERS_FILE);
        for (User* user : users) {
            file << user->getUserID() << "|" << user->getName() << "|" << user->getContact() << "|" << user->getPassword() << "|" << user->getRole();
            if (user->getRole() == "Donor") {
                Donor* donor = dynamic_cast<Donor*>(user);
                if (donor) {
                    file << "|" << donor->getBloodType();
                }
            }
            file << endl;
        }
        file.close();
    }
        void loadBloodInventory() {
    ifstream file(BLOOD_FILE);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        vector<string> tokens = Utility::split(line, '|');
        if (tokens.size() != 4) continue; 
        string bloodType = tokens[0];
        int qty = stoi(tokens[1]);
        string date = tokens[2];
        string donorName = tokens[3];
        bloodInventory.emplace_back(bloodType, qty, date, donorName);
    }
    file.close();
}

    void saveBloodInventory() {
    ofstream file(BLOOD_FILE);
    for (const BloodUnit& unit : bloodInventory) {
        file << unit.getBloodType() << "|" << unit.getQuantity() << "|" << unit.getDonationDate()
             << "|" << unit.getDonorName() << "\n"; 
    }
    file.close();
}

    void loadBloodRequests() {
        ifstream file(REQUESTS_FILE);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            vector<string> tokens = Utility::split(line, '|');
            if (tokens.size() != 6) continue;
            string reqID = tokens[0];
            string reqorID = tokens[1];
            string bloodType = tokens[2];
            int qty = stoi(tokens[3]);
            string reqDate = tokens[4];
            string status = tokens[5];
            bloodRequests.emplace_back(reqID, reqorID, bloodType, qty, reqDate, status);
        }
        file.close();
    }

    void saveBloodRequests() {
        ofstream file(REQUESTS_FILE);
        for (const BloodRequest& req : bloodRequests) {
            file << req.getRequestID() << "|" << req.getRequestorID() << "|" << req.getBloodType() << "|"
                 << req.getQuantity() << "|" << req.getRequestDate() << "|" << req.getStatus() << "\n";
        }
        file.close();
    }

    void saveAllData() {
        saveUsers();
        saveBloodInventory();
        saveBloodRequests();
    }
};


int main() {
    BloodBankSystem system;
    system.run();
    return 0;
}