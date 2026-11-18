
#include <iostream>
#include <string>
using namespace std;

class Book
{
    string title;
    int copies;

public:
    Book() : Book("Unknown", 0) {}

    Book(string t) : Book(t, 1) {}

    Book(string t, int c) : title(t), copies(c >= 0 ? c : 0) {}

    void display()
    {
        cout << "Book Title: " << title << endl;
        cout << "Number of Copies: " << copies << endl;
    }
};

int main()
{
    string title;
    int copies;

    cout << "Enter book title: ";
    getline(cin, title);
    Book b1(title);

    cout << "Enter book title: ";
    getline(cin, title);

    cout << "Enter number of copies: ";
    cin >> copies;
    Book b2(title, copies);

    cout << "\nBook Details:\n";
    b1.display();
    b2.display();

    return 0;
}
