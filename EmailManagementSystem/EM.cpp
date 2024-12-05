#include "EM.h"
#include <iostream>
#include <ctime>
#include <regex>

using namespace std;

// Constructor
Email::Email(const string& sender, const string& recipient, const string& subject,
             const string& body, const string& date)
    : sender(sender), recipient(recipient), subject(subject), body(body), date(date) {}

// Getters
string Email::getSender() const {
    return sender;
}

string Email::getRecipient() const {
    return recipient;
}

string Email::getSubject() const {
    return subject;
}

string Email::getBody() const {
    return body;
}

string Email::getDate() const {
    return date;
}

// Display email details
void Email::display() const {
    cout << "Date: " << date << "\n";
    cout << "From: " << sender << "\n";
    cout << "To: " << recipient << "\n";
    cout << "Subject: " << subject << "\n";
    cout << "Body: " << body << "\n";
    cout << "--------------------------\n";
}

// Function to validate email format
bool isValidEmailFormat(const std::string& email) {
    const regex email_regex(R"(^[\w\.-]+@[\w\.-]+\.\w+$)");
    return regex_match(email, email_regex);
}

// Function to create an email
Email createEmail(const string& sender) {
    string recipient, subject, body;

    // Recipient email validation loop
    while (true) {
        cout << "Enter recipient: ";
        getline(cin, recipient);

        // Validate recipient email format
        if (!isValidEmailFormat(recipient)) {
            cout << "Invalid email format. Please enter a valid recipient email.\n";
        } else {
            break;
        }
    }

    cout << "Enter subject: ";
    getline(cin, subject);
    cout << "Enter body: ";
    getline(cin, body);

    // Automatically set current date and time
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d %H:%M:%S", localtm);
    string date(date_buffer);

    return Email(sender, recipient, subject, body, date);
}
