#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <typeinfo>
#include "double_operations.hpp"


namespace matrix {

using namespace doperations;

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
    matrix_t(int cols, int rows, It start, It fin) : rows_(rows), cols_(cols) {
        int size = rows_ * cols_;

        data_ = new T[size];
        rows_buff_ = new T*[rows_];

        int cnt = 0;
        for (int i = 0; i < rows_; ++i) rows_buff_[i] = &data_[i * cols_];
        for (It it = start; (it != fin) && (cnt != size); ++it) {
            data_[cnt] = *it;
            cnt++;
        }
    }


    ~matrix_t() {
        delete[] data_;
        delete[] rows_buff_;
    }


    proxy_row operator[](int n) const { return proxy_row{rows_buff_[n]}; }


    matrix_t& negate() & {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j)
                rows_buff_[i][j] *= -1;
        }

        return *this;
    }


    matrix_t& transpose() & {
        int size = rows_ * cols_,
            rows = cols_,
            cols = rows_;

        T*  new_data = new T[size];
        T** new_rows_buff = new T*[rows];

        for (int i = 0; i < rows; ++i) new_rows_buff[i] = &new_data[i * cols];

        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                new_rows_buff[i][j] = rows_buff_[j][i];

        delete[] data_;
        delete[] rows_buff_;

        data_ = new_data;
        rows_buff_ = new_rows_buff;
        cols_ = cols;
        rows_ = rows;

        return *this;
    }


    void dump() const {
        std::cout << typeid(rows_buff_[0][0]).name() << std::endl;
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j)
                std::cout << std::setw(9) << rows_buff_[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }


    static matrix_t eye(int size) {
        matrix_t mtx{size, size, 0};
        for (int i = 0; i < size; ++i) mtx[i][i] = 1;

        return mtx;
    }


    double determinant() {
        return gauss_algorithm();
    }


    /**
     * \brief finds max element of collon + sets its row number
     *
     * \param [in]  i - collon number
     * \param [out] row_number - var of primary element's row
     *
     * \return max element value
    */
    double find_primary_elem(int col, int &row_number) {
        double max = rows_buff_[col][col],
               absmax = std::abs(max);
        int prim_row = col;

        for (int i = col + 1; i < rows_; ++i) { 
            double val = rows_buff_[i][col];
            double absval = std::abs(val);

            if (absval > absmax) {
                absmax = absval;
                max    = val;
                prim_row = i;
            }
        }

        row_number = prim_row;
        std::cout << "max=" << max << std::endl;
        return max;
    }


    bool swap_rows(int fst, int snd) {
        if (fst == snd) return false;

        T* fstrow = rows_buff_[fst];

        rows_buff_[fst] = rows_buff_[snd];
        rows_buff_[snd] = fstrow;

        return true;
    }


    void add_rows(int src, int dest, double multiplier = 1.0) {
        if (src == dest) {
            std::cerr << "adding row to itself\n";
            return;
        }

        double val  = 0.0;
        for (int i = 0; i < cols_; ++i) {
            val = multiplier * rows_buff_[src][i] + rows_buff_[dest][i];
            rows_buff_[dest][i] = is_equal(0.0, val) ? 0.0 : val;
        }
    }

private:
    double gauss_algorithm() {
        matrix_t<double> mtx{rows_, cols_, NAN};

        copy_to_double(mtx);
        mtx.dump();

        int primary_row = 0;
        double elem = 0.0;
        double det = 1.0;

        for (int i = 0; i < cols_; ++i) {
            elem = mtx.find_primary_elem(i, primary_row);
            std::cout << "primary row = " << primary_row << std::endl;

            if (mtx.swap_rows(i, primary_row)) {
                det *= -elem;
                std::cout << "sign swap\n";
            }
            else det *= elem;

            for (int j = i + 1; j < cols_; ++j) {
                double multiplier = -1 * mtx[j][i] / elem;
                mtx.add_rows(i, j, multiplier);
            }
            mtx.dump();
        }

        return det;
    }


    void copy_to_double(matrix_t<double> &mtx) {
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < cols_; ++j)
                mtx[i][j] = static_cast<double>(rows_buff_[i][j]);
    }
};

}
