#include "bernSampling.h"
#include "Triple.h"
#include "DataSet.h"

#include <utility>
#include <vector>
#include <set>

using namespace sysukg;

bernSampling::bernSampling(const DataSet & ds) :
    unifSampling(ds), _counter([](const DataSet & ds) -> std::pair<fltvec, fltvec> {
        fltvec left_num(0, ds.relationNum()), right_num(0, ds.relationNum());
        std::set<unsigned> leftset, rightset;
        for (unsigned i = 0; i < ds.relationNum(); ++i) {
            const DataSet::tplset & rset = ds.getIndex_r(i);
            leftset.clear();
            rightset.clear();
            for (auto & item : rset)
                if (item.f && (ds.trainset().find(item) != ds.trainset().end() || ds.updateset().find(item) != ds.updateset().end())) {
                    leftset.insert(item.h);
                    rightset.insert(item.t);
                }
            left_num[i] = static_cast<float>(rset.size()) / leftset.size();
            right_num[i] = static_cast<float>(rset.size()) / rightset.size();
        }
        return std::pair<fltvec, fltvec>(left_num, right_num);
    }(ds)) {}

Triple bernSampling::getNegSamp(const Triple & posSamp) const {
    Triple result(posSamp);
    float pr = 1000*_counter.first[posSamp.r]/(_counter.second[posSamp.r]+_counter.first[posSamp.r]);
    if (_rd(1000) > pr)
        do {
            result.h = _rd(_entNum);
        } while (_alltriples.find(result) == _alltriples.end());
    else
        do {
            result.t = _rd(_entNum);
        } while (_alltriples.find(result) == _alltriples.end());
    result.f = false;
    return result;
}