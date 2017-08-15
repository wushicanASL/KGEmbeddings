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

float TransE::update(const std::pair<Triple, Triple> * samples,
                     unsigned size, float rate, float margin) {
    float posval, negval;
    float res = 0, x, y;
    for (unsigned i = 0; i < size; ++i) {
        const Triple & pos = samples[i].first, & neg = samples[i].second;
        posval = calc_sum(pos);
        negval = calc_sum(neg);
        if (posval + margin > negval) {
            res += margin + posval - negval;
            if (_L1_flag)
                for (unsigned j = 0; j < _dim; ++j) {
                    if (frht(pos, j) < 0)
                        x = 1 * rate;
                    else
                        x = -1 * rate;
                    if (frht(neg, j) < 0)
                        y = 1 * rate;
                    else
                        y = -1 * rate;
                    cvh(pos)[j] += x;
                    cvr(pos)[j] += x;
                    cvt(pos)[j] -= x;
                    cvh(neg)[j] -= y;
                    cvr(neg)[j] -= y;
                    cvt(neg)[j] += y;
                }
            else
                for (unsigned j = 0; j < _dim; ++j) {
                    x = -2 * rate * frht(pos, j);
                    y = -2 * rate * frht(neg, j);
                    cvh(pos)[j] += x;
                    cvr(pos)[j] += x;
                    cvt(pos)[j] -= x;
                    cvh(neg)[j] -= y;
                    cvr(neg)[j] -= y;
                    cvt(neg)[j] += y;
                }
        }
    }
    return res;
}

void TransE::output(const std::string & ext) const {
    EmbeddingModel::output(ext);
}