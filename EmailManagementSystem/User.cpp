#include "User.h"

// Constructor
User::User(const std::string& email) : email(email) {
    // Load inbox and outbox from files if needed
}

// Get user's email
std::string User::getEmail() const {
    return email;
}

// Get user's inbox
Inbox& User::getInbox() {
    return inbox;
}

// Get user's outbox
Outbox& User::getOutbox() {
    return outbox;
}
