#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

using namespace std;

class BigInt {
    string digits;
public:
    BigInt(unsigned long long n = 0);
    BigInt(const string &s);
    BigInt(const char *s);
    BigInt(const BigInt &a);

    // Helper Functions
    friend void divide_by_2(BigInt &a);
    friend bool isZero(const BigInt &a);
    friend size_t length(const BigInt &a);
    int operator[](const int index) const;

    // Operator Overloading
    BigInt &operator=(const BigInt &a);

    // Increment/Decrement
    BigInt &operator++();
    BigInt operator++(int);
    BigInt &operator--();
    BigInt operator--(int);

    // Arithmetic Operators
    friend BigInt &operator+=(BigInt &a, const BigInt &b);
    friend BigInt operator+(BigInt a, const BigInt &b);
    friend BigInt operator-(BigInt a, const BigInt &b);
    friend BigInt &operator-=(BigInt &a, const BigInt &b);
    friend BigInt &operator*=(BigInt &a, const BigInt &b);
    friend BigInt operator*(BigInt a, const BigInt &b);
    friend BigInt &operator/=(BigInt &a, const BigInt &b);
    friend BigInt operator/(BigInt a, const BigInt &b);
    friend BigInt &operator%=(BigInt &a, const BigInt &b);
    friend BigInt operator%(BigInt a, const BigInt &b);
    friend BigInt &operator^=(BigInt &a, const BigInt &b);
    friend BigInt operator^(BigInt a, const BigInt &b);

    // Comparison Operators
    friend bool operator==(const BigInt &a, const BigInt &b);
    friend bool operator!=(const BigInt &a, const BigInt &b);
    friend bool operator<(const BigInt &a, const BigInt &b);
    friend bool operator>(const BigInt &a, const BigInt &b);
    friend bool operator<=(const BigInt &a, const BigInt &b);
    friend bool operator>=(const BigInt &a, const BigInt &b);

    // Special Functions
    friend BigInt sqrt(BigInt &a);
    friend BigInt nthCatalan(int n);
    friend BigInt nthFibonacci(int n);
    friend BigInt factorial(int n);

    // Input/Output
    friend ostream &operator<<(ostream &out, const BigInt &a);
    friend istream &operator>>(istream &in, BigInt &a);
};

// Constructor Definitions
BigInt::BigInt(const string &s) {
    digits = s;
    reverse(digits.begin(), digits.end());
    for (char &c : digits) {
        if (!isdigit(c)) throw invalid_argument("Invalid digit in BigInt initialization.");
        c -= '0';
    }
}

BigInt::BigInt(unsigned long long nr) {
    do {
        digits.push_back(nr % 10);
        nr /= 10;
    } while (nr);
}

BigInt::BigInt(const char *s) : BigInt(string(s)) {}

BigInt::BigInt(const BigInt &a) : digits(a.digits) {}

bool isZero(const BigInt &a) {
    return a.digits.size() == 1 && a.digits[0] == 0;
}

size_t length(const BigInt &a) {
    return a.digits.size();
}

int BigInt::operator[](const int index) const {
    if (index < 0 || static_cast<size_t>(index) >= digits.size()) throw out_of_range("Index out of range.");
    return digits[index];
}

// Operator Overloading Definitions
BigInt &BigInt::operator=(const BigInt &a) {
    digits = a.digits;
    return *this;
}

BigInt &BigInt::operator++() {
    size_t n = digits.size();
    for (size_t i = 0; i < n && digits[i] == 9; ++i) {
        digits[i] = 0;
        if (i == n - 1) digits.push_back(1);
    }
    if (digits[0] != 0) ++digits[0];
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++(*this);
    return temp;
}

BigInt &BigInt::operator--() {
    if (isZero(*this)) throw underflow_error("Underflow error");
    size_t i = 0, n = digits.size();
    while (i < n && digits[i] == 0) digits[i++] = 9;
    --digits[i];
    if (digits.back() == 0 && digits.size() > 1) digits.pop_back();
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --(*this);
    return temp;
}

BigInt &operator+=(BigInt &a, const BigInt &b) {
    int carry = 0, sum;
    size_t n = length(a), m = length(b);
    if (m > n) a.digits.append(m - n, 0);
    n = length(a);
    for (size_t i = 0; i < n; ++i) {
        sum = a.digits[i] + (i < m ? b.digits[i] : 0) + carry;
        carry = sum / 10;
        a.digits[i] = sum % 10;
    }
    if (carry) a.digits.push_back(carry);
    return a;
}

BigInt operator+(BigInt a, const BigInt &b) {
    return a += b;
}

