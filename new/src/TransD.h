#include "EmbeddingModel.h"

namespace sysukg {

class TransD : public EmbeddingModel {
private:
    float ** _rp, ** _ep, ** _rp_cache, ** _ep_cache,
          _last_rate;
    inline float frhti(const Triple & t, float hdot, float tdot, unsigned i) const {
        return (vt(t)[i] + tdot * _rp[t.r][i]) - vr(t)[i] -
               (vh(t)[i] + hdot * _rp[t.r][i]);
    }
protected:
    inline void norm_all_cache() {
        // TODO
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