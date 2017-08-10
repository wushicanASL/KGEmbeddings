#include "TransE.h"

using namespace sysukg;

TransE::TransE(const DataSet & ds, unsigned dim, const EmbeddedData & ed, bool L1_flag) :
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

float TransE::update(const Triple & pos, const Triple & neg, float rate, float margin) {
    float posval = calc_sum(pos), negval = calc_sum(neg);
    float res = 0;
    if (posval + margin > negval) {
        res = margin + posval - negval;
        float x, y;
        if (_L1_flag)
            for (unsigned i = 0; i < _dim; ++i) {
                if (frht(pos, i) < 0)
                    x = 1 * rate;
                else
                    x = -1 * rate;
                if (frht(neg, i) < 0)
                    y = 1 * rate;
                else
                    y = -1 * rate;
                vh(pos)[i] += x;
                vr(pos)[i] += x;
                vt(pos)[i] -= x;
                vh(neg)[i] -= y;
                vr(neg)[i] -= y;
                vt(neg)[i] += y;
            }
        else
            for (unsigned i = 0; i < _dim; ++i) {
                x = -2 * rate * frht(pos, i);
                y = -2 * rate * frht(neg, i);
                vh(pos)[i] += x;
                vr(pos)[i] += x;
                vt(pos)[i] -= x;
                vh(neg)[i] -= y;
                vr(neg)[i] -= y;
                vt(neg)[i] += y;
            }
        norm(vh(pos));
        norm(vr(pos));
        norm(vt(pos));
        if (pos.h != neg.h)
            norm(vh(neg));
        else
            norm(vt(neg));
/*
        float tposval, tnegval, tres;
        tposval = calc_sum(pos), tnegval = calc_sum(neg);
        tres = margin + tposval - tnegval;
        if (tres > res)
            std::cout << "ops" << std::endl;
*/
    }
    return res;
}

void TransE::output(const std::string & ext) const {
    EmbeddingModel::output(ext);
}