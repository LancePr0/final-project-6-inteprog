#define _CRT_SECURE_NO_WARNINGS // To suppress warnings for functions like strcpy, scanf in Visual Studio

#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers like unique_ptr
#include <algorithm> // For std::find_if
#include <limits> // For numeric_limits
#include <stdexcept> // For standard exceptions
#include <regex> // For email validation
#include <ctime>   // For std::time and std::mktime
#include <iomanip> // For std::put_time

// Forward Declarations
class User;
class Admin;
class Staff;
class Donor;
class Requestor;
class DonorRecord;
class BloodInventoryItem;
class BloodRequest;
class IValidationStrategy;
class BloodBankManagementSystem;

// --- 1. OOP Classes (Models) ---

/**
 * @brief Abstract base class for all users in the system.
 * Implements Abstraction and Encapsulation.
 */
class User {
protected:
    std::string id;
    std::string name;
    std::string email;
    std::string password; // In a real app, this would be hashed!
    std::string role;

public:
    // Constructor
    User(const std::string& id, const std::string& name, const std::string& email, const std::string& password, const std::string& role)
        : id(id), name(name), email(email), password(password), role(role) {
        if (role.empty()) {
            throw std::invalid_argument("User role cannot be empty.");
        }
    }

    // Virtual Destructor
    virtual ~User() = default;

    // Getters (Encapsulation)
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPassword() const { return password; } // For demo, in real app, never expose
    std::string getRole() const { return role; }

    // Setters (Encapsulation with potential validation)
    void setName(const std::string& newName) {
        if (newName.empty() || newName.find_first_not_of(' ') == std::string::npos) {
            throw std::invalid_argument("Name cannot be empty.");
        }
        name = newName;
    }
    void setEmail(const std::string& newEmail) {
        // Simple email regex for demonstration
        const std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (!std::regex_match(newEmail, email_regex)) {
            throw std::invalid_argument("Invalid email format.");
        }
        email = newEmail;
    }
    void setPassword(const std::string& newPassword) {
        // In a real app, hash and salt here
        if (newPassword.length() < 6) {
            throw std::invalid_argument("Password must be at least 6 characters.");
        }
        password = newPassword;
    }
    // Role should generally not be set after creation in a typical system,
    // but for completeness, if it were, validation would be here.

    // Polymorphic method
    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Name: " << name << ", Email: " << email << ", Role: " << role << std::endl;
    }

    // Static method to generate unique IDs
    static std::string generateUniqueId() {
        static int counter = 0;
        return "USR" + std::to_string(++counter) + std::to_string(std::time(0));
    }
};

/**
 * @brief Admin user class, inherits from User.
 * Implements Inheritance and Polymorphism.
 */
class Admin : public User {
public:
    Admin(const std::string& id, const std::string& name, const std::string& email, const std::string& password)
        : User(id, name, email, password, "admin") {}

    // Admin-specific methods
    void generateReport() const {
        std::cout << "Admin " << name << " is generating a comprehensive report." << std::endl;
        // In a real system, this would interact with DatabaseService to fetch data
    }

    void manageUsers() const {
        std::cout << "Admin " << name << " is managing user accounts." << std::endl;
        // Functionality for adding/deleting/modifying other users
    }

    void displayInfo() const override {
        std::cout << "Admin - ";
        User::displayInfo();
    }
};

/**
 * @brief Staff user class, inherits from User.
 * Implements Inheritance and Polymorphism.
 */
class Staff : public User {
public:
    Staff(const std::string& id, const std::string& name, const std::string& email, const std::string& password)
        : User(id, name, email, password, "staff") {}

    // Staff-specific methods
    void manageDonors() const {
        std::cout << "Staff " << name << " is managing donor records." << std::endl;
    }

    void manageInventory() const {
        std::cout << "Staff " << name << " is managing blood inventory." << std::endl;
    }

    void processRequests() const {
        std::cout << "Staff " << name << " is processing blood requests." << std::endl;
    }

    void displayInfo() const override {
        std::cout << "Staff - ";
        User::displayInfo();
    }
};

/**
 * @brief Donor user class, inherits from User.
 * Implements Inheritance and Polymorphism.
 */
class Donor : public User {
private:
    std::string contactInfo;
    std::string bloodType;

public:
    Donor(const std::string& id, const std::string& name, const std::string& email, const std::string& password,
          const std::string& contactInfo, const std::string& bloodType)
        : User(id, name, email, password, "donor"), contactInfo(contactInfo), bloodType(bloodType) {}

    std::string getContactInfo() const { return contactInfo; }
    std::string getBloodType() const { return bloodType; }

    void setContactInfo(const std::string& info) {
        if (info.empty() || info.find_first_not_of(' ') == std::string::npos) {
            throw std::invalid_argument("Contact info cannot be empty.");
        }
        contactInfo = info;
    }
    void setBloodType(const std::string& type) {
        const std::vector<std::string> validBloodTypes = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        if (std::find(validBloodTypes.begin(), validBloodTypes.end(), type) == validBloodTypes.end()) {
            throw std::invalid_argument("Invalid blood type. Must be one of: A+, A-, B+, B-, AB+, AB-, O+, O-.");
        }
        bloodType = type;
    }

    void viewDonationHistory() const {
        std::cout << "Donor " << name << " is viewing their donation history." << std::endl;
    }

    void inquireToGiveBlood() const {
        std::cout << "Donor " << name << " is inquiring to give blood." << std::endl;
    }

    void displayInfo() const override {
        std::cout << "Donor - ";
        User::displayInfo();
        std::cout << "  Contact: " << contactInfo << ", Blood Type: " << bloodType << std::endl;
    }
};

/**
 * @brief Requestor (HospitalUser) user class, inherits from User.
 * Implements Inheritance and Polymorphism.
 */
class Requestor : public User {
private:
    std::string hospitalName;

public:
    Requestor(const std::string& id, const std::string& name, const std::string& email, const std::string& password,
              const std::string& hospitalName)
        : User(id, name, email, password, "requestor"), hospitalName(hospitalName) {}

    std::string getHospitalName() const { return hospitalName; }
    void setHospitalName(const std::string& name) {
        if (name.empty() || name.find_first_not_of(' ') == std::string::npos) {
            throw std::invalid_argument("Hospital name cannot be empty.");
        }
        hospitalName = name;
    }

    void requestBlood() const {
        std::cout << "Requestor " << name << " from " << hospitalName << " is requesting blood." << std::endl;
    }

    void cancelRequest() const {
        std::cout << "Requestor " << name << " from " << hospitalName << " is cancelling a request." << std::endl;
    }

    void viewStatus() const {
        std::cout << "Requestor " << name << " from " << hospitalName << " is viewing request status." << std::endl;
    }

