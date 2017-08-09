#include "Train.h"
#include "bernSampling.h"
#include "TransE.h"

using namespace sysukg;

int main() {
    DataSet FB15k("FB15k");
    TransE transE_FB15k(FB15k, 50);
    bernSampling bern_FB15k(FB15k);
    Train train(&transE_FB15k, &bern_FB15k);
    train.launch(3000);
    return 0;
}