#pragma once

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

    const Triple * _postriples, * _pool, * _posend;
    const unsigned _size;
    const unsigned _entNum;
    const std::string _DSNAME;

    virtual Triple getPosSamp() const = 0;
    virtual Triple getNegSamp(const Triple & posSamp) const = 0;
public:
    explicit SamplingModel(const DataSet & ds, bool update = false);

    virtual void sample(std::pair<Triple, Triple> * target, unsigned num = 1) const;

    virtual std::string methodName() const = 0;
    inline unsigned size() const {
        return _size;
    }
    inline const std::string & dsname() const {
        return _DSNAME;
    }
};

}
