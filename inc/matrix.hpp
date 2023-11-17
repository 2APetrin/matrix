#pragma once

#include <vector>
#include <iostream>


namespace matrix {

template <typename T>
class matrix_t final {

    class proxy_row {
        T* row_;

    public:
        const T& operator[](int n) const { return row_[n]; }
        T& operator[](int n) { return row_[n]; }

        proxy_row(T* row) : row_(row) {}
    };

    int rows_;
    int cols_;

    T*  data_;
    T** rows_buff_;

public:
    matrix_t(int rows = 1, int cols = 1, T val = T{}) : rows_(rows), cols_(cols) {
        int size = rows_ * cols_;

        data_ = new T[size];
        rows_buff_ = new T*[rows_];

        for (int i = 0; i < rows_; ++i) rows_buff_[i] = &data_[i * cols_];
        for (int i = 0; i < size;  ++i) data_[i] = val;
    }


    template <typename It>
    matrix_t(int cols, int rows, It start, It fin) {
        int size = rows_ * cols_;

        data_ = new T[size];
        rows_buff_ = new T*[rows_];

        int cnt = 0;
        for (int i = 0; i < rows_; ++i) rows_buff_[i] = &data_[i * cols_];
        for (It it = start; it != fin; ++it) {
            data_[cnt] = *it;
            cnt++;
        }
    }


    ~matrix_t() {
        delete[] data_;
        delete[] rows_buff_;
    }


    proxy_row opertator[](int n) const { return proxy_row{rows_buff_[n]}; }


    void print() const {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j)
                std::cout << rows_buff_[i][j] << " ";
            std::cout << std::endl;
        }
    }

    static const matrix_t& eye(int size) {
        matrix_t mtx{size, size, 0};
        for (int i = 0; i < size; ++i) mtx[i][i] = 1;

        return mtx;
    }
};

}
