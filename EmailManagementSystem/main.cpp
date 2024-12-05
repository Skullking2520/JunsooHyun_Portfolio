#include "UserManager.h"
#include "EM.h"
#include "Filter.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>

using namespace std;

// Function to display the main menu
void displayMainMenu() {
    cout << "\n==== Email Management System ====\n";
    cout << "1. Sign Up\n";
    cout << "2. Log In\n";
    cout << "3. Exit\n";
    cout << "Choose an option: ";
}

// Function to display the user menu
void displayUserMenu() {
    cout << "\n==== User Menu ====\n";
    cout << "1. Write Email\n";
    cout << "2. Send Next Email\n";
    cout << "3. View Inbox\n";
    cout << "4. View Outbox\n";
    cout << "5. Log Out\n";
    cout << "Choose an option: ";
}

// Function to get the current timestamp
string getCurrentTimestamp() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtm);
    return string(timestamp);
}

// Function to log actions to a log file
void logAction(const string& filename, const string& action) {
    ofstream file(filename.c_str(), ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentTimestamp() << "] " << action << "\n";
        file.close();
    } else {
        cerr << "Error: Unable to open log file.\n";
    }
}

// Function to log email to a file
void logEmail(const string& filename, const Email& email) {
    ofstream file(filename.c_str(), ios::app);
    if (file.is_open()) {
        file << "Date: " << email.getDate() << "\n";
        file << "From: " << email.getSender() << "\n";
        file << "To: " << email.getRecipient() << "\n";
        file << "Subject: " << email.getSubject() << "\n";
        file << "Body: " << email.getBody() << "\n";
        file << "--------------------------\n";
        file.close();
    } else {
        cerr << "Error: Unable to open email log file.\n";
    }
}

// Main function
int main() {
    UserManager userManager;
    int choice;

    while (true) {
        if (!userManager.isLoggedIn()) {
            displayMainMenu();
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                string email, password;
                cout << "Enter email: ";
                getline(cin, email);
                cout << "Enter password: ";
                getline(cin, password);
                userManager.addUser(email, password);
            } else if (choice == 2) {
                string email, password;
                cout << "Enter email: ";
                getline(cin, email);
                cout << "Enter password: ";
                getline(cin, password);
                userManager.login(email, password);
            } else if (choice == 3) {
                cout << "Exiting Email Management System. Goodbye!\n";
                break;
            } else {
                cout << "Invalid option. Please try again.\n";
            }
        } else {
            displayUserMenu();
            cin >> choice;
            cin.ignore();

            User* currentUser = userManager.getCurrentUser();
            string currentEmail = currentUser->getEmail();

            if (choice == 1) {
                // Write email and add to outbox
                Email email = createEmail(currentEmail);
                currentUser->getOutbox().addEmail(email);
                logAction("action_log.txt", "Email drafted by " + currentEmail);
            } else if (choice == 2) {
                // Send next email in outbox
                if (!currentUser->getOutbox().isEmpty()) {
                    Email email = currentUser->getOutbox().sendEmail();
                    SpamFilter spamFilter;
                    spamFilter.loadSpamWords("spamword.txt");

                   if (spamFilter.isSpam(email, currentEmail)){
                        cout << "Email detected as spam and will not be sent.\n";
                        logAction("action_log.txt", "Spam email blocked from " + currentEmail);
                        continue;
                    }

                    // Log to sender's outbox log
                    string senderLogFile = currentEmail + "_OutboxLog.txt";
                    logEmail(senderLogFile, email);
                    currentUser->getOutbox().saveToFile(senderLogFile);

                    // Add email to recipient's inbox
                    string recipientEmail = email.getRecipient();
                    User recipient(recipientEmail);
                    recipient.getInbox().addEmail(email);

                    // Log to recipient's inbox log
                    string recipientLogFile = recipientEmail + "_InboxLog.txt";
                    logEmail(recipientLogFile, email);
                    recipient.getInbox().saveToFile(recipientLogFile);

                    cout << "Email sent to " << recipientEmail << ".\n";
                    logAction("action_log.txt", "Email sent from " + currentEmail + " to " + recipientEmail);

                } else {
                    cout << "Outbox is empty.\n";
                }
            } else if (choice == 3) {
                // View inbox
                currentUser->getInbox().showAllEmails();
            } else if (choice == 4) {
                // View outbox
                currentUser->getOutbox().showAllEmails();
            } else if (choice == 5) {
                // Log out
                string inboxFile = currentEmail + "_InboxLog.txt";
                string outboxFile = currentEmail + "_OutboxLog.txt";
                currentUser->getInbox().saveToFile(inboxFile);
                currentUser->getOutbox().saveToFile(outboxFile);

                userManager.logout();
            } else {
                cout << "Invalid option. Please try again.\n";
            }
        }
    }

    return 0;
}
