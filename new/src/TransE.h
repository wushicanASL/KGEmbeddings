#pragma once

#include "EmbeddingModel.h"
#include "Triple.h"

#include <cmath>
#include <string>

namespace sysukg {

class TransE : public EmbeddingModel {
private:
    bool _L1_flag;

    inline float frht(const Triple & t, unsigned i) const {
        return vh(t)[i] + vr(t)[i] - vt(t)[i];
    }
public:
    TransE(const DataSet & ds, unsigned dim,
            const EmbeddedData * ed = nullptr,
            bool L1_flag = true);

    inline std::string methodName() const {
        return "TransE";
    }
    float calc_sum(const Triple & t) const;
    float update(const std::pair<Triple, Triple> * samples,
                 unsigned size, float rate, float margin);
    void output(const std::string & ext) const;
};

}
