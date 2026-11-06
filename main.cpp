#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include "stack.h"       
#include "Queue.h"
#include "LinkedList.h"
#include "BINARY_SEARCH.h"
#include "MergeSort.h"


RequestQueue::~RequestQueue() {
    while (!isEmpty()) {
        DocumentRequest* request = dequeue();
        delete request;
    }
}

void RequestQueue::enqueue(std::string id, std::string name, std::string docType,
                                std::string date, std::string reqs) {
    DocumentRequest* node = new DocumentRequest(id, name, docType, date, reqs);

    if (rear == nullptr) {
        front = rear = node;
    } else {
        rear->next = node;
        rear = node;
    }
    ++count;
}

DocumentRequest* RequestQueue::dequeue() {
    if (isEmpty()) return nullptr;

    DocumentRequest* node = front;
    front = front->next;

    if (front == nullptr) rear = nullptr;

    node->next = nullptr;
    --count;
    return node;
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
        std::cout << "\nThere are no pending requests!\n";
        return;
    }

    std::cout << "\n======= PENDING REQUESTS =======\n";
    for (DocumentRequest* current = front; current != nullptr;
         current = current->next) {
        std::cout << "\nTicket Number : " << current->ticketID;
        std::cout << "\nResident Name: " << current->residentName;
        std::cout << "\nDocument Type: " << current->documentType;
        std::cout << "\nDate Submitted: " << current->dateSubmitted;
        std::cout << "\nRequirements  : " << current->requirements;
        std::cout << std::endl;
    }
}

bool convertStatus(const std::string& text, RequestStatus& status) {
    if (text == "1") status = RequestStatus::OnHold;
    else if (text == "2") status = RequestStatus::InProgress;
    else if (text == "3") status = RequestStatus::PendingApproval;
    else if (text == "4") status = RequestStatus::Ready;
    else if (text == "5") status = RequestStatus::Received;
    else return false;

    return true;
}

RequestStatus statusFromText(const std::string& text) {
    if (text == "On Hold") return RequestStatus::OnHold;
    if (text == "In Progress") return RequestStatus::InProgress;
    if (text == "Pending Approval") return RequestStatus::PendingApproval;
    if (text == "Ready") return RequestStatus::Ready;
    if (text == "Received") return RequestStatus::Received;
    return RequestStatus::OnHold;
}

