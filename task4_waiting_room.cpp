#include <iostream>
using namespace std;

class WaitingRoom {
    int queue[100];
    int front, rear;
    int nextId;

public:
    WaitingRoom() {
        front = 0;
        rear = -1;
        nextId = 1;
    }

    bool isEmpty() {
        return front > rear;
    }

    void RegisterPatient() {
        if (rear == 99) {
            cout << "Waiting room is full" << endl;
            return;
        }
        rear++;
        queue[rear] = nextId;
        cout << "Patient registered with ID " << nextId << endl;
        nextId++;
    }

    void ServePatient() {
        if (isEmpty()) {
            cout << "No patient is waiting" << endl;
            return;
        }
        cout << "Serving patient ID " << queue[front] << endl;
        front++;

        // reset when queue becomes empty
        if (isEmpty()) {
            front = 0;
            rear = -1;
        }
    }

    void CancelAll() {
        front = 0;
        rear = -1;
        cout << "All appointments cancelled, doctor can go to lunch" << endl;
    }

    bool CanDoctorGoHome() {
        return isEmpty();
    }

    void ShowAllPatient() {
        if (isEmpty()) {
            cout << "No patient is waiting" << endl;
            return;
        }

        // copy to temp array and sort using bubble sort
        int temp[100];
        int n = 0;
        for (int i = front; i <= rear; i++) {
            temp[n] = queue[i];
            n++;
        }

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (temp[j] > temp[j + 1]) {
                    int t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }

        cout << "Waiting patients: ";
        for (int i = 0; i < n; i++)
            cout << temp[i] << " ";
        cout << endl;
    }
};

int main() {
    WaitingRoom room;
    int choice;

    do {
        cout << endl;
        cout << "1. Register Patient" << endl;
        cout << "2. Serve Patient" << endl;
        cout << "3. Cancel All" << endl;
        cout << "4. Can Doctor Go Home" << endl;
        cout << "5. Show All Patients" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) room.RegisterPatient();
        else if (choice == 2) room.ServePatient();
        else if (choice == 3) room.CancelAll();
        else if (choice == 4) {
            if (room.CanDoctorGoHome()) cout << "Yes" << endl;
            else cout << "No, patients are waiting" << endl;
        }
        else if (choice == 5) room.ShowAllPatient();
        else if (choice != 0) cout << "Invalid choice" << endl;

    } while (choice != 0);

    return 0;
}
