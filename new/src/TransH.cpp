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
    float ht = dot(_rp[t.r], _ed->second[t.h], _dim),
          tt = dot(_rp[t.r], _ed->second[t.t], _dim);
    float result = 0;
    for (unsigned i = 0; i < _dim; ++i)
        result += fabs(frhti(t, ht, tt, i));
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

void TransH::update_core(const Triple & triple, short label, float rate) {
    const float hdot = dot(_rp[triple.r], _ed->second[triple.h], _dim),
                tdot = dot(_rp[triple.r], _ed->second[triple.t], _dim);
    float x, sumx = 0;
    for (int i = 0; i < _dim; i++) {
		if (frhti(triple, hdot, tdot, i) > 0) {
			x = label * rate;
			sumx += _rp[triple.r][i];
		} else {
			x = -label * rate;
			sumx -= _rp[triple.r][i];
		}
		cvr(triple)[i] -= x;
		cvh(triple)[i] -= x;
        cvt(triple)[i] += x;
		_rp_cache[triple.r][i] += x * (hdot - tdot);
	}
	for (int i = 0; i < _dim; i++)
		_rp_cache[triple.r][i] += label * rate * sumx * (vh(triple)[i] - vt(triple)[i]);
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