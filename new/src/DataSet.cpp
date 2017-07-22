#include "DataSet.h"

#include <fstream>

DataSet::tplset DataSet::readTriples(std::string filename, bool have_flag) {
    tplset result;
    Triple temp;
    std::ifstream fin((_NAME + '/' + filename).c_str());
    if (fin.is_open()) {
        if (have_flag) {
            short flag;
            while (fin >> temp.h >> temp.r >> temp.t >> flag) {
                if (flag == 1) {
                    temp.f = true;
                    makeIndex(temp);
                } else {
                    temp.f = false;
                }
                result.insert(temp);
            }
        } else {
            temp.f = true;
            while(fin >> temp.h >> temp.r >> temp.t) {
                result.insert(temp);
                makeIndex(temp);
            }
        }
        fin.close();
    }
    return result;
}

DataSet::DataSet(const std::string & name, unsigned short testnum) : _NAME(name) {
    std::string str;
    unsigned id;
    std::ifstream fin((_NAME + "/entity2id.txt").c_str());
    while (fin >> str >> id)
        _entity2id[str] = id;
    fin.close();
    fin.open((_NAME + "/relation2id.txt").c_str());
    while (fin >> str >> id)
        _relation2id[str] = id;
    fin.close();

    _id2relation.resize(_relation2id.size());
    _id2entity.resize(_entity2id.size());
    for (auto & item : _relation2id)
        _id2relation[item.second] = item.first;
    for (auto & item : _entity2id)
        _id2entity[item.second] = item.first;

    for (id = _relation2id.size() - 1; id >= 0; --id) {
        _index_r[id] = tplset();
        _index_r_h[id] = _index_r_t[id] = tplsetmap();
        for (unsigned eid = _entity2id.size() - 1; id >= 0; --id)
            _index_r_h[id][eid] = _index_r_t[id][eid] = tplset();
    }

    _trainset = readTriples("train.txt", false);
    _testset = readTriples("test.txt", true);
    _validset = readTriples("valid.txt", true);
    for (unsigned short i = 0; i < testnum; ++i)
        _testsets.push_back(readTriples(std::string(1, 'A' + i) + "test.txt"));
}
