#ifndef MERGESORT_H
#define MERGESORT_H

#include <vector>
#include <string>
#include "Queue.h"

using DocReq = DocumentRequest;

inline bool byDate(const DocReq* a, const DocReq* b) {
    return a->dateSubmitted < b->dateSubmitted;
}

inline bool byDocType(const DocReq* a, const DocReq* b) {
    return a->documentType < b->documentType;
}

inline bool byTicketID(const DocReq* a, const DocReq* b) {
    return a->ticketID < b->ticketID;
}

template <typename Compare>
void merge(std::vector<DocReq*>& arr, int left, int mid, int right, Compare comp) {
    std::vector<DocReq*> temp;
    temp.reserve(right - left + 1);

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (comp(arr[j], arr[i])) temp.push_back(arr[j++]);
        else                      temp.push_back(arr[i++]);
    }
    while (i <= mid)   temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = 0; k < static_cast<int>(temp.size()); k++) {
        arr[left + k] = temp[k];
    }
}

template <typename Compare>
void mergeSortHelper(std::vector<DocReq*>& arr, int left, int right, Compare comp) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSortHelper(arr, left, mid, comp);
    mergeSortHelper(arr, mid + 1, right, comp);
    merge(arr, left, mid, right, comp);
}

template <typename Compare>
void mergeSort(std::vector<DocReq*>& arr, Compare comp) {
    if (arr.size() < 2) return;
    mergeSortHelper(arr, 0, static_cast<int>(arr.size()) - 1, comp);
}

#endif
