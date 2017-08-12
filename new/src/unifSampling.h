#pragma once

#include "SamplingModel.h"
#include "Triple.h"

namespace sysukg {

class unifSampling : public SamplingModel {
protected:
    inline Triple getPosSamp() const {
        return _pool[_rd(_size)];
    }

    Triple getNegSamp(const Triple & posSamp) const;

public:
    explicit unifSampling(const DataSet & ds) : SamplingModel(ds) {}
    inline std::string methodName() const {
        return "unif";
    }
};

}
