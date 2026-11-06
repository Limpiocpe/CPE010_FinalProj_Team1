#include "Queue.h"

RequestQueue::~RequestQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

void RequestQueue::enqueue(std::string id, std::string name, std::string docType, std::string date, std::string reqs) {
    DocumentRequest* newRequest = new DocumentRequest(id, name, docType, date, reqs);
    if (isEmpty()) {
        front = rear = newRequest;
    } else {
        rear->next = newRequest;
        rear = newRequest;
    }
    count++;
}

DocumentRequest* RequestQueue::dequeue() {
    if (isEmpty()) return nullptr;
    DocumentRequest* temp = front;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    count--;
    return temp;
}

DocumentRequest* RequestQueue::peek() const {
    return front;
}

bool RequestQueue::isEmpty() const {
    return front == nullptr;
}

int RequestQueue::size() const {
    return count;
}

void RequestQueue::displayQueue() const {
    if (isEmpty()) {
        std::cout << "Queue is empty.\n";
        return;
    }
    DocumentRequest* curr = front;
    std::cout << "\n--- Current Pending Processing Queue ---\n";
    while (curr != nullptr) {
        std::cout << "[" << curr->ticketID << "] " << curr->residentName 
                  << " - " << curr->documentType << " (" << curr->dateSubmitted << ")\n";
        curr = curr->next;
    }
}