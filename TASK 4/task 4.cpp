#include <chrono>
#include <iomanip>
#include <iostream>
using namespace std;
using namespace std::chrono;

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    Money operator+(const Money& o) const
    {
        Money r;
        r.paise = paise + o.paise;
        return r;
    }

    Money operator-(const Money& o) const
    {
        Money r;
        r.paise = paise - o.paise;
        return r;
    }

    Money& operator+=(const Money& o)
    {
        paise += o.paise;
        return *this;
    }

    Money operator*(int times) const
    {
        Money r;
        r.paise = paise * times;
        return r;
    }

    bool operator==(const Money& o) const { return paise == o.paise; }
    bool operator<(const Money& o) const { return paise < o.paise; }

    Money add(const Money& o) const
    {
        Money r;
        r.paise = paise + o.paise;
        return r;
    }

    friend ostream& operator<<(ostream& os, const Money& m)
    {
        long long v = m.paise < 0 ? -m.paise : m.paise;
        os << (m.paise < 0 ? "-Rs " : "Rs ") << v / 100 << '.'
           << setw(2) << setfill('0') << v % 100 << setfill(' ');
        return os;
    }

    friend istream& operator>>(istream& is, Money& m)
    {
        long long rupees;
        int p;
        is >> rupees >> p;
        m = Money(rupees, p);
        return is;
    }
};

int main()
{
    cout << "=== Part 1 : Complete the Code ===" << endl;
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    cout << c << '\n' << boolalpha << (c == Money(200)) << '\n';

    cout << endl << "=== Part 2 : Paise carry into rupees ===" << endl;
    Money x(0, 75), y(0, 50);
    cout << x << " + " << y << " = " << x + y << endl;

    cout << endl << "=== Part 3 : Negative result ===" << endl;
    Money small(5), big(8);
    cout << small << " - " << big << " = " << small - big << endl;
    cout << small << " < " << big << " ? " << (small < big) << endl;

    cout << endl << "=== Part 4 : Bill calculation ===" << endl;
    Money item1, item2, item3, discount;
    cout << "Enter price of item 1 (rupees paise): ";
    cin >> item1;
    cout << "Enter price of item 2 (rupees paise): ";
    cin >> item2;
    cout << "Enter price of item 3 (rupees paise): ";
    cin >> item3;
    cout << "Enter discount (rupees paise): ";
    cin >> discount;

    Money bill = item1 + item2 + item3 - discount;
    Money oldStyle = item1.add(item2).add(item3);
    Money running;
    running += item1;
    running += item2;
    running += item3;

    cout << "Bill with operators      : " << bill << endl;
    cout << "Same with add() calls    : " << oldStyle - discount << endl;
    cout << "Total using +=           : " << running << endl;
    cout << "Three copies of item 1   : " << item1 * 3 << endl;

    cout << endl << "=== Part 5 : Money vs double, 1,000,000 additions of Rs 0.10 ===" << endl;
    const int N = 1000000;
    Money total;
    Money tenPaise(0, 10);
    auto t1 = steady_clock::now();
    for (int i = 0; i < N; i++) total += tenPaise;
    double msMoney = duration<double, milli>(steady_clock::now() - t1).count();

    double dTotal = 0;
    auto t2 = steady_clock::now();
    for (int i = 0; i < N; i++) dTotal += 0.10;
    double msDouble = duration<double, milli>(steady_clock::now() - t2).count();

    cout << "Money  : " << total << "   (" << msMoney << " ms)" << endl;
    cout << "double : Rs " << fixed << setprecision(6) << dTotal << "   (" << msDouble << " ms)" << endl;
    cout << "double equals 100000 exactly? " << (dTotal == 100000.0) << endl;

    return 0;
}