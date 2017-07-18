#ifndef __TRIPLE_H__
#define __TRIPLE_H__

class Triple {
public:
    unsigned h, r, t;
    bool f;
    Triple(unsigned h_ = 0, unsigned r_ = 0, unsigned t_ = 0, bool f_ = true) :
        h(h_), r(r_), t(t_), f(f_) {}
    inline bool operator<(const Triple & other) const {
        if (h < other.h) return true;
        if (h > other.h) return false;
        if (r < other.r) return true;
        if (r > other.r) return false;
        return t < other.t;
    }
    inline bool operator<=(const Triple & other) const {
        if (h < other.h) return true;
        if (h > other.h) return false;
        if (r < other.r) return true;
        if (r > other.r) return false;
        return t <= other.t;
    }
    inline bool operator==(const Triple & other) const {
        return h == other.h && r == other.r && t == other.t;
    }
    inline bool operator!=(const Triple & other) const {
        return h != other.h || r != other.r || t != other.t;
    }
    inline bool operator>(const Triple & other) const {
        if (h > other.h) return true;
        if (h < other.h) return false;
        if (r > other.r) return true;
        if (r < other.r) return false;
        return t > other.t;
    }
    inline bool operator>=(const Triple & other) const {
        if (h > other.h) return true;
        if (h < other.h) return false;
        if (r > other.r) return true;
        if (r < other.r) return false;
        return t >= other.t;
    }
};

#endif
