#include "SamplingModel.h"

using namespace sysukg;

random_device & SamplingModel::_rd = random_device::getInstance();

SamplingModel::SamplingModel(const DataSet & ds) :
    _pool([](const DataSet & ds) -> tplvec {
        tplvec res(ds.trainset().begin(), ds.trainset().end());
        res.insert(res.begin(), ds.updateset().begin(), ds.updateset().end());
        return res;
    }(ds)),
    _entNum(ds.entityNum()), _alltriples(ds.allPosTriples()) {}