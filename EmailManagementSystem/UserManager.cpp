#include "UserManager.h"
#include <iostream>
#include <fstream>

using namespace std;

UserManager::UserManager() : currentUser(nullptr), userList(nullptr) {
    loadUsers();
}

// Load users from a file
void UserManager::loadUsers() {
    ifstream file("users.txt");
    if (file.is_open()) {
        string email, password;
        while (getline(file, email) && getline(file, password)) {
            UserNode* newUser = new UserNode();
            newUser->email = email;
            newUser->password = password;
            newUser->next = userList;
            userList = newUser;
        }
        file.close();
    }
}

// Save users to a file
void UserManager::saveUsers() {
    ofstream file("users.txt");
    if (file.is_open()) {
        UserNode* temp = userList;
        while (temp != nullptr) {
            file << temp->email << "\n" << temp->password << "\n";
            temp = temp->next;
        }
        file.close();
    }
}

// Add a new user
void UserManager::addUser(const std::string& email, const std::string& password) {
    // Check if user already exists
    UserNode* temp = userList;
    while (temp != nullptr) {
        if (temp->email == email) {
            cout << "User already exists.\n";
            return;
        }
        temp = temp->next;
    }

    // Validate email format
    if (!isValidEmailFormat(email)) {
        cout << "Invalid email format. Please enter a valid email.\n";
        return;
    }

    // Add new user to the list
    UserNode* newUser = new UserNode();
    newUser->email = email;
    newUser->password = password;
    newUser->next = userList;
    userList = newUser;

    saveUsers();
    cout << "User registered successfully.\n";
}

// Log in a user
void UserManager::login(const std::string& email, const std::string& password) {
    UserNode* temp = userList;
    while (temp != nullptr) {
        if (temp->email == email && temp->password == password) {
            currentUser = new User(email);
            cout << "Logged in successfully.\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Invalid email or password.\n";
}

// Log out the current user
void UserManager::logout() {
    if (currentUser != nullptr) {
        delete currentUser;
        currentUser = nullptr;
        cout << "Logged out successfully.\n";
    }
}

// Check if a user is logged in
bool UserManager::isLoggedIn() const {
    return currentUser != nullptr;
}

// Get the current user
User* UserManager::getCurrentUser() {
    return currentUser;
}
