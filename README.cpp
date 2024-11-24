#LibraryBorrowingSystem
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

// Book Class
class Book {
public:
    int bookID;
    string title, author, genre;
    bool isAvailable;

    Book(int id, string t, string a, string g, bool available = true)
        : bookID(id), title(t), author(a), genre(g), isAvailable(available) {}

    void displayDetails() {
        cout << setw(5) << bookID << setw(20) << title << setw(20)
             << author << setw(15) << genre << setw(10)
             << (isAvailable ? "Available" : "Borrowed") << endl;
    }
};

// User Class
class User {
public:
    int userID;
    string name;
    vector<int> borrowedBooks;
    double fine;

    User(int id, string n) : userID(id), name(n), fine(0.0) {}

    void displayDetails() {
        cout << "User ID: " << userID << ", Name: " << name << ", Fine: $" << fine << endl;
        if (!borrowedBooks.empty()) {
            cout << "Borrowed Books: ";
            for (int bookID : borrowedBooks)
                cout << bookID << " ";
            cout << endl;
        }
    }
};

// BorrowingSystem Class
class BorrowingSystem {
private:
    vector<Book> books;
    vector<User> users;

    void saveDataToFile() {
        // Save books
        ofstream bookFile("books.txt");
        for (auto &book : books) {
            bookFile << book.bookID << "," << book.title << "," << book.author
                     << "," << book.genre << "," << book.isAvailable << endl;
        }
        bookFile.close();

        // Save users
        ofstream userFile("users.txt");
        for (auto &user : users) {
            userFile << user.userID << "," << user.name << "," << user.fine;
            for (int bookID : user.borrowedBooks) {
                userFile << "," << bookID;
            }
            userFile << endl;
        }
        userFile.close();
    }

    void loadDataFromFile() {
        // Load books
        ifstream bookFile("books.txt");
        if (bookFile.is_open()) {
            string line;
            while (getline(bookFile, line)) {
                string title, author, genre;
                int id;
                bool available;
                sscanf(line.c_str(), "%d,%[^,],%[^,],%[^,],%d", &id, title.c_str(),
                       author.c_str(), genre.c_str(), &available);
                books.emplace_back(id, title, author, genre, available);
            }
            bookFile.close();
        }

        // Load users
        ifstream userFile("users.txt");
        if (userFile.is_open()) {
            string line;
            while (getline(userFile, line)) {
                string name;
                int id;
                double fine;
                vector<int> borrowed;
                sscanf(line.c_str(), "%d,%[^,],%lf", &id, name.c_str(), &fine);
                users.emplace_back(id, name);
                users.back().fine = fine;
            }
            userFile.close();
        }
    }

public:
    BorrowingSystem() {
        loadDataFromFile();
    }

    ~BorrowingSystem() {
        saveDataToFile();
    }

    void addBook() {
        int id;
        string title, author, genre;
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Genre: ";
        getline(cin, genre);
        books.emplace_back(id, title, author, genre);
        cout << "Book added successfully.\n";
    }

    void listBooks() {
        cout << setw(5) << "ID" << setw(20) << "Title" << setw(20) << "Author"
             << setw(15) << "Genre" << setw(10) << "Status" << endl;
        for (auto &book : books) {
            book.displayDetails();
        }
    }

    void borrowBook(int userID) {
        int bookID;
        cout << "Enter Book ID to borrow: ";
        cin >> bookID;
        for (auto &book : books) {
            if (book.bookID == bookID) {
                if (book.isAvailable) {
                    book.isAvailable = false;
                    for (auto &user : users) {
                        if (user.userID == userID) {
                            user.borrowedBooks.push_back(bookID);
                            cout << "Book borrowed successfully.\n";
                            return;
                        }
                    }
                } else {
                    cout << "Book is currently unavailable.\n";
                    return;
                }
            }
        }
        cout << "Book not found.\n";
    }

    void returnBook(int userID) {
        int bookID;
        cout << "Enter Book ID to return: ";
        cin >> bookID;
        for (auto &user : users) {
            if (user.userID == userID) {
                for (size_t i = 0; i < user.borrowedBooks.size(); ++i) {
                    if (user.borrowedBooks[i] == bookID) {
                        user.borrowedBooks.erase(user.borrowedBooks.begin() + i);
                        for (auto &book : books) {
                            if (book.bookID == bookID) {
                                book.isAvailable = true;
                                cout << "Book returned successfully.\n";
                                return;
                            }
                        }
                    }
                }
            }
        }
        cout << "Book not found in user's borrowed list.\n";
    }

    void registerUser() {
        int id;
        string name;
        cout << "Enter User ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        users.emplace_back(id, name);
        cout << "User registered successfully.\n";
    }

    void listUsers() {
        for (auto &user : users) {
            user.displayDetails();
        }
    }
};

// Main Function
int main() {
    BorrowingSystem system;
    int choice, userID;

    while (true) {
        cout << "\nMenu:\n1. Add Book\n2. List Books\n3. Borrow Book\n4. Return Book\n5. Register User\n6. List Users\n7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            system.addBook();
            break;
        case 2:
            system.listBooks();
            break;
        case 3:
            cout << "Enter User ID: ";
            cin >> userID;
            system.borrowBook(userID);
            break;
        case 4:
            cout << "Enter User ID: ";
            cin >> userID;
            system.returnBook(userID);
            break;
        case 5:
            system.registerUser();
            break;
        case 6:
            system.listUsers();
            break;
        case 7:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}
