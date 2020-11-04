#ifndef HW4_BIGINT_H
#define HW4_BIGINT_H

#endif //HW4_BIGINT_H

#include <cstdlib>
#include <string>
#include <iomanip>

enum {
    MAX_NUM = 1000000000
};

class BigInt
{
private:
    int *num_;
    bool pos_;
    int size_;

public:

    BigInt(): num_(nullptr), pos_(true), size_(0)
    {
    }

    BigInt(const std::string &s)
    {
        int len = s.length();
        pos_ = s[0] != '-';
        int offset = !pos_;
        while (offset < len && s[offset] == '0') {
            offset++;
        }
        if (len == offset) {
            num_ = (int*)malloc(sizeof(*num_));
            size_ = 1;
            pos_ = true;
            num_[0] = 0;
        } else {
            len -= offset;
            size_ = len / 9 + (len % 9 != 0);
            num_ = (int*)malloc(size_ * sizeof(*num_));
            if (num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            int j = 0;
            if (len % 9) {
                num_[0] = std::stoi(s.substr(offset, len % 9));
                j++;
            }
            for (int i = len % 9 + offset; i < len + offset; i += 9) {
                num_[j] = std::stoi(s.substr(i, 9));
                j++;
            }
        }
    }

    BigInt(int num): num_(nullptr), pos_(false), size_(0)
    {
        std::string s = std::to_string(num);
        *this = BigInt(s);
    }

    BigInt(const BigInt &n)
    {
        num_ = (int*)malloc(n.size_ * sizeof(*num_));
        size_ = n.size_;
        pos_ = n.pos_;
        for (int i = 0; i < size_; i++) {
            num_[i] = n.num_[i];
        }
    }

    BigInt(BigInt &&n): size_(std::move(n.size_)), pos_(std::move(n.pos_))
    {
        num_ = n.num_;
        n.num_ = nullptr;
        n.size_ = 0;
        n.pos_ = false;
    }

    BigInt &operator =(const BigInt &n)
    {
        if (size_ == n.size_) {
            for (int i = 0; i < size_; i++) {
                num_[i] = n.num_[i];
            }
            pos_ = n.pos_;
        } else {
            free(num_);
            num_ = (int*)malloc(n.size_ * sizeof(*num_));
            if (num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            size_ = n.size_;
            pos_ = n.pos_;
            for (int i = 0; i < size_; i++) {
                num_[i] = n.num_[i];
            }
        }
        return *this;
    }

    BigInt &operator =(BigInt &&n)
    {
        if (*this == n) {
            return *this;
        }
        free(num_);
        num_ = n.num_;
        size_ = std::move(n.size_);
        pos_ = std::move(n.pos_);
        n.num_ = nullptr;
        n.size_ = 0;
        return *this;
    }

    bool operator >(const BigInt &rhs) const
    {
        if (pos_ && !rhs.pos_) {
            return true;
        }
        if (!pos_ && rhs.pos_) {
            return false;
        }
        if (size_ > rhs.size_) {
            return pos_;
        }
        if (size_ < rhs.size_) {
            return !pos_;
        }
        for (int i = 0; i < size_; i++) {
            if (num_[i] > rhs.num_[i]) {
                return pos_;
            }
            if (num_[i] < rhs.num_[i]) {
                return !pos_;
            }
        }
        return false;
    }

    bool operator <(const BigInt &rhs) const
    {
        if (pos_ && !rhs.pos_) {
            return false;
        }
        if (!pos_ && rhs.pos_) {
            return true;
        }
        if (size_ > rhs.size_) {
            return !pos_;
        }
        if (size_ < rhs.size_) {
            return pos_;
        }
        for (int i = 0; i < size_; i++) {
            if (num_[i] > rhs.num_[i]) {
                return !pos_;
            }
            if (num_[i] < rhs.num_[i]) {
                return pos_;
            }
        }
        return false;
    }

    bool operator >=(const BigInt &rhs) const
    {
        return !(*this < rhs);
    }

    bool operator <=(const BigInt &rhs) const
    {
        return !(*this > rhs);
    }

    bool operator ==(const BigInt &rhs) const
    {
        return !(*this > rhs || *this < rhs);
    }

    bool operator !=(const BigInt &rhs) const
    {
        return !(*this == rhs);
    }

    BigInt operator -() const
    {
        BigInt res(*this);
        res.pos_ = !pos_;
        return res;
    }

    BigInt operator +(const BigInt &rhs) const
    {
        if (!pos_ && rhs.pos_) {
            return rhs - (-*this);
        }
        if (pos_ && !rhs.pos_) {
            return *this - (-rhs);
        }
        if (!pos_ && !rhs.pos_) {
            return -(-*this + (-rhs));
        }
        if (rhs > *this) {
            return rhs + *this;
        }
        BigInt res(*this);
        bool dec = false;
        for (int i = 0; i < std::min(size_, rhs.size_); i++) {
            res.num_[res.size_ - 1 - i] = (num_[size_ - 1 - i] + rhs.num_[rhs.size_ - 1 - i] + dec) % MAX_NUM;
            dec = (num_[size_ - 1 - i] + rhs.num_[rhs.size_ - 1 - i] + dec) / MAX_NUM;
        }
        for (int i = size_ - rhs.size_ - 1; i >= 0; i--) {
            res.num_[i] = (num_[i] + dec) % MAX_NUM;
            dec = (num_[i] + dec) / MAX_NUM;
        }
        if (dec) {
            res.num_ = (int*)realloc(res.num_, res.size_ + 1);
            if (res.num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            res.size_++;
            for (int i = res.size_ - 2; i >= 0; i--) {
                res.num_[i + 1] = res.num_[i];
            }
            res.num_[0] = 1;
        }
        return res;
    }

    BigInt operator -(const BigInt &rhs) const
    {
        if (pos_ && !rhs.pos_) {
            return -rhs + *this;
        }
        if (!pos_ && rhs.pos_) {
            return -(-*this + rhs);
        }
        if (!pos_ && !rhs.pos_) {
            return -rhs - (-*this);
        }
        if (rhs > *this) {
            return -(rhs - *this);
        }
        if (*this == rhs) {
            return BigInt(0);
        }
        BigInt res(*this);
        bool dec = false;
        bool cur_dec = false;
        for (int i = 0; i < rhs.size_; i++) {
            cur_dec = (num_[size_ - 1 - i] - rhs.num_[rhs.size_ - 1 - i] - dec) < 0;
            res.num_[res.size_ - 1 - i] = (cur_dec * MAX_NUM + num_[size_ - 1 - i] - rhs.num_[rhs.size_ - 1 - i] - dec);
            dec = cur_dec;
        }
        for (int i = size_ - rhs.size_ - 1; i >= 0; i--) {
            cur_dec = (num_[i] - dec) < 0;
            res.num_[i] = (cur_dec * MAX_NUM + num_[i] - dec);
            dec = cur_dec;
        }
        int offset = 0;
        while (!res.num_[offset]) {
            offset++;
        }
        for (int i = 0; i < res.size_ - offset; i++) {
            res.num_[i] = res.num_[i + offset];
        }
        res.size_ -= offset;
        res.num_[res.size_] = 0;
        res.num_ = (int*)realloc(res.num_, res.size_);
        if (res.num_ == nullptr) {
            throw std::runtime_error("Can not allocate memory");
        }
        return res;
    }

    BigInt operator *(const BigInt &rhs) const
    {
        if (rhs > *this) {
            return rhs * *this;
        }
        BigInt res(0);
        std::string zeros{};
        for (int i = rhs.size_ - 1; i >= 0; i--) {
            std::string zeros1{};
            for (int j = size_ - 1; j >= 0; j--) {
                long long cur_n = static_cast<long long>(num_[j]) * static_cast<long long>(rhs.num_[i]);
                std::string str_cur_n = std::to_string(cur_n);
                str_cur_n += zeros;
                str_cur_n += zeros1;
                res = res + BigInt(str_cur_n);
                zeros1 += "000000000";
            }
            zeros += "000000000";
        }
        if (pos_ != rhs.pos_) {
            return -res;
        }
        return res;
    }

    ~BigInt()
    {
        free(num_);
    }

    friend std::ostream &operator <<(std::ostream &os, const BigInt &n);
};

std::ostream &operator <<(std::ostream &os, const BigInt &n) {
    if (!n.pos_) {
        os << '-';
    }
    os << n.num_[0];
    for (int i = 1; i < n.size_; i++) {
        os << std::setfill('0') << std::setw(9) << n.num_[i];
    }
    return os;
}
