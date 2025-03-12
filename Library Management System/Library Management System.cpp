#include <iostream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

int users = 0;
int count_attempt = 0;
int curr_user = 0;
map<string, string> issued;
string loggedin_user = "";

class Stack {
private:
    vector<string> history;
public:
    void push(const string& action) {
        history.push_back(action);
    }
    void pop() {
        if (history.empty()) {
            cout << "No actions in history!" << endl;
            return;
        }
    }
    void display() {
        if (history.empty()) {
            cout << "No actions in history!" << endl;
            return;
        }
        cout << "Action History:" << endl;
        for (auto& action : history) {
            cout << action << endl;
        }
    }
    void clearHistory() {
        history.clear();
        cout << "Action history cleared!" << endl;
    }
};

Stack actionHistory;

class Queue {
private:
    vector<string> queue;
public:
    void enqueue(const string& action) {
        queue.push_back(action);
        cout << "Added to queue: " << action << endl;
        actionHistory.push("Added to queue: " + action);
    }
    void dequeue() {
        if (queue.empty()) {
            cout << "The queue is empty!" << endl;
            actionHistory.push("Tried to remove from queue but queue was empty");
            return;
        }
        cout << "Removed from queue: " << queue.front() << endl;
        actionHistory.push("Removed from queue: " + queue.front());
        queue.erase(queue.begin());
    }
    void displayQueue() {
        if (queue.empty()) {
            cout << "The queue is empty!" << endl;
            actionHistory.push("Displayed queue");
            return;
        }
        cout << "Current Queue:" << endl;
        for (const auto& action : queue) {
            cout << action << endl;
        }
    }
};

Queue actionQueue;

struct BookNode {
    int bookNo;
    string bookName;
    string authorName;
    string issuedBy;
    BookNode* next;

    BookNode(int bookNo, string bookName, string authorName)
        : bookNo(bookNo), bookName(bookName), authorName(authorName), issuedBy("None"), next(nullptr) {}
};

class LinkedList {
private:
    BookNode* head;
public:
    LinkedList() : head(nullptr) {}

    void addBook(int bookNo, string bookName, string authorName) {
        BookNode* newBook = new BookNode(bookNo, bookName, authorName);
        if (!head) {
            head = newBook;
        }
        else {
            BookNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newBook;
        }
        cout << "Book added successfully!" << endl;
    }
    void displayBooks() {
        if (!head) {
            cout << "No books available!" << endl;
            return;
        }
        BookNode* temp = head;
        while (temp) {
            cout << "No: " << temp->bookNo
                << ", Name: " << temp->bookName
                << ", Author: " << temp->authorName
                << ", Issued by: " << temp->issuedBy << endl;
            temp = temp->next;
        }
    }
    BookNode* searchBook(int bno) {
        BookNode* temp = head;
        while (temp) {
            if (temp->bookNo == bno) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }
};

class User {
private:
    string username;
    string password;
public:
    static vector<pair<string, string>> userDB;

    void adduser() {
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        for (auto& user : userDB) {
            if (user.first == username) {
                cout << " \t \t \t \t \t Username already exists. Try again." << endl;
                return;
            }
        }
        userDB.push_back(make_pair(username, password));
        cout << " \t \t \t \t \t Record Inserted Successfully" << endl;
        users++;
    }
    void login() {
        if (curr_user == 1) {
            cout << " \t \t \t \t \t Already logged in" << endl;
            return;
        }
        if (count_attempt == 3) {
            cout << endl << endl;
            cout << " \t \t \t \t \t No more attempts" << endl;
            exit(0);
        }
        cout << "Enter Username and Password to Login" << endl;
        string user_name, pass_word;
        cout << "Username: ";
        cin >> user_name;
        cout << "Password: ";
        cin >> pass_word;

        bool found = false;
        for (auto& user : userDB) {
            if (user.first == user_name && user.second == pass_word) {
                loggedin_user = user.first;
                cout << " \t \t \t \t \t \t Login Successful" << endl;
                cout << " \t \t \t \t \t \t WELCOME " << loggedin_user << endl;
                curr_user = 1;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << " \t \t \t \t \t \t Incorrect Username or Password" << endl;
            count_attempt++;
            login();
        }
    }
    void logout() {
        curr_user = 0;
        loggedin_user = "";
        cout << " \t \t \t \t \t Logged out successfully." << endl;
    }
};

vector<pair<string, string>> User::userDB;

class Book {
private:
    LinkedList bookList;
public:
    void addbook() {
        if (loggedin_user != "Fatima") {
            cout << " \t \t \t \t \t Not Authorized. Only admin can add new books" << endl;
            return;
        }

        int bno;
        string bookname, aname;
        cout << "Enter book number: ";
        cin >> bno;
        cin.ignore();
        cout << "Enter book name: ";
        getline(cin, bookname);
        cout << "Enter author name: ";
        getline(cin, aname);

        bookList.addBook(bno, bookname, aname);
        actionHistory.push("Added Book: " + bookname);
    }
    void allbooks() {
        bookList.displayBooks();
        actionHistory.push("All Books Displayed");
    }
    void searchbook() {
        int bno;
        cout << "Enter book number to search: ";
        cin >> bno;

        BookNode* book = bookList.searchBook(bno);
        if (book) {
            cout << "Book Found: No: " << book->bookNo
                << ", Name: " << book->bookName
                << ", Author: " << book->authorName
                << ", Issued by: " << book->issuedBy << endl;
            actionHistory.push("Searched Book: " + book->bookName);
        }
        else {
            cout << "Book Not Found!" << endl;
        }
    }
    void issueBook() {
        if (loggedin_user.empty()) {
            cout << " \t \t \t \t \t Please login to issue a book!" << endl;
            return;
        }
        int bno;
        cout << "Enter the book number to issue: ";
        cin >> bno;

        BookNode* book = bookList.searchBook(bno);
        if (book) {
            if (book->issuedBy == "None") {
                book->issuedBy = loggedin_user;
                issued[loggedin_user] = to_string(bno);
                cout << " \t \t \t \t \t Book issued successfully to " << loggedin_user << endl;
                actionHistory.push("Issued Book: " + book->bookName);
            }
            else {
                cout << " \t \t \t \t \t Book is already issued by another user: " << book->issuedBy << endl;
            }
        }
        else {
            cout << " \t \t \t \t \t Book not found!" << endl;
        }
    }