    void displayInfo() const override {
        std::cout << "Requestor - ";
        User::displayInfo();
        std::cout << "  Hospital: " << hospitalName << std::endl;
    }
};

/**
 * @brief Represents a donor record.
 * Implements Encapsulation.
 */
class DonorRecord {
private:
    std::string id;
    std::string name;
    std::string contact;
    std::string bloodType;
    std::string dateRegistered; // Store as YYYY-MM-DD string for simplicity

public:
    DonorRecord(const std::string& id, const std::string& name, const std::string& contact,
                const std::string& bloodType, const std::string& dateRegistered)
        : id(id), name(name), contact(contact), bloodType(bloodType), dateRegistered(dateRegistered) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getContact() const { return contact; }
    std::string getBloodType() const { return bloodType; }
    std::string getDateRegistered() const { return dateRegistered; }

    void setName(const std::string& newName) {
        if (newName.empty() || newName.find_first_not_of(' ') == std::string::npos) throw std::invalid_argument("Donor name cannot be empty.");
        name = newName;
    }
    void setContact(const std::string& newContact) {
        if (newContact.empty() || newContact.find_first_not_of(' ') == std::string::npos) throw std::invalid_argument("Donor contact cannot be empty.");
        contact = newContact;
    }
    void setBloodType(const std::string& newType) {
        const std::vector<std::string> validBloodTypes = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        if (std::find(validBloodTypes.begin(), validBloodTypes.end(), newType) == validBloodTypes.end()) {
            throw std::invalid_argument("Invalid blood type. Must be one of: A+, A-, B+, B-, AB+, AB-, O+, O-.");
        }
        bloodType = newType;
    }
    void setDateRegistered(const std::string& newDate) {
        // Simple date format validation YYYY-MM-DD
        if (!std::regex_match(newDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            throw std::invalid_argument("Invalid date format. Use YYYY-MM-DD.");
        }
        dateRegistered = newDate;
    }

    void display() const {
        std::cout << "  ID: " << id << ", Name: " << name << ", Contact: " << contact
                  << ", Blood Type: " << bloodType << ", Registered: " << dateRegistered << std::endl;
    }

    static std::string generateUniqueId() {
        static int counter = 0;
        return "DR" + std::to_string(++counter) + std::to_string(std::time(0));
    }
};

/**
 * @brief Represents a blood inventory item.
 * Implements Encapsulation.
 */
class BloodInventoryItem {
private:
    std::string id;
    std::string bloodType;
    int quantity; // In mL or units
    std::string dateDonated;    // YYYY-MM-DD
    std::string expirationDate; // YYYY-MM-DD

public:
    BloodInventoryItem(const std::string& id, const std::string& bloodType, int quantity,
                       const std::string& dateDonated, const std::string& expirationDate)
        : id(id), bloodType(bloodType), quantity(quantity), dateDonated(dateDonated), expirationDate(expirationDate) {}

    std::string getId() const { return id; }
    std::string getBloodType() const { return bloodType; }
    int getQuantity() const { return quantity; }
    std::string getDateDonated() const { return dateDonated; }
    std::string getExpirationDate() const { return expirationDate; }

