#ifndef __SAMPLINGMODEL_H__
#define __SAMPLINGMODEL_H__

#include "DataSet.h"
#include "Triple.h"
#include "random.h"

#include <utility>
#include <vector>
#include <string>

namespace sysukg {
class SamplingModel {
protected:
    static random_device & _rd;

    const DataSet::tplset _alltriples;
    typedef std::vector<Triple> tplvec;
    const tplvec _pool;
    const unsigned _entNum;

    virtual Triple getPosSamp() const = 0;
    virtual Triple getNegSamp(const Triple & posSamp) const = 0;
public:
    explicit SamplingModel(const DataSet & ds);

    typedef std::pair<Triple, Triple> tplpair;
    inline virtual tplpair sample() const {
        Triple posSamp = getPosSamp();
        return tplpair(posSamp, getNegSamp(posSamp));
    }

    virtual std::string methodName() const = 0;
    inline unsigned size() const {
        return _pool.size();
    }
};
}

#endif
