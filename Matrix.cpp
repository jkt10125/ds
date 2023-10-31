#include <bits/stdc++.h>

// using T = int;

template <typename T>
struct Matrix : std::vector<std::vector<T>> {
    public:
    Matrix(int r, int c, int v = 0) : std::vector<std::vector<T>> (r, std::vector<T> (c, v)) { }

    static Matrix IDENTITY(int n) {
        Matrix I(n, n, 0);
        for (int i = 0; i < n; i++) {
            I[i][i] = 1;
        }
        return I;
    }
    
    Matrix operator + (const Matrix &rhs) const {
        Matrix c((*this).size(), (*this)[0].size());
        for (int i = 0; i < (int) c.size(); i++) {
            for (int j = 0; j < (int) c[0].size(); j++) {
                c[i][j] = (*this)[i][j] + rhs[i][j];
            }
        }
        return c;
    }

    Matrix operator - (const Matrix &rhs) const {
        Matrix c((*this).size(), (*this)[0].size());
        for (int i = 0; i < (int) c.size(); i++) {
            for (int j = 0; j < (int) c[0].size(); j++) {
                c[i][j] = (*this)[i][j] - rhs[i][j];
            }
        }
        return c;
    }

    Matrix operator * (const Matrix &rhs) const {
        Matrix c((*this).size(), rhs[0].size(), 0);
        for (int i = 0; i < (int) (*this).size(); i++) {
            for (int j = 0; j < (int) rhs[0].size(); j++) {
                for (int k = 0; k < (int) rhs.size(); k++) {
                    c[i][j] += (*this)[i][k] * rhs[k][j];
                }
            }
        }
        return c;
    }

    Matrix operator * (const T &rhs) {
        Matrix c = (*this);
        for (int i = 0; i < (int) c.size(); i++) {
            for (int j = 0; j < (int) c[0].size(); j++) {
                c[i][j] = c[i][j] * rhs;
            }
        }
    }

    Matrix operator / (const T &rhs) {
        Matrix c = (*this);
        for (int i = 0; i < (int) c.size(); i++) {
            for (int j = 0; j < (int) c[0].size(); j++) {
                c[i][j] = c[i][j] / rhs;
            }
        }
    }

    Matrix pow(long long n) const {
        Matrix c = IDENTITY(n), a = (*this);
        for (; n; n >>= 1) {
            if (n & 1) c = c * a;
            a = a * a;
        }
        return c;
    }

    Matrix operator ! () const {
        Matrix c((*this)[0].size(), (*this).size());
        for (int i = 0; i < (int) c[0].size(); i++) {
            for (int j = 0; j < (int) c.size(); j++) {
                c[j][i] = (*this)[i][j];
            }
        }
        return c;
    }

    friend std::ostream &operator << (std::ostream &out, const Matrix &M) {
        for (int i = 0; i < (int) M.size(); i++) {
            for (int j = 0; j < (int) M[0].size(); j++) {
                out << M[i][j] << " \n"[j == (int) M[0].size() - 1];
            }
        }
        return out;
    }

    friend std::istream &operator >> (std::istream &in, Matrix &M) {
        for (int i = 0; i < (int) M.size(); i++) {
            for (int j = 0; j < (int) M[0].size(); j++) {
                in >> M[i][j];
            }
        }
        return in;
    }

    T det() const {
		Matrix a = *this;
		T res = 1;
		for (int i = 0; i < (int) a.size(); i++) {
			int id = -1;
			for (int j = i; j < (int) a.size(); j++) {
                if (a[j][i] != T(0)) {
                    id = j;
                    break;
                }
            }
			if (id == -1) { return T(0); }
			if (id != i) {
				res = -res;
				for (int j = i; j < (int) a.size(); j++) {
                    std::swap(a[id][j], a[i][j]);
                }
			}
			res *= a[i][i];
			T t = T(1) / a[i][i];
			for (int j = i; j < (int) a.size(); j++) {
                a[i][j] *= t;
            }
			for (int j = i + 1; j < (int) a.size(); j++) {
				auto s = a[j][i];
				for (int k = i; k < (int) a.size(); k++) {
                    a[j][k] -= a[i][k] * s;
                }
			}
		}
		return res;
	}
    
    Matrix inverse() {
		Matrix a = *this;
		std::vector<std::array<int, 2>> swaps;
		for (int i = 0; i < (int) a.size(); i++) {
			int id = -1;
			for (int j = i; j < (int) a.size(); j++) {
                if (a[j][i] != T(0)) {
                    id = j;
                    break;
                }
            }
			if (id == -1) {
                return Matrix(0, 0);
            }
			if (id != i) {
				swaps.push_back({id, i});
				for (int j = 0; j < (int) a.size(); j++) {
                    std::swap(a[i][j], a[id][j]);
                }
			}
			a[i][i] =  T(1) / a[i][i];
			for (int j = 0; j < (int) a.size(); j++) {
                if (j != i) {
                    a[i][j] *= a[i][i];
                }
            }
			for (int j = 0; j < (int) a.size(); j++) {
                if (j != i) {
                    for (int k = 0; k < (int) a.size(); k++) {
                        if (k != i) {
                            a[j][k] -= a[j][i] * a[i][k];
                        }
                    }
                    a[j][i] *= -a[i][i];
			    }
            }
		}
		for (int i = swaps.size(); i--;) {
			for (int j = 0; j < (int) a.size(); j++) {
                std::swap(a[j][swaps[i][0]], a[j][swaps[i][1]]);
            }
        }
		return a;
	}
};

int main() {
    Matrix<double> A(5, 5, 0);
    std::cin >> A;

    std::cout << A.det();
}
