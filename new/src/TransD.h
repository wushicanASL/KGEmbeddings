#include "EmbeddingModel.h"

namespace sysukg {

class TransD : public EmbeddingModel {
private:
    float ** _rp, ** _ep, ** _rp_cache, ** _ep_cache,
          _last_rate;
    void norm(float * ev, float * epv, float * rpv, float rate);
protected:
    inline float * vrp(const Triple & t) {
        return _rp[t.r];
    }
    inline float * cvrp(const Triple & t) {
        return _rp_cache[t.r];
    }
    inline float * vhp(const Triple & t) {
        return _ep[t.h];
    }
    inline float * cvhp(const Triple & t) {
        return _ep_cache[t.h];
    }
    inline float * vtp(const Triple & t) {
        return _ep[t.t];
    }
    inline float * cvtp(const Triple & t) {
        return _ep_cache[t.t];
    }
    inline const float * vrp(const Triple & t) const {
        return _rp[t.r];
    }
    inline const float * cvrp(const Triple & t) const {
        return _rp_cache[t.r];
    }
    inline const float * vhp(const Triple & t) const {
        return _ep[t.h];
    }
    inline const float * cvhp(const Triple & t) const {
        return _ep_cache[t.h];
    }
    inline const float * vtp(const Triple & t) const {
        return _ep[t.t];
    }
    inline const float * cvtp(const Triple & t) const {
        return _ep_cache[t.t];
    }

    inline void norm_cache(const std::pair<Triple, Triple> & sample) {
        EmbeddingModel::norm_cache(sample);
        norm(cvh(sample.first), cvhp(sample.first), cvrp(sample.first), _last_rate);
        norm(cvt(sample.first), cvtp(sample.first), cvrp(sample.first), _last_rate);
        if (sample.first.h == sample.second.h)
            norm(cvt(sample.second), cvtp(sample.second), cvrp(sample.second), _last_rate);
        else
            norm(cvh(sample.second), cvhp(sample.second), cvrp(sample.second), _last_rate);
    }
    void update_core(const Triple & triple, short label, float rate);
public:
    TransD(const DataSet & ds, unsigned dim, const EmbeddedData * ed = nullptr,
           const float ** rp = nullptr, const float ** ep = nullptr);
    std::string methodName() const {
        return "TransD";
    }
    inline void cache_store() {
        EmbeddingModel::cache_store();
        matrixCopy(_rp_cache, _rp, _relSize, _dim);
        matrixCopy(_ep_cache, _ep, _entSize, _dim);
    }
    inline void cache_load() {
        EmbeddingModel::cache_load();
        matrixCopy(_rp, _rp_cache, _relSize, _dim);
        matrixCopy(_ep, _ep_cache, _entSize, _dim);
    }

    float calc_sum(const Triple & t) const;
    inline float update(const std::pair<Triple, Triple> * samples, unsigned size,
                 float rate, float margin) {
        _last_rate = rate;
        EmbeddingModel::update(samples, size, rate, margin);
    }
    void output(const std::string & ext) const;
    ~TransD();
};

}