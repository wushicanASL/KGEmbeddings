#include "Train.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <iomanip>

using namespace sysukg;

Train::Train(EmbeddingModel * em, SamplingModel * sm, float rate, float margin, unsigned threads) :
    _em(em), _sm(sm), _rate(rate), _margin(margin), _threads(threads) {}

void Train::launch(unsigned nepoch, unsigned output) {
    if (output == 0)
        output = nepoch;
    std::string ext = getExt();
    float result;
    std::thread ** threads = new std::thread*[_threads];
    for (unsigned i = 0; i < nepoch; ++i) {
        result = 0;
        for (unsigned j = 0; j < _threads; ++j)
            threads[j] = new std::thread(
                [this, &result]() -> void {
                    std::pair<Triple, Triple> t;
                    float temp;
                    for (unsigned k = _sm->size() / _threads + 1; k > 0; --k) {
                        t = _sm->sample();
                        temp = _em->update(t.first, t.second, _rate, _margin);
                        result += temp;
                    }
                }
            );
        for (unsigned j = 0; j < _threads; ++j)
            threads[j]->join();
        std::cout << "epoch " << std::setw(5) << i;
        std::cout << " : " << std::fixed << std::setw(10) << std::setprecision(3) << result << std::endl;
        if ((i + 1) % output == 0) {
            std::stringstream ss;
            ss << ext << "-epoch-" << i;
            _em->output(ss.str());
        }
        for (unsigned j = 0; j < _threads; ++j)
            delete threads[j];
    }
    delete threads;
}
