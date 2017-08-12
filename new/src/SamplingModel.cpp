#include "SamplingModel.h"

using namespace sysukg;

random_device & SamplingModel::_rd = random_device::getInstance();

SamplingModel::SamplingModel(const DataSet & ds) :
    _pool(ds.ptu()), _postriples(ds.pos_hrt()), _size(ds.ptuSize()),
    _posend(ds.pos_hrt() + ds.posSize()), _entNum(ds.entityNum()) {}

void SamplingModel::sample(std::pair<Triple, Triple> * target, unsigned num) const {
    while (num > 0) {
        --num;
        target[num].first = getPosSamp();
        target[num].second = getNegSamp(target[num].first);
    }
}