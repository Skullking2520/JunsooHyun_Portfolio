#include "EL.h"
#include <iostream>

using namespace std;

EmailList::EmailList() : head(nullptr) {}

EmailList::~EmailList() {
    while (head != nullptr) {
        removeEmailAt(1);
    }
}
void EmailList::addEmail(const Email& email) {
    Node* newNode = new Node(email);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    cout << "New email added to list.\n";
}

void EmailList::removeEmailAt(int index) {
    if (head == nullptr) {
        cout << "No emails to remove.\n";
        return;
    }
    if (index < 1) {
        cout << "Invalid index. Index must be 1 or greater.\n";
        return;
    }

    // delete first node
    if (index == 1) {
        Node* temp = head;
        head = head->next;
        delete temp;
        cout << "Email removed from list at index 1.\n";
        return;
    }

    // delete certain node
    Node* current = head;
    int currentIndex = 1;
    while (current != nullptr && currentIndex < index - 1) {
        current = current->next;
        currentIndex++;
    }

    // check if index exceed the length
    if (current == nullptr || current->next == nullptr) {
        cout << "Invalid index. No email exists at the specified index.\n";
        return;
    }

    // found the node to delete
    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
    cout << "Email removed from list at index " << index << ".\n";
}


void EmailList::displayEmails() const {
    if (head == nullptr) {
        cout << "No emails in the list.\n";
        return;
    }
    Node* temp = head;
    int index = 1;  // clear index to 1
    while (temp != nullptr) {
        cout << index << ". ";  // display index
        temp->email.display();  // diplay email
        temp = temp->next;
        index++;  // increase index by 1
    }
}

bool EmailList::isThere(int index) const{
    bool isthere = true;
    if (index < 1) {
        throw out_of_range("Index must be 1 or greater.");
    }

    Node* temp = head;
    int currentIndex = 1;
    while (temp != nullptr && currentIndex < index) {
        temp = temp->next;
        currentIndex++;
    }

    if (temp == nullptr) {
        isthere = false;
    }

    return isthere;
}

Email EmailList::getEmailAt(int index) const {
    if (index < 1) {
        throw out_of_range("Index must be 1 or greater.");
    }

    Node* temp = head;
    int currentIndex = 1;
    while (temp != nullptr && currentIndex < index) {
        temp = temp->next;
        currentIndex++;
    }

    if (temp == nullptr) {
        throw out_of_range("Index out of range.");
    }

    return temp->email;
}
