#ifndef __BERNSAMPLING_H__
#define __BERNSAMPLING_H__

#include "unifSampling.h"
#include "Triple.h"

#include <vector>
#include <utility>

namespace sysukg {
class bernSampling : public unifSampling {
private:
    const float * _prob;
public:
    explicit bernSampling(const DataSet & ds);
    Triple getNegSamp(const Triple & posSamp) const;
    inline std::string methodName() const {
        return "bern";
    }
    ~bernSampling();
};
}

#endif
