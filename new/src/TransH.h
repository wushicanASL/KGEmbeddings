#pragma once

#include "EmbeddingModel.h"

namespace sysukg {

class TransH : public EmbeddingModel {
public:
    TransH(const DataSet & ds, unsigned dim, unsigned dim2 = 0, const EmbeddedData * ed);
    float update(const std::pair<Triple, Triple> * samples, unsigned size,
                 float rate, float margin);
    float calc_sum(const Triple & t) const;
    void output(const std::string & ext) const;
};

}