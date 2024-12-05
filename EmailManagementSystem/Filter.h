#ifndef FILTER_H
#define FILTER_H

#include <string>
#include "EM.h"  

// spam words
struct SpamWordNode {
    std::string word;
    SpamWordNode* next;
};

// timestamp for each sender
struct TimestampNode {
    std::string timestamp;
    TimestampNode* next;
};

// sender email data
struct SenderData {
    std::string senderEmail;
    TimestampNode* timestamps;
    SenderData* next;
};

class SpamFilter {
private:
    SpamWordNode* spamWords; // spam words
    SenderData* senderList;  // senders and email timestamps

public:
    SpamFilter();  
    ~SpamFilter(); 

    void loadSpamWords(const std::string& filename);    
    bool containsSpamWords(const std::string& text);    
    bool isSendingTooFast(const std::string& senderEmail);  
    void addTimestamp(const std::string& timestamp, const std::string& senderEmail);  //timestamp for sender
    bool isSpam(const Email& email, const std::string& senderEmail);  //an email is spam
};

#endif 
