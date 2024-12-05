#ifndef EL_H
#define EL_H

#include "EM.h"  // Include Email class

class EmailList {
public:
    EmailList();                  // creator
    ~EmailList();                 // extincter

    void addEmail(const Email& email);  // add email
    void removeEmailAt(int index);      // remove email by index
    void displayEmails() const;         // display every email
    bool isThere(int index) const;      // check presence of email
    Email getEmailAt(int index) const;  // get email at index


private:
    struct Node {                       // Node for linked list
        Email email;
        Node* next;
        Node(const Email& email) : email(email), next(nullptr) {}
    };

    Node* head;  // First node of the list
};

#endif
