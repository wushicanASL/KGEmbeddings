#include "bernSampling.h"
#include "Triple.h"
#include "DataSet.h"

#include <utility>
#include <set>

using namespace sysukg;

bernSampling::bernSampling(const DataSet & ds) :
    unifSampling(ds), _prob([](const DataSet & ds) -> const float * {
        const unsigned relNum = ds.relationNum();
        float * res = new float[relNum];
        std::set<unsigned> leftset[relNum], rightset[relNum];
        for (const Triple * i = ds.ptu(); i < ds.ptu() + ds.ptuSize(); ++i) {
            leftset[i->r].insert(i->h);
            rightset[i->r].insert(i->t);
        }
        for (unsigned i = 0; i < relNum; ++i)
            if (leftset[i].size() == 0 && rightset[i].size() == 0)
                res[i] = 0.5;
            else
                res[i] = static_cast<float>(rightset[i].size()) /
                    (leftset[i].size() + rightset[i].size());
        return res;
    }(ds)) {}

Triple bernSampling::getNegSamp(const Triple & posSamp) const {
    Triple result(posSamp);
    unsigned * toChange;
    if (_rd(1000) < _prob[posSamp.r])
        toChange = &(result.h);
    else
        toChange = &(result.t);
    do {
        *toChange = _rd(_entNum);
    } while (look_for(_postriples, _posend, result) != nullptr);
    result.f = false;
    return std::move(result);
}

bernSampling::~bernSampling() {
    delete []_prob;
}