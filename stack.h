#ifndef STACK_H
#define STACK_H
#include <string>
#include <iostream>

struct Action {
    std::string Action_name;
    std::string Ticket_number;
    std::string Previous_status;
    std::string Updated_status;
};

class Stack {
    private:
    static const int MAX = 100;
    Action arr[MAX];
    int top;

    public:
    Stack() {
        top = -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX - 1;
    }

    void push(Action action) {
        if (isFull()) {
            std::cout << "The stack is full!\n";
            return;
        }
        arr[++top] = action;
        std::cout << "This action has been successfully added!\n";
    }

    Action pop() {
        if (isEmpty()) {
            std::cout << "There is no actions available to undo!\n";
            return {"", "", "", ""};
        }
        return arr[top--];
    }

    void peek() {
        if (isEmpty()) {
            std::cout << "The stack is empty!\n";
            return;
        }
        std::cout << "\nLatest Action: " << arr[top].Action_name;
        std::cout << "\nTicket Number: " << arr[top].Ticket_number;
        std::cout << "\nPrevious Status: " << arr[top].Previous_status;
        std::cout << "\nUpdated Status: " << arr[top].Updated_status;
        std::cout << std::endl;
    }

    void display() {
        if (isEmpty()) {
            std::cout << "there are no recorded actions!\n";
            return;
        }
        std::cout << "\n======= AVAILABLE UNDO ACTIONS =======\n";

        for (int i = top; i >= 0; i--) {
            std::cout << "\nAction Taken: " << arr[i].Action_name;
            std::cout << "\nTicket Number: " << arr[i].Ticket_number;
            std::cout << "\nPrevious Status: " << arr[i].Previous_status;
            std::cout << "\nUpdated Status: " << arr[i].Updated_status;
            std::cout << std::endl;
        }
    }
};

#endif
