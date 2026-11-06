#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

#include "BINARY_SEARCH.h"
#include "Queue.h"
#include "MergeSort.h"

using namespace std;
using namespace chrono;

// QUEUE FUNCTION DEFINITIONS

RequestQueue::~RequestQueue() {
    while (!isEmpty()) {
        DocumentRequest* temp = dequeue();
        delete temp;
    }
}

bool RequestQueue::isEmpty() const {
    return front == nullptr;
}

void RequestQueue::enqueue(string id, string name, string type,
                           string date, string reqs) {
    DocumentRequest* node =
        new DocumentRequest(id, name, type, date, reqs);

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

    DocumentRequest* temp = front;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    temp->next = nullptr;
    --count;

    return temp;
}

// GENERATE TICKET NUMBER

string makeID(int i) {
    stringstream ss;

    ss << "BRG-2026-"
       << setfill('0') << setw(6) << i;

    return ss.str();
}

// MAIN BENCHMARK PROGRAM

int main() {

    int sizes[] = {100, 1000, 10000, 100000};

    cout << fixed << setprecision(6);

    cout << "\n========== RUNNING TIME RESULTS ==========\n";

    cout << left
         << setw(15) << "Input Size"
         << setw(20) << "Binary Search"
         << setw(20) << "Merge Sort"
         << setw(20) << "Queue Operations"
         << endl;

    for (int n : sizes) {

        // OPERATION 1: BINARY SEARCH

        DocumentList documents;

        for (int i = 1; i <= n; i++) {
            BarangayDocument doc;

            doc.specialId = makeID(i);
            doc.residentName = "Resident";
            doc.documentType = "Certificate";
            doc.dateSubmitted = i;
            doc.fee = 0.0;

            documents.append(doc);
        }

        string target = makeID(n / 2);

        auto start = high_resolution_clock::now();

        DocumentNode* result = documents.FindById(target);

        auto stop = high_resolution_clock::now();

        double searchTime =
            duration<double, milli>(stop - start).count();


        // OPERATION 2: MERGE SORT

        vector<DocReq*> requests;

        for (int i = n; i >= 1; i--) {
            requests.push_back(
                new DocReq(
                    makeID(i),
                    "Resident",
                    "Certificate",
                    "2026-11-06",
                    "Valid ID"
                )
            );
        }

        start = high_resolution_clock::now();

        mergeSort(requests, byTicketID);

        stop = high_resolution_clock::now();

        double sortTime =
            duration<double, milli>(stop - start).count();

        for (DocReq* r : requests) {
            delete r;
        }


        // OPERATION 3: QUEUE OPERATIONS

        RequestQueue queue;

        start = high_resolution_clock::now();

        for (int i = 1; i <= n; i++) {
            queue.enqueue(
                makeID(i),
                "Resident",
                "Certificate",
                "2026-11-06",
                "Valid ID"
            );
        }

        while (!queue.isEmpty()) {
            delete queue.dequeue();
        }

        stop = high_resolution_clock::now();

        double queueTime =
            duration<double, milli>(stop - start).count();


        // DISPLAY RESULTS

        cout << left
             << setw(15) << n
             << setw(20) << searchTime
             << setw(20) << sortTime
             << setw(20) << queueTime
             << endl;
    }

    cout << "\n==========================================\n";

    return 0;
}