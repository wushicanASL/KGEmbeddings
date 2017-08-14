#include "EmbeddingModel.h"

#include <string>
#include <fstream>
#include <cstring>
#include <thread>
#include <algorithm>
#include <functional>
#include <atomic>

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

void EmbeddingModel::runLinkPrediction(std::ostream & os, unsigned threadnum) const {
    std::atomic<unsigned> hrsum, trsum, hrhit, trhit,
                          hfsum, tfsum, hfhit, tfhit;
    std::atomic<unsigned> testnum;
    testnum.store(0);
    hrsum.store(0); trsum.store(0); hrhit.store(0); trhit.store(0);
    hfsum.store(0); tfsum.store(0); hfhit.store(0); tfhit.store(0);
    const Triple * ds_pos_hrt_end = _ds.pos_hrt() + _ds.posSize();
    std::function<void(unsigned, unsigned)> func =
        [&](unsigned s, unsigned t) -> void {
            float baseScore;
            unsigned hraw, hfiltered, traw, tfiltered;
            Triple hr, tr;
            for (unsigned i = s; i < t; ++i)
                if (_ds.testset()[i].f) {
                    ++testnum;
                    baseScore = calc_sum(_ds.testset()[i]);
                    hraw = traw = 1;
                    hfiltered = tfiltered = 0;
                    hr = tr = _ds.testset()[i];
                    for (unsigned j = 0; j < _entSize; ++j) {
                        hr.h = tr.t = j;
                        if (calc_sum(hr) < baseScore)
                            if (hr.h != _ds.testset()[i].h) {
                                ++hraw;
                                if (look_for(_ds.pos_hrt(), ds_pos_hrt_end, hr, Triple_hrt_less) != nullptr)
                                    ++hfiltered;
                            }
                        if (calc_sum(tr) < baseScore)
                            if (tr.t != _ds.testset()[i].t) {
                                ++traw;
                                if (look_for(_ds.pos_hrt(), ds_pos_hrt_end, tr, Triple_hrt_less) != nullptr)
                                    ++tfiltered;
                            }
                    }
                    hrsum += hraw;
                    hfiltered = hraw - hfiltered;
                    hfsum += hfiltered;
                    if (hraw <= 10) ++hrhit;
                    if (hfiltered <= 10) ++hfhit;
                    trsum += traw;
                    tfiltered = traw - tfiltered;
                    tfsum += tfiltered;
                    if (traw <= 10) ++trhit;
                    if (tfiltered <= 10) ++tfhit;
                }
        };
    float psize = static_cast<float>(_ds.testSize()) / threadnum;
    std::thread * threads[threadnum];
    float sum = 0;
    unsigned last = 0, temp;
    for (unsigned i = 0; i < threadnum - 1; ++i) {
        sum += psize;
        temp = static_cast<unsigned>(sum + 0.5);
        threads[i] = new std::thread(func, last, temp);
        last = temp;
    }
    threads[threadnum - 1] = new std::thread(func, last, _ds.testSize());
    for (unsigned i = 0; i < threadnum; ++i)
        threads[i]->join();
    float ftestnum = static_cast<float>(testnum);
    os << hrsum/ftestnum << ' ' << hrhit/ftestnum << ' ' << hfsum/ftestnum << ' ' << hfhit/ftestnum << ' '
       << trsum/ftestnum << ' ' << trhit/ftestnum << ' ' << tfsum/ftestnum << ' ' << tfhit/ftestnum << std::endl;
    for (unsigned i = 0; i < threadnum; ++i)
        delete threads[i];
}

void EmbeddingModel::runClassificationTest(std::ostream & os) const {
    TripleWithScore * valid = new TripleWithScore[_ds.validSize()],
                    * test = new TripleWithScore[_ds.testSize()];
    for (unsigned i = 0; i < _ds.validSize(); ++i) {
        valid[i].t = _ds.validset() + i;
        valid[i].score = calc_sum(*valid[i].t);
    }
    for (unsigned i = 0; i < _ds.testSize(); ++i) {
        test[i].t = _ds.testset() + i;
        test[i].score = calc_sum(*test[i].t);
    }
    std::sort(valid, valid + _ds.validSize());
    float bestThres = 0;
    unsigned bestCorrect = 0, leftpos = 0;
    for (unsigned i = 1; i < _ds.validSize(); ++i) {
        if (valid[i - 1].t->f)
            ++leftpos;
        if (leftpos + (_ds.validSize() >> 1) - (i - leftpos) > bestCorrect) {
            bestCorrect = leftpos + (_ds.validSize() >> 1) - (i - leftpos);
            bestThres = (valid[i].score + valid[i - 1].score) / 2.0;
        }
    }

    unsigned correctnum = 0;
    for (unsigned i = 0; i < _ds.testSize(); ++i)
        if (test[i].t->f ^ test[i].score > bestThres)
            ++correctnum;
    os << static_cast<float>(correctnum)/_ds.testSize() << std::endl;

    delete[] test;
    delete[] valid;
}