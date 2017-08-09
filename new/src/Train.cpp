#include "Train.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace sysukg;

Train::Train(EmbeddingModel * em, SamplingModel * sm, float rate, float margin, unsigned threads) :
    _em(em), _sm(sm), _rate(rate), _margin(margin), _threads(threads) {}

void Train::launch(unsigned nepoch, unsigned output) {
    if (output == 0)
        output = nepoch;
    std::string ext = getExt();
    std::atomic<unsigned> active_threads;
    active_threads.store(0);
    for (unsigned i = 0; i < nepoch; ++i) {
        for (unsigned j = 0; j < _threads; ++j)
            std::thread (
                [this, &active_threads]() -> void {
                    ++active_threads;
                    std::pair<Triple, Triple> t;
                    for (unsigned k = _sm->size() / _threads + 1; k > 0; --k) {
                        t = _sm->sample();
                        _em->update(t.first, t.second, _rate);
                    }
                    --active_threads;
                }
            ).detach();
        while (active_threads.load() > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if ((i + 1) % output == 0)
            std::thread (
                [&ext, this, i, &active_threads]() -> void {
                    ++active_threads;
                    std::stringstream ss;
                    ss << ext << "-epoch-" << i;
                    _em->output(ss.str());
                    --active_threads;
                }
            ).detach();
    }
    while (active_threads.load() > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
