#ifndef __TEST_H__
#define __TEST_H__

#include <ostream>
#include <fstream>
#include <string>

#include "EmbeddingModel.h"

class Test {
private:
    const EmbeddingModel * _em;
public:
    explicit Test(const EmbeddingModel * em) : _em(em) {}
    void doLinkPredictionTest(std::ostream & os = std::cout) const;
    inline void doLinkPredictionTest(const std::string & filename) const {
        std::ofstream fout(filename);
        doLinkPredictionTest(fout);
        fout.close();
    }
};

#endif