BigInt &operator-=(BigInt &a, const BigInt &b) {
    if (a < b) throw underflow_error("Underflow error");
    int borrow = 0, diff;
    size_t n = length(a), m = length(b);
    for (size_t i = 0; i < n; ++i) {
        diff = a.digits[i] - (i < m ? b.digits[i] : 0) + borrow;
        borrow = (diff < 0) ? -1 : 0;
        a.digits[i] = (diff + 10) % 10;
    }
    while (a.digits.size() > 1 && a.digits.back() == 0) a.digits.pop_back();
    return a;
}

BigInt operator-(BigInt a, const BigInt &b) {
    return a -= b;
}

BigInt &operator*=(BigInt &a, const BigInt &b) {
    if (isZero(a) || isZero(b)) {
        a = BigInt();
        return a;
    }
    vector<int> result(a.digits.size() + b.digits.size(), 0);
    for (size_t i = 0; i < a.digits.size(); ++i) {
        for (size_t j = 0; j < b.digits.size(); ++j) {
            result[i + j] += a.digits[i] * b.digits[j];
            if (result[i + j] >= 10) {
                result[i + j + 1] += result[i + j] / 10;
                result[i + j] %= 10;
            }
        }
    }
    a.digits.clear();
    for (size_t i = 0; i < result.size(); ++i) {
        if (i == result.size() - 1 && result[i] == 0) continue;
        a.digits.push_back(result[i]);
    }
    return a;
}

BigInt operator*(BigInt a, const BigInt &b) {
    return a *= b;
}

BigInt &operator/=(BigInt &a, const BigInt &b) {
    if (isZero(b)) throw invalid_argument("Division by zero");
    if (a < b) {
        a = BigInt();
        return a;
    }
    if (a == b) {
        a = BigInt(1);
        return a;
    }
    BigInt quotient, remainder;
    for (int i = a.digits.size() - 1; i >= 0; --i) {
        remainder = remainder * 10 + a.digits[i];
        int count = 0;
        while (remainder >= b) {
            remainder -= b;
            ++count;
        }
        quotient.digits.push_back(count);
    }
    reverse(quotient.digits.begin(), quotient.digits.end());
    a = quotient;
    return a;
}

BigInt operator/(BigInt a, const BigInt &b) {
    return a /= b;
}

BigInt &operator%=(BigInt &a, const BigInt &b) {
    if (isZero(b)) throw invalid_argument("Division by zero");
    if (a < b) return a;
    BigInt quotient, remainder;
    for (int i = a.digits.size() - 1; i >= 0; --i) {
        remainder = remainder * 10 + a.digits[i];
        while (remainder >= b) {
            remainder -= b;
        }
    }
    a = remainder;
    return a;
}

BigInt operator%(BigInt a, const BigInt &b) {
    return a %= b;
}

BigInt &operator^=(BigInt &a, const BigInt &b) {
    BigInt result(1);
    BigInt base = a;
    BigInt exponent = b;
    while (!isZero(exponent)) {
        if (exponent.digits[0] % 2 != 0) {
            result *= base;
        }
        base *= base;
        divide_by_2(exponent);
    }
    a = result;
    return a;
}

BigInt operator^(BigInt a, const BigInt &b) {
    return a ^= b;
}

void divide_by_2(BigInt &a) {
    int carry = 0;
    for (int i = a.digits.size() - 1; i >= 0; --i) {
        int current = a.digits[i] + carry * 10;
        a.digits[i] = current / 2;
        carry = current % 2;
    }
    if (a.digits.back() == 0 && a.digits.size() > 1) a.digits.pop_back();
}

BigInt sqrt(BigInt &a) {
    BigInt low(0), high(a), mid, result;
    while (low <= high) {
        mid = (low + high) / 2;
        if (mid * mid <= a) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

BigInt nthCatalan(int n) {
    BigInt result(1);
    for (int i = 0; i < n; ++i) {
        result *= (4 * i + 2);
        result /= (i + 2);
    }
    return result;
}

BigInt nthFibonacci(int n) {
    if (n <= 1) return BigInt(n);
    BigInt a(0), b(1), c;
    for (int i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

BigInt factorial(int n) {
    BigInt result(1);
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

ostream &operator<<(ostream &out, const BigInt &a) {
    for (auto it = a.digits.rbegin(); it != a.digits.rend(); ++it) {
        out << static_cast<char>(*it + '0');
    }
    return out;
}

istream &operator>>(istream &in, BigInt &a) {
    string s;
    in >> s;
    a = BigInt(s);
    return in;
}

int main() {
    int sep=0;
    BigInt a = "12345678901234567890ULL";
    BigInt b = "98765432109876543210";
    BigInt c = a + b;
    cout << "a + b = " << c << endl;
    return 0;
}
