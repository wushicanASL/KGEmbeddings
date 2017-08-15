#include "Train.h"
#include "bernSampling.h"
#include "unifSampling.h"
#include "updateSampling.h"
#include "TransE.h"
#include "TransH.h"

#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>

using namespace sysukg;

int ArgPos(char *str, int argc, char **argv) {
    for (int a = 1; a < argc; a++)
        if (!strcmp(str, argv[a])) {
            if (a == argc - 1) {
                printf("Argument missing for %s\n", str);
                exit(1);
            }
            return a;
        }
    return -1;
}

int main(int argc, char ** argv) {
    std::ios::sync_with_stdio(false);

    DataSet * ds;
    EmbeddingModel * em;
    SamplingModel * sm;
    std::string dsname = "WN18", emname = "TransE", smname = "unif";
    unsigned nepoch = 1000, dim1 = 50, dim2 = 50, threads = 4;
    float margin = 1, rate = 0.001;

    int i;
    if ((i = ArgPos((char*)"-embedding", argc, argv)) > 0) emname = argv[i + 1];
    if ((i = ArgPos((char*)"-dataset", argc, argv)) > 0) dsname = argv[i + 1];
    if ((i = ArgPos((char*)"-sampling", argc, argv)) > 0) smname = argv[i + 1];
    if ((i = ArgPos((char*)"-nepoch", argc, argv)) > 0) nepoch = atoi(argv[i + 1]);
    if ((i = ArgPos((char*)"-dim1", argc, argv)) > 0) dim1 = dim2 = atoi(argv[i + 1]);
    if ((i = ArgPos((char*)"-dim2", argc, argv)) > 0) dim2 = atoi(argv[i + 1]);
    if ((i = ArgPos((char*)"-threads", argc, argv)) > 0) threads = atoi(argv[i + 1]);
    if ((i = ArgPos((char*)"-margin", argc, argv)) > 0) margin = atof(argv[i + 1]);
    if ((i = ArgPos((char*)"-rate", argc, argv)) > 0) rate = atof(argv[i + 1]);

    ds = new DataSet(dsname);
    if (emname == "TransE") {
        em = new TransE(*ds, dim1);
    }
    else if (emname == "TransH") {
        em = new TransH(*ds, dim1);
    } else {
        exit(1);
    }
    if (smname == "unif") {
        sm = new unifSampling(*ds);
    } else if (smname == "bern") {
        sm = new bernSampling(*ds);
    } else if (smname == "update") {
        sm = new updateSampling(*ds);
    } else {
        exit(1);
    }
    Train train(em, sm, rate, margin, threads);
    train.launch(nepoch);
    if (dsname == "FB15k" || dsname == "WN18")
        em->runLinkPrediction(std::cout);
    else
        em->runClassificationTest(std::cout);
    return 0;
}