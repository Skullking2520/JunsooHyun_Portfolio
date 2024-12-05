#ifndef OUTBOX_H
#define OUTBOX_H

#include "EM.h"
#include <string>

class Outbox {
public:
    Outbox();
    ~Outbox();

    // Add email to outbox
    void addEmail(const Email& email);

    // Send the next email
    Email sendEmail();

    // Show the next email
    void showNextEmail() const;

    // Show all emails in outbox
    void showAllEmails() const;

    // Check if outbox is empty
    bool isEmpty() const;

    // Load emails from file
    void loadFromFile(const std::string& filename);

    // Save emails to file
    void saveToFile(const std::string& filename) const;

    // Clear the outbox
    void clear();

private:
    struct Node {
        Email email;
        Node* next;
        Node(const Email& email) : email(email), next(nullptr) {}
    };

    Node* front;
    Node* rear;
};

#endif
