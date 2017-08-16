#include "TransE.h"

using namespace sysukg;

TransE::TransE(const DataSet & ds, unsigned dim, const EmbeddedData * ed) :
    EmbeddingModel(ds, dim, ed, false) {}

float TransE::calc_sum(const Triple & t) const {
    float sum = 0;
    for (unsigned i = 0; i < _dim; ++i)
        sum += fabs(vt(t)[i] - vh(t)[i] - vr(t)[i]);
    return sum;
}

void TransE::update_core(const Triple & triple, short label, float rate) {
    float x;
    for (unsigned i = 0; i < _dim; ++i) {
        if (vt(triple)[i] - vh(triple)[i] - vr(triple)[i] > 0)
            x = -label * rate;
        else
            x = label * rate;
        vh(triple)[i] -= x;
        vr(triple)[i] -= x;
        vt(triple)[i] += x;
    }
}