int main() {
    RequestQueue requestQueue;
    RequestList requestList;
    DocumentList documentList;
    Stack undoStack;
    std::vector<DocReq*> processedRequests; 

    int ticketCounter = 1;
    int choice;

    do {
        std::cout << "\n========================================\n";
        std::cout << "       e-BARANGAY DOCUMENT SYSTEM\n";
        std::cout << "========================================\n";
        std::cout << "[1] Submit Document Request\n";
        std::cout << "[2] Process Next Request\n";
        std::cout << "[3] Update Request Status\n";
        std::cout << "[4] View All Request Records\n";
        std::cout << "[5] Search Request by Ticket Number\n";
        std::cout << "[6] View Pending Requests\n";
        std::cout << "[7] View Undo History\n";
        std::cout << "[8] Undo Last Action\n";
        std::cout << "[9] Sort Released Requests (Merge Sort)\n";
        std::cout << "[10] Exit\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "\nInvalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string name, documentType, date, requirements;

            std::cout << "\n========== SUBMIT DOCUMENT REQUEST ==========\n";
            std::cout << "Resident Name: ";
            std::getline(std::cin, name);
            std::cout << "Document Type: ";
            std::getline(std::cin, documentType);
            std::cout << "Date Submitted (YYYY-MM-DD): ";
            std::getline(std::cin, date);
            std::cout << "Requirements: ";
            std::getline(std::cin, requirements);

            if (name.empty() || documentType.empty() ||
                date.empty() || requirements.empty()) {
                std::cout << "\nRequest not submitted. All fields are required.\n";
                continue;
            }

            std::string ticket = "BRG-2026-";
            if (ticketCounter < 10) ticket += "000";
            else if (ticketCounter < 100) ticket += "00";
            else if (ticketCounter < 1000) ticket += "0";
            ticket += std::to_string(ticketCounter++);

            requestQueue.enqueue(ticket, name, documentType, date, requirements);

            std::cout << "\nRequest successfully submitted!\n";
            std::cout << "Your Ticket Number: " << ticket << "\n";
            std::cout << "Your request has been added to the processing queue.\n";
        }

        else if (choice == 2) {
            DocumentRequest* nextRequest = requestQueue.dequeue();

            if (nextRequest == nullptr) {
                std::cout << "\nThere are no requests waiting to be processed.\n";
                continue;
            }

            RequestNode* record = requestList.addRequest(
                nextRequest->ticketID,
                nextRequest->documentType,
                nextRequest->residentName
            );

            if (record != nullptr) {
                record->addStatus(RequestStatus::OnHold,
                                  nextRequest->dateSubmitted, "Barangay Staff");

                std::cout << "\nRequest successfully moved for processing.\n";
                std::cout << "Ticket Number: " << nextRequest->ticketID << "\n";
                std::cout << "Resident Name: " << nextRequest->residentName << "\n";

                BarangayDocument document;
                document.specialId = nextRequest->ticketID;
                document.residentName = nextRequest->residentName;
                document.documentType = nextRequest->documentType;
                document.dateSubmitted = 0;
                document.fee = 0.0;
                documentList.insertSortedById(document);
            } else {
                std::cout << "\nUnable to create the request record.\n";
            }

            if (record != nullptr) {
                processedRequests.push_back(nextRequest);
            } else {
                delete nextRequest;
            }
        }

        else if (choice == 3) {
            std::string ticket, dateTime, staff;
            std::string oldStatus;
            RequestStatus newStatus;

            std::cout << "\n========== UPDATE REQUEST STATUS ==========\n";
            std::cout << "Enter Ticket Number: ";
            std::getline(std::cin, ticket);

            RequestNode* record = requestList.searchById(ticket);

            if (record == nullptr) {
                std::cout << "\nRequest not found. Process the request first.\n";
                continue;
            }

            if (record->historyTail != nullptr)
                oldStatus = statusToString(record->historyTail->status);
            else
                oldStatus = "On Hold";

            std::cout << "\nSelect New Status:\n";
            std::cout << "[1] On Hold\n";
            std::cout << "[2] In Progress\n";
            std::cout << "[3] Pending Approval\n";
            std::cout << "[4] Ready\n";
            std::cout << "[5] Received\n";
            std::cout << "Enter choice: ";

            std::string statusChoice;
            std::getline(std::cin, statusChoice);

            if (!convertStatus(statusChoice, newStatus)) {
                std::cout << "\nInvalid status selection.\n";
                continue;
            }

            std::cout << "Date and Time: ";
            std::getline(std::cin, dateTime);
            std::cout << "Staff Member: ";
            std::getline(std::cin, staff);

            if (dateTime.empty() || staff.empty()) {
                std::cout << "\nUpdate cancelled. Please complete all fields.\n";
                continue;
            }

            std::string updatedStatus = statusToString(newStatus);
            record->addStatus(newStatus, dateTime, staff);

            Action action;
            action.Action_name = "Update Request Status";
            action.Ticket_number = ticket;
            action.Previous_status = oldStatus;
            action.Updated_status = updatedStatus;
            undoStack.push(action);

            std::cout << "\nRequest status successfully updated!\n";
            std::cout << "Previous Status: " << oldStatus << "\n";
            std::cout << "Updated Status : " << updatedStatus << "\n";
        }

        else if (choice == 4) {
            std::cout << "\n========== ALL REQUEST RECORDS ==========\n";

            if (processedRequests.empty()) {
                std::cout << "No processed request records yet.\n";
            } else {
            requestList.printAll();
            }
        }

        else if (choice == 5) {
            std::string ticket;
            std::cout << "\n========== SEARCH REQUEST ==========\n";
            std::cout << "Enter Ticket Number: ";
            std::getline(std::cin, ticket);

            DocumentNode* result = documentList.FindById(ticket);

            if (result == nullptr) {
                std::cout << "\nNo matching document was found.\n";
            } else {
                std::cout << "\nDocument found!\n";
                std::cout << "Ticket Number: " << result->data.specialId << "\n";
                std::cout << "Resident Name: " << result->data.residentName << "\n";
                std::cout << "Document Type: " << result->data.documentType << "\n";
            }
        }

        else if (choice == 6) {
            requestQueue.displayQueue();
            std::cout << "\nTotal pending requests: " << requestQueue.size() << "\n";
        }

        else if (choice == 7) {
            std::cout << "\n========== UNDO HISTORY ==========\n";
            undoStack.display();
        }

        else if (choice == 8) {
            if (undoStack.isEmpty()) {
                std::cout << "\nThere are no actions available to undo.\n";
                continue;
            }

            Action lastAction = undoStack.pop();
            RequestNode* record =
                requestList.searchById(lastAction.Ticket_number);

            if (record != nullptr) {
                record->addStatus(statusFromText(lastAction.Previous_status),
                                  "Undo operation", "Barangay Staff");

                std::cout << "\nLast action successfully undone!\n";
                std::cout << "Ticket Number: " << lastAction.Ticket_number << "\n";
                std::cout << "Restored Status: " << lastAction.Previous_status << "\n";
            } else {
                std::cout << "\nThe related request record could not be found.\n";
            }
        }

        else if (choice == 9) {
            if (processedRequests.empty()) {
                std::cout << "\nThere are no released requests to sort yet.\n";
                continue;
            }

            std::cout << "\n========== SORT RELEASED REQUESTS ==========";
            std::cout << "\n[1] Sort by Date Submitted";
            std::cout << "\n[2] Sort by Document Type";
            std::cout << "\n[3] Sort by Ticket Number";
            std::cout << "\nEnter sorting option: ";

            int sortChoice;
            if (!(std::cin >> sortChoice)) {
                std::cout << "\nInvalid input. Please enter a number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Sort a copy so the original processed-request collection is retained.
            std::vector<DocReq*> sortedRequests = processedRequests;

            if (sortChoice == 1) {
                mergeSort(sortedRequests, byDate);
                std::cout << "\n--- RELEASED REQUESTS SORTED BY DATE ---\n";
            } else if (sortChoice == 2) {
                mergeSort(sortedRequests, byDocType);
                std::cout << "\n--- RELEASED REQUESTS SORTED BY DOCUMENT TYPE ---\n";
            } else if (sortChoice == 3) {
                mergeSort(sortedRequests, byTicketID);
                std::cout << "\n--- RELEASED REQUESTS SORTED BY TICKET NUMBER ---\n";
            } else {
                std::cout << "\nInvalid sorting option. Please select from 1 to 3.\n";
                continue;
            }

            std::cout << "\n";
            for (const DocReq* request : sortedRequests) {
                std::cout << "Ticket Number : " << request->ticketID;
                std::cout << "\nResident Name: " << request->residentName;
                std::cout << "\nDocument Type: " << request->documentType;
                std::cout << "\nDate Submitted: " << request->dateSubmitted;
                std::cout << "\n----------------------------------------\n";
            }
        }

        else if (choice == 10) {
            std::cout << "\nThank you for using e-BarangayDocs!\n";
        }

        else {
            std::cout << "\nInvalid choice. Please select from 1 to 10.\n";
        }

    } while (choice != 10);

    // Delete processed request nodes after the program finishes.
    for (DocReq* request : processedRequests) {
        delete request;
    }

    return 0;
}
