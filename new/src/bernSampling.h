#ifndef __BERNSAMPLING_H__
#define __BERNSAMPLING_H__

#include "unifSampling.h"
#include "Triple.h"

#include <vector>
#include <utility>

namespace sysukg {
class bernSampling : public unifSampling {
private:
    typedef std::vector<float> fltvec;
    const std::pair<fltvec, fltvec> _counter;
public:
    explicit bernSampling(const DataSet & ds);
    Triple getNegSamp(const Triple & posSamp) const;
    inline std::string methodName() const {
        return "bern";
    }
};
}

#endif
