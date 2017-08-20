#pragma once

#include <utility>
#include <cmath>
#include <ostream>

#include "bernSampling.h"

namespace sysukg {

class updateSampling : public bernSampling {
private:
    struct node {
        unsigned terminal;
        float weight;
        node * next;
        node(unsigned t, float w, node * n) :
            terminal(t), weight(w), next(n) {}
    };

    const unsigned _relNum;
    float * _pool_weight;
    float * _ew, * _rw;
    float * _alias_prob;
    unsigned * _alias; 

    constexpr static const float INF_DISTANCE = -1;

    inline void undirected_insert(node ** graph, unsigned v1, unsigned v2, float weight) {
        node * temp = graph[v1];
        graph[v1] = new node(v2, weight, temp);
        temp = graph[v2];
        graph[v2] = new node(v1, weight, temp);
    }
    inline float elementWeight(const float distance) {
        if (distance == INF_DISTANCE)
            return 0;
        else
            return exp(-distance);
    }
    inline float edge_weight(unsigned count) const {
        return 1.0 / count;
    }
    inline float triple_weight(const Triple & t) const {
        return _ew[t.h] + _rw[t.r] + _ew[t.t];
    }

    static void norm(float * a, unsigned size);
    static void spfa(float * distance, unsigned size, node ** graph, std::set<unsigned> sources);
    static float agv(const float * a, unsigned size);

protected:
    inline Triple getPosSamp() const {
        unsigned id = _rd(_size);
        if (_rd(0, 1) < _alias_prob[id])
            return _pool[id];
        return _pool[_alias[id]];
    }

public:
    explicit updateSampling(const DataSet & ds, bool with_update_set = true);
    void output(std::ostream & os) const;
    ~updateSampling();
};

}