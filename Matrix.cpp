#include <bits/stdc++.h>

template <typename T>
class Matrix : std::vector<std::vector<T>> {
    public:
    Matrix(int r, int c, int v = 0) {
        this->resize(r, std::vector<T>(c, v));
    }

    T operator[] (const std::array<int, 2> idx) const {
        return this->at(idx[0]).at(idx[1]);
    }

    T &operator[] (const std::array<int, 2> idx) {
        return this->at(idx[0]).at(idx[1]);
    }

    bool operator == (const Matrix &rhs) const {
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < this->at(0).size(); j++) {
                if ((*this)[{i, j}] != rhs[{i, j}]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator != (const Matrix &rhs) const {
        return !(*this == rhs);
    }

    Matrix operator + (const Matrix &rhs) const {
        Matrix c(this->size(), this->at(0).size());
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < this->at(0).size(); j++) {
                c[{i, j}] = (*this)[{i, j}] + rhs[{i, j}];
            }
        }
        return c;
    }

    Matrix operator - (const Matrix &rhs) const {
        Matrix c(this->size(), this->at(0).size());
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < this->at(0).size(); j++) {
                c[{i, j}] = (*this)[{i, j}] - rhs[{i, j}];
            }
        }
        return c;
    }

    Matrix operator * (const Matrix &rhs) const {
        Matrix c(this->size(), rhs.at(0).size(), 0);
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < rhs.at(0).size(); j++) {
                for (int k = 0; k < this->at(0).size(); k++) {
                    c[{i, j}] += (*this)[{i, j}] * rhs[{i, j}];
                }
            }
        }
        return c;
    }

    Matrix operator / (const Matrix &rhs) const {
        return (*this) * rhs.inverse();
    }

    void op (char c, T v) {
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < this->at(0).size(); j++) {
                if (c == '+') { this->at(i).at(j) += v; }
                else if (c == '-') { this->at(i).at(j) -= v; }
                else if (c == '*') { this->at(i).at(j) *= v; }
                else if (c == '/') { this->at(i).at(j) /= v; }
                else if (c == '%') { this->at(i).at(j) %= v; }
            }
        }
    }

    Matrix operator + (const T &v) { op ('+', v); return *this; }
    Matrix operator - (const T &v) { op ('-', v); return *this; }
    Matrix operator * (const T &v) { op ('*', v); return *this; }
    Matrix operator / (const T &v) { op ('/', v); return *this; }
    Matrix operator % (const T &v) { op ('%', v); return *this; }


    Matrix pow(int64_t n) const {
        Matrix c(this->size(), this->at(0).size(), 0);
        for (int i = 0; i < this->size(); i++) { c[{i, i}] = 1; }
        
        Matrix a = *this;
        for (; n; n >>= 1) {
            if (n & 1) c = c * a;
            a = a * a;
        }
        return c;
    }

    Matrix operator ! () const {
        Matrix c(this->at(0).size(), this->size());
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < this->at(0).size(); j++) {
                c[{j, i}] = (*this)[{i, j}];
            }
        }
        return c;
    }

    friend std::ostream &operator << (std::ostream &out, const Matrix &M) {
        for (int i = 0; i < M.size(); i++) {
            for (int j = 0; j < M.at(0).size(); j++) {
                out << M[{i, j}] << " ";
            }
            out << std::endl;
        }
        return out;
    }

    friend std::istream &operator >> (std::istream &in, Matrix &M) {
        for (int i = 0; i < M.size(); i++) {
            for (int j = 0; j < M.at(0).size(); j++) {
                in >> M[{i, j}];
            }
        }
        return in;
    }



    Matrix cofactor(int row, int col) const { // O(n^2)
        int n = this->size();
        Matrix C(n - 1, n - 1, 0);
        int i = 0, j = 0;
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (r != row && c != col) {
                    C[{i, j++}] = (*this)[{r, c}];
                    if (j == n - 1) { j = 0, i++; }
                }
            }
        }
        return C;
    }

    T det() const { // O(n^3)
        T det = 1, total = 1; // Initialize result
        Matrix tmp_matrix = *this;
        T tmp[this->size() + 1];

        for (int i = 0; i < tmp_matrix.size(); i++) {
            int index = i;
            while (index < tmp_matrix.size() && tmp_matrix[{index, i}] == 0) { index++; }
            
            if (index == tmp_matrix.size()) { continue; }

            if (index != i) {
                for (int j = 0; j < tmp_matrix.size(); j++) {
                    std::swap(tmp_matrix[{index, j}], tmp_matrix[{i, j}]);
                }
                det = det * (((index - i) & 1) ? -1 : 1);
            }
            for (int j = 0; j < tmp_matrix.size(); j++) {
                tmp[j] = tmp_matrix[{i, j}];
            }
            for (int j = i + 1; j < tmp_matrix.size(); j++) {
                T n1 = tmp[i], n2 = tmp_matrix[{j, i}];
                
                for (int k = 0; k < tmp_matrix.size(); k++) {
                    tmp_matrix[{j, k}] = (n1 * tmp_matrix[{j, k}]) - (n2 * tmp[k]);
                }
                total = total * n1;
            }
        }
        for (int i = 0; i < tmp_matrix.size(); i++) {
            det = det * tmp_matrix.at(i).at(i);
        }
        return (det / total);
    }
    
    Matrix adjoint() const { // O(n^4)
        int n = this->size();
        Matrix adj(n, n, 0);
        Matrix submatrix(n - 1, n - 1, 0);

        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) {
                adj[{i, j}] = this->cofactor(i, j).det() * (((i + j) & 1) ? -1 : 1);
            }
        }
        return !adj;
    }

    Matrix inverse() const { // O(n^4)
        int n = this->size();
        Matrix inv(n, n, 0);
        T det = this->det();
        if (det == 0) { return inv; }
        Matrix adj = this->adjoint();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                inv.at(i).at(j) = adj.at(i).at(j) / det;
            }
        }
        return inv;
    }

};

int main() {
    Matrix<int> A(3, 3, 0);
    std::cin >> A;
    auto c = A;
    A = A + c;
    
    std::cout << A.inverse() << std::endl;
    std::cout << std::endl;
    std::cout << A << std::endl;
}
