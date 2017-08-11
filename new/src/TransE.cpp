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
        posval = calc_sum(samples[i].first);
        negval = calc_sum(samples[i].second);
        if (posval + margin > negval) {
            res += margin + posval - negval;
            if (_L1_flag)
                for (unsigned j = 0; j < _dim; ++j) {
                    if (frht(samples[i].first, j) < 0)
                        x = 1 * rate;
                    else
                        x = -1 * rate;
                    if (frht(samples[i].second, j) < 0)
                        y = 1 * rate;
                    else
                        y = -1 * rate;
                    vh(samples[i].first)[j] += x;
                    vr(samples[i].first)[j] += x;
                    vt(samples[i].first)[j] -= x;
                    vh(samples[i].second)[j] -= y;
                    vr(samples[i].second)[j] -= y;
                    vt(samples[i].second)[j] += y;
                }
            else
                for (unsigned j = 0; j < _dim; ++j) {
                    x = -2 * rate * frht(samples[i].first, j);
                    y = -2 * rate * frht(samples[i].second, j);
                    vh(samples[i].first)[j] += x;
                    vr(samples[i].first)[j] += x;
                    vt(samples[i].first)[j] -= x;
                    vh(samples[i].second)[j] -= y;
                    vr(samples[i].second)[j] -= y;
                    vt(samples[i].second)[j] += y;
                }
            norm(vh(samples[i].first));
            norm(vr(samples[i].first));
            norm(vt(samples[i].first));
            if (samples[i].first.h != samples[i].second.h)
                norm(vh(samples[i].second));
            else
                norm(vt(samples[i].second));
        }
    }
    return res;
}

void TransE::output(const std::string & ext) const {
    EmbeddingModel::output(ext);
}