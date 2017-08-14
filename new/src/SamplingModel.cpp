#include "SamplingModel.h"

using namespace sysukg;

random_device & SamplingModel::_rd = random_device::getInstance();

SamplingModel::SamplingModel(const DataSet & ds, bool update) :
    _pool(update?ds.ptu():ds.pt()), _postriples(ds.pos_hrt()),
    _size(update?ds.ptuSize():ds.ptSize()), _DSNAME(ds.name()),
    _posend(ds.pos_hrt() + ds.posSize()), _entNum(ds.entityNum()) {}

void SamplingModel::sample(std::pair<Triple, Triple> * target, unsigned num) const {
    while (num > 0) {
        --num;
        target[num].first = getPosSamp();
        target[num].second = getNegSamp(target[num].first);
    }
}