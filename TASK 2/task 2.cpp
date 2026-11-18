#include <chrono>
#include <iostream>
#include <string>

class Prescription {
    int n;
    std::string* medicines;

public:
    explicit Prescription(int count) : n(count), medicines(new std::string[count]) {}

#ifndef SHALLOW
    Prescription(const Prescription& other) : n(other.n), medicines(new std::string[other.n])
    {
        std::cout << "[copy constructor called]\n";
        for (int i = 0; i < n; i++)
            medicines[i] = other.medicines[i];
    }

    Prescription& operator=(const Prescription& other)
    {
        if (this == &other) {
            std::cout << "[self-assignment detected, nothing done]\n";
            return *this;
        }
        std::string* fresh = new std::string[other.n];
        for (int i = 0; i < other.n; i++)
            fresh[i] = other.medicines[i];
        delete[] medicines;
        medicines = fresh;
        n = other.n;
        return *this;
    }
#endif

    ~Prescription()
    {
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
};

void byValue(Prescription p) {}
void byConstRef(const Prescription& p) {}
Prescription makeCopy(const Prescription& p) { Prescription c = p; return c; }

int main()
{
    std::cout << "=== Part 1: Complete the Code ===\n";
    {
        Prescription p(2);
        p.set(0, "Paracetamol");
        p.set(1, "ORS");
        Prescription q = p;
        q.set(1, "Cetirizine");
        Prescription r(1);
        r = p;
        p.set(0, "Ibuprofen");
        std::cout << p.get(0) << ' ' << p.get(1) << " | " << q.get(0) << ' ' << q.get(1)
                  << " | " << r.get(0) << ' ' << r.get(1) << '\n';
    }

#ifndef SHALLOW
    std::cout << "\n=== Part 2: Copy count ===\n";
    Prescription a(3);
    a.set(0, "Paracetamol");
    a.set(1, "ORS");
    a.set(2, "Ibuprofen");
    std::cout << "Passed by value:\n";
    byValue(a);
    std::cout << "Passed by const reference:\n";
    byConstRef(a);
    std::cout << "Returned from function:\n";
    Prescription t = makeCopy(a);

    std::cout << "\n=== Part 3: Self-assignment ===\n";
    a = a;
    std::cout << "a after a = a: " << a.get(0) << ' ' << a.get(1) << ' ' << a.get(2) << '\n';

    std::cout << "\n=== Part 4: Copy time ===\n";
    int sizes[] = {10, 1000, 100000};
    for (int k = 0; k < 3; k++) {
        int s = sizes[k];
        Prescription big(s);
        for (int i = 0; i < s; i++)
            big.set(i, "Medicine" + std::to_string(i));
        auto start = std::chrono::high_resolution_clock::now();
        Prescription* copy = new Prescription(big);
        auto stop = std::chrono::high_resolution_clock::now();
        double us = std::chrono::duration<double, std::micro>(stop - start).count();
        std::cout << s << " medicines: " << us << " microseconds\n";
        delete copy;
    }
#endif
    return 0;
}