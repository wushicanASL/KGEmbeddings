#include "EmbeddingModel.h"

#include <string>
#include <fstream>
#include <cstring>
#include <thread>
#include <functional>

using namespace sysukg;

random_device & EmbeddingModel::_rd = random_device::getInstance();

EmbeddingModel::EmbeddingModel(const DataSet & ds, unsigned dim, const EmbeddedData * ed) :
    _ds(ds), _dim(dim), _relSize(ds.relationNum()), _entSize(ds.entityNum()) {
    _ed = new EmbeddedData;
    _ed->first = new float*[_relSize];
    for (unsigned i = 0; i < _relSize; ++i)
        _ed->first[i] = new float[dim];
    _ed->second = new float*[_entSize];
    for (unsigned i = 0; i < _entSize; ++i)
        _ed->second[i] = new float[dim];

    if (ed == nullptr)
        EDreset();
    else
        EDcopy(ed);
}


void EmbeddingModel::single_output(const std::string & filename,
        unsigned num, unsigned dim, float ** mat) {
    std::ofstream fout(filename);
    for (unsigned i = 0; i < num; ++i) {
        fout << mat[i][0];
        for (unsigned j = 1; j < dim; ++j)
            fout << ' ' << mat[i][j];
        fout << std::endl;
    }
    fout.close();
}

void EmbeddingModel::output(const std::string & ext) const {
    std::thread * threads[2] = {
        new std::thread{single_output, std::move("relation2vec." + ext),
            std::ref(_relSize), std::ref(_dim),std::ref(_ed->first)},
        new std::thread{single_output, std::move("entity2vec." + ext),
            std::ref(_entSize), std::ref(_dim), std::ref(_ed->second)}
    };
    for (char i = 0; i < 2; ++i)
        threads[i]->join();
    for (char i = 0; i < 2; ++i)
        delete threads[i];
}

void EmbeddingModel::EDreset() {
    for (unsigned i = 0; i < _relSize; ++i)
        for (unsigned j = 0; j < _dim; ++j)
            _ed->first[i][j] = _rd.randn(0, 1.0 / _dim, -6 / sqrt(_dim), 6 / sqrt(_dim));

    for (unsigned i = 0; i < _entSize; ++i)
        for (unsigned j = 0; j < _dim; ++j)
            _ed->second[i][j] = _rd.randn(0, 1.0 / _dim, -6 / sqrt(_dim), 6 / sqrt(_dim));
}

void EmbeddingModel::EDcopy(const EmbeddedData * ed) {
    for (unsigned i = 0; i < _relSize; ++i)
        memcpy(_ed->first[i], ed->first[i], _dim * sizeof(float));
    for (unsigned i = 0; i < _entSize; ++i)
        memcpy(_ed->second[i], ed->second[i], _dim * sizeof(float));
}