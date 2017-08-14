#include <functional>

#include "unifSampling.h"
#include "Triple.h"
#include "DataSet.h"

using namespace sysukg;

unifSampling::unifSampling(const DataSet & ds, bool update) : SamplingModel(ds, update) {}

Triple unifSampling::getNegSamp(const Triple & posSamp) const {
    Triple result(posSamp);
    unsigned * toChange;
    if (_rd() & 1)
        toChange = &(result.h);
    else
        toChange = &(result.t);
    do {
        *toChange = _rd(_entNum);
    } while (look_for(_postriples, _posend, result) != nullptr);
    result.f = false;
    return std::move(result);
}