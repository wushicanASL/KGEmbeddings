#include "EmbeddingModel.h"

#include <string>
#include <fstream>

using namespace sysukg;

random_device & EmbeddingModel::_rd = random_device::getInstance();

EmbeddingModel::EmbeddingModel(const DataSet & ds, unsigned dim, const EmbeddedData & ed) :
    _ds(ds), _dim(dim) {
    if (ed == emptyED())
        _ed = init(dim, ds.relationNum(), ds.entityNum());
    else
        _ed = ed;
}

void EmbeddingModel::output(const std::string & ext) const {
    std::ofstream fout("entity2vec." + ext);
    for (unsigned i = 0; i < _ed.second.size(); ++i) {
        fout << _ed.second[i][0];
        for (unsigned j = 1; j < _dim; ++j)
            fout << ' ' << _ed.second[i][j];
        fout << std::endl;
    }
    fout.close();

    fout.open("relation2vec." + ext);
    for (unsigned i = 0; i < _ed.first.size(); ++i) {
        fout << _ed.first[i][0];
        for (unsigned j = 1; j < _dim; ++j)
            fout << ' ' << _ed.first[i][j];
        fout << std::endl;
    }
    fout.close();
}

EmbeddingModel::EmbeddedData EmbeddingModel::init(unsigned dim, unsigned relNum, unsigned entNum) {
    EmbeddedData res(fltmat(relNum, fltvec(dim, 0)), fltmat(entNum, fltvec(dim, 0)));
    for (auto & vec : res.first)
        for (auto & item : vec)
            item = _rd.randn(0, 1.0 / dim, -6 / sqrt(dim), 6 / sqrt(dim));
    for (auto & vec : res.second)
        for (auto & item : vec)
            item = _rd.randn(0, 1.0 / dim, -6 / sqrt(dim), 6 / sqrt(dim));
    return res;
}