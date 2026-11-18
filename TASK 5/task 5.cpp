#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
using namespace std::chrono;

namespace predictDemo {
    struct Counter {
        int v;
        Counter& operator++() { ++v; return *this; }
        Counter operator++(int) { Counter old = *this; ++v; return old; }
    };
}

class Time {
    int h, m, s;

    void normalise()
    {
        long long total = ((long long)h * 3600 + m * 60 + s) % 86400;
        if (total < 0) total += 86400;
        h = total / 3600;
        m = (total % 3600) / 60;
        s = total % 60;
    }

public:
    Time(int hh = 0, int mm = 0, int ss = 0) : h(hh), m(mm), s(ss) { normalise(); }

    Time& operator++()
    {
        ++s;
        normalise();
        return *this;
    }

    Time operator++(int)
    {
        Time old = *this;
        ++s;
        normalise();
        return old;
    }

    bool operator==(const Time& o) const { return h == o.h && m == o.m && s == o.s; }

    bool operator<(const Time& o) const
    {
        if (h != o.h) return h < o.h;
        if (m != o.m) return m < o.m;
        return s < o.s;
    }

    friend ostream& operator<<(ostream& os, const Time& t)
    {
        os << t.h << ':' << setfill('0') << setw(2) << t.m << ':' << setw(2) << t.s << setfill(' ');
        return os;
    }
};

struct Track {
    string title;
    int seconds;
};

class Playlist {
    static const int MAX = 10;
    Track tracks[MAX];
    int count;
    Track invalid;

public:
    Playlist() : count(0), invalid{"(invalid track)", 0} {}

    void add(const string& t, int sec)
    {
        if (count < MAX) {
            tracks[count].title = t;
            tracks[count].seconds = sec;
            count++;
        }
    }

    int size() const { return count; }

    Track& operator[](int i)
    {
        if (i < 0 || i >= count) {
            cout << "  error: index " << i << " is out of range (0 to " << count - 1 << ")" << endl;
            return invalid;
        }
        return tracks[i];
    }

    const Track& operator[](int i) const
    {
        if (i < 0 || i >= count) {
            cout << "  error: index " << i << " is out of range (0 to " << count - 1 << ")" << endl;
            return invalid;
        }
        return tracks[i];
    }

    Track& at(int i)
    {
        if (i < 0 || i >= count) throw out_of_range("track index out of range");
        return tracks[i];
    }

    Track& unchecked(int i) { return tracks[i]; }
};

int main()
{
    cout << "=== Part 1 : Predict the Output ===" << endl;
    predictDemo::Counter c{5};
    predictDemo::Counter a = c++;
    predictDemo::Counter b = ++c;
    cout << a.v << ' ' << b.v << ' ' << c.v << '\n';

    cout << endl << "=== Part 2 : Time ++ and comparisons ===" << endl;
    Time t(0, 10, 5);
    Time x = t++;
    Time y = ++t;
    cout << "t after both : " << t << endl;
    cout << "x = t++      : " << x << "  (old value)" << endl;
    cout << "y = ++t      : " << y << "  (new value)" << endl;
    cout << x << " < " << y << " ? " << boolalpha << (x < y) << endl;
    cout << x << " == " << x << " ? " << (x == x) << endl;

    cout << endl << "=== Part 3 : Roll-over ===" << endl;
    Time r1(0, 59, 59), r2(23, 59, 59);
    cout << r1 << " + 1 second = " << ++r1 << endl;
    cout << r2 << " + 1 second = " << ++r2 << endl;

    cout << endl << "=== Part 4 : Playback simulation ===" << endl;
    Playlist list;
    int n1, n2, n3;
    cout << "Enter lengths in seconds of three tracks: ";
    cin >> n1 >> n2 >> n3;
    list.add("Track One", n1);
    list.add("Track Two", n2);
    list.add("Track Three", n3);

    Time total;
    for (int i = 0; i < list.size(); i++) {
        Time pos;
        Time end(0, 0, list[i].seconds);
        while (pos < end) ++pos;
        cout << list[i].title << " finished at " << pos << (pos == end ? "  (matches length)" : "") << endl;
        for (int k = 0; k < list[i].seconds; k++) ++total;
    }
    cout << "Total playing time: " << total << endl;

    cout << endl << "=== Part 5 : Range checks ===" << endl;
    const Playlist& view = list;
    cout << "list[-1]:" << endl;
    string bad1 = list[-1].title;
    cout << "  returned " << bad1 << endl;
    cout << "list[3]:" << endl;
    string bad2 = list[3].title;
    cout << "  returned " << bad2 << endl;
    cout << "const [1]   : " << view[1].title << endl;
    try {
        list.at(7);
    } catch (const out_of_range& e) {
        cout << "  at(7) threw: " << e.what() << endl;
    }

    cout << endl << "=== Part 6 : Timing, 10,000,000 operations ===" << endl;
    const int N = 10000000;

    Time pre, post;
    auto t1 = steady_clock::now();
    for (int i = 0; i < N; i++) ++pre;
    double msPre = duration<double, milli>(steady_clock::now() - t1).count();

    auto t2 = steady_clock::now();
    for (int i = 0; i < N; i++) post++;
    double msPost = duration<double, milli>(steady_clock::now() - t2).count();

    cout << "Prefix  ++t : " << msPre << " ms   (" << pre << ")" << endl;
    cout << "Postfix t++ : " << msPost << " ms   (" << post << ")" << endl;

    long long sum = 0;
    auto t3 = steady_clock::now();
    for (int i = 0; i < N; i++) sum += list.unchecked(i % 3).seconds;
    double msRaw = duration<double, milli>(steady_clock::now() - t3).count();

    auto t4 = steady_clock::now();
    for (int i = 0; i < N; i++) sum += list[i % 3].seconds;
    double msChecked = duration<double, milli>(steady_clock::now() - t4).count();

    auto t5 = steady_clock::now();
    for (int i = 0; i < N; i++) sum += list.at(i % 3).seconds;
    double msAt = duration<double, milli>(steady_clock::now() - t5).count();

    cout << "Unchecked access : " << msRaw << " ms" << endl;
    cout << "Checked [] access: " << msChecked << " ms" << endl;
    cout << "at() access      : " << msAt << " ms" << endl;
    cout << "Check sum        : " << sum << endl;

    return 0;
}