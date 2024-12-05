#include "Inbox.h"
#include <iostream>
#include <fstream>

using namespace std;

Inbox::Inbox() : top(nullptr) {}

Inbox::~Inbox() {
    saveToFile(""); // Save to default file if needed
    clear();
}

// Add email to inbox
void Inbox::addEmail(const Email& email) {
    Node* newNode = new Node(email);
    newNode->next = top;
    top = newNode;
    cout << "New email added to inbox.\n";
}

// Get the latest email
Email Inbox::getLatestEmail() {
    if (isEmpty()) {
        throw runtime_error("Inbox is empty.");
    }
    Node* temp = top;
    top = top->next;
    Email latestEmail = temp->email;
    delete temp;
    return latestEmail;
}

// Show the latest email
void Inbox::showLatestEmail() const {
    if (isEmpty()) {
        cout << "Inbox is empty.\n";
    } else {
        top->email.display();
    }
}

// Show all emails in inbox
void Inbox::showAllEmails() const {
    if (isEmpty()) {
        cout << "Inbox is empty.\n";
    } else {
        Node* temp = top;
        while (temp != nullptr) {
            temp->email.display();
            temp = temp->next;
        }
    }
}

// Placeholder for search emails function
void Inbox::searchEmails(const string& keyword) const {
    // TODO: Implement the search functionality.
    // Your team member can implement this function to search emails based on the keyword.
    // For example, you can search in the subject, sender, or body of the emails.
    cout << "Search functionality is not implemented yet.\n";
}

// Check if inbox is empty
bool Inbox::isEmpty() const {
    return top == nullptr;
}

// Load emails from file
void Inbox::loadFromFile(const string& filename) {
    if (filename.empty()) return;

    ifstream file(filename);
    if (file.is_open()) {
        clear();
        string line, sender, recipient, subject, body, date;
        while (getline(file, line)) {
            if (line.find("Date: ") == 0) {
                date = line.substr(6);
                getline(file, line); // From
                sender = line.substr(6);
                getline(file, line); // To
                recipient = line.substr(4);
                getline(file, line); // Subject
                subject = line.substr(9);
                getline(file, line); // Body
                body = line.substr(6);
                getline(file, line); // Separator
                addEmail(Email(sender, recipient, subject, body, date));
            }
        }
        file.close();
    }
}

// Save emails to file
void Inbox::saveToFile(const string& filename) const {
    if (filename.empty()) return;

    ofstream file(filename);
    if (file.is_open()) {
        Node* temp = top;
        while (temp != nullptr) {
            const Email& email = temp->email;
            file << "Date: " << email.getDate() << "\n";
            file << "From: " << email.getSender() << "\n";
            file << "To: " << email.getRecipient() << "\n";
            file << "Subject: " << email.getSubject() << "\n";
            file << "Body: " << email.getBody() << "\n";
            file << "--------------------------\n";
            temp = temp->next;
        }
        file.close();
    }
}

// Clear the inbox
void Inbox::clear() {
    while (!isEmpty()) {
        getLatestEmail();
    }
}
