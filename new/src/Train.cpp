#include "Train.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>

using namespace sysukg;

Train::Train(EmbeddingModel * em, SamplingModel * sm, float rate, float margin, unsigned threads) :
    _em(em), _sm(sm), _rate(rate), _margin(margin), _threads(threads) {}

void Train::launch(unsigned nepoch, const std::string & runtest, unsigned output) {
    if (output == 0)
        output = nepoch;
    std::string ext = getExt();
    std::ofstream testFout;
    if (!runtest.empty())
        testFout.open((runtest + "." + ext).c_str());
    float result;
    std::thread ** threads = new std::thread*[_threads];
    for (unsigned i = 0; i < nepoch; ++i) {
        result = 0;
        _em->cache_store();
        for (unsigned j = 0; j < _threads; ++j)
            threads[j] = new std::thread(
                [this, &result]() -> void {
                    float temp;
                    const unsigned len = _sm->size() / _threads;
                    std::pair<Triple, Triple> * t = new std::pair<Triple, Triple>[len];
                    _sm->sample(t, len);
                    temp = _em->update(t, len, _rate, _margin);
                    result += temp;
                    delete []t;
                }
            );
        for (unsigned j = 0; j < _threads; ++j)
            threads[j]->join();
        _em->cache_load();
        std::cout << "epoch " << std::setw(5) << i;
        std::cout << " : " << std::fixed << std::setw(10) << std::setprecision(3) << result << std::endl;
        if ((i + 1) % output == 0) {
            std::stringstream ss;
            ss << ext << "-epoch-" << i + 1;
            _em->output(ss.str());
        }
        for (unsigned j = 0; j < _threads; ++j)
            delete threads[j];
        if (runtest == "classification") {
            testFout << i + 1 << ' ';
            _em->runClassificationTest(testFout);
        } else if (runtest == "linkprediction") {
            testFout << i + 1 << ' ';
            _em->runLinkPrediction(testFout, _threads);
        }
    }
    if (!runtest.empty())
        testFout.close();
    delete[] threads;
}
