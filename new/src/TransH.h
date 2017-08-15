#pragma once

#include "EmbeddingModel.h"

namespace sysukg {

class TransH : public EmbeddingModel {
private:
    float ** _rp, ** _rp_cache, _last_rate;
    inline float dot(const float * x, const float * y, unsigned size) const {
        float result = 0;
        for (unsigned i = 0; i < size; ++i)
            result += x[i] * y[i];
        return result;
    }
    inline float dot(unsigned rel, unsigned ent) const {
        return dot(_rp[rel], _ed->second[ent], _dim);
    }
    inline float frhti(const Triple & t, unsigned i) const {
        return (vt(t)[i] - dot(t.r, t.t) * _rp[t.r][i]) - vr(t)[i] -
               (vh(t)[i] - dot(t.r, t.h) * _rp[t.r][i]);
    }
    inline void strict_norm(float * v) {
        float x = vec_len(v, _dim);
        for (unsigned i = 0; i < _dim; ++i)
            v[i] /= x;
    }
    void norm(float * rv, float * rp, float rate);
    inline void norm_all_cache() override {
        EmbeddingModel::norm_all_cache();
        for (unsigned i = 0; i < _relSize; ++i)
            norm(_ed->first[i], _rp_cache[i], _last_rate);
    }
public:
    TransH(const DataSet & ds, unsigned dim,
           const EmbeddedData * ed = nullptr, const float ** rp = nullptr);
    float update(const std::pair<Triple, Triple> * samples, unsigned size,
                 float rate, float margin);
    float calc_sum(const Triple & t) const;
    void output(const std::string & ext) const;
    std::string methodName() const {
        return "TransH";
    }
    ~TransH();
};

}