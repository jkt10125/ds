#include <bits/stdc++.h>
// using namespace std;

class BigInteger {
    std::string value;
    bool sign;

    public:

    BigInteger() : value(""), sign(0) {}

    BigInteger (std::string val) {
        sign = (val.front() == '-');
        value = std::string (val.begin() + sign, val.end());
        clean();
    }

    BigInteger (std::string Value, bool Sign) {
        value = Value;
        sign = Sign;
        clean();
    }

    BigInteger (int32_t a) {
        sign = (a < 0);
        a = std::abs(a);
        value = std::to_string(a);  
    }

    // This function adds zeroes to the left
    void addZeroesToLeft (const int64_t a) {
        value = std::string (a, '0') + value;
    }

    // This function adds zeroes to the right
    void addZeroesToRight (const int64_t a) {
        value = value + std::string (a, '0');
    }

    BigInteger operator - () {
        return BigInteger (value, !sign);
    }

    BigInteger &operator ++ (int) {
        return (*this += 1);
    }

    BigInteger &operator ++ () {
        return (*this += 1);
    }

    BigInteger &operator -- (int) {
        return (*this -= 1);
    }

    BigInteger &operator -- () {
        return (*this -= 1);
    }

    BigInteger operator + (const BigInteger &rhs) {
        BigInteger B = rhs;
        if (B.sign) {
            if (!sign) {
                return (*this - (-B));
            }
            else {
                return -(-(*this) + (-B));
            }
        }
        if (sign) {
            return -(-(*this) - B);
        }

        if ((*this) < B) {
            return B.operator + (*this);
        }

        int numOfZeroes = value.size() - B.value.size();
        B.addZeroesToLeft (numOfZeroes);

        bool carry = 0;
        for (int i = B.value.size() - 1; i >= 0; i--) {
            int val = B.value[i] + value[i] + carry - 96;
            B.value[i] = val % 10 + 48;
            carry = val / 10;
        }

        if (carry) {
            B.value = "1" + B.value;
        }

        return B;
    }

    BigInteger &operator += (const BigInteger &rhs) {
        return (*this = (*this + rhs));
    }

    BigInteger operator - (const BigInteger &rhs) {
        BigInteger B = rhs;
        if(B.sign) { 
            if(!sign) {
                return (*this + (-B));
            } 
            else {
                return -(-(*this) - (-B));
            }
        }
        if (sign) {
            return -(-(*this) + B);
        }
        
        if ((*this) < B) {
            return -(B.operator - (*this));
        }

        int numOfZeroes = value.size() - B.value.size();
        B.addZeroesToLeft (numOfZeroes);

        bool carry = 0;
        for (int i = B.value.size() - 1; i >= 0; i--) {
            int val = value[i] - B.value[i] - carry;
            carry = 0;
            if (val < 0) {
                val += 10;
                carry = 1;
            }
            B.value[i] = val + 48;
        }
        B.clean ();
        return B;
    }

    BigInteger &operator -= (const BigInteger &rhs) {
        return *this = (*this - rhs);
    }

    BigInteger operator * (const BigInteger &rhs) const {
        BigInteger ans = 0;
        if (rhs.value == "0" || value == "0") return BigInteger (0);
        for (int i = rhs.value.size() - 1; i >= 0; i--) {
            BigInteger B = *this;
            int carry = 0, sum;
            for (int j = value.size() - 1; j >= 0; j--) {
                sum = (value[j] - 48) * (rhs.value[i] - 48) + carry;
                B.value[j] = sum % 10 + 48;
                carry = sum / 10;
            }
            if (carry) {
                B.value = char(carry + 48) + B.value;
            }
            B.addZeroesToRight (rhs.value.size() - i - 1);
            ans += B;

        }
        ans.sign = sign ^ rhs.sign;
        return ans;
    }
    
    BigInteger &operator *= (const BigInteger &rhs) {
        return *this = (*this * rhs);
    }

    BigInteger operator / (const BigInteger &rhs) {
        if (abs() < rhs.abs()) {
            return BigInteger (0);
        }
        BigInteger ans = 0, tmp = 0;
        size_t idx = 0, ctr = 0;
        while (tmp < rhs) {
            if (tmp.value == "0") tmp.value.clear();
            tmp.value.push_back(value[idx++]);
        }
        while (idx < value.size()) {
            while (tmp >= rhs) {
                tmp -= rhs; ctr++;
            }
            
            if (tmp.value == "0") tmp.value.clear();
            tmp.value.push_back(value[idx++]);

            if (ans.value == "0") ans.value.clear();
            ans.value.push_back(ctr + 48);
            
            ctr = 0;
        }
        while (tmp >= rhs) {
            tmp -= rhs, ctr++;
        }
        if (ans.value == "0") ans.value.clear();
        ans.value.push_back(ctr + 48);
        ans.sign = sign ^ rhs.sign;
        return ans;
    }

    BigInteger &operator /= (const BigInteger &rhs) {
        return *this = (*this / rhs);
    }

    BigInteger operator % (const BigInteger &rhs) {
        return *this - *this / rhs * rhs;
    }

    BigInteger &operator %= (const BigInteger &rhs) {
        return *this = (*this % rhs);
    }

    // less than operator
    bool operator < (const BigInteger &rhs) const {
        if (sign ^ rhs.sign) return (sign);
        if (value.size() ^ rhs.value.size()) {
            return (value.size() < rhs.value.size()) ^ sign;
        }
        return (value < rhs.value) ^ sign;
    }

