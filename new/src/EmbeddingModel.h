#pragma once

#include <cmath>
#include <cstdlib>
#include <string>

#include "DataSet.h"
#include "Triple.h"
#include "random.h"

namespace sysukg {
    class EmbeddingModel {
    protected:
        DataSet _ds;
        static random_device & _rd;
        unsigned _dim, _relSize, _entSize;
        typedef std::pair<float **, float **> EmbeddedData; // first = relations, second = entities
        EmbeddedData * _ed;

        inline static float sqr(float x) {
            return x * x;
        }
        inline void norm(float * v) {
            float x = 0;
            for (unsigned i = 0; i < _dim; ++i)
                x += sqr(v[i]);
            x = sqrt(x);
            if (x>1)
                for (unsigned i = 0; i < _dim; ++i)
                    v[i] /= x;
        }
    
        inline const float * vh(const Triple & t) const {
            return _ed->second[t.h];
        }
        inline const float * vr(const Triple & t) const {
            return _ed->first[t.r];
        }
        inline const float * vt(const Triple & t) const {
            return _ed->second[t.t];
        }
        inline float * vh(const Triple & t) {
            return _ed->second[t.h];
        }
        inline float * vr(const Triple & t) {
            return _ed->first[t.r];
        }
        inline float * vt(const Triple & t) {
            return _ed->second[t.t];
        }

        static void single_output(const std::string & filename,
                        unsigned num, unsigned dim, float ** mat);

    public:
        EmbeddingModel(const DataSet & ds, unsigned dim,
                    const EmbeddedData * ed = nullptr);

        void EDreset();
        void EDcopy(const EmbeddedData * ed);

        inline unsigned dim() const {
            return _dim;
        }
        virtual std::string methodName() const = 0;

        virtual float calc_sum(const Triple & t) const = 0;
        virtual float update(const std::pair<Triple, Triple> * samples, unsigned size,
                             float rate, float margin) = 0;
        virtual void output(const std::string & ext) const;
    };
}
