#ifndef INBOX_H
#define INBOX_H

#include "EM.h"
#include <string>

class Inbox {
public:
    Inbox();
    ~Inbox();

    // Add email to inbox
    void addEmail(const Email& email);

    // Get the latest email
    Email getLatestEmail();

    // Show the latest email
    void showLatestEmail() const;

    // Show all emails in inbox
    void showAllEmails() const;

    // Placeholder for search emails function
    void searchEmails(const std::string& keyword) const; // Function to be implemented by your team member

    // Check if inbox is empty
    bool isEmpty() const;

    // Load emails from file
    void loadFromFile(const std::string& filename);

    // Save emails to file
    void saveToFile(const std::string& filename) const;

    // Clear the inbox
    void clear();

private:
    struct Node {
        Email email;
        Node* next;
        Node(const Email& email) : email(email), next(nullptr) {}
    };

    Node* top;
};

#endif
