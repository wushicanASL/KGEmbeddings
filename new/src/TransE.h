#pragma once

#include "EmbeddingModel.h"
#include "Triple.h"

#include <cmath>
#include <string>

namespace sysukg {

class TransE : public EmbeddingModel {
private:
    inline float frhti(const Triple & t, unsigned i) const {
        return vh(t)[i] + vr(t)[i] - vt(t)[i];
    }
protected:
    void update_core(const Triple & triple, short label, float rate);
public:
    TransE(const DataSet & ds, unsigned dim,
            const std::string & ext = "");

    inline std::string methodName() const {
        return "TransE";
    }
    float calc_sum(const Triple & t) const;
};

}
