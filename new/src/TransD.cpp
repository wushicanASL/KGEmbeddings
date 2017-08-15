#include "TransD.h"

#include <thread>

using namespace sysukg;

TransD::TransD(const DataSet & ds, unsigned dim, const EmbeddedData * ed,
               const float ** rp, const float ** ep) :
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

    _ep = new float*[_entSize];
    _ep_cache = new float*[_entSize];
    for (unsigned i = 0; i < _entSize; ++i) {
        _ep[i] = new float[dim];
        _ep_cache[i] = new float[dim];
    }
    if (ep == nullptr)
        matrixReset(_ep, _entSize, dim);
    else
        matrixCopy(_ep, ep, _entSize, dim);
}

TransD::~TransD() {
    for (unsigned i = 0; i < _relSize; ++i) {
        delete[] _rp[i];
        delete[] _rp_cache[i];
    }
    delete[] _rp;
    delete[] _rp_cache;
    for (unsigned i = 0; i < _entSize; ++i) {
        delete[] _ep[i];
        delete[] _ep_cache[i];
    }
    delete[] _ep;
    delete[] _ep_cache;
}

float TransD::calc_sum(const Triple & t) const {
    float hdot = dot(vh(t), _ep[t.h], _dim), tdot = dot(vt(t), _ep[t.t], _dim);
    float result = 0;
    for (unsigned i = 0; i < _dim; ++i)
        result += fabs(frhti(t, hdot, tdot, i));
    return result;
}

void TransD::update_core(const Triple & triple, short label, float rate) {
    const float hdot = dot(vh(triple), _ep[triple.h], _dim),
                tdot = dot(vt(triple), _ep[triple.t], _dim);
    float x, s = 0;
	for (int i = 0; i < _dim; i++) {
		if (frhti(triple, hdot, tdot, i) > 0)
			x = label * rate;
		else
			x = -label * rate;
		s += x * _rp[triple.r][i];
		_rp_cache[triple.r][i] += x * (tdot - hdot);
		cvr(triple)[i] -= x;
        cvh(triple)[i] -= x;
        cvt(triple)[i] += x;
	}
	for (int i = 0; i < _dim; i++) {
		cvh(triple)[i] -= s * _ep[triple.h][i];
		_ep_cache[triple.h][i] -= s * vh(triple)[i];
		cvt(triple)[i] += s * _ep[triple.t][i];
		_ep_cache[triple.t][i] += s * vt(triple)[i];
	}
}

void TransD::output(const std::string & ext) const {
    static const unsigned short filenum = 4;
    std::thread * threads[filenum] = {
        new std::thread{single_output, std::move("relation2vec." + ext),
            std::ref(_relSize), std::ref(_dim),std::ref(_ed->first)},
        new std::thread{single_output, std::move("entity2vec." + ext),
            std::ref(_entSize), std::ref(_dim), std::ref(_ed->second)},
        new std::thread{single_output, std::move("relproj2vec." + ext),
            std::ref(_relSize), std::ref(_dim), std::ref(_rp)},
        new std::thread{single_output, std::move("entproj2vec." + ext),
            std::ref(_relSize), std::ref(_dim), std::ref(_rp)}
    };
    for (char i = 0; i < filenum; ++i)
        threads[i]->join();
    for (char i = 0; i < filenum; ++i)
        delete threads[i];
}
