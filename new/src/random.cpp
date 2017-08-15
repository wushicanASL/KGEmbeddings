#include "random.h"

using namespace sysukg;

random_device * random_device::_instance = nullptr;

float random_device::randn(float miu, float sigma, float min , float max) {
    float x, y, dScope;
    do {
        x = operator()(min, max);
        y = normal(x, miu, sigma);
        dScope = operator()(0.0, normal(miu, miu, sigma));
    } while (dScope > y);
    return x;
}
