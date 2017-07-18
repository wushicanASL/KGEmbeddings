#ifndef __EMBEDDINGMODEL_H__
#define __EMBEDDINGMODEL_H__

#include <cmath>
#include <cstdlib>

#include "DataSet.h"
#include "Triple.h"

class EmbeddingModel {
private:
    unsigned long long * next_random;

protected:
    DataSet _ds;
    unsigned _dim;
    typedef std::vector<float> fltvec;
    typedef std::vector< std::vector<float> > fltmat;
    typedef std::pair<fltmat, fltmat> EmbeddedData; // first = relations, second = entities
    EmbeddedData _ed;

    inline unsigned long long randd(int id) {
        next_random[id] = next_random[id] * (unsigned long long)25214903917 + 11;
        return next_random[id];
    }
    inline int rand_max(int id, int x) {
        int res = randd(id) % x;
        while (res<0)
            res+=x;
        return res;
    }
    inline float rand(float min, float max) const {
        return min + (max - min) * rand() / (RAND_MAX + 1.0);
    }
    inline float normal(float x, float miu, float sigma) const {
        return 1.0/sqrt(2*pi)/sigma*exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
    }
    inline float randn(float miu,float sigma, float min ,float max) const {
        float x, y, dScope;
        do {
            x = rand(min,max);
            y = normal(x,miu,sigma);
            dScope=rand(0.0,normal(miu,miu,sigma));
        } while (dScope > y);
        return x;
    }
    inline float sqr(float x) const {
        return x * x;
    }
    inline void norm(fltvec & v) {
        float x = 0;
        for (auto & item : v)
            x += sqr(item);
        x = sqrt(x);
        if (x>1)
            for (auto & item : v)
                item /= x;
    }

    inline EmbeddedData init(unsigned dim, unsigned relNum, unsigned entNum) const {
        return EmbeddedData(
            fltmat(relNum, norm(fltvec(dim, randn(0, 1.0 / dim, -6 / sqrt(dim), 6 / sqrt(dim))))),
            fltmat(entNum, norm(fltvec(dim, randn(0, 1.0 / dim, -6 / sqrt(dim), 6 / sqrt(dim)))))
        );
    }

public:
    EmbeddingModel(DataSet ds, unsigned dim,
                   EmbeddedData ed = init(dim, ds.relationNum(), ds.entityNum())) :
        _ds(ds), _dim(dim), _ed(ed) {}
    virtual float calc_sum(Triple t) = 0;
};

#endif