    bool operator >= (const BigInteger &rhs) const {
        return !(*this < rhs);
    }

    bool operator > (const BigInteger &rhs) const {
        return (rhs.operator < (*this));
    }

    bool operator <= (const BigInteger &rhs) const {
        return !(rhs < *this);
    }

    bool operator == (const BigInteger &rhs) const {
        return (value == rhs.value && sign == rhs.sign);
    }

    bool operator != (const BigInteger &rhs) const {
        return !(*this == rhs);
    }

    BigInteger operator & (const BigInteger &rhs) {
        BigInteger B = rhs;
        if (*this < B) {
            return B.operator & (*this); 
        }
        std::string s = toBinary ();
        B.value = B.toBinary ();
        int k = s.size() - B.value.size();
        for (int i = B.value.size() - 1; i >= 0; i--) {
            B.value[i] = ((s[i + k] - 48) & (B.value[i] - 48)) + 48;
        }
        B.toNumber (B.value);
        return B;
    }

    BigInteger &operator &= (const BigInteger &rhs) {
        return *this = (*this & rhs);
    }

    BigInteger operator | (const BigInteger &rhs) {
        BigInteger B = rhs;
        if (*this > B) {
            return B.operator | (*this);
        } 
        std::string s = toBinary ();
        B.value = B.toBinary ();
        int k = B.value.size() - s.size();
        for (int i = s.size() - 1; i >= 0; i--) {
            B.value[i + k] = ((s[i] - 48) | (B.value[i + k] - 48)) + 48;
        }
        B.toNumber(B.value);
        return B;
    }

    BigInteger &operator |= (const BigInteger &rhs) {
        return *this = (*this | rhs);
    }

    BigInteger operator ^ (const BigInteger &rhs) {
        BigInteger B = rhs;
        if (*this > B) return B.operator ^ (*this); 
        std::string s = toBinary ();
        B.value = B.toBinary ();
        int k = B.value.size() - s.size();
        for (int i = s.size() - 1; i >= 0; i--) {
            B.value[i + k] = ((s[i] - 48) ^ (B.value[i + k] - 48)) + 48;
        }
        B.toNumber(B.value);
        return B;
    }

    BigInteger &operator ^= (const BigInteger &rhs) {
        return *this = (*this ^ rhs);
    }

    BigInteger operator << (const BigInteger &rhs) {
        if (rhs.value == "1") {
            return (*this * 2);
        }
        BigInteger ans = *this;
        std::string s = toBinary ();
        for (BigInteger i=0; i<rhs; i++) {
            s.push_back('0');
        }
        ans.toNumber (s);
        return ans;
    }
    
    BigInteger &operator <<= (const BigInteger &rhs) {
        return *this = (*this << rhs);
    }

    BigInteger operator >> (const BigInteger &rhs) {
        if (rhs.value == "1") {
            return (*this / 2ll);
        }
        BigInteger ans = *this;
        std::string s = toBinary ();
        for (BigInteger i = 0; i < rhs && s.size() > 0; i++) {
            s.pop_back();
        }
        ans.toNumber(s);
        return ans;
    }

    BigInteger &operator >>= (const BigInteger &rhs) {
        return *this = (*this >> rhs);
    }

    BigInteger operator ! () {
        BigInteger B = *this;
        B.value = B.toBinary();
        B.addZeroesToLeft (100ll);
        for (int i = B.value.size() - 1; i >= 0; i--) {
            B.value[i] = 1 - B.value[i] + 96;
        }
        B.toNumber (B.value);
        return B;
    }

    friend std::ostream &operator << (std::ostream &os, const BigInteger &A) {
        if (A.sign) os << "-";
        os << A.value;
        return os;
    }

    friend std::istream &operator >> (std::istream &is, BigInteger &A) {
        is >> A.value;
        A = BigInteger(A.value);
        return is;
    }

    void clean () {
        size_t ctr = 0;
        ctr = (value[0] == '-');
        while (value[ctr] == '0' && ctr < value.size()) {
            ctr++;
        }
        if (ctr != value.size()) {
            value = &value[ctr];
        }
        else value = "0";
    }

    BigInteger abs() const {
        return BigInteger(value, 0);
    }

    bool isOdd() {
        return (value.back() - 48) & 1;
    }
    
    bool isEven() {
        return !isOdd();
    }

    std::string toBinary() {
        std::string s;

        BigInteger A = abs();

        while(A > BigInteger(0)) {
            if(A.isOdd()) s.push_back('1');
            else s.push_back('0');
            A /= 2;
        }
        std::reverse(s.begin(), s.end());
        return s;
    }

    void toNumber(std::string s) {
        value = "0";
        const bool SIGN = sign;
        for (char c : s) {
            *this *= 2;
            if (c == '1') (*this)++;
        }
        sign = SIGN;
    }
};

int main() {
    BigInteger a = BigInteger("116209860204796185512923222020452964563895324824394701417696651592133737573424572777178256277052743845");
    BigInteger b = BigInteger("3408956734908734295843738945734985734589347539248577");
    BigInteger c = BigInteger("3408000000000000009567349087342958437389457349857345893475392485");

    BigInteger d = BigInteger("1258394756000450005");

    // std::cout << b << std::endl;
    std::cout << a / d * d + a % d << std::endl;
}