    void setQuantity(int newQuantity) {
        if (newQuantity < 0) throw std::invalid_argument("Quantity cannot be negative.");
        quantity = newQuantity;
    }
    void setExpirationDate(const std::string& newDate) {
        if (!std::regex_match(newDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            throw std::invalid_argument("Invalid date format. Use YYYY-MM-DD.");
        }
        expirationDate = newDate;
    }
     void setBloodType(const std::string& newType) {
        const std::vector<std::string> validBloodTypes = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        if (std::find(validBloodTypes.begin(), validBloodTypes.end(), newType) == validBloodTypes.end()) {
            throw std::invalid_argument("Invalid blood type. Must be one of: A+, A-, B+, B-, AB+, AB-, O+, O-.");
        }
        bloodType = newType;
    }
     void setDateDonated(const std::string& newDate) {
        if (!std::regex_match(newDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            throw std::invalid_argument("Invalid date format. Use YYYY-MM-DD.");
        }
        dateDonated = newDate;
    }


    void display() const {
        std::cout << "  ID: " << id << ", Type: " << bloodType << ", Quantity: " << quantity
                  << ", Donated: " << dateDonated << ", Expires: " << expirationDate << std::endl;
    }

    static std::string generateUniqueId() {
        static int counter = 0;
        return "BI" + std::to_string(++counter) + std::to_string(std::time(0));
    }
};

/**
 * @brief Represents a blood request.
 * Implements Encapsulation.
 */
class BloodRequest {
private:
    std::string id;
    std::string requestorId;
    std::string requestorName;
    std::string bloodType;
    int quantity;
    std::string status; // e.g., 'pending', 'fulfilled', 'cancelled'
    std::string dateRequested; // YYYY-MM-DD

public:
    BloodRequest(const std::string& id, const std::string& requestorId, const std::string& requestorName,
                 const std::string& bloodType, int quantity, const std::string& status,
                 const std::string& dateRequested)
        : id(id), requestorId(requestorId), requestorName(requestorName), bloodType(bloodType),
          quantity(quantity), status(status), dateRequested(dateRequested) {}

    std::string getId() const { return id; }
    std::string getRequestorId() const { return requestorId; }
    std::string getRequestorName() const { return requestorName; }
    std::string getBloodType() const { return bloodType; }
    int getQuantity() const { return quantity; }
    std::string getStatus() const { return status; }
    std::string getDateRequested() const { return dateRequested; }

    void setQuantity(int newQuantity) {
        if (newQuantity < 0) throw std::invalid_argument("Quantity cannot be negative.");
        quantity = newQuantity;
    }
    void setStatus(const std::string& newStatus) {
        const std::vector<std::string> validStatuses = {"pending", "fulfilled", "cancelled"};
        if (std::find(validStatuses.begin(), validStatuses.end(), newStatus) == validStatuses.end()) {
            throw std::invalid_argument("Invalid status. Must be pending, fulfilled, or cancelled.");
        }
        status = newStatus;
    }
    void setBloodType(const std::string& newType) {
        const std::vector<std::string> validBloodTypes = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        if (std::find(validBloodTypes.begin(), validBloodTypes.end(), newType) == validBloodTypes.end()) {
            throw std::invalid_argument("Invalid blood type. Must be one of: A+, A-, B+, B-, AB+, AB-, O+, O-.");
        }
        bloodType = newType;
    }
    void setDateRequested(const std::string& newDate) {
        if (!std::regex_match(newDate, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            throw std::invalid_argument("Invalid date format. Use YYYY-MM-DD.");
        }
        dateRequested = newDate;
    }

    void display() const {
        std::cout << "  ID: " << id << ", Requestor: " << requestorName << ", Type: " << bloodType
                  << ", Quantity: " << quantity << ", Status: " << status << ", Date: " << dateRequested << std::endl;
    }

    static std::string generateUniqueId() {
        static int counter = 0;
        return "BR" + std::to_string(++counter) + std::to_string(std::time(0));
    }
};

// --- 2. Strategy Pattern for Validation ---

/**
 * @brief Interface for validation strategies.
 * Implements Abstraction.
 */
class IValidationStrategy {
public:
    virtual bool validate(const std::string& value) const = 0;
    virtual std::string getErrorMessage(const std::string& fieldName) const = 0;
    virtual ~IValidationStrategy() = default;
};

/**
 * @brief Concrete strategy for email validation.
 */
class EmailValidationStrategy : public IValidationStrategy {
public:
    bool validate(const std::string& email) const override {
        const std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, email_regex);
    }
    std::string getErrorMessage(const std::string& fieldName) const override {
        return "Invalid " + fieldName + " format. Please use a valid email address.";
    }
};

/**
 * @brief Concrete strategy for password validation.
 */
class PasswordValidationStrategy : public IValidationStrategy {
public:
    bool validate(const std::string& password) const override {
        return password.length() >= 6; // Minimum 6 characters
    }
    std::string getErrorMessage(const std::string& fieldName) const override {
        return fieldName + " must be at least 6 characters long.";
    }
};

/**
 * @brief Concrete strategy for non-empty string validation.
 */
class NonEmptyStringValidationStrategy : public IValidationStrategy {
public:
    bool validate(const std::string& str) const override {
        return !str.empty() && str.find_first_not_of(' ') != std::string::npos;
    }
    std::string getErrorMessage(const std::string& fieldName) const override {
        return fieldName + " cannot be empty.";
    }
};

/**
 * @brief Concrete strategy for positive integer validation.
 */
class PositiveIntegerValidationStrategy : public IValidationStrategy {
public:
    bool validate(const std::string& s) const override {
        if (s.empty()) return false;
        for (char c : s) {
            if (!std::isdigit(c)) return false;
        }
        try {
            int num = std::stoi(s);
            return num > 0;
        } catch (const std::out_of_range& oor) {
            return false; // Number too large/small for int
        }
    }
    std::string getErrorMessage(const std::string& fieldName) const override {
        return fieldName + " must be a positive number.";
    }
};

/**
 * @brief Concrete strategy for blood type validation.
 */
class BloodTypeValidationStrategy : public IValidationStrategy {
public:
    bool validate(const std::string& bloodType) const override {
        const std::vector<std::string> validBloodTypes = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
        return std::find(validBloodTypes.begin(), validBloodTypes.end(), bloodType) != validBloodTypes.end();
    }
    std::string getErrorMessage(const std::string& fieldName) const override {
        return "Invalid " + fieldName + ". Must be one of: A+, A-, B+, B-, AB+, AB-, O+, O-.";
    }
};

/**
 * @brief Concrete strategy for date validation (YYYY-MM-DD).
 */
class DateValidationStrategy : public IValidationStrategy {
public:
    bool validate(const std::string& date) const override {
        if (!std::regex_match(date, std::regex(R"(\d{4}-\d{2}-\d{2})"))) {
            return false;
        }
        // Basic check for month and day ranges. More robust validation would involve leap years etc.
        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));
        return (month >= 1 && month <= 12) && (day >= 1 && day <= 31);
    }
    std::string getErrorMessage(const std::string& fieldName) const override {
        return "Invalid " + fieldName + " format. Use YYYY-MM-DD.";
    }
};

/**
 * @brief Context for applying validation strategies.
 * Implements Strategy Pattern.
 */
class Validator {
private:
    std::unique_ptr<IValidationStrategy> strategy;

public:
    Validator(std::unique_ptr<IValidationStrategy> s) : strategy(std::move(s)) {
        if (!strategy) {
            throw std::invalid_argument("Validation strategy cannot be null.");
        }
    }

    void setStrategy(std::unique_ptr<IValidationStrategy> s) {
        if (!s) {
            throw std::invalid_argument("Validation strategy cannot be null.");
        }
        strategy = std::move(s);
    }

    bool validate(const std::string& value) const {
        return strategy->validate(value);
    }

    std::string getErrorMessage(const std::string& fieldName) const {
        return strategy->getErrorMessage(fieldName);
    }
};

// --- 3. Database (In-memory for this example) ---
// In a real application, this would interact with a database system (e.g., SQLite, MySQL).
// For demonstration, we use vectors.

class BloodBankDatabase {
public:
    std::vector<std::unique_ptr<User>> users;
    std::vector<DonorRecord> donors;
    std::vector<BloodInventoryItem> inventory;
    std::vector<BloodRequest> requests;

    // A simple way to check if an email already exists
    bool isEmailTaken(const std::string& email) const {
        for (const auto& user : users) {
            if (user->getEmail() == email) {
                return true;
            }
        }
        return false;
    }
};

// --- 4. Blood Bank Management System (Singleton Pattern) ---

/**
 * @brief Core system class, implementing the Singleton pattern.
 * Ensures only one instance of the system runs.
 */
class BloodBankManagementSystem {
private:
    static BloodBankManagementSystem* instance;
    BloodBankDatabase db;
    std::unique_ptr<User> loggedInUser;

    // Private constructor to prevent direct instantiation
    BloodBankManagementSystem() {
        // Initialize with a default admin for testing
        try {
            db.users.push_back(std::make_unique<Admin>("admin_id", "AdminUser", "admin@bbms.com", "admin123"));
            db.users.push_back(std::make_unique<Staff>("staff_id", "StaffUser", "staff@bbms.com", "staff123"));
        } catch (const std::exception& e) {
            std::cerr << "Initialization error: " << e.what() << std::endl;
        }
    }

    // Private copy constructor and assignment operator to prevent cloning
    BloodBankManagementSystem(const BloodBankManagementSystem&) = delete;
    BloodBankManagementSystem& operator=(const BloodBankManagementSystem&) = delete;

public:
    // Static method to get the single instance
    static BloodBankManagementSystem* getInstance() {
        if (instance == nullptr) {
            instance = new BloodBankManagementSystem();
        }
        return instance;
    }

    // Static method to clean up the instance
    static void destroyInstance() {
        delete instance;
        instance = nullptr;
    }

    // --- User Management ---

    std::unique_ptr<User>& getLoggedInUser() {
        return loggedInUser;
    }

