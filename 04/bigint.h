#include <cstdlib>
#include <string>

#ifndef HW4_BIGINT_H
#define HW4_BIGINT_H

#endif //HW4_BIGINT_H

class BigInt
{
private:
    char *num_;
    bool pos_;
    int size_;

public:

    BigInt(): num_(nullptr), pos_(true), size_(0)
    {
    }

    BigInt(const std::string &s)
    {
        if (s[0] == '-') {
            pos_ = false;
            size_ = s.length() - 1;
            num_ = (char*)malloc(size_ * sizeof(*num_));
            if (num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            for (int i = 0; i < size_; i++) {
                num_[i] = s[i + 1];
            }
        } else {
            pos_ = true;
            size_ = s.length();
            num_ = (char*)malloc(size_ * sizeof(*num_));
            if (num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            for (int i = 0; i < size_; i++) {
                num_[i] = s[i];
            }
        }
    }

    BigInt(int num)
    {
        std::string s = std::to_string(num);
        if (s[0] == '-') {
            pos_ = false;
            size_ = s.length() - 1;
            num_ = (char*)malloc(size_ * sizeof(*num_));
            if (num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            for (int i = 0; i < size_; i++) {
                num_[i] = s[i + 1];
            }
        } else {
            pos_ = true;
            size_ = s.length();
            num_ = (char*)malloc(size_ * sizeof(*num_));
            if (num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            for (int i = 0; i < size_; i++) {
                num_[i] = s[i];
            }
        }
    }

    BigInt(const BigInt &n)
    {
        num_ = (char*)malloc(n.size_ * sizeof(*num_));
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
            num_ = (char*)malloc(n.size_ * sizeof(*num_));
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
            res.num_[res.size_ - 1 - i] = (num_[size_ - 1 - i] - '0' + rhs.num_[rhs.size_ - 1 - i] - '0' + dec) % 10 + '0';
            dec = (num_[size_ - 1 - i] - '0' + rhs.num_[rhs.size_ - 1 - i] - '0' + dec) / 10;
        }
        for (int i = size_ - rhs.size_ - 1; i >= 0; i--) {
            res.num_[i] = (num_[i] - '0' + dec) % 10 + '0';
            dec = (num_[i] - '0' + dec) / 10;
        }
        if (dec) {
            res.num_ = (char*)realloc(res.num_, res.size_ + 1);
            if (res.num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            res.size_++;
            for (int i = res.size_ - 2; i >= 0; i--) {
                res.num_[i + 1] = res.num_[i];
            }
            res.num_[0] = '1';
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
            res.num_[res.size_ - 1 - i] = (cur_dec * 10 + num_[size_ - 1 - i] - rhs.num_[rhs.size_ - 1 - i] - dec) + '0';
            dec = cur_dec;
        }
        for (int i = size_ - rhs.size_ - 1; i >= 0; i--) {
            cur_dec = (num_[i] - '0' - dec) < 0;
            res.num_[i] = (cur_dec * 10 + num_[i] - '0' - dec) + '0';
            dec = cur_dec;
        }
        int offset = 0;
        while (res.num_[offset] == '0') {
            offset++;
        }
        for (int i = 0; i < res.size_ - offset; i++) {
            res.num_[i] = res.num_[i + offset];
        }
        res.size_ -= offset;
        res.num_[res.size_] = 0;
        res.num_ = (char*)realloc(res.num_, res.size_);
        if (res.num_ == nullptr) {
            throw std::runtime_error("Can not allocate memory");
        }
        return res;
    }

    BigInt mult_digit(int digit, int offset) const
    {
        if (!digit) {
            return BigInt(0);
        }
        BigInt res(*this);
        int dec = 0;
        for (int i = res.size_ - 1; i >= 0; i--) {
            res.num_[i] = ((num_[i] - '0') * digit + dec) % 10 + '0';
            dec = ((num_[i] - '0') * digit + dec) / 10;
        }
        if (dec) {
            res.num_ = (char*)realloc(res.num_, res.size_ + 1);
            if (res.num_ == nullptr) {
                throw std::runtime_error("Can not allocate memory");
            }
            res.size_++;
            for (int i = res.size_ - 2; i >= 0; i--) {
                res.num_[i + 1] = res.num_[i];
            }
            res.num_[0] = dec + '0';
        }
        res.num_ = (char*)realloc(res.num_, res.size_ + offset);
        if (res.num_ == nullptr) {
            throw std::runtime_error("Can not allocate memory");
        }
        res.size_ += offset;
        for (int i = res.size_ - offset; i < res.size_; i++) {
            res.num_[i] = '0';
        }
        return res;
    }

    BigInt operator *(const BigInt &rhs) const
    {
        if (rhs > *this) {
            return rhs * *this;
        }
        BigInt res(0);
        for (int i = 0; i < rhs.size_; i++) {
            res = res + this->mult_digit(rhs.num_[rhs.size_ - i - 1] - '0', i);
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
    os << n.num_;
    return os;
}
