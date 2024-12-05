#include "Filter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>  

// Constructor
SpamFilter::SpamFilter() : spamWords(nullptr), senderList(nullptr) {}

// Destructor
SpamFilter::~SpamFilter() {
    SpamWordNode* current = spamWords;
    while (current) {
        SpamWordNode* temp = current;
        current = current->next;
        delete temp;
    }

    SenderData* senderCurrent = senderList;
    while (senderCurrent) {
        TimestampNode* timestampCurrent = senderCurrent->timestamps;
        while (timestampCurrent) {
            TimestampNode* tempTimestamp = timestampCurrent;
            timestampCurrent = timestampCurrent->next;
            delete tempTimestamp;
        }
        SenderData* tempSender = senderCurrent;
        senderCurrent = senderCurrent->next;
        delete tempSender;
    }
}

void SpamFilter::loadSpamWords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening spam words file." << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        SpamWordNode* newNode = new SpamWordNode{word, spamWords};
        spamWords = newNode;
    }
    file.close();
}

bool SpamFilter::containsSpamWords(const std::string& text) {
    SpamWordNode* current = spamWords;
    while (current) {
        if (text.find(current->word) != std::string::npos) {
            return true; // Found a spam word
        }
        current = current->next;
    }
    return false;
}

bool SpamFilter::isSendingTooFast(const std::string& senderEmail) {
    //current time compared to the last timestamp
    SenderData* senderData = senderList;
    while (senderData) {
        if (senderData->senderEmail == senderEmail) {
            TimestampNode* timestamp = senderData->timestamps;
            //  5 emails in 10 seconds
            int emailCount = 0;
            std::time_t now = std::time(nullptr);
            while (timestamp) {
                // "%Y-%m-%d %H:%M:%S" time format
                struct tm tm;
                std::stringstream ss(timestamp->timestamp);
                ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

                // Convert timestamp to time_t
                std::time_t ts = std::mktime(&tm);
                if (std::difftime(now, ts) <= 10) {
                    emailCount++;
                }

                timestamp = timestamp->next;
            }

            if (emailCount > 5) {
                return true; // too many emails
            }
            break;
        }
        senderData = senderData->next;
    }
    return false;
}

// timestamp add
void SpamFilter::addTimestamp(const std::string& timestamp, const std::string& senderEmail) {
    SenderData* senderData = senderList;
    while (senderData) {
        if (senderData->senderEmail == senderEmail) {
            TimestampNode* newTimestamp = new TimestampNode{timestamp, senderData->timestamps};
            senderData->timestamps = newTimestamp;
            return;
        }
        senderData = senderData->next;
    }

    // If sender data does not exist, create a new entry
    SenderData* newSender = new SenderData{senderEmail, nullptr, senderList};
    senderList = newSender;
    addTimestamp(timestamp, senderEmail); // Recursively add the timestamp
}

//check if an email is spam based on words and sending freq
bool SpamFilter::isSpam(const Email& email, const std::string& senderEmail) {
    if (containsSpamWords(email.getBody())) {
        std::cout << "Email is detected as spam due to spam words.\n";
        return true;
    }

    if (isSendingTooFast(senderEmail)) {
        std::cout << "Email is detected as spam due to too many emails sent in a short time.\n";
        return true;
    }

    return false;
}
