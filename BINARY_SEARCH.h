#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <cstddef>
#include <string>
#include <utility>

struct BarangayDocument {
    std::string specialId;      
    std::string residentName;
    std::string documentType;   
    int dateSubmitted;  
    double fee;
};

struct DocumentNode {
    BarangayDocument data;
    DocumentNode* next;
    explicit DocumentNode(const BarangayDocument& doc) : data(doc), next(nullptr) {}
};

class DocumentList {
private:
    DocumentNode* headPtr;
    DocumentNode* tailPtr;
    std::size_t count;

    static DocumentNode* middle(DocumentNode* start, DocumentNode* end) {
        DocumentNode* slow = start;
        DocumentNode* fast = start;
        while (fast != end && fast->next != end) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

public:
    DocumentList() : headPtr(nullptr), tailPtr(nullptr), count(0) {}
    ~DocumentList() {
        while (headPtr != nullptr) {
            DocumentNode* temp = headPtr;
            headPtr = headPtr->next;
            delete temp;
        }
    }

    DocumentList(const DocumentList&) = delete;
    DocumentList& operator=(const DocumentList&) = delete;
    DocumentNode* head() const { return headPtr; }
    std::size_t size() const { return count; }
    bool empty() const { return count == 0; }


    void append(const BarangayDocument& doc) {
        DocumentNode* node = new DocumentNode(doc);
        if (tailPtr == nullptr) {
            headPtr = tailPtr = node;
        } else {
            tailPtr->next = node;
            tailPtr = node;
        }
        ++count;
    }

    void insertSortedById(const BarangayDocument& doc) {
        DocumentNode* node = new DocumentNode(doc);

        if (headPtr == nullptr || doc.specialId < headPtr->data.specialId) {
            node->next = headPtr;
            headPtr = node;
            if (tailPtr == nullptr) tailPtr = node;
        } else {
            DocumentNode* cur = headPtr;
            while (cur->next != nullptr &&
                   cur->next->data.specialId < doc.specialId) {
                cur = cur->next;
            }
            node->next = cur->next;
            cur->next = node;
            if (node->next == nullptr) tailPtr = node;
        }
        ++count;
    }

    bool isSortedById() const {
        for (DocumentNode* cur = headPtr; cur && cur->next; cur = cur->next) {
            if (cur->data.specialId > cur->next->data.specialId) return false;
        }
        return true;
    }

    bool isSortedByDate() const {
        for (DocumentNode* cur = headPtr; cur && cur->next; cur = cur->next) {
            if (cur->data.dateSubmitted > cur->next->data.dateSubmitted) return false;
        }
        return true;
    }

    DocumentNode* FindById(const std::string& targetId) const {
        DocumentNode* start = headPtr;
        DocumentNode* end = nullptr;          

        while (start != end) {
            DocumentNode* mid = middle(start, end);
            int cmp = mid->data.specialId.compare(targetId);
            if (cmp == 0) return mid;     
            else if (cmp < 0) start = mid->next;   
            else end = mid;         
        }
        return nullptr;                         
    }

    DocumentNode* LowerBoundByDate(int date) const {
        DocumentNode* start = headPtr;
        DocumentNode* end = nullptr;

        while (start != end) {
            DocumentNode* mid = middle(start, end);
            if (mid->data.dateSubmitted < date) start = mid->next;
            else end = mid;
        }
        return start;
    }

    DocumentNode* UpperBoundByDate(int date) const {
        DocumentNode* start = headPtr;
        DocumentNode* end = nullptr;

        while (start != end) {
            DocumentNode* mid = middle(start, end);
            if (mid->data.dateSubmitted <= date) start = mid->next;
            else end = mid;
        }
        return start;
    }

    std::pair<DocumentNode*, DocumentNode*> findDateRange(int fromDate, int toDate) const {
        if (fromDate > toDate) return std::make_pair(nullptr, nullptr);
        return std::make_pair(LowerBoundByDate(fromDate), UpperBoundByDate(toDate));
    }

    static std::size_t CountRange(const DocumentNode* first, const DocumentNode* last) {
        std::size_t total = 0;
        for (const DocumentNode* p = first; p != last; p = p->next) ++total;
        return total;
    }
};

#endif 