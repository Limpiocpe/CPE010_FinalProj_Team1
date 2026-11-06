#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <iostream>

struct DocumentRequest {
    std::string ticketID;
    std::string residentName;
    std::string documentType;
    std::string dateSubmitted;
    std::string requirements;
    DocumentRequest* next;

    DocumentRequest(std::string id, std::string name, std::string docType, std::string date, std::string reqs)
        : ticketID(id), residentName(name), documentType(docType), dateSubmitted(date), requirements(reqs), next(nullptr) {}
};

class RequestQueue {
private:
    DocumentRequest* front;
    DocumentRequest* rear;
    int count;

public:
    RequestQueue() : front(nullptr), rear(nullptr), count(0) {}
    ~RequestQueue();

    void enqueue(std::string id, std::string name, std::string docType, std::string date, std::string reqs);
    DocumentRequest* dequeue();
    DocumentRequest* peek() const;
    bool isEmpty() const;
    int size() const;
    void displayQueue() const;
};

#endif