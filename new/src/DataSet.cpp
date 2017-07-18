#include "DataSet.h"

#include <fstream>

DataSet::tplset DataSet::readTriples(std::string filename, bool have_flag) {
    tplset result;
    Triple temp;
    std::fin((_NAME + '/' + filename).c_str());
    if (have_flag) {
        short flag;
        while (fin >> temp.h >> temp.r >> temp.t >> flag) {
            if (flag == 1) {
                temp.f = true;
                makeIndex(temp);
            } else {
                temp.f = false;
            }
            result.insert(temp)
        }
    } else {
        temp.f = true;
        while(fin >> temp.h >> temp.r >> temp.t) {
            result.insert(temp);
            makeIndex(temp);
        }
    }
    fin.close();
    return result;
}

DataSet::DataSet(const std::string name, unsigned short testnum) : _NAME(name) {
    std::string str;
    unsigned id;
    std::fin((_NAME + '/entity2id.txt').c_str());
    while (fin >> str >> id)
        _entity2id[str] = id;
    fin.close();
    fin.open((_NAME + '/relation2id.txt').c_str());
    while (fin >> str >> id)
        _relation2id[str] = id;
    fin.close();

    for (id = _relation2id.size() - 1; id >= 0; --id) {
        _index_r[id] = tplset();
        _index_r_h[id] = tplsetmap();
        for (unsigned hid = _entity2id.size() - 1; id >= 0; --id)
            _index_r_h[id][hid] = tplset();
    }

    _trainset = readTriples("train.txt", false);
    _test = readTriples("test.txt", true);
    _validset = readTriple("valid.txt", true);
    for (unsigned short i = 0; i < testnum; ++i)
        _testsets.push_back(readTriples(std::string(1, 'A' + i) + 'test.txt'));
}
