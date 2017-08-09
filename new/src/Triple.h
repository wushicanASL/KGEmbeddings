#ifndef __TRIPLE_H__
#define __TRIPLE_H__

namespace sysukg {
    class Triple {
    public:
        unsigned h, r, t;
        bool f;
        Triple(unsigned h_ = 0, unsigned r_ = 0, unsigned t_ = 0, bool f_ = true) :
            h(h_), r(r_), t(t_), f(f_) {}
        inline Triple symm() const {
            return Triple(t, r, h, f);
        }
        inline bool operator<(const Triple & other) const {
            if (h < other.h) return true;
            if (h > other.h) return false;
            if (r < other.r) return true;
            if (r > other.r) return false;
            if (t < other.t) return true;
            if (t > other.t) return false;
            return !f && other.f;
        }
        inline bool operator<=(const Triple & other) const {
            if (h < other.h) return true;
            if (h > other.h) return false;
            if (r < other.r) return true;
            if (r > other.r) return false;
            if (t < other.t) return true;
            if (t > other.t) return false;
            return !f;
        }
        inline bool operator==(const Triple & other) const {
            return h == other.h && r == other.r && t == other.t && f == other.f;
        }
        inline bool operator!=(const Triple & other) const {
            return h != other.h || r != other.r || t != other.t || f != other.f;
        }
        inline bool operator>(const Triple & other) const {
            if (h > other.h) return true;
            if (h < other.h) return false;
            if (r > other.r) return true;
            if (r < other.r) return false;
            if (t > other.t) return true;
            if (t < other.t) return false;
            return f && !other.f;
        }
        inline bool operator>=(const Triple & other) const {
            if (h > other.h) return true;
            if (h < other.h) return false;
            if (r > other.r) return true;
            if (r < other.r) return false;
            if (t > other.t) return true;
            if (t < other.t) return false;
            return f;
        }
    };
}

#endif
