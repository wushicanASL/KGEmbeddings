#include <algorithm>
#include <cstring>
#include <set>
#include <queue>

#include "updateSampling.h"

using namespace sysukg;

void updateSampling::spfa(float * distance, unsigned size, node ** graph,
        std::set<unsigned> sources) {
    bool * inque = new bool[size];
    for (unsigned i = 0; i < size; ++i) {
        distance[i] = INF_DISTANCE;
        inque[i] = false;
    }
    std::queue<unsigned> que;
    for (auto & item : sources) {
        que.push(item);
        distance[item] = 0;
        inque[item] = true;
    }
    while (!que.empty()) {
        for (node * it = graph[que.front()]; it != nullptr; it = it->next)
            if (distance[it->terminal] == INF_DISTANCE ||
                distance[it->terminal] > distance[que.front()] + it->weight) {
                distance[it->terminal] = distance[que.front()] + it->weight;
                if (!inque[it->terminal])
                    que.push(it->terminal);
            }
        inque[que.front()] = false;
        que.pop();
    }
    delete[] inque;
}

updateSampling::updateSampling(const DataSet & ds, bool with_update_set) :
        bernSampling(ds, with_update_set), _relNum(ds.relationNum()) {
    unsigned ** eGraph = new unsigned*[_entNum],
             ** rGraph = new unsigned*[_relNum];
    for (unsigned i = 0; i < _entNum; ++i) {
        eGraph[i] = new unsigned[_entNum];
        memset(eGraph[i], 0, _entNum * sizeof(unsigned));
    }
    for (unsigned i = 0; i < _relNum; ++i) {
        rGraph[i] = new unsigned[_relNum];
        memset(rGraph[i], 0, _relNum * sizeof(unsigned));
    }

    std::set<unsigned> * hr = new std::set<unsigned>[_entNum],
                       * tr = new std::set<unsigned>[_entNum];
    for (unsigned i = 0; i < _size; ++i) {
        ++eGraph[_pool[i].h][_pool[i].t];
        ++eGraph[_pool[i].t][_pool[i].h];
        hr[_pool[i].h].insert(_pool[i].r);
        tr[_pool[i].t].insert(_pool[i].r);
    }
    for (unsigned i = 0; i < _entNum; ++i)
        for (auto & rel1 : hr[i])
            for (auto & rel2 : tr[i]) {
                ++rGraph[rel1][rel2];
                ++rGraph[rel2][rel1];
            }
    delete[] hr;
    delete[] tr;
    
    node ** feGraph = new node*[_entNum],
          ** frGraph = new node*[_relNum];
    memset(feGraph, static_cast<int>(NULL), _entNum * sizeof(node*));
    memset(frGraph, static_cast<int>(NULL), _relNum * sizeof(node*));
    for (unsigned i = 0; i < _entNum; ++i) {
        for (unsigned j = i + 1; j < _entNum; ++j)
            if (eGraph[i][j] && i != j)
                undirected_insert(feGraph, i, j, edge_weight(eGraph[i][j]));
        delete[] eGraph[i];
    }
    delete[] eGraph;
    for (unsigned i = 0; i < _relNum; ++i) {
        for (unsigned j = i + 1; j < _relNum; ++j)
            if (rGraph[i][j] && i != j)
                undirected_insert(frGraph, i, j, edge_weight(rGraph[i][j]));
        delete[] rGraph[i];
    }
    delete[] rGraph;

    _pool_weight = new float[_size];
    if (with_update_set) {
        const Triple * _updateset = ds.updateset();
        const unsigned _updatesize = ds.updateSize();
        std::set<unsigned> ueset, urset;
        for (unsigned i = 0; i < _updatesize; ++i) {
            ueset.insert(_updateset[i].h);
            urset.insert(_updateset[i].r);
            ueset.insert(_updateset[i].t);
        }
        _ew = new float[_entNum],
        _rw = new float[_relNum];
        spfa(_ew, _entNum, feGraph, ueset);
        spfa(_rw, _relNum, frGraph, urset);
        for (unsigned i = 0; i < _entNum; ++i)
            _ew[i] = elementWeight(_ew[i]);
        for (unsigned i = 0; i < _relNum; ++i)
            _rw[i] = elementWeight(_rw[i]);
        float eavg = avg(_ew, _entNum),
              ravg = avg(_rw, _relNum);
        for (unsigned i = 0; i < _relNum; ++i)
            _rw[i] *= (eavg / ravg);

        for (unsigned i = 0; i < _entNum; ++i)
            delete[] feGraph[i];
        delete[] feGraph;
        for (unsigned i = 0; i < _relNum; ++i)
            delete[] frGraph[i];
        delete[] frGraph;

        for (unsigned i = 0; i < _size; ++i) {
            _pool_weight[i] = triple_weight(_pool[i].t);
        }
    } else {
        memset(_pool_weight, static_cast<float>(1), _size * sizeof(float));
    }
    norm(_pool_weight, _size);

    // init alias method
    struct alias_node {
        unsigned id;
        float prob;
        alias_node(unsigned a, float b) : id(a), prob(b) {}
    };
    std::queue<alias_node> more, less;
    float temp;
    for (unsigned i = 0; i < _size; ++i) {
        temp = _pool_weight[i] * _size;
        if (temp < 1)
            less.push(alias_node(i, temp));
        else
            more.push(alias_node(i, temp));
    }
    _alias = new unsigned[_size];
    _alias_prob = new float[_size];
    for (unsigned i = 1; i < _size; ++i) {
        _alias[less.front().id] = more.front().id;
        _alias_prob[less.front().id] = less.front().prob;
        more.front().prob -= less.front().prob;
        less.pop();
        if (more.front().prob < 1) {
            less.push(more.front());
            more.pop();
        }
    }
    if (less.empty()) {
        _alias[more.front().id] = more.front().id;
        _alias_prob[more.front().id] = 1;
    } else {
        _alias[less.front().id] = less.front().id;
        _alias_prob[less.front().id] = 1;
    }
}

void updateSampling::norm(float * a, unsigned size) {
    float sum = 0;
    for (unsigned i = 0; i < size; ++i)
        sum += a[i];
    for (unsigned i = 0; i < size; ++i)
        a[i] /= sum;
}

void updateSampling::output(std::ostream & os) const {
    os << _ew[0];
    for (unsigned i = 1; i < _entNum; ++i)
        os << ' ' << _ew[i];
    os << std::endl << _rw[0];
    for (unsigned i = 1; i < _relNum; ++i)
        os << ' ' << _rw[i];
    os << std::endl;
    for (unsigned i = 0; i < _size; ++i)
        os << _pool[i].h << ' ' << _pool[i].r << ' ' << _pool[i].t
           << ' ' << _pool_weight[i] << std::endl;
}

float updateSampling::avg(const float * a, unsigned size) {
    float sum = 0;
    for (unsigned i = 0; i < sum; ++i)
        sum += a[i];
    return sum / size;
}

updateSampling::~updateSampling() {
    delete[] _ew;
    delete[] _rw;
    delete[] _pool_weight;
    delete[] _alias;
    delete[] _alias_prob;
}