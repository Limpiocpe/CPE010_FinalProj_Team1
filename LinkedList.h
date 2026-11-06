 
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
 
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
 

enum class RequestStatus {
    OnHold, //in queue/waiting
    InProgress,
    PendingApproval, //by the captain
    Ready, //when approved
    Received
};
 

inline const char *statusToString(RequestStatus s) {
    switch (s) {
        case RequestStatus::OnHold:          return "On Hold";
        case RequestStatus::InProgress:      return "In Progress";
        case RequestStatus::PendingApproval: return "Pending Approval";
        case RequestStatus::Ready:           return "Ready";
        case RequestStatus::Received:        return "Received";
    }
    return "Unknown";
}
 
struct StatusNode {
    RequestStatus status;
    std::string dateTime;
    std::string staff;
    StatusNode *next;
 
    StatusNode(RequestStatus s, const std::string &dt, const std::string &st)
        : status(s), dateTime(dt), staff(st), next(nullptr) {}
};

struct RequestNode {
    std::string requestId;  
    std::string documentType;
    std::string requesterName;
 
    StatusNode *historyHead;
    StatusNode *historyTail;
    RequestNode *next;
 
    RequestNode(const std::string &id, const std::string &type, const std::string &requester)
        : requestId(id), documentType(type), requesterName(requester),
          historyHead(nullptr), historyTail(nullptr), next(nullptr) {}
 
    ~RequestNode() {
        while (historyHead) {
            StatusNode *n = historyHead->next;
            delete historyHead;
            historyHead = n;
        }
    }
 
    RequestNode(const RequestNode &) = delete;
    RequestNode &operator=(const RequestNode &) = delete;
 

    void addStatus(RequestStatus status, const std::string &dateTime,
                   const std::string &staff) {
        StatusNode *node = new StatusNode(status, dateTime, staff);
        if (historyTail) historyTail->next = node;
        else             historyHead = node;
        historyTail = node;
    }
};
 

class RequestList {
private:
    RequestNode *head;
    RequestNode *tail;
 
public:
    RequestList() : head(nullptr), tail(nullptr) {}
 
    RequestList(const RequestList &) = delete;
    RequestList &operator=(const RequestList &) = delete;
 
    ~RequestList() {
        while (head) {
            RequestNode *n = head->next;
            delete head;
            head = n;
        }
    }
 
    // Checks ID if format is "BRG-YYYY-****"
    static bool isValidRequestId(const std::string &id) {
        if (id.size() != 13) return false;
        if (id.compare(0, 4, "BRG-") != 0) return false;
        if (id[8] != '-') return false;
        for (int i = 4; i < 8; ++i)
            if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
        for (int i = 9; i < 13; ++i)
            if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
        return true;
    }
 
    RequestNode *addRequest(const std::string &requestId,
                            const std::string &documentType,
                            const std::string &requesterName) {
        if (!isValidRequestId(requestId)) return nullptr;
        RequestNode *node = new RequestNode(requestId, documentType, requesterName);
        if (tail) tail->next = node;
        else      head = node;
        tail = node;
        return node;
    }
 
    RequestNode *searchById(const std::string &requestId) const {
        for (RequestNode *r = head; r; r = r->next)
            if (r->requestId == requestId) return r;
        return nullptr;
    }
 
    void printAll() const {
        for (const RequestNode *r = head; r; r = r->next) {
            std::cout << "Request " << r->requestId << " | " << r->documentType
                      << " | " << r->requesterName << "\n";
            int i = 1;
            for (const StatusNode *s = r->historyHead; s; s = s->next, ++i) {
                std::cout << "  " << i << ". " << std::left
                          << std::setw(18) << statusToString(s->status)
                          << std::setw(22) << s->dateTime
                          << s->staff << "\n";
            }
            std::cout << "\n";
        }
    }
};
 
#endif