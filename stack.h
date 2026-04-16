#pragma once

template <class T>
class TStack {
private:
    size_t last;
    size_t memSize;
    T* pMem;
public:
    TStack(size_t n = 1): last(-1), memSize(n), pMem(new T[memSize]) {}
    ~TStack() { delete[] pMem; }
    TStack(const TStack& c) {
        memSize = c.memSize;
        last = c.last;
        pMem = new T[memSize];
        for (size_t i = 0; i <= last; i++)
            pMem[i] = c.pMem[i];
    }

    TStack& operator=(const TStack& c) {
        if (this == &c) return *this;
        last = c.last;
        if (memSize != c.memSize) {
            T* tmp = new T[c.memSize];
            memSize = c.memSize;
            delete[] pMem;
            pMem = tmp;
        }
        for (size_t i = 0; i <= last; i++)
            pMem[i] = c.pMem[i];
        return *this;
    }

    bool operator==(const TStack& other) const {
        if (last != other.last) return false;
        for (size_t i = 0; i <= last; i++) {
            if (pMem[i] != other.pMem[i]) return false;
        }
        return true;
    }

    bool operator!=(const TStack& other) const {
        return !(*this == other);
    }

    bool empty() {
        return last == -1;
    }

    bool full() {
        return last == memSize - 1;
    }

    void push(T elem) {
        if (full()) throw -1;
        pMem[++last] = elem;
    }

    T top() {
        if (empty()) throw -1;
        return pMem[last];
    }

    void pop() {
        if (empty()) throw -1;
        last--;
    }
};