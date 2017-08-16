#pragma once

#include "EmbeddingModel.h"

namespace sysukg {

class TransH : public EmbeddingModel {
private:
    float ** _rp, ** _rp_cache, _last_rate;
protected:
    inline float * vrp(const Triple & t) {
        return _rp[t.r];
    }
    inline float * cvrp(const Triple & t) {
        return _rp_cache[t.r];
    }
    inline const float * vrp(const Triple & t) const {
        return _rp[t.r];
    }
    inline const float * cvrp(const Triple & t) const {
        return _rp_cache[t.r];
    }
    inline float frhti(const Triple & t, float hdot, float tdot, unsigned i) const {
        return (vt(t)[i] - tdot * vrp(t)[i]) - vr(t)[i] -
               (vh(t)[i] - hdot * vrp(t)[i]);
    }
    inline void strict_norm(float * v) {
        float x = vec_len(v, _dim);
        for (unsigned i = 0; i < _dim; ++i)
            v[i] /= x;
    }
    void norm(float * rv, float * rp, float rate);
    inline void norm_cache(const std::pair<Triple, Triple> & sample) {
        EmbeddingModel::norm_cache(sample);
        norm(cvh(sample.first), cvrp(sample.first), _last_rate);
        norm(cvt(sample.first), cvrp(sample.first), _last_rate);
        if (sample.first.h == sample.second.h)
            norm(cvt(sample.second), cvrp(sample.first), _last_rate);
        else
            norm(cvt(sample.second), cvrp(sample.first), _last_rate);
    }
    void update_core(const Triple & triple, short label, float rate);
public:
    TransH(const DataSet & ds, unsigned dim,
           const EmbeddedData * ed = nullptr, const float ** rp = nullptr);
    inline void cache_store() {
        EmbeddingModel::cache_store();
        matrixCopy(_rp_cache, _rp, _relSize, _dim);
    }
    inline void cache_load() {
        EmbeddingModel::cache_load();
        matrixCopy(_rp, _rp_cache, _relSize, _dim);
    }
    inline float update(const std::pair<Triple, Triple> * samples, unsigned size,
                        float rate, float margin) {
        _last_rate = rate;
        EmbeddingModel::update(samples, size, rate, margin);
    }
    float calc_sum(const Triple & t) const;
    void output(const std::string & ext) const;
    std::string methodName() const {
        return "TransH";
    }
    ~TransH();
};

}