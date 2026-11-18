#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;

class Text {
    char* data;
    size_t len;

public:
    static int allocations;
    static int copies;

    Text(const char* s = "")
    {
        len = strlen(s);
        data = new char[len + 1];
        memcpy(data, s, len + 1);
        allocations++;
    }

    Text(const Text& o)
    {
        len = o.len;
        data = new char[len + 1];
        memcpy(data, o.data, len + 1);
        allocations++;
        copies++;
    }

    Text& operator=(const Text& o)
    {
        if (this == &o) return *this;
        char* fresh = new char[o.len + 1];
        memcpy(fresh, o.data, o.len + 1);
        delete[] data;
        data = fresh;
        len = o.len;
        allocations++;
        return *this;
    }

    ~Text() { delete[] data; }

    Text operator+(const Text& o) const
    {
        Text r;
        delete[] r.data;
        r.len = len + o.len;
        r.data = new char[r.len + 1];
        memcpy(r.data, data, len);
        memcpy(r.data + len, o.data, o.len + 1);
        return r;
    }

    Text& operator+=(const Text& o)
    {
        char* fresh = new char[len + o.len + 1];
        memcpy(fresh, data, len);
        memcpy(fresh + len, o.data, o.len + 1);
        delete[] data;
        data = fresh;
        len += o.len;
        allocations++;
        return *this;
    }

    bool operator==(const Text& o) const { return len == o.len && memcmp(data, o.data, len) == 0; }
    bool operator!=(const Text& o) const { return !(*this == o); }

    size_t length() const { return len; }

    friend ostream& operator<<(ostream& os, const Text& t)
    {
        return os << t.data;
    }
};

int Text::allocations = 0;
int Text::copies = 0;

Text plusByValue(Text a, const Text& b)
{
    a += b;
    return a;
}

int main()
{
    string m1, m2, m3;
    cout << "Enter message 1: ";
    getline(cin, m1);
    cout << "Enter message 2: ";
    getline(cin, m2);
    cout << "Enter message 3: ";
    getline(cin, m3);

    Text a(m1.c_str()), b(m2.c_str()), c(m3.c_str());

    cout << endl << "=== Part 1 : Operators ===" << endl;
    Text joined = a + b + c;
    cout << "a + b + c = " << joined << "  (length " << joined.length() << ")" << endl;
    cout << "a == b ? " << boolalpha << (a == b) << endl;
    cout << "a != b ? " << (a != b) << endl;
    Text copyOfA = a;
    cout << "copyOfA == a ? " << (copyOfA == a) << endl;

    cout << endl << "=== Part 2 : Empty and long messages ===" << endl;
    Text empty("");
    Text longText(string(500, 'x').c_str());
    cout << "empty + a      : " << (empty + a) << endl;
    cout << "a + empty      : " << (a + empty) << endl;
    cout << "empty length   : " << empty.length() << endl;
    cout << "long length    : " << longText.length() << endl;
    cout << "long + long    : " << (longText + longText).length() << " characters" << endl;

    cout << endl << "=== Part 3 : Self-assignment ===" << endl;
    Text s = a;
    s = s;
    cout << "after s = s    : " << s << endl;
    cout << "still equal to a ? " << (s == a) << endl;

    cout << endl << "=== Part 4 : Copies and allocations for a + b + c ===" << endl;
    Text::allocations = 0;
    Text::copies = 0;
    Text r1 = a + b + c;
    cout << "operator+ member    : allocations " << Text::allocations
         << ", copy constructor calls " << Text::copies << endl;

    Text::allocations = 0;
    Text::copies = 0;
    Text r2 = plusByValue(plusByValue(a, b), c);
    cout << "plusByValue (a += b): allocations " << Text::allocations
         << ", copy constructor calls " << Text::copies << endl;
    cout << "same result ? " << (r1 == r2) << endl;

    cout << endl << "=== Part 5 : Text vs std::string, 100,000 concatenations ===" << endl;
    const int N = 100000;
    Text piece("x");

    Text big;
    auto t1 = steady_clock::now();
    for (int i = 0; i < N; i++) big = big + piece;
    double msText = duration<double, milli>(steady_clock::now() - t1).count();

    Text big2;
    auto t2 = steady_clock::now();
    for (int i = 0; i < N; i++) big2 += piece;
    double msTextAppend = duration<double, milli>(steady_clock::now() - t2).count();

    string str;
    auto t3 = steady_clock::now();
    for (int i = 0; i < N; i++) str = str + "x";
    double msString = duration<double, milli>(steady_clock::now() - t3).count();

    string str2;
    auto t4 = steady_clock::now();
    for (int i = 0; i < N; i++) str2 += "x";
    double msStringAppend = duration<double, milli>(steady_clock::now() - t4).count();

    cout << "Text   big = big + piece : " << msText << " ms  (length " << big.length() << ")" << endl;
    cout << "Text   big += piece      : " << msTextAppend << " ms  (length " << big2.length() << ")" << endl;
    cout << "string s = s + \"x\"       : " << msString << " ms  (length " << str.size() << ")" << endl;
    cout << "string s += \"x\"          : " << msStringAppend << " ms  (length " << str2.size() << ")" << endl;

    return 0;
}