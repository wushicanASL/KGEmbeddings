#include "TransH.h"
#include <cstring>
#include <cmath>
#include <thread>

using namespace sysukg;

TransH::TransH(const DataSet & ds, unsigned dim,
               const EmbeddedData * ed, const float ** rp) :
    EmbeddingModel(ds, dim, ed), _last_rate(0) {
    _rp = new float*[_relSize];
    _rp_cache = new float*[_relSize];
    for (unsigned i = 0; i < _relSize; ++i) {
        _rp[i] = new float[dim];
        _rp_cache[i] = new float[dim];
    }
    if (rp == nullptr)
        matrixReset(_rp, _relSize, dim);
    else
        matrixCopy(_rp, rp, _relSize, dim);
}

float TransH::calc_sum(const Triple & t) const {
    float ht = dot(t.r, t.h), tt = dot(t.r, t.t);
    float result = 0;
    for (unsigned i = 0; i < _dim; ++i)
        result += fabs(frhti(t, i));
    return result;
}

void TransH::norm(float * rv, float * rp, float rate) {
    strict_norm(rp);
    float x = dot(rv, rp, _dim);
    if (x > 0.1)
        for (unsigned i = 0; i < _dim; ++i) {
            rv[i] -= rate * rp[i];
            rp[i] -= rate * rv[i];
        }
    strict_norm(rp);
}

float TransH::update(const std::pair<Triple, Triple> * samples, unsigned size,
                     float rate, float margin) {
    _last_rate = rate;
    float posval, negval, res = 0;
    float posx, negx, possumx, negsumx;
    for (unsigned i = 0; i < size; ++i) {
        const Triple & pos = samples[i].first, & neg = samples[i].second;
        posval = calc_sum(pos);
        negval = calc_sum(neg);
        possumx = negsumx = 0;
        if (posval + margin > negval) {
            res += margin + posval - negval;
            for (unsigned j = 0; j < _dim; ++j) {
                if (frhti(pos, j) > 0)
                    posx = rate;
                else
                    posx = -rate;
                if (frhti(neg, j) > 0)
                    negx = rate;
                else
                    negx = -rate;
                possumx += posx * _rp[pos.r][j];
                negsumx += negx * _rp[neg.r][j];
                cvr(pos)[j] -= posx;
                cvh(pos)[j] -= posx;
                cvt(pos)[j] += posx;
                _rp_cache[pos.r][j] += posx * (dot(pos.r, pos.h) - dot(pos.r, pos.t));
                cvr(neg)[j] += negx;
                cvr(neg)[j] += negx;
                cvt(neg)[j] -= negx;
                _rp_cache[neg.r][j] -= negx * (dot(neg.r, neg.h) - dot(neg.r, neg.t));
            }
            for (unsigned j = 0; j < _dim; ++j) {
                _rp_cache[pos.r][j] += possumx * (vh(pos)[j] - vt(pos)[j]);
                _rp_cache[neg.r][j] -= negsumx * (vh(neg)[j] - vt(neg)[j]);
            }
        }
    }
}

void TransH::output(const std::string & ext) const {
    static const unsigned short filenum = 3;
    std::thread * threads[filenum] = {
        new std::thread{single_output, std::move("relation2vec." + ext),
            std::ref(_relSize), std::ref(_dim),std::ref(_ed->first)},
        new std::thread{single_output, std::move("entity2vec." + ext),
            std::ref(_entSize), std::ref(_dim), std::ref(_ed->second)},
        new std::thread{single_output, std::move("relproj2vec." + ext),
            std::ref(_relSize), std::ref(_dim), std::ref(_rp)}
    };
    for (char i = 0; i < filenum; ++i)
        threads[i]->join();
    for (char i = 0; i < filenum; ++i)
        delete threads[i];
}

TransH::~TransH() {
    for (unsigned i = 0; i < _relSize; ++i) {
        delete[] _rp[i];
        delete[] _rp_cache[i];
    }
    delete[] _rp;
    delete[] _rp_cache;
}