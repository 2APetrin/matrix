#pragma once

#include "double_operations.hpp"
#include "matrix_exceptions.hpp"
#include <iostream>
#include <iomanip>
#include <new>

namespace matrix {
using namespace doperations;


template <typename T> class matrix_t;

namespace detail {

double find_primary_elem(const matrix_t<double> &mtx, int col, int* row_number);
void add_rows(matrix_t<double> &mtx, int src, int dest, double multiplier = 1.0);


template <typename T>  void destroy(T* p) { p->~T(); }
template <typename it> void destroy(it beg, it end) {
    while (beg != end) destroy(&(*beg++));
}

template <typename T> void construct(T* p, T &rhs)  { new (p) T(rhs); }
template <typename T> void construct(T* p, T &&rhs) { new (p) T(rhs); }


template <typename T>
class MMM { // Matrix Memory Manager
protected:
    int rows_;
    int cols_;
    int used_ = 0;

    T*  data_;
    T** rows_buff_;

protected:

    MMM(const int rows, const int cols) : rows_(rows), cols_(cols) { // Mavrodi Sergey Panteleevich (Constructor)
        if (rows_ <= 0 || cols_ <= 0)
            throw matrix_exceptions::matrix_bad_size();

        data_ = static_cast<T*>(::operator new (sizeof(T) * rows * cols));

        rows_buff_ = static_cast<T**>(::operator new (sizeof(T*) * rows, std::nothrow));
        if (!rows_buff_) {
            ::operator delete(data_);
            throw std::bad_alloc();
        }

        // std::cout << "nothrow check=" << noexcept(std::is_nothrow_copy_constructible<T*>::value) << std::endl; // конструирование указателей noexcept

        for (int i = 0; i < rows; ++i) construct(rows_buff_ + i, data_ + cols_ * i); // конструирование указателей noexcept
    }


    MMM            (const MMM &) = delete;
    MMM& operator= (const MMM &) = delete;


    MMM(MMM&& rhs) noexcept : data_(rhs.data_), rows_buff_(rhs.rows_buff_), rows_(rhs.rows_), cols_(rhs.cols_), used_(rhs.used_) {
        rhs.rows_buff_ = nullptr;
        rhs.data_ = nullptr;
        rhs.used_ = 0;
        rhs.rows_ = 0;
        rhs.cols_ = 0;
    }


    MMM& operator= (MMM &&rhs) noexcept {
        if (this == &rhs) return *this;

        std::swap(rows_buff_, rhs.rows_buff_);
        std::swap(data_, rhs.data_);
        std::swap(rows_, rhs.rows_);
        std::swap(cols_, rhs.cols_);
        std::swap(used_, rhs.used_);

        return *this;
    }


    ~MMM() {
        destroy(data_, data_ + used_);
        destroy(rows_buff_, rows_buff_ + rows_);
        ::operator delete(data_);
        ::operator delete(rows_buff_);
    }
};

}



template <typename T>
class matrix_t final : private detail::MMM<T> {
    using detail::MMM<T>::data_;
    using detail::MMM<T>::rows_buff_;
    using detail::MMM<T>::rows_;
    using detail::MMM<T>::cols_;
    using detail::MMM<T>::used_;


    class proxy_row {
        T* row_;

    public:
        const T& operator[](int n) const { return row_[n]; }
        T& operator[](int n) { return row_[n]; }

        proxy_row(T* row) : row_(row) {}
    };

public:
    matrix_t(int rows = 1, int cols = 1, T val = T{}) : detail::MMM<T>(rows, cols) {
        for (int i = 0; i < cols_ * rows_;  ++i) {
            detail::construct(&data_[i], val);
            ++used_;
        }
    }


    template <typename It>
    matrix_t(int cols, int rows, It start, It last) : detail::MMM<T>(rows, cols) {
        int cnt = 0, size = cols_ * rows_;

        It it = start;
        for (; (it != last) && (cnt != size); ++it) {
            data_[cnt] = *it;
            ++used_;
            ++cnt;
        }

        if ((cnt != size) || (it != last)) throw matrix_exceptions::bad_elements_cnt();
    }


    proxy_row operator[](int n) const { return proxy_row{rows_buff_[n]}; }


    matrix_t& negate() & {
        matrix_t tmp_mtrx{rows_, cols_};

        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j)
                tmp_mtrx[i][j] = -rows_buff_[i][j];
        }

    //-------------Kalb's line-------------//

        swap(tmp_mtrx);
        return *this;
    }


    matrix_t& transpose() & {
        matrix_t tmp_mtrx{cols_, rows_};

        for (int i = 0; i < cols_; ++i)
            for (int j = 0; j < rows_; ++j)
                tmp_mtrx[i][j] = rows_buff_[j][i];

    //-------------Kalb's line-------------//

        swap(tmp_mtrx);
        return *this;
    }


    static matrix_t eye(int size) {
        matrix_t mtx{size, size, 0};
        for (int i = 0; i < size; ++i) mtx[i][i] = 1;

        return mtx;
    }


    double determinant() const { // Gauss algorithm
        if (cols_ != rows_) throw matrix_exceptions::matrix_bad_shape();

        matrix_t<double> mtx{rows_, cols_, NAN};

        copy_to_double(mtx);
        mtx.dump();

        int primary_row = 0;
        double elem = 0.0;
        double det = 1.0;

        for (int i = 0; i < cols_; ++i) {
            elem = detail::find_primary_elem(mtx, i, &primary_row);
            std::cout << "max="<< elem << " row=" << primary_row << std::endl;
            if (is_equal(0.0, elem)) return 0.0;

            if (mtx.swap_rows(i, primary_row)) det *= -elem;
            else det *= elem;

            for (int j = i + 1; j < cols_; ++j) {
                double multiplier = -1 * mtx[j][i] / elem;
                detail::add_rows(mtx, i, j, multiplier);
            }
            mtx.dump();
        }

        return det;
    }


    bool swap_rows(int fst, int snd) {
        if (fst == snd) return false;

        T* fstrow = rows_buff_[fst];

        rows_buff_[fst] = rows_buff_[snd];
        rows_buff_[snd] = fstrow;

        return true;
    }


    void dump() const {
        std::cout << typeid(rows_buff_[0][0]).name() << std::endl;
        std::cout << "allocated=" << rows_ * cols_ << "\nused=" << used_ << std::endl;

        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j)
                std::cout << std::setw(9) << rows_buff_[i][j] << " ";
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }


    int get_nrows() const { return rows_; }
    int get_ncols() const { return cols_; }

private:
    void swap(matrix_t &rhs) noexcept {
        std::swap(rows_buff_, rhs.rows_buff_);
        std::swap(data_, rhs.data_);
        std::swap(rows_, rhs.rows_);
        std::swap(cols_, rhs.cols_);
        std::swap(used_, rhs.used_);
    }


    void copy_to_double(matrix_t& mtx) const {
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < cols_; ++j)
                mtx[i][j] = static_cast<double>(rows_buff_[i][j]);
    }
};


namespace detail {
    double find_primary_elem(const matrix_t<double>& mtx, int col, int* row_number) {
        double max = mtx[col][col],
               absmax = std::abs(max);
        int prim_row = col;

        for (int i = col + 1, rows = mtx.get_nrows(); i < rows; ++i) {
            double val    = mtx[i][col];
            double absval = std::abs(val);

            if (absval > absmax) {
                absmax = absval;
                max    = val;
                prim_row = i;
            }
        }

        *row_number = prim_row;
        std::cout << "max=" << max << std::endl;
        return max;
    }


    void add_rows(matrix_t<double> &mtx, int src, int dest, double multiplier) {
        double val  = 0.0;
        for (int i = 0; i < mtx.get_ncols(); ++i) {
            val = multiplier * mtx[src][i] + mtx[dest][i];
            mtx[dest][i] = is_equal(0.0, val) ? 0.0 : val;
        }
    }
}

}