    // Registration for Donor/Requestor
    bool registerUser(const std::string& name, const std::string& email, const std::string& password, const std::string& role,
                      const std::string& specificDetail1 = "", const std::string& specificDetail2 = "") {
        try {
            Validator emailValidator(std::make_unique<EmailValidationStrategy>());
            Validator passwordValidator(std::make_unique<PasswordValidationStrategy>());
            Validator nameValidator(std::make_unique<NonEmptyStringValidationStrategy>());

            if (!nameValidator.validate(name)) throw std::invalid_argument(nameValidator.getErrorMessage("Name"));
            if (!emailValidator.validate(email)) throw std::invalid_argument(emailValidator.getErrorMessage("Email"));
            if (!passwordValidator.validate(password)) throw std::invalid_argument(passwordValidator.getErrorMessage("Password"));

            if (db.isEmailTaken(email)) {
                throw std::runtime_error("User with this email already exists.");
            }

            std::string newId = User::generateUniqueId();
            if (role == "donor") {
                Validator contactValidator(std::make_unique<NonEmptyStringValidationStrategy>());
                Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());
                if (!contactValidator.validate(specificDetail1)) throw std::invalid_argument(contactValidator.getErrorMessage("Contact Info"));
                if (!bloodTypeValidator.validate(specificDetail2)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("Blood Type"));

                db.users.push_back(std::make_unique<Donor>(newId, name, email, password, specificDetail1, specificDetail2));
            } else if (role == "requestor") {
                Validator hospitalValidator(std::make_unique<NonEmptyStringValidationStrategy>());
                if (!hospitalValidator.validate(specificDetail1)) throw std::invalid_argument(hospitalValidator.getErrorMessage("Hospital Name"));

                db.users.push_back(std::make_unique<Requestor>(newId, name, email, password, specificDetail1));
            } else {
                throw std::invalid_argument("Invalid role for registration. Only 'donor' or 'requestor' are allowed.");
            }
            std::cout << "Successfully registered as a " << role << "!" << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Registration failed: " << e.what() << std::endl;
            return false;
        }
    }

    bool login(const std::string& email, const std::string& password) {
        try {
            Validator emailValidator(std::make_unique<EmailValidationStrategy>());
            Validator passwordValidator(std::make_unique<PasswordValidationStrategy>());

            if (!emailValidator.validate(email)) throw std::invalid_argument(emailValidator.getErrorMessage("Email"));
            if (!passwordValidator.validate(password)) throw std::invalid_argument(passwordValidator.getErrorMessage("Password"));

            for (const auto& user : db.users) {
                if (user->getEmail() == email && user->getPassword() == password) {
                    // Create a copy of the logged-in user, specific to their type
                    if (user->getRole() == "admin") {
                        loggedInUser = std::make_unique<Admin>(static_cast<const Admin&>(*user));
                    } else if (user->getRole() == "staff") {
                        loggedInUser = std::make_unique<Staff>(static_cast<const Staff&>(*user));
                    } else if (user->getRole() == "donor") {
                        loggedInUser = std::make_unique<Donor>(static_cast<const Donor&>(*user));
                    } else if (user->getRole() == "requestor") {
                        loggedInUser = std::make_unique<Requestor>(static_cast<const Requestor&>(*user));
                    } else {
                        loggedInUser = std::make_unique<User>(*user); // Fallback
                    }
                    std::cout << "Login successful! Welcome, " << loggedInUser->getName() << " (" << loggedInUser->getRole() << ")." << std::endl;
                    return true;
                }
            }
            throw std::runtime_error("Invalid email or password.");
        } catch (const std::exception& e) {
            std::cerr << "Login failed: " << e.what() << std::endl;
            loggedInUser = nullptr; // Ensure no user is logged in on failure
            return false;
        }
    }

    void logout() {
        if (loggedInUser) {
            std::cout << "Logging out " << loggedInUser->getName() << "." << std::endl;
            loggedInUser = nullptr;
        } else {
            std::cout << "No user is currently logged in." << std::endl;
        }
    }

