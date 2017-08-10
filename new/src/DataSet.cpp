#include "DataSet.h"

#include <fstream>

using namespace sysukg;

DataSet::tplset DataSet::readTriples(const std::string & filename, bool have_flag) {
    tplset result;
    Triple temp;
    std::string h, r, t;
    std::ifstream fin((_NAME + "/" + filename).c_str());
    if (fin.is_open()) {
        if (have_flag) {
            short flag;
            while (fin >> h >> r >> t >> flag) {
                temp = Triple(_entity2id[h], _relation2id[r], _entity2id[t]);
                if (flag == 1) {
                    temp.f = true;
                    makeIndex(temp);
                } else {
                    temp.f = false;
                }
                result.insert(temp);
            }
        } else {
            while(fin >> h >> r >> t) {
                temp = Triple(_entity2id[h], _relation2id[r], _entity2id[t], true);
                result.insert(temp);
                makeIndex(temp);
            }
        }
        fin.close();
    }
    return result;
}

DataSet::DataSet(const std::string & name, unsigned short testnum) : _NAME("data/" + name) {
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

    for (id = 0; id < _relation2id.size(); ++id) {
        _index_r[id] = tplset();
    }

    _trainset = readTriples("train.txt", false);
    _testset = readTriples("test.txt", true);
    _validset = readTriples("valid.txt", true);
    for (unsigned short i = 0; i < testnum; ++i)
        _testsets.push_back(readTriples(std::string(1, 'A' + i) + "test.txt"));
}

DataSet::tplset DataSet::allPosTriples() const {
    tplset result;
    for (auto & item : _trainset)
        if (item.f)
            result.insert(item);
    for (auto & item : _testset)
        if (item.f)
            result.insert(item);
    for (auto & item : _validset)
        if (item.f)
            result.insert(item);
    return result;
}
