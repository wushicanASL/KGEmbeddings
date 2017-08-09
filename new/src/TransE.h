#ifndef __TRANSE_H__
#define __TRANSE_H__

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
            const EmbeddedData & ed = emptyED(),
            bool L1_flag = true);

    inline std::string methodName() const {
        return "TransE";
    }
    float calc_sum(const Triple & t) const;
    void update(const Triple & pos, const Triple & neg, float rate);
    void output(const std::string & ext) const;
};
}

#endif
