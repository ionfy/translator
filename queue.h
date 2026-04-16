#pragma once

template <class T>
class TQueue {
private:
    size_t s;
    size_t f;
    size_t memSize;
    T* pMem;

    size_t next(size_t n) const {
        return (n + 1) % memSize;
    }
public:
    TQueue(size_t n = 1): s(0), f(n), memSize(n+1), pMem(new T[n + 1]) {}
    ~TQueue() { delete[] pMem; }
    TQueue(const TQueue& c) {
        memSize = c.memSize;
        s = c.s;
        f = c.f;
        pMem = new T[memSize];
        for (size_t i = s; i != next(f); i = next(i))
            pMem[i] = c.pMem[i];
    }

    TQueue& operator=(const TQueue& c) {
        if (this == &c) return *this;
        s = c.s;
        f = c.f;
        if (memSize != c.memSize) {
            T* tmp = new T[c.memSize];
            memSize = c.memSize;
            delete[] pMem;
            pMem = tmp;
        }
        for (size_t i = s; i != next(f); i = next(i))
            pMem[i] = c.pMem[i];
        return *this;
    }

    bool operator==(const TQueue& other) const {
        if (size() != other.size()) return false;
        
        size_t i1 = s;
        size_t i2 = other.s;
        
        while (i1 != next(f)) {
            if (pMem[i1] != other.pMem[i2]) return false;
            i1 = next(i1);
            i2 = other.next(i2);
        }
        return true;
    }

    bool operator!=(const TQueue& other) const {
        return !(*this == other);
    }

    bool empty() const {
        return s == next(f);
    } 

    bool full() const {
        return s == next(next(f));
    }

    void push(T elem) {
        if (full()) throw -1;
        f = next(f);
        pMem[f] = elem;
    }

    T top() {
        if (empty()) throw -1;
        return pMem[s];
    }

    T last() {
        if (empty()) throw -1;
        return pMem[f];
    }

    size_t size() const{
        if(empty()) return 0;
        if (s > f) return (memSize - s + f + 1);
        return (f - s + 1);
    }

    void pop() {
        if (empty()) throw -1;
        s = next(s);
    }
};