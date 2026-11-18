#include <iostream>
#include <string>
using namespace std;

namespace predictDemo {
    struct Logger {
        string n;
        Logger(const string& s) : n(s) { cout << '+' << n << ' '; }
        Logger(const Logger& o) : n(o.n + "c") { cout << "copy" << n << ' '; }
        ~Logger() { cout << '-' << n << ' '; }
    };

    void use(Logger) { cout << "use "; }
}

class Event {
    string name;
    int rows;
    int cols;
    char** seats;
    int booked;

public:
    Event(const string& n, int r, int c) : name(n), rows(0), cols(0), seats(nullptr), booked(0)
    {
        if (r > 0 && c > 0) {
            rows = r;
            cols = c;
            seats = new char*[rows];
            for (int i = 0; i < rows; i++) {
                seats[i] = new char[cols];
                for (int j = 0; j < cols; j++) seats[i][j] = '.';
            }
        }
        cout << "  [" << name << "] created: " << rows << " x " << cols << " seats" << endl;
    }

    ~Event()
    {
        cout << "  [" << name << "] closing summary: " << booked << " of "
             << rows * cols << " seats booked" << endl;
        for (int i = 0; i < rows; i++) delete[] seats[i];
        delete[] seats;
    }

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    bool book(int r, int c)
    {
        if (r < 0 || r >= rows || c < 0 || c >= cols || seats[r][c] == 'X') return false;
        seats[r][c] = 'X';
        booked++;
        return true;
    }

    bool cancel(int r, int c)
    {
        if (r < 0 || r >= rows || c < 0 || c >= cols || seats[r][c] != 'X') return false;
        seats[r][c] = '.';
        booked--;
        return true;
    }

    void show() const
    {
        for (int i = 0; i < rows; i++) {
            cout << "    ";
            for (int j = 0; j < cols; j++) cout << seats[i][j] << ' ';
            cout << endl;
        }
    }

    static long long memoryBytes(int r, int c)
    {
        return (long long)r * sizeof(char*) + (long long)r * c;
    }
};

int main()
{
    cout << "=== Part 1 : Predict the Output ===" << endl;
    {
        using namespace predictDemo;
        Logger a("A");
        {
            Logger b("B");
        }
        use(a);
        Logger* p = new Logger("P");
        delete p;
        cout << "end ";
    }
    cout << endl;

    int r, c;
    cout << endl << "Enter rows and seats per row: ";
    cin >> r >> c;

    cout << endl << "=== Part 2 : Stack object ===" << endl;
    Event stackEvent("Stack event", r, c);
    stackEvent.book(0, 0);
    stackEvent.book(1, 2);
    stackEvent.book(0, 0) ? cout << "  double booking allowed" << endl
                          : cout << "  seat (0,0) already booked" << endl;
    stackEvent.cancel(1, 2);
    stackEvent.show();

    cout << endl << "=== Part 3 : Block-scope object ===" << endl;
    {
        Event blockEvent("Block event", 2, 3);
        blockEvent.book(1, 1);
        cout << "  leaving the block" << endl;
    }

    cout << endl << "=== Part 4 : Heap object (new / delete) ===" << endl;
    Event* heapEvent = new Event("Heap event", 2, 2);
    heapEvent->book(0, 1);
    cout << "  calling delete" << endl;
    delete heapEvent;

    cout << endl << "=== Part 5 : Event with zero rows ===" << endl;
    {
        Event emptyEvent("Empty event", 0, 20);
        cout << "  booking attempt: " << (emptyEvent.book(0, 0) ? "booked" : "refused") << endl;
    }

    cout << endl << "=== Part 6 : Memory for the seat map ===" << endl;
    cout << "10 x 20       : " << Event::memoryBytes(10, 20) << " bytes" << endl;
    cout << "100 x 200     : " << Event::memoryBytes(100, 200) << " bytes" << endl;
    cout << "1000 x 2000   : " << Event::memoryBytes(1000, 2000) << " bytes" << endl;

    cout << endl << "=== End of main ===" << endl;
    return 0;
}