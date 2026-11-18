#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>
using namespace std;

class Complex {
    double re, im;

public:
    Complex() : re(0), im(0) {}
    Complex(double r) : re(r), im(0) {}
    Complex(double r, double i) : re(r), im(i) {}

    double real() const { return re; }
    double imag() const { return im; }
    double magnitude() const { return hypot(re, im); }

    friend Complex operator+(const Complex& a, const Complex& b)
    {
        return Complex(a.re + b.re, a.im + b.im);
    }

    friend Complex operator-(const Complex& a, const Complex& b)
    {
        return Complex(a.re - b.re, a.im - b.im);
    }

    friend Complex operator*(const Complex& a, const Complex& b)
    {
        return Complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
    }

    friend Complex operator/(const Complex& a, const Complex& b)
    {
        double d = b.re * b.re + b.im * b.im;
        if (d < 1e-18) throw domain_error("division by a zero impedance");
        return Complex((a.re * b.re + a.im * b.im) / d, (a.im * b.re - a.re * b.im) / d);
    }

    bool operator==(const Complex& o) const
    {
        return fabs(re - o.re) < 1e-9 && fabs(im - o.im) < 1e-9;
    }

    friend ostream& operator<<(ostream& os, const Complex& z)
    {
        os << z.re << (z.im < 0 ? " - " : " + ") << fabs(z.im) << "j";
        return os;
    }
};

Complex resistor(double R) { return Complex(R, 0); }
Complex inductor(double L, double w) { return Complex(0, w * L); }
Complex capacitor(double C, double w) { return Complex(0, -1.0 / (w * C)); }

Complex parallel(const Complex& z1, const Complex& z2)
{
    return (z1 * z2) / (z1 + z2);
}

int main()
{
    double R, L, C, w;
    cout << "Enter R (ohm), L (henry), C (farad), omega (rad/s): ";
    cin >> R >> L >> C >> w;

    cout << fixed << setprecision(4);

    Complex zr = resistor(R), zl = inductor(L, w), zc = capacitor(C, w);

    cout << endl << "=== Part 1 : Element impedances ===" << endl;
    cout << "Z(R) = " << zr << endl;
    cout << "Z(L) = " << zl << endl;
    cout << "Z(C) = " << zc << endl;

    cout << endl << "=== Part 2 : Series RLC ===" << endl;
    Complex zs = zr + zl + zc;
    cout << "Z series = " << zs << "   |Z| = " << zs.magnitude() << endl;

    cout << endl << "=== Part 3 : Parallel ===" << endl;
    Complex zrl = parallel(zr, zl);
    cout << "R || L      = " << zrl << "   |Z| = " << zrl.magnitude() << endl;
    Complex zrlc = parallel(zrl, zc);
    cout << "R || L || C = " << zrlc << "   |Z| = " << zrlc.magnitude() << endl;

    cout << endl << "=== Part 4 : Operator checks ===" << endl;
    Complex p(3, 4), q(1, -2);
    cout << "(3+4j) + (1-2j) = " << p + q << endl;
    cout << "(3+4j) - (1-2j) = " << p - q << endl;
    cout << "(3+4j) * (1-2j) = " << p * q << endl;
    cout << "(3+4j) / (1-2j) = " << p / q << endl;
    cout << "5 + (3+4j)      = " << 5 + p << endl;
    cout << "(3+4j) == (3+4j) ? " << boolalpha << (p == Complex(3, 4)) << endl;
    cout << "|3+4j|          = " << p.magnitude() << endl;

    cout << endl << "=== Part 5 : Division by zero ===" << endl;
    try {
        Complex bad = p / Complex(0, 0);
        cout << bad << endl;
    } catch (const domain_error& e) {
        cout << "caught: " << e.what() << endl;
    }
    try {
        Complex bad = parallel(zl, Complex(0, -zl.imag()));
        cout << bad << endl;
    } catch (const domain_error& e) {
        cout << "caught (parallel of +jX and -jX): " << e.what() << endl;
    }

    cout << endl << "=== Part 6 : Same circuit with std::complex<double> ===" << endl;
    complex<double> sr(R, 0), sl(0, w * L), sc(0, -1.0 / (w * C));
    complex<double> ss = sr + sl + sc;
    complex<double> sp = (sr * sl) / (sr + sl);
    cout << "Z series   = " << ss.real() << (ss.imag() < 0 ? " - " : " + ") << fabs(ss.imag()) << "j   |Z| = " << abs(ss) << endl;
    cout << "R || L     = " << sp.real() << (sp.imag() < 0 ? " - " : " + ") << fabs(sp.imag()) << "j   |Z| = " << abs(sp) << endl;
    complex<double> inf = complex<double>(3, 4) / complex<double>(0, 0);
    cout << "std::complex (3+4j)/0 = " << inf.real() << " , " << inf.imag() << "  (no exception, no check)" << endl;

    return 0;
}