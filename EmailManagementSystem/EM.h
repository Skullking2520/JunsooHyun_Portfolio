#ifndef EM_H
#define EM_H

#include <string>

// Email class definition
class Email {
public:
    // Constructor
    Email(const std::string& sender = "", const std::string& recipient = "", const std::string& subject = "",
          const std::string& body = "", const std::string& date = "");

    // Getters
    std::string getSender() const;
    std::string getRecipient() const;
    std::string getSubject() const;
    std::string getBody() const;
    std::string getDate() const;

    // Display email details
    void display() const;

private:
    std::string sender;    // Sender's email
    std::string recipient; // Recipient's email
    std::string subject;   // Email subject
    std::string body;      // Email body
    std::string date;      // Date of email
};

// Function to create an email
Email createEmail(const std::string& sender);

// Function to validate email format
bool isValidEmailFormat(const std::string& email);

#endif