    // --- Donor Management (Staff/Admin) ---
    void addDonor() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can add donors." << std::endl;
            return;
        }
        std::string name, contact, bloodType, dateRegistered;
        std::cout << "\n--- Add New Donor ---" << std::endl;
        try {
            Validator nameValidator(std::make_unique<NonEmptyStringValidationStrategy>());
            Validator contactValidator(std::make_unique<NonEmptyStringValidationStrategy>());
            Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());
            Validator dateValidator(std::make_unique<DateValidationStrategy>());

            std::cout << "Enter Donor Name: ";
            // Removed: std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, name);
            if (!nameValidator.validate(name)) throw std::invalid_argument(nameValidator.getErrorMessage("Donor Name"));

            std::cout << "Enter Contact Info (e.g., Phone No.): ";
            std::getline(std::cin, contact);
            if (!contactValidator.validate(contact)) throw std::invalid_argument(contactValidator.getErrorMessage("Contact Info"));

            std::cout << "Enter Blood Type (e.g., O+): ";
            std::getline(std::cin, bloodType);
            if (!bloodTypeValidator.validate(bloodType)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("Blood Type"));

            // Get current date for registration
            time_t t = time(0);
            struct tm* now = localtime(&t);
            char buffer[11]; // YYYY-MM-DD\0
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);
            dateRegistered = buffer;

            db.donors.emplace_back(DonorRecord::generateUniqueId(), name, contact, bloodType, dateRegistered);
            std::cout << "Donor added successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error adding donor: " << e.what() << std::endl;
        }
    }

    void viewDonors() const {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can view donors." << std::endl;
            return;
        }
        std::cout << "\n--- List of Donors ---" << std::endl;
        if (db.donors.empty()) {
            std::cout << "No donors registered." << std::endl;
            return;
        }
        for (const auto& donor : db.donors) {
            donor.display();
        }
    }

    void updateDonor() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can update donors." << std::endl;
            return;
        }
        std::string donorId;
        std::cout << "\n--- Update Donor Information ---" << std::endl;
        std::cout << "Enter Donor ID to update: ";
        std::cin >> donorId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::find_if(db.donors.begin(), db.donors.end(),
                               [&](const DonorRecord& d) { return d.getId() == donorId; });

        if (it != db.donors.end()) {
            std::cout << "Donor found. Enter new details (leave blank to keep current):" << std::endl;
            std::string newName, newContact, newBloodType;
            try {
                Validator nameValidator(std::make_unique<NonEmptyStringValidationStrategy>());
                Validator contactValidator(std::make_unique<NonEmptyStringValidationStrategy>());
                Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());

                std::cout << "New Name (" << it->getName() << "): ";
                std::getline(std::cin, newName);
                if (!newName.empty()) {
                    if (!nameValidator.validate(newName)) throw std::invalid_argument(nameValidator.getErrorMessage("New Donor Name"));
                    it->setName(newName);
                }

                std::cout << "New Contact Info (" << it->getContact() << "): ";
                std::getline(std::cin, newContact);
                if (!newContact.empty()) {
                    if (!contactValidator.validate(newContact)) throw std::invalid_argument(contactValidator.getErrorMessage("New Contact Info"));
                    it->setContact(newContact);
                }

                std::cout << "New Blood Type (" << it->getBloodType() << "): ";
                std::getline(std::cin, newBloodType);
                if (!newBloodType.empty()) {
                    if (!bloodTypeValidator.validate(newBloodType)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("New Blood Type"));
                    it->setBloodType(newBloodType);
                }
                std::cout << "Donor updated successfully!" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error updating donor: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Donor with ID " << donorId << " not found." << std::endl;
        }
    }

    void deleteDonor() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can delete donors." << std::endl;
            return;
        }
        std::string donorId;
        std::cout << "\n--- Delete Donor Record ---" << std::endl;
        std::cout << "Enter Donor ID to delete: ";
        std::cin >> donorId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::remove_if(db.donors.begin(), db.donors.end(),
                                 [&](const DonorRecord& d) { return d.getId() == donorId; });

        if (it != db.donors.end()) {
            db.donors.erase(it, db.donors.end());
            std::cout << "Donor with ID " << donorId << " deleted successfully." << std::endl;
        } else {
            std::cout << "Donor with ID " << donorId << " not found." << std::endl;
        }
    }

    // --- Inventory Management (Staff/Admin) ---
    void addInventoryItem() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can add inventory." << std::endl;
            return;
        }
        std::string bloodType, dateDonated, expirationDate;
        int quantity;
        std::cout << "\n--- Add New Blood Unit to Inventory ---" << std::endl;
        try {
            Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());
            Validator quantityValidator(std::make_unique<PositiveIntegerValidationStrategy>());
            Validator dateValidator(std::make_unique<DateValidationStrategy>());

            std::cout << "Enter Blood Type (e.g., A+): ";
            // Removed: std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, bloodType);
            if (!bloodTypeValidator.validate(bloodType)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("Blood Type"));

            std::cout << "Enter Quantity (mL or units): ";
            std::string qty_str;
            std::getline(std::cin, qty_str);
            if (!quantityValidator.validate(qty_str)) throw std::invalid_argument(quantityValidator.getErrorMessage("Quantity"));
            quantity = std::stoi(qty_str);

            std::cout << "Enter Date Donated (YYYY-MM-DD): ";
            std::getline(std::cin, dateDonated);
            if (!dateValidator.validate(dateDonated)) throw std::invalid_argument(dateValidator.getErrorMessage("Date Donated"));

            std::cout << "Enter Expiration Date (YYYY-MM-DD): ";
            std::getline(std::cin, expirationDate);
            if (!dateValidator.validate(expirationDate)) throw std::invalid_argument(dateValidator.getErrorMessage("Expiration Date"));

            // Basic check: expiration date should be after donation date
            if (expirationDate < dateDonated) {
                throw std::invalid_argument("Expiration date cannot be before donation date.");
            }

            db.inventory.emplace_back(BloodInventoryItem::generateUniqueId(), bloodType, quantity, dateDonated, expirationDate);
            std::cout << "Blood unit added to inventory successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error adding inventory item: " << e.what() << std::endl;
        }
    }

    void viewInventory() const {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can view inventory." << std::endl;
            return;
        }
        std::cout << "\n--- Current Blood Inventory ---" << std::endl;
        if (db.inventory.empty()) {
            std::cout << "Inventory is empty." << std::endl;
            return;
        }
        for (const auto& item : db.inventory) {
            item.display();
        }
    }

    void updateInventoryItem() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can update inventory." << std::endl;
            return;
        }
        std::string itemId;
        std::cout << "\n--- Update Inventory Item ---" << std::endl;
        std::cout << "Enter Inventory Item ID to update: ";
        std::cin >> itemId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::find_if(db.inventory.begin(), db.inventory.end(),
                               [&](const BloodInventoryItem& item) { return item.getId() == itemId; });

        if (it != db.inventory.end()) {
            std::cout << "Item found. Enter new details (leave blank to keep current):" << std::endl;
            std::string newQuantityStr, newBloodType, newDateDonated, newExpirationDate;
            try {
                Validator quantityValidator(std::make_unique<PositiveIntegerValidationStrategy>());
                Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());
                Validator dateValidator(std::make_unique<DateValidationStrategy>());

                std::cout << "New Quantity (" << it->getQuantity() << "): ";
                std::getline(std::cin, newQuantityStr);
                if (!newQuantityStr.empty()) {
                    if (!quantityValidator.validate(newQuantityStr)) throw std::invalid_argument(quantityValidator.getErrorMessage("New Quantity"));
                    it->setQuantity(std::stoi(newQuantityStr));
                }

                std::cout << "New Blood Type (" << it->getBloodType() << "): ";
                std::getline(std::cin, newBloodType);
                if (!newBloodType.empty()) {
                    if (!bloodTypeValidator.validate(newBloodType)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("New Blood Type"));
                    it->setBloodType(newBloodType);
                }

                std::cout << "New Date Donated (" << it->getDateDonated() << "): ";
                std::getline(std::cin, newDateDonated);
                if (!newDateDonated.empty()) {
                    if (!dateValidator.validate(newDateDonated)) throw std::invalid_argument(dateValidator.getErrorMessage("New Date Donated"));
                    it->setDateDonated(newDateDonated);
                }

                std::cout << "New Expiration Date (" << it->getExpirationDate() << "): ";
                std::getline(std::cin, newExpirationDate);
                if (!newExpirationDate.empty()) {
                    if (!dateValidator.validate(newExpirationDate)) throw std::invalid_argument(dateValidator.getErrorMessage("New Expiration Date"));
                    it->setExpirationDate(newExpirationDate);
                }

                 // Re-check date consistency if both are updated
                if (!newDateDonated.empty() && !newExpirationDate.empty() && newExpirationDate < newDateDonated) {
                    throw std::invalid_argument("New expiration date cannot be before new donation date.");
                } else if (!newExpirationDate.empty() && newExpirationDate < it->getDateDonated()) { // If only expiration is updated
                     throw std::invalid_argument("New expiration date cannot be before current donation date.");
                } else if (!newDateDonated.empty() && it->getExpirationDate() < newDateDonated) { // If only donation is updated
                     throw std::invalid_argument("New donation date cannot be after current expiration date.");
                }

                std::cout << "Inventory item updated successfully!" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error updating inventory item: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Inventory item with ID " << itemId << " not found." << std::endl;
        }
    }

    void deleteInventoryItem() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can delete inventory items." << std::endl;
            return;
        }
        std::string itemId;
        std::cout << "\n--- Delete Inventory Item ---" << std::endl;
        std::cout << "Enter Inventory Item ID to delete: ";
        std::cin >> itemId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::remove_if(db.inventory.begin(), db.inventory.end(),
                                 [&](const BloodInventoryItem& item) { return item.getId() == itemId; });

        if (it != db.inventory.end()) {
            db.inventory.erase(it, db.inventory.end());
            std::cout << "Inventory item with ID " << itemId << " deleted successfully." << std::endl;
        } else {
            std::cout << "Inventory item with ID " << itemId << " not found." << std::endl;
        }
    }

    // --- Request Management (Staff/Admin for processing, Requestor for creating/viewing/cancelling own) ---
    void makeBloodRequest() {
        if (!loggedInUser || (loggedInUser->getRole() != "requestor")) {
            std::cout << "Access Denied: Only Requestors can make blood requests." << std::endl;
            return;
        }

        std::string bloodType;
        int quantity;
        std::cout << "\n--- Make New Blood Request ---" << std::endl;
        try {
            Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());
            Validator quantityValidator(std::make_unique<PositiveIntegerValidationStrategy>());

            std::cout << "Enter Blood Type needed (e.g., O+): ";
            // Removed: std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, bloodType);
            if (!bloodTypeValidator.validate(bloodType)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("Blood Type"));

            std::cout << "Enter Quantity needed: ";
            std::string qty_str;
            std::getline(std::cin, qty_str);
            if (!quantityValidator.validate(qty_str)) throw std::invalid_argument(quantityValidator.getErrorMessage("Quantity"));
            quantity = std::stoi(qty_str);

            // Check availability (simple check: total available quantity)
            int availableQuantity = 0;
            for (const auto& item : db.inventory) {
                if (item.getBloodType() == bloodType) {
                    availableQuantity += item.getQuantity();
                }
            }

            if (availableQuantity < quantity) {
                throw std::runtime_error("Insufficient blood stock for " + bloodType + ". Available: " + std::to_string(availableQuantity));
            }

            time_t t = time(0);
            struct tm* now = localtime(&t);
            char buffer[11];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);
            std::string dateRequested = buffer;

            db.requests.emplace_back(BloodRequest::generateUniqueId(), loggedInUser->getId(), loggedInUser->getName(), bloodType, quantity, "pending", dateRequested);
            std::cout << "Blood request submitted successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error making blood request: " << e.what() << std::endl;
        }
    }

    void viewRequests(bool allRequests = false) const {
        if (!loggedInUser) {
            std::cout << "Access Denied: Please log in." << std::endl;
            return;
        }

        std::cout << "\n--- Blood Requests ---" << std::endl;
        if (db.requests.empty()) {
            std::cout << "No blood requests found." << std::endl;
            return;
        }

        bool foundAny = false;
        for (const auto& req : db.requests) {
            if (allRequests || (loggedInUser->getRole() == "requestor" && req.getRequestorId() == loggedInUser->getId())) {
                req.display();
                foundAny = true;
            }
        }
        if (!foundAny) {
            std::cout << "No requests found for your account." << std::endl;
        }
    }

    void processRequest() {
        if (!loggedInUser || (loggedInUser->getRole() != "admin" && loggedInUser->getRole() != "staff")) {
            std::cout << "Access Denied: Only Admin/Staff can process requests." << std::endl;
            return;
        }
        std::string requestId;
        std::cout << "\n--- Process Blood Request ---" << std::endl;
        std::cout << "Enter Request ID to process: ";
        std::cin >> requestId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::find_if(db.requests.begin(), db.requests.end(),
                               [&](const BloodRequest& r) { return r.getId() == requestId; });

        if (it != db.requests.end()) {
            if (it->getStatus() == "fulfilled") {
                std::cout << "Request " << requestId << " is already fulfilled." << std::endl;
                return;
            }
            if (it->getStatus() == "cancelled") {
                std::cout << "Request " << requestId << " was cancelled." << std::endl;
                return;
            }

            // Attempt to fulfill the request
            int neededQuantity = it->getQuantity();
            std::string neededBloodType = it->getBloodType();
            int fulfilledQuantity = 0;

            for (auto& item : db.inventory) {
                if (item.getBloodType() == neededBloodType && item.getQuantity() > 0) {
                    int quantityToTake = std::min(neededQuantity - fulfilledQuantity, item.getQuantity());
                    item.setQuantity(item.getQuantity() - quantityToTake);
                    fulfilledQuantity += quantityToTake;
                    if (fulfilledQuantity == neededQuantity) break;
                }
            }

            if (fulfilledQuantity == neededQuantity) {
                it->setStatus("fulfilled");
                std::cout << "Request " << requestId << " fulfilled successfully." << std::endl;
            } else {
                it->setStatus("pending"); // Keep status pending if not fully fulfilled
                std::cout << "Could not fully fulfill request " << requestId << ". Remaining needed: " << (neededQuantity - fulfilledQuantity) << std::endl;
                std::cout << "Please update inventory or try again later." << std::endl;
            }
        } else {
            std::cout << "Request with ID " << requestId << " not found." << std::endl;
        }
    }

    void updateOwnRequest() {
        if (!loggedInUser || loggedInUser->getRole() != "requestor") {
            std::cout << "Access Denied: Only Requestors can update their own requests." << std::endl;
            return;
        }
        std::string requestId;
        std::cout << "\n--- Update Your Blood Request ---" << std::endl;
        std::cout << "Enter Request ID to update: ";
        std::cin >> requestId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::find_if(db.requests.begin(), db.requests.end(),
                               [&](const BloodRequest& r) { return r.getId() == requestId && r.getRequestorId() == loggedInUser->getId(); });

        if (it != db.requests.end()) {
            if (it->getStatus() == "fulfilled" || it->getStatus() == "cancelled") {
                std::cout << "Request " << requestId << " cannot be updated as it is already " << it->getStatus() << "." << std::endl;
                return;
            }
            std::string newQuantityStr, newBloodType;
            try {
                Validator quantityValidator(std::make_unique<PositiveIntegerValidationStrategy>());
                Validator bloodTypeValidator(std::make_unique<BloodTypeValidationStrategy>());

                std::cout << "Enter New Quantity (" << it->getQuantity() << ", leave blank to keep current): ";
                std::getline(std::cin, newQuantityStr);
                if (!newQuantityStr.empty()) {
                    if (!quantityValidator.validate(newQuantityStr)) throw std::invalid_argument(quantityValidator.getErrorMessage("New Quantity"));
                    it->setQuantity(std::stoi(newQuantityStr));
                }

                std::cout << "Enter New Blood Type (" << it->getBloodType() << ", leave blank to keep current): ";
                std::getline(std::cin, newBloodType);
                if (!newBloodType.empty()) {
                    if (!bloodTypeValidator.validate(newBloodType)) throw std::invalid_argument(bloodTypeValidator.getErrorMessage("New Blood Type"));
                    it->setBloodType(newBloodType);
                }
                std::cout << "Request updated successfully!" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error updating request: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Request with ID " << requestId << " not found or you don't have permission to update it." << std::endl;
        }
    }

    void cancelOwnRequest() {
        if (!loggedInUser || loggedInUser->getRole() != "requestor") {
            std::cout << "Access Denied: Only Requestors can cancel their own requests." << std::endl;
            return;
        }
        std::string requestId;
        std::cout << "\n--- Cancel Your Blood Request ---" << std::endl;
        std::cout << "Enter Request ID to cancel: ";
        std::cin >> requestId;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto it = std::find_if(db.requests.begin(), db.requests.end(),
                               [&](const BloodRequest& r) { return r.getId() == requestId && r.getRequestorId() == loggedInUser->getId(); });

        if (it != db.requests.end()) {
            if (it->getStatus() == "fulfilled" || it->getStatus() == "cancelled") {
                std::cout << "Request " << requestId << " cannot be cancelled as it is already " << it->getStatus() << "." << std::endl;
                return;
            }
            it->setStatus("cancelled");
            std::cout << "Request " << requestId << " cancelled successfully." << std::endl;
        } else {
            std::cout << "Request with ID " << requestId << " not found or you don't have permission to cancel it." << std::endl;
        }
    }

    // --- Admin specific (User Management, General Reports) ---
    void adminManageUsers() {
        if (!loggedInUser || loggedInUser->getRole() != "admin") {
            std::cout << "Access Denied: Only Admin can manage users." << std::endl;
            return;
        }
        std::cout << "\n--- Admin User Management ---" << std::endl;
        std::cout << "1. View All Users" << std::endl;
        std::cout << "2. Delete User" << std::endl;
        std::cout << "Enter choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::cout << "\n--- All System Users ---" << std::endl;
                if (db.users.empty()) {
                    std::cout << "No users registered in the system." << std::endl;
                    return;
                }
                for (const auto& user : db.users) {
                    user->displayInfo();
                }
                break;
            }
            case 2: {
                std::string userIdToDelete;
                std::cout << "Enter User ID to delete: ";
                std::cin >> userIdToDelete;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                auto it = std::remove_if(db.users.begin(), db.users.end(),
                                         [&](const std::unique_ptr<User>& u) { return u->getId() == userIdToDelete; });

                if (it != db.users.end()) {
                    db.users.erase(it, db.users.end());
                    std::cout << "User with ID " << userIdToDelete << " deleted successfully." << std::endl;
                } else {
                    std::cout << "User with ID " << userIdToDelete << " not found." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    void adminGenerateReports() {
        if (!loggedInUser || loggedInUser->getRole() != "admin") {
            std::cout << "Access Denied: Only Admin can generate reports." << std::endl;
            return;
        }
        std::cout << "\n--- Admin Report Generation ---" << std::endl;
        std::cout << "1. Enrollment Summary Report (simulated)" << std::endl;
        std::cout << "2. Blood Stock Levels Report" << std::endl;
        std::cout << "3. Requests Status Report" << std::endl;
        std::cout << "Enter choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::cout << "\n--- Enrollment Summary Report ---" << std::endl;
                // This is a simple simulation; real report would query more intelligently
                std::cout << "Total registered users: " << db.users.size() << std::endl;
                long donorCount = 0;
                long requestorCount = 0;
                long adminCount = 0;
                long staffCount = 0;
                for(const auto& user : db.users) {
                    if (user->getRole() == "donor") donorCount++;
                    else if (user->getRole() == "requestor") requestorCount++;
                    else if (user->getRole() == "admin") adminCount++;
                    else if (user->getRole() == "staff") staffCount++;
                }
                std::cout << "  Donors: " << donorCount << std::endl;
                std::cout << "  Requestors: " << requestorCount << std::endl;
                std::cout << "  Admins: " << adminCount << std::endl;
                std::cout << "  Staff: " << staffCount << std::endl;
                break;
            }
            case 2: {
                std::cout << "\n--- Blood Stock Levels Report ---" << std::endl;
                if (db.inventory.empty()) {
                    std::cout << "No blood units in inventory." << std::endl;
                } else {
                    for (const auto& item : db.inventory) {
                        item.display();
                    }
                }
                break;
            }
            case 3: {
                std::cout << "\n--- Requests Status Report ---" << std::endl;
                if (db.requests.empty()) {
                    std::cout << "No blood requests." << std::endl;
                } else {
                    for (const auto& req : db.requests) {
                        req.display();
                    }
                }
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
};

// Initialize the static instance pointer
BloodBankManagementSystem* BloodBankManagementSystem::instance = nullptr;

// --- Main Application Loop ---
void displayMainMenu() {
    std::cout << "\n--- Welcome to the Blood Bank Management System ---" << std::endl;
    std::cout << "1. Register as a Donor" << std::endl;
    std::cout << "2. Register as a Requestor" << std::endl;
    std::cout << "3. Login" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayAdminMenu() {
    std::cout << "\n--- Admin Dashboard ---" << std::endl;
    std::cout << "1. Manage Donors (Add/View/Update/Delete)" << std::endl;
    std::cout << "2. Manage Inventory (Add/View/Update/Delete)" << std::endl;
    std::cout << "3. Manage Requests (View/Process)" << std::endl;
    std::cout << "4. Manage Users (View/Delete)" << std::endl; // Admin-specific
    std::cout << "5. Generate Reports" << std::endl;           // Admin-specific
    std::cout << "6. Logout" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayStaffMenu() {
    std::cout << "\n--- Staff Dashboard ---" << std::endl;
    std::cout << "1. Manage Donors (Add/View/Update/Delete)" << std::endl;
    std::cout << "2. Manage Inventory (Add/View/Update/Delete)" << std::endl;
    std::cout << "3. Manage Requests (View/Process)" << std::endl;
    std::cout << "4. Logout" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayDonorMenu() {
    std::cout << "\n--- Donor Dashboard ---" << std::endl;
    std::cout << "1. View Donation History (Simulated)" << std::endl;
    std::cout << "2. Inquire to Give Blood (Simulated)" << std::endl;
    std::cout << "3. Logout" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayRequestorMenu() {
    std::cout << "\n--- Requestor Dashboard ---" << std::endl;
    std::cout << "1. Make Blood Request" << std::endl;
    std::cout << "2. View My Requests" << std::endl;
    std::cout << "3. Update My Request" << std::endl;
    std::cout << "4. Cancel My Request" << std::endl;
    std::cout << "5. Logout" << std::endl;
    std::cout << "Enter your choice: ";
}

void displayCrudSubMenu(const std::string& entityName) {
    std::cout << "\n--- Manage " << entityName << " ---" << std::endl;
    std::cout << "1. Add " << entityName << std::endl;
    std::cout << "2. View " << entityName << "s" << std::endl;
    std::cout << "3. Update " << entityName << std::endl;
    std::cout << "4. Delete " << entityName << std::endl;
    std::cout << "5. Back to Dashboard" << std::endl;
    std::cout << "Enter your choice: ";
}


void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    BloodBankManagementSystem* system = BloodBankManagementSystem::getInstance();
    int choice;
    std::string email, password, name, specificDetail1, specificDetail2;

    while (true) {
        if (!system->getLoggedInUser()) {
            displayMainMenu();
            std::cin >> choice;
            clearInputBuffer();

            switch (choice) {
                case 1: { // Register Donor
                    std::cout << "\n--- Register as Donor ---" << std::endl;
                    std::cout << "Enter Name: "; std::getline(std::cin, name);
                    std::cout << "Enter Email: "; std::getline(std::cin, email);
                    std::cout << "Enter Password: "; std::getline(std::cin, password);
                    std::cout << "Enter Contact Info (e.g., Phone): "; std::getline(std::cin, specificDetail1);
                    std::cout << "Enter Blood Type (e.g., O+): "; std::getline(std::cin, specificDetail2);
                    system->registerUser(name, email, password, "donor", specificDetail1, specificDetail2);
                    break;
                }
                case 2: { // Register Requestor
                    std::cout << "\n--- Register as Requestor ---" << std::endl;
                    std::cout << "Enter Name: "; std::getline(std::cin, name);
                    std::cout << "Enter Email: "; std::getline(std::cin, email);
                    std::cout << "Enter Password: "; std::getline(std::cin, password);
                    std::cout << "Enter Hospital Name: "; std::getline(std::cin, specificDetail1);
                    system->registerUser(name, email, password, "requestor", specificDetail1);
                    break;
                }
                case 3: { // Login
                    std::cout << "\n--- Login ---" << std::endl;
                    std::cout << "Enter Email: "; std::getline(std::cin, email);
                    std::cout << "Enter Password: "; std::getline(std::cin, password);
                    system->login(email, password);
                    break;
                }
                case 4: { // Exit
                    std::cout << "Exiting Blood Bank Management System. Goodbye!" << std::endl;
                    BloodBankManagementSystem::destroyInstance();
                    return 0;
                }
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } else {
            // Logged In User Dashboard
            std::string role = system->getLoggedInUser()->getRole();
            if (role == "admin") {
                displayAdminMenu();
                std::cin >> choice;
                clearInputBuffer();
                switch (choice) {
                    case 1: { // Manage Donors
                        displayCrudSubMenu("Donor");
                        int subChoice;
                        std::cin >> subChoice;
                        clearInputBuffer();
                        if (subChoice == 1) system->addDonor();
                        else if (subChoice == 2) system->viewDonors();
                        else if (subChoice == 3) system->updateDonor();
                        else if (subChoice == 4) system->deleteDonor();
                        break;
                    }
                    case 2: { // Manage Inventory
                        displayCrudSubMenu("Inventory Item");
                        int subChoice;
                        std::cin >> subChoice;
                        clearInputBuffer();
                        if (subChoice == 1) system->addInventoryItem();
                        else if (subChoice == 2) system->viewInventory();
                        else if (subChoice == 3) system->updateInventoryItem();
                        else if (subChoice == 4) system->deleteInventoryItem();
                        break;
                    }
                    case 3: { // Manage Requests
                        std::cout << "\n--- Manage Requests ---" << std::endl;
                        std::cout << "1. View All Requests" << std::endl;
                        std::cout << "2. Process Request" << std::endl;
                        std::cout << "3. Back to Dashboard" << std::endl;
                        int subChoice;
                        std::cin >> subChoice;
                        clearInputBuffer();
                        if (subChoice == 1) system->viewRequests(true);
                        else if (subChoice == 2) system->processRequest();
                        break;
                    }
                    case 4: system->adminManageUsers(); break;
                    case 5: system->adminGenerateReports(); break;
                    case 6: system->logout(); break;
                    default: std::cout << "Invalid choice. Please try again." << std::endl;
                }
            } else if (role == "staff") {
                displayStaffMenu();
                std::cin >> choice;
                clearInputBuffer();
                switch (choice) {
                    case 1: { // Manage Donors
                        displayCrudSubMenu("Donor");
                        int subChoice;
                        std::cin >> subChoice;
                        clearInputBuffer();
                        if (subChoice == 1) system->addDonor();
                        else if (subChoice == 2) system->viewDonors();
                        else if (subChoice == 3) system->updateDonor();
                        else if (subChoice == 4) system->deleteDonor();
                        break;
                    }
                    case 2: { // Manage Inventory
                        displayCrudSubMenu("Inventory Item");
                        int subChoice;
                        std::cin >> subChoice;
                        clearInputBuffer();
                        if (subChoice == 1) system->addInventoryItem();
                        else if (subChoice == 2) system->viewInventory();
                        else if (subChoice == 3) system->updateInventoryItem();
                        else if (subChoice == 4) system->deleteInventoryItem();
                        break;
                    }
                    case 3: { // Manage Requests
                        std::cout << "\n--- Manage Requests ---" << std::endl;
                        std::cout << "1. View All Requests" << std::endl;
                        std::cout << "2. Process Request" << std::endl;
                        std::cout << "3. Back to Dashboard" << std::endl;
                        int subChoice;
                        std::cin >> subChoice;
                        clearInputBuffer();
                        if (subChoice == 1) system->viewRequests(true);
                        else if (subChoice == 2) system->processRequest();
                        break;
                    }
                    case 4: system->logout(); break;
                    default: std::cout << "Invalid choice. Please try again." << std::endl;
                }
            } else if (role == "donor") {
                displayDonorMenu();
                std::cin >> choice;
                clearInputBuffer();
                switch (choice) {
                    case 1: static_cast<Donor*>(system->getLoggedInUser().get())->viewDonationHistory(); break; // Polymorphic call
                    case 2: static_cast<Donor*>(system->getLoggedInUser().get())->inquireToGiveBlood(); break; // Polymorphic call
                    case 3: system->logout(); break;
                    default: std::cout << "Invalid choice. Please try again." << std::endl;
                }
            } else if (role == "requestor") {
                displayRequestorMenu();
                std::cin >> choice;
                clearInputBuffer();
                switch (choice) {
                    case 1: system->makeBloodRequest(); break;
                    case 2: system->viewRequests(false); break; // Only view own requests
                    case 3: system->updateOwnRequest(); break;
                    case 4: system->cancelOwnRequest(); break;
                    case 5: system->logout(); break;
                    default: std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
    }

    return 0;
}
