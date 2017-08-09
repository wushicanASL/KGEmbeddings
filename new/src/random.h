#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <random>
#include <cmath>

#define pi 3.141592653589793238462643383

namespace sysukg {
class random_device {
private:
    typedef std::random_device engine;
    engine _eng;

    inline float normal(float x, float miu, float sigma) const {
        return 1.0/sqrt(2*pi)/sigma*exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
    }

    random_device() {}
    static random_device * _instance;
public:
    inline static random_device & getInstance() {
        if (_instance == nullptr)
            _instance = new random_device;
        return *_instance;
    }
    inline engine::result_type operator()(){
        return _eng();
    }
    inline float operator()(float min, float max) {
        return min + (max - min) * (_eng() - _eng.min()) / static_cast<float>((_eng.max() - _eng.min()));
    }
    inline engine::result_type operator()(unsigned max) {
        return _eng() % max;
    }
    float randn(float miu,float sigma, float min ,float max);
};
}

#endif
