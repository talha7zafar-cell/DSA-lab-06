#include <iostream>
using namespace std;

struct Request {
    int id;
    int arrival;
    int processing;
};

class CircularQueue {
    Request *arr;
    int front, rear, count, capacity;

public:
    CircularQueue(int cap) {
        capacity = cap;
        arr = new Request[capacity];
        front = 0;
        rear = -1;
        count = 0;
    }

    ~CircularQueue() {
        delete[] arr;
    }

    bool isEmpty() { return count == 0; }
    bool isFull() { return count == capacity; }

    void resize(int newCapacity) {
        cout << "Resizing from " << capacity << " to " << newCapacity << endl;
        Request *temp = new Request[newCapacity];

        // copy elements in order starting from front
        for (int i = 0; i < count; i++) {
            temp[i] = arr[(front + i) % capacity];
        }

        delete[] arr;
        arr = temp;
        capacity = newCapacity;
        front = 0;
        rear = count - 1;
    }

    void enqueue(Request x) {
        if (isFull()) {
            resize(capacity * 2);     // double when full
        }
        rear = (rear + 1) % capacity;
        arr[rear] = x;
        count++;
    }

    Request dequeue() {
        Request r = {-1, 0, 0};
        if (isEmpty()) {
            cout << "Queue is empty" << endl;
            return r;
        }
        r = arr[front];
        front = (front + 1) % capacity;
        count--;

        // half the size when only 1/4 is used
        if (count > 0 && count <= capacity / 4 && capacity > 2) {
            resize(capacity / 2);
        }
        return r;
    }

    Request getFront() {
        if (isEmpty()) {
            Request r = {-1, 0, 0};
            return r;
        }
        return arr[front];
    }

    void display() {
        cout << "Queue (size " << count << ", capacity " << capacity << "): ";
        if (isEmpty()) {
            cout << "empty" << endl;
            return;
        }
        for (int i = 0; i < count; i++) {
            int index = (front + i) % capacity;
            cout << "[ID:" << arr[index].id << " A:" << arr[index].arrival
                 << " P:" << arr[index].processing << "] ";
        }
        cout << endl;
    }
};

CircularQueue q(2);
int currentTime = 0;
int totalWait = 0;
int served = 0;

void addRequest(int id, int arrival, int processing) {
    Request r = {id, arrival, processing};
    q.enqueue(r);
    cout << "Enqueued request " << id << endl;
    q.display();
    cout << endl;
}

void processRequest() {
    Request r = q.dequeue();
    if (r.id == -1) return;

    // printer can only start when request has arrived
    if (currentTime < r.arrival)
        currentTime = r.arrival;

    int wait = currentTime - r.arrival;
    totalWait += wait;
    served++;
    currentTime += r.processing;

    cout << "Dequeued request " << r.id << " (waited " << wait << ")" << endl;
    q.display();
    cout << endl;
}

int main() {
    // id, arrival time, processing time
    addRequest(1, 0, 4);
    addRequest(2, 1, 3);
    addRequest(3, 2, 2);
    addRequest(4, 3, 5);
    addRequest(5, 4, 1);

    cout << "Front request is: " << q.getFront().id << endl << endl;

    processRequest();
    processRequest();

    addRequest(6, 6, 2);
    addRequest(7, 8, 3);

    while (!q.isEmpty()) {
        processRequest();
    }

    cout << "Total requests served: " << served << endl;
    cout << "Average waiting time: " << (double)totalWait / served << endl;

    return 0;
}
