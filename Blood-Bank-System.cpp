#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;

const int MAX_DONORS = 1000;
const int MAX_BLOOD_UNITS = 5000;
const int MAX_REQUESTS = 1000;
const int MAX_USERS = 100;
const int MAX_NAME_LENGTH = 50;
const int MAX_ADDRESS_LENGTH = 100;
const int MAX_PHONE_LENGTH = 15;
const int MAX_EMAIL_LENGTH = 50;

const char* BLOOD_TYPES[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
const int BLOOD_TYPE_COUNT = 8;

struct Date {
    int day;
    int month;
    int year;
};

struct Donor {
    int id;
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char bloodType[3];
    char phone[MAX_PHONE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    Date lastDonationDate;
    bool isActive;
};

struct BloodUnit {
    int id;
    int donorId;
    char bloodType[3];
    Date donationDate;
    Date expiryDate;
    bool isAvailable;
};

struct Request {
    int id;
    int requesterId;
    char bloodType[3];
    int quantity;
    Date requestDate;
    char status[20];
    char hospitalName[MAX_NAME_LENGTH];
    char patientName[MAX_NAME_LENGTH];
};

struct User {
    int id;
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char role[20];
};

class BloodBankSystem {
private:
    Donor donors[MAX_DONORS];
    BloodUnit bloodUnits[MAX_BLOOD_UNITS];
    Request requests[MAX_REQUESTS];
    User users[MAX_USERS];

    int donorCount;
    int bloodUnitCount;
    int requestCount;
    int userCount;
    int currentUserId;

    bool isEmailValid(const char* email) {
        int atCount = 0;
        int dotCount = 0;
        for (int i = 0; email[i] != '\0'; i++) {
            if (email[i] == '@') atCount++;
            if (email[i] == '.') dotCount++;
        }
        return (atCount == 1 && dotCount >= 1);
    }

    bool isPhoneValid(const char* phone) {
        for (int i = 0; phone[i] != '\0'; i++) {
            if (!(phone[i] >= '0' && phone[i] <= '9') && phone[i] != '+' && phone[i] != '-') {
                return false;
            }
        }
        return true;
    }

    bool isValidBloodType(const char* bloodType) {
        for (int i = 0; i < BLOOD_TYPE_COUNT; i++) {
            if (strcmp(bloodType, BLOOD_TYPES[i]) == 0) {
                return true;
            }
        }
        return false;
    }

    Date getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        Date currentDate;
        currentDate.day = ltm->tm_mday;
        currentDate.month = 1 + ltm->tm_mon;
        currentDate.year = 1900 + ltm->tm_year;
        return currentDate;
    }

    Date calculateExpiryDate(Date donationDate) {
        Date expiryDate = donationDate;
        expiryDate.month += 1;
        if (expiryDate.month > 12) {
            expiryDate.month -= 12;
            expiryDate.year++;
        }
        return expiryDate;
    }

    bool isDateValid(Date date) {
        if (date.year < 1900 || date.year > 2100) return false;
        if (date.month < 1 || date.month > 12) return false;
        if (date.day < 1 || date.day > 31) return false;
        if ((date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) && date.day > 30) {
            return false;
        }
        if (date.month == 2) {
            bool isLeap = (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0);
            if (isLeap && date.day > 29) return false;
            if (!isLeap && date.day > 28) return false;
        }
        return true;
    }

    bool isDateBefore(Date date1, Date date2) {
        if (date1.year < date2.year) return true;
        if (date1.year > date2.year) return false;
        if (date1.month < date2.month) return true;
        if (date1.month > date2.month) return false;
        return date1.day < date2.day;
    }

    void displayDate(Date date) {
        cout << date.day << "/" << date.month << "/" << date.year;
    }

    int getBloodTypeIndex(const char* bloodType) {
        for (int i = 0; i < BLOOD_TYPE_COUNT; i++) {
            if (strcmp(bloodType, BLOOD_TYPES[i]) == 0) {
                return i;
            }
        }
        return -1;
    }

    int findDonorById(int id) {
        for (int i = 0; i < donorCount; i++) {
            if (donors[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    int findBloodUnitById(int id) {
        for (int i = 0; i < bloodUnitCount; i++) {
            if (bloodUnits[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    int findRequestById(int id) {
        for (int i = 0; i < requestCount; i++) {
            if (requests[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    int findUserById(int id) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    int findUserByUsername(const char* username) {
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, username) == 0) {
                return i;
            }
        }
        return -1;
    }

    bool isAdmin() {
        if (currentUserId == -1) return false;
        int userIndex = findUserById(currentUserId);
        if (userIndex == -1) return false;
        return strcmp(users[userIndex].role, "Admin") == 0;
    }

    bool isRequestor() {
        if (currentUserId == -1) return false;
        int userIndex = findUserById(currentUserId);
        if (userIndex == -1) return false;
        return strcmp(users[userIndex].role, "Requestor") == 0;
    }

    bool isDonor() {
        if (currentUserId == -1) return false;
        int userIndex = findUserById(currentUserId);
        if (userIndex == -1) return false;
        return strcmp(users[userIndex].role, "Donor") == 0;
    }

public:
    BloodBankSystem() {
        donorCount = 0;
        bloodUnitCount = 0;
        requestCount = 0;
        userCount = 0;
        currentUserId = -1;

        User admin;
        admin.id = 1;
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin123");
        strcpy(admin.role, "Admin");
        users[userCount++] = admin;
    }

    bool login(const char* username, const char* password) {
        int userIndex = findUserByUsername(username);
        if (userIndex == -1) return false;

        if (strcmp(users[userIndex].password, password) == 0) {
            currentUserId = users[userIndex].id;
            return true;
        }
        return false;
    }

    void logout() {
        currentUserId = -1;
    }

    bool registerUser(const char* username, const char* password, const char* role) {
        if (userCount >= MAX_USERS) return false;
        if (findUserByUsername(username) != -1) return false;

        User newUser;
        newUser.id = userCount + 1;
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        strcpy(newUser.role, role);

        users[userCount++] = newUser;
        return true;
    }

    bool addDonor(const char* firstName, const char* lastName, const char* bloodType,
                  const char* phone, const char* email, const char* address) {
        if (donorCount >= MAX_DONORS) return false;
        if (!isValidBloodType(bloodType)) return false;
        if (!isEmailValid(email)) return false;
        if (!isPhoneValid(phone)) return false;

        Donor newDonor;
        newDonor.id = donorCount + 1;
        strcpy(newDonor.firstName, firstName);
        strcpy(newDonor.lastName, lastName);
        strcpy(newDonor.bloodType, bloodType);
        strcpy(newDonor.phone, phone);
        strcpy(newDonor.email, email);
        strcpy(newDonor.address, address);
        newDonor.lastDonationDate = {0, 0, 0};
        newDonor.isActive = true;

        donors[donorCount++] = newDonor;
        return true;
    }

    bool updateDonor(int id, const char* firstName, const char* lastName, const char* bloodType,
                     const char* phone, const char* email, const char* address) {
        int donorIndex = findDonorById(id);
        if (donorIndex == -1) return false;
        if (!isValidBloodType(bloodType)) return false;
        if (!isEmailValid(email)) return false;
        if (!isPhoneValid(phone)) return false;

        strcpy(donors[donorIndex].firstName, firstName);
        strcpy(donors[donorIndex].lastName, lastName);
        strcpy(donors[donorIndex].bloodType, bloodType);
        strcpy(donors[donorIndex].phone, phone);
        strcpy(donors[donorIndex].email, email);
        strcpy(donors[donorIndex].address, address);

        return true;
    }

    bool deleteDonor(int id) {
        int donorIndex = findDonorById(id);
        if (donorIndex == -1) return false;

        donors[donorIndex].isActive = false;
        return true;
    }

    void displayDonor(int id) {
        int donorIndex = findDonorById(id);
        if (donorIndex == -1) {
            cout << "Donor not found." << endl;
            return;
        }

        Donor d = donors[donorIndex];
        cout << "ID: " << d.id << endl;
        cout << "Name: " << d.firstName << " " << d.lastName << endl;
        cout << "Blood Type: " << d.bloodType << endl;
        cout << "Phone: " << d.phone << endl;
        cout << "Email: " << d.email << endl;
        cout << "Address: " << d.address << endl;
        cout << "Last Donation Date: ";
        if (d.lastDonationDate.day == 0) {
            cout << "Never donated";
        } else {
            displayDate(d.lastDonationDate);
        }
        cout << endl;
        cout << "Status: " << (d.isActive ? "Active" : "Inactive") << endl;
    }

    void displayAllDonors() {
        if (donorCount == 0) {
            cout << "No donors registered." << endl;
            return;
        }

        for (int i = 0; i < donorCount; i++) {
            cout << "ID: " << donors[i].id << " | Name: " << donors[i].firstName << " " << donors[i].lastName
                 << " | Blood Type: " << donors[i].bloodType << " | Status: "
                 << (donors[i].isActive ? "Active" : "Inactive") << endl;
        }
    }

    bool addBloodUnit(int donorId, const char* bloodType, Date donationDate) {
        if (bloodUnitCount >= MAX_BLOOD_UNITS) return false;
        if (!isValidBloodType(bloodType)) return false;
        if (!isDateValid(donationDate)) return false;

        int donorIndex = findDonorById(donorId);
        if (donorIndex == -1) return false;

        BloodUnit newUnit;
        newUnit.id = bloodUnitCount + 1;
        newUnit.donorId = donorId;
        strcpy(newUnit.bloodType, bloodType);
        newUnit.donationDate = donationDate;
        newUnit.expiryDate = calculateExpiryDate(donationDate);
        newUnit.isAvailable = true;

        bloodUnits[bloodUnitCount++] = newUnit;

        donors[donorIndex].lastDonationDate = donationDate;

        return true;
    }

    bool removeBloodUnit(int id) {
        int unitIndex = findBloodUnitById(id);
        if (unitIndex == -1) return false;

        for (int i = unitIndex; i < bloodUnitCount - 1; i++) {
            bloodUnits[i] = bloodUnits[i + 1];
        }
        bloodUnitCount--;
        return true;
    }

    void displayBloodUnit(int id) {
        int unitIndex = findBloodUnitById(id);
        if (unitIndex == -1) {
            cout << "Blood unit not found." << endl;
            return;
        }

        BloodUnit unit = bloodUnits[unitIndex];
        int donorIndex = findDonorById(unit.donorId);

        cout << "Blood Unit ID: " << unit.id << endl;
        cout << "Blood Type: " << unit.bloodType << endl;
        cout << "Donation Date: ";
        displayDate(unit.donationDate);
        cout << endl;
        cout << "Expiry Date: ";
        displayDate(unit.expiryDate);
        cout << endl;
        cout << "Status: " << (unit.isAvailable ? "Available" : "Used") << endl;

        if (donorIndex != -1) {
            cout << "Donor: " << donors[donorIndex].firstName << " " << donors[donorIndex].lastName
                 << " (ID: " << donors[donorIndex].id << ")" << endl;
        }
    }

    void displayAllBloodUnits() {
        if (bloodUnitCount == 0) {
            cout << "No blood units in inventory." << endl;
            return;
        }

        for (int i = 0; i < bloodUnitCount; i++) {
            cout << "ID: " << bloodUnits[i].id << " | Blood Type: " << bloodUnits[i].bloodType
                 << " | Donation Date: ";
            displayDate(bloodUnits[i].donationDate);
            cout << " | Expiry Date: ";
            displayDate(bloodUnits[i].expiryDate);
            cout << " | Status: " << (bloodUnits[i].isAvailable ? "Available" : "Used") << endl;
        }
    }

    void displayBloodStock() {
        int stock[BLOOD_TYPE_COUNT] = {0};
        Date currentDate = getCurrentDate();

        for (int i = 0; i < bloodUnitCount; i++) {
            if (bloodUnits[i].isAvailable && isDateBefore(currentDate, bloodUnits[i].expiryDate)) {
                int typeIndex = getBloodTypeIndex(bloodUnits[i].bloodType);
                if (typeIndex != -1) {
                    stock[typeIndex]++;
                }
            }
        }

        cout << "Current Blood Stock:" << endl;
        for (int i = 0; i < BLOOD_TYPE_COUNT; i++) {
            cout << BLOOD_TYPES[i] << ": " << stock[i] << " units" << endl;
        }
    }

    void removeExpiredBloodUnits() {
        Date currentDate = getCurrentDate
        ();
        int removedCount = 0;

        for (int i = 0; i < bloodUnitCount; i++) {
            if (!isDateBefore(currentDate, bloodUnits[i].expiryDate)) {
                for (int j = i; j < bloodUnitCount - 1; j++) {
                    bloodUnits[j] = bloodUnits[j + 1];
                }
                bloodUnitCount--;
                i--;
                removedCount++;
            }
        }

        cout << "Removed " << removedCount << " expired blood units." << endl;
    }

    bool addRequest(int requesterId, const char* bloodType, int quantity,
                    const char* hospitalName, const char* patientName) {
        if (requestCount >= MAX_REQUESTS) return false;
        if (!isValidBloodType(bloodType)) return false;
        if (quantity <= 0) return false;

        Request newRequest;
        newRequest.id = requestCount + 1;
        newRequest.requesterId = requesterId;
        strcpy(newRequest.bloodType, bloodType);
        newRequest.quantity = quantity;
        newRequest.requestDate = getCurrentDate();
        strcpy(newRequest.status, "Pending");
        strcpy(newRequest.hospitalName, hospitalName);
        strcpy(newRequest.patientName, patientName);

        requests[requestCount++] = newRequest;
        return true;
    }

    bool updateRequestStatus(int id, const char* status) {
        int requestIndex = findRequestById(id);
        if (requestIndex == -1) return false;

        strcpy(requests[requestIndex].status, status);
        return true;
    }

    bool fulfillRequest(int id) {
        int requestIndex = findRequestById(id);
        if (requestIndex == -1) return false;

        Request& req = requests[requestIndex];
        if (strcmp(req.status, "Pending") != 0) return false;

        int availableCount = 0;
        Date currentDate = getCurrentDate();

        for (int i = 0; i < bloodUnitCount; i++) {
            if (strcmp(bloodUnits[i].bloodType, req.bloodType) == 0 &&
                bloodUnits[i].isAvailable &&
                isDateBefore(currentDate, bloodUnits[i].expiryDate)) {
                availableCount++;
            }
        }

        if (availableCount < req.quantity) {
            cout << "Not enough blood units available. Only " << availableCount << " units of "
                 << req.bloodType << " available." << endl;
            return false;
        }

        int unitsFulfilled = 0;
        for (int i = 0; i < bloodUnitCount && unitsFulfilled < req.quantity; i++) {
            if (strcmp(bloodUnits[i].bloodType, req.bloodType) == 0 &&
                bloodUnits[i].isAvailable &&
                isDateBefore(currentDate, bloodUnits[i].expiryDate)) {
                bloodUnits[i].isAvailable = false;
                unitsFulfilled++;
            }
        }

        strcpy(req.status, "Fulfilled");
        cout << "Request fulfilled successfully. " << unitsFulfilled << " units of "
             << req.bloodType << " used." << endl;
        return true;
    }

    bool cancelRequest(int id) {
        int requestIndex = findRequestById(id);
        if (requestIndex == -1) return false;

        if (strcmp(requests[requestIndex].status, "Pending") != 0) {
            cout << "Only pending requests can be cancelled." << endl;
            return false;
        }

        strcpy(requests[requestIndex].status, "Cancelled");
        return true;
    }

    void displayRequest(int id) {
        int requestIndex = findRequestById(id);
        if (requestIndex == -1) {
            cout << "Request not found." << endl;
            return;
        }

        Request req = requests[requestIndex];
        int requesterIndex = findUserById(req.requesterId);

        cout << "Request ID: " << req.id << endl;
        cout << "Blood Type: " << req.bloodType << endl;
        cout << "Quantity: " << req.quantity << endl;
        cout << "Request Date: ";
        displayDate(req.requestDate);
        cout << endl;
        cout << "Status: " << req.status << endl;
        cout << "Hospital: " << req.hospitalName << endl;
        cout << "Patient: " << req.patientName << endl;

        if (requesterIndex != -1) {
            cout << "Requestor: " << users[requesterIndex].username << endl;
        }
    }

    void displayAllRequests() {
        if (requestCount == 0) {
            cout << "No requests found." << endl;
            return;
        }

        for (int i = 0; i < requestCount; i++) {
            cout << "ID: " << requests[i].id << " | Blood Type: " << requests[i].bloodType
                 << " | Quantity: " << requests[i].quantity << " | Status: " << requests[i].status
                 << " | Request Date: ";
            displayDate(requests[i].requestDate);
            cout << endl;
        }
    }

    void adminMenu() {
        int choice;
        do {
            cout << "\n===== ADMIN MENU =====" << endl;
            cout << "1. Manage Donors" << endl;
            cout << "2. Manage Blood Inventory" << endl;
            cout << "3. Manage Requests" << endl;
            cout << "4. View Reports" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: donorManagementMenu(); break;
                case 2: bloodInventoryMenu(); break;
                case 3: requestManagementMenu(); break;
                case 4: reportsMenu(); break;
                case 5: logout(); break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (currentUserId != -1);
    }

    void donorManagementMenu() {
        int choice;
        do {
            cout << "\n===== DONOR MANAGEMENT =====" << endl;
            cout << "1. Add Donor" << endl;
            cout << "2. Update Donor" << endl;
            cout << "3. Delete Donor" << endl;
            cout << "4. View Donor Details" << endl;
            cout << "5. List All Donors" << endl;
            cout << "6. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    char firstName[MAX_NAME_LENGTH], lastName[MAX_NAME_LENGTH], bloodType[3];
                    char phone[MAX_PHONE_LENGTH], email[MAX_EMAIL_LENGTH], address[MAX_ADDRESS_LENGTH];

                    cout << "Enter first name: ";
                    cin.ignore();
                    cin.getline(firstName, MAX_NAME_LENGTH);
                    cout << "Enter last name: ";
                    cin.getline(lastName, MAX_NAME_LENGTH);
                    cout << "Enter blood type: ";
                    cin >> bloodType;
                    cout << "Enter phone: ";
                    cin.ignore();
                    cin.getline(phone, MAX_PHONE_LENGTH);
                    cout << "Enter email: ";
                    cin.getline(email, MAX_EMAIL_LENGTH);
                    cout << "Enter address: ";
                    cin.getline(address, MAX_ADDRESS_LENGTH);

                    if (addDonor(firstName, lastName, bloodType, phone, email, address)) {
                        cout << "Donor added successfully." << endl;
                    } else {
                        cout << "Failed to add donor. Invalid data or maximum donors reached." << endl;
                    }
                    break;
                }
                case 2: {
                    int id;
                    char firstName[MAX_NAME_LENGTH], lastName[MAX_NAME_LENGTH], bloodType[3];
                    char phone[MAX_PHONE_LENGTH], email[MAX_EMAIL_LENGTH], address[MAX_ADDRESS_LENGTH];

                    cout << "Enter donor ID to update: ";
                    cin >> id;
                    cout << "Enter new first name: ";
                    cin.ignore();
                    cin.getline(firstName, MAX_NAME_LENGTH);
                    cout << "Enter new last name: ";
                    cin.getline(lastName, MAX_NAME_LENGTH);
                    cout << "Enter new blood type: ";
                    cin >> bloodType;
                    cout << "Enter new phone: ";
                    cin.ignore();
                    cin.getline(phone, MAX_PHONE_LENGTH);
                    cout << "Enter new email: ";
                    cin.getline(email, MAX_EMAIL_LENGTH);
                    cout << "Enter new address: ";
                    cin.getline(address, MAX_ADDRESS_LENGTH);

                    if (updateDonor(id, firstName, lastName, bloodType, phone, email, address)) {
                        cout << "Donor updated successfully." << endl;
                    } else {
                        cout << "Failed to update donor. Invalid data or donor not found." << endl;
                    }
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter donor ID to delete: ";
                    cin >> id;

                    if (deleteDonor(id)) {
                        cout << "Donor marked as inactive." << endl;
                    } else {
                        cout << "Failed to delete donor. Donor not found." << endl;
                    }
                    break;
                }
                case 4: {
                    int id;
                    cout << "Enter donor ID to view: ";
                    cin >> id;
                    displayDonor(id);
                    break;
                }
                case 5: {
                    displayAllDonors();
                    break;
                }
                case 6: {
                    return;
                }
                default: {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        } while (true);
    }

    void bloodInventoryMenu() {
        int choice;
        do {
            cout << "\n===== BLOOD INVENTORY MANAGEMENT =====" << endl;
            cout << "1. Add Blood Unit" << endl;
            cout << "2. Remove Blood Unit" << endl;
            cout << "3. View Blood Unit Details" << endl;
            cout << "4. List All Blood Units" << endl;
            cout << "5. View Blood Stock" << endl;
            cout << "6. Remove Expired Blood Units" << endl;
            cout << "7. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int donorId, day, month, year;
                    char bloodType[3];

                    cout << "Enter donor ID: ";
                    cin >> donorId;
                    cout << "Enter blood type: ";
                    cin >> bloodType;
                    cout << "Enter donation date (DD MM YYYY): ";
                    cin >> day >> month >> year;

                    Date donationDate = {day, month, year};
                    if (!isDateValid(donationDate)) {
                        cout << "Invalid date." << endl;
                        break;
                    }

                    if (addBloodUnit(donorId, bloodType, donationDate)) {
                        cout << "Blood unit added successfully." << endl;
                    } else {
                        cout << "Failed to add blood unit. Invalid data or maximum units reached." << endl;
                    }
                    break;
                }
                case 2: {
                    int id;
                    cout << "Enter blood unit ID to remove: ";
                    cin >> id;

                    if (removeBloodUnit(id)) {
                        cout << "Blood unit removed successfully." << endl;
                    } else {
                        cout << "Failed to remove blood unit. Unit not found." << endl;
                    }
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter blood unit ID to view: ";
                    cin >> id;
                    displayBloodUnit(id);
                    break;
                }
                case 4: {
                    displayAllBloodUnits();
                    break;
                }
                case 5: {
                    displayBloodStock();
                    break;
                }
                case 6: {
                    removeExpiredBloodUnits();
                    break;
                }
                case 7: {
                    return;
                }
                default: {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        } while (true);
    }

    void requestManagementMenu() {
        int choice;
        do {
            cout << "\n===== REQUEST MANAGEMENT =====" << endl;
            cout << "1. Add Request" << endl;
            cout << "2. Fulfill Request" << endl;
            cout << "3. Cancel Request" << endl;
            cout << "4. View Request Details" << endl;
            cout << "5. List All Requests" << endl;
            cout << "6. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    char bloodType[3], hospitalName[MAX_NAME_LENGTH], patientName[MAX_NAME_LENGTH];
                    int quantity;

                    cout << "Enter blood type: ";
                    cin >> bloodType;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    cout << "Enter hospital name: ";
                    cin.ignore();
                    cin.getline(hospitalName, MAX_NAME_LENGTH);
                    cout << "Enter patient name: ";
                    cin.getline(patientName, MAX_NAME_LENGTH);

                    if (addRequest(currentUserId, bloodType, quantity, hospitalName, patientName)) {
                        cout << "Request added successfully." << endl;
                    } else {
                        cout << "Failed to add request. Invalid data or maximum requests reached." << endl;
                    }
                    break;
                }
                case 2: {
                    int id;
                    cout << "Enter request ID to fulfill: ";
                    cin >> id;

                    if (fulfillRequest(id)) {
                        cout << "Request fulfilled successfully." << endl;
                    } else {
                        cout << "Failed to fulfill request." << endl;
                    }
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter request ID to cancel: ";
                    cin >> id;

                    if (cancelRequest(id)) {
                        cout << "Request cancelled successfully." << endl;
                    } else {
                        cout << "Failed to cancel request." << endl;
                    }
                    break;
                }
                case 4: {
                    int id;
                    cout << "Enter request ID to view: ";
                    cin >> id;
                    displayRequest(id);
                    break;
                }
                case 5: {
                    displayAllRequests();
                    break;
                }
                case 6: {
                    return;
                }
                default: {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        } while (true);
    }

    void reportsMenu() {
        int choice;
        do {
            cout << "\n===== REPORTS =====" << endl;
            cout << "1. Blood Stock Report" << endl;
            cout << "2. Donor List Report" << endl;
            cout << "3. Blood Unit List Report" << endl;
            cout << "4. Request List Report" << endl;
            cout << "5. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    displayBloodStock();
                    break;
                }
                case 2: {
                    displayAllDonors();
                    break;
                }
                case 3: {
                    displayAllBloodUnits();
                    break;
                }
                case 4: {
                    displayAllRequests();
                    break;
                }
                case 5: {
                    return;
                }
                default: {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        } while (true);
    }

    void requestorMenu() {
        int choice;
        do {
            cout << "\n===== REQUESTOR MENU =====" << endl;
            cout << "1. Add Request" << endl;
            cout << "2. View My Requests" << endl;
            cout << "3. Back to Main Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    char bloodType[3], hospitalName[MAX_NAME_LENGTH], patientName[MAX_NAME_LENGTH];
                    int quantity;

                    cout << "Enter blood type: ";
                    cin >> bloodType;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    cout << "Enter hospital name: ";
                    cin.ignore();
                    cin.getline(hospitalName, MAX_NAME_LENGTH);
                    cout << "Enter patient name: ";
                    cin.getline(patientName, MAX_NAME_LENGTH);

                    if (addRequest(currentUserId, bloodType, quantity, hospitalName, patientName)) {
                        cout << "Request added successfully." << endl;
                        } else {
                        cout << "Failed to add request. Invalid data or maximum requests reached." << endl;
                    }
                    break;
                }
                case 2: {
                    cout << "\n===== YOUR REQUESTS =====" << endl;
                    bool found = false;
                    for (int i = 0; i < requestCount; i++) {
                        if (requests[i].requesterId == currentUserId) {
                            displayRequest(requests[i].id);
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "No requests found." << endl;
                    }
                    break;
                }
                case 3: {
                    return;
                }
                default: {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        } while (true);
    }

    void donorMenu() {
        cout << "\n===== DONOR MENU =====" << endl;
        cout << "Welcome, Donor (Functionality not fully implemented in this version)." << endl;
        cout << "1. View Your Donation History (Not Implemented)" << endl;
        cout << "2. Update Your Contact Information (Not Implemented)" << endl;
        cout << "3. Back to Main Menu" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: cout << "Functionality not yet implemented." << endl; break;
            case 2: cout << "Functionality not yet implemented." << endl; break;
            case 3: return;
            default: cout << "Invalid choice." << endl;
        }
    }

    void mainMenu() {
        int choice;
        char username[MAX_NAME_LENGTH];
        char password[MAX_NAME_LENGTH];
        char role[20];

        do {
            cout << "\n===== BLOOD BANK SYSTEM =====" << endl;
            if (currentUserId == -1) {
                cout << "1. Login" << endl;
                cout << "2. Register as Requestor" << endl;
                cout << "3. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        cout << "Enter username: ";
                        cin >> username;
                        cout << "Enter password: ";
                        cin >> password;
                        if (login(username, password)) {
                            int userIndex = findUserById(currentUserId);
                            if (userIndex != -1) {
                                cout << "Login successful. Welcome, " << users[userIndex].username << " (" << users[userIndex].role << ")" << endl;
                                if (isAdmin()) adminMenu();
                                else if (isRequestor()) requestorMenu();
                                else if (isDonor()) donorMenu();
                            }
                        } else {
                            cout << "Login failed. Invalid username or password." << endl;
                        }
                        break;
                    case 2:
                        cout << "Enter username: ";
                        cin >> username;
                        cout << "Enter password: ";
                        cin >> password;
                        if (registerUser(username, password, "Requestor")) {
                            cout << "Registration successful." << endl;
                        } else {
                            cout << "Registration failed. Username already exists or maximum users reached." << endl;
                        }
                        break;
                    case 3:
                        cout << "Exiting system." << endl;
                        break;
                    default:
                        cout << "Invalid choice. Try again." << endl;
                }
            } else {
                int userIndex = findUserById(currentUserId);
                if (userIndex != -1) {
                    cout << "Logged in as: " << users[userIndex].username << " (" << users[userIndex].role << ")" << endl;
                    if (isAdmin()) adminMenu();
                    else if (isRequestor()) requestorMenu();
                    else if (isDonor()) donorMenu();
                } else {
                    cout << "Error: Current user ID invalid. Logging out." << endl;
                    logout();
                }
            }
        } while (currentUserId != -1 || choice != 3);
    }
};

int main() {
    BloodBankSystem bms;
    bms.mainMenu();
    return 0;
}