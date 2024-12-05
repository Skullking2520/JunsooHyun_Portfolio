#include "Outbox.h"
#include <iostream>
#include <fstream>

using namespace std;

Outbox::Outbox() : front(nullptr), rear(nullptr) {}

Outbox::~Outbox() {
    saveToFile(""); // Save to default file if needed
    clear();
}

// Add email to outbox
void Outbox::addEmail(const Email& email) {
    Node* newNode = new Node(email);
    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    cout << "New email added to outbox.\n";
}

// Send the next email
Email Outbox::sendEmail() {
    if (isEmpty()) {
        throw runtime_error("Outbox is empty.");
    }
    Node* temp = front;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    Email sentEmail = temp->email;
    delete temp;
    return sentEmail;
}

// Show the next email
void Outbox::showNextEmail() const {
    if (isEmpty()) {
        cout << "Outbox is empty.\n";
    } else {
        front->email.display();
    }
}

// Show all emails in outbox
void Outbox::showAllEmails() const {
    if (isEmpty()) {
        cout << "Outbox is empty.\n";
    } else {
        Node* temp = front;
        while (temp != nullptr) {
            temp->email.display();
            temp = temp->next;
        }
    }
}

// Check if outbox is empty
bool Outbox::isEmpty() const {
    return front == nullptr;
}

// Load emails from file
void Outbox::loadFromFile(const string& filename) {
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
void Outbox::saveToFile(const string& filename) const {
    if (filename.empty()) return;

    ofstream file(filename);
    if (file.is_open()) {
        Node* temp = front;
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

// Clear the outbox
void Outbox::clear() {
    while (!isEmpty()) {
        sendEmail();
    }
}