    void returnBook() {
        if (loggedin_user.empty()) {
            cout << " \t \t \t \t \t Please login to return a book!" << endl;
            return;
        }

        if (issued.find(loggedin_user) == issued.end()) {
            cout << " \t \t \t \t \t No book to return for " << loggedin_user << endl;
            return;
        }

        int bno = stoll(issued[loggedin_user]);
        BookNode* book = bookList.searchBook(bno);

        if (book) {
            book->issuedBy = "None";
            issued.erase(loggedin_user);
            cout << " \t \t \t \t \t Book returned successfully by " << loggedin_user << endl;
            actionHistory.push("Returned Book: " + book->bookName);
        }
        else {
            cout << " \t \t \t \t \t Book not found in the library!" << endl;
        }
    }
};

int main() {
    User::userDB.push_back({ "Fatima", "2910" });

    cout << "--------------------------------------------------------------------------------------------------------------------- " << endl << endl;
    cout << "|                                                 LIBRARY                                                           | " << endl << endl;
    cout << "|                                                MANAGEMENT                                                         | " << endl << endl;
    cout << "|                                                 SYSTEM                                                            | " << endl << endl;
    cout << "|                                                                                                                   | " << endl << endl;
    cout << "|                                        click ''y'' to show the menu                                               | " << endl << endl;
    cout << "|                                        click ''n'' to exit the program                                            | " << endl << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------- " << endl << endl;

    char startOption;
    cout << "Enter your choice (Y/y || N/n): ";
    cin >> startOption;

    if (startOption == 'n' || startOption == 'N') {
        cout << "Exiting the program. Goodbye!" << endl;
        return 0;
    }
    else if (startOption != 'y' && startOption != 'Y') {
        cout << "\nYou can only enter y or n!\n Try again!!\n" << endl;
        cout << "Enter your choice (y/n): ";
        cin >> startOption;
    }

    User u1;
    Book b1;

    int x;
    int choice = 0;
    do {
        cout << "\nMenu:\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. All Books\n";
        cout << "4. Search Book\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. View Action History\n";
        cout << "8. Clear Action History\n";
        cout << "9. Add Book\n";
        cout << "10. Manage Queue(Add, Remove, Display)\n";
        cout << "11. Logout\n";
        cout << "12. Exit\n";
        cin >> x;

        switch (x) {
        case 1: u1.login(); break;
        case 2: u1.adduser(); break;
        case 3: b1.allbooks(); break;
        case 4: b1.searchbook(); break;
        case 5: b1.issueBook(); break;
        case 6: b1.returnBook(); break;
        case 7: actionHistory.display(); break;
        case 8: actionHistory.clearHistory(); break;
        case 9: b1.addbook(); break;
        case 10: {
            cout << "1. Add book to Queue\n";
            cout << "2. Remove book from Queue\n";
            cout << "3. Display all the books in Queue\n";
            int qChoice;
            cin >> qChoice;
            if (qChoice == 1) {
                string action;
                cin.ignore();
                cout << "Enter book to add to queue: ";
                getline(cin, action);
                actionQueue.enqueue(action);
            }
            else if (qChoice == 2) {
                actionQueue.dequeue();
            }
            else if (qChoice == 3) {
                actionQueue.displayQueue();
            }
            else {
                cout << "Invalid choice for Queue." << endl;
            }
            break;
        }
        case 11: u1.logout(); break;
        case 12: choice = 1; break;
        default: cout << "Invalid option. Try again." << endl; break;
        }
    } while (choice == 0);

    return 0;
}