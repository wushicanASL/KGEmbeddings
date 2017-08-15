#include "TransE.h"

using namespace sysukg;

TransE::TransE(const DataSet & ds, unsigned dim, const EmbeddedData * ed, bool L1_flag) :
    EmbeddingModel(ds, dim, ed), _L1_flag(L1_flag) {}

float TransE::calc_sum(const Triple & t) const {
    float sum = 0;
    if (_L1_flag) {
        for (unsigned i = 0; i < _dim; ++i)
            sum += fabs(frht(t, i));
    } else {
        for (unsigned i = 0; i < _dim; ++i)
            sum += sqr(frht(t, i));
        sum = sqrt(sum);
    }
    return sum;
}

void TransE::update_core(const Triple & triple, short label, float rate) {
    float x;
    if (_L1_flag)
        for (unsigned j = 0; j < _dim; ++j) {
            if (frht(triple, j) < 0)
                x = label * rate;
            else
                x = -label * rate;
            cvh(triple)[j] += x;
            cvr(triple)[j] += x;
            cvt(triple)[j] -= x;
        }
    else
        for (unsigned j = 0; j < _dim; ++j) {
            x = -2 * label * rate * frht(triple, j);
            cvh(triple)[j] += x;
            cvr(triple)[j] += x;
            cvt(triple)[j] -= x;
        }
}
