#include "unifSampling.h"
#include "Triple.h"

using namespace sysukg;

Triple unifSampling::getNegSamp(const Triple & posSamp) const {
    Triple result(posSamp);
    if (_rd() & 1)
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