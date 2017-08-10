#ifndef __EMBEDDINGMODEL_H__
#define __EMBEDDINGMODEL_H__

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
        unsigned _dim;
        typedef std::vector<float> fltvec;
        typedef std::vector< std::vector<float> > fltmat;
        typedef std::pair<fltmat, fltmat> EmbeddedData; // first = relations, second = entities
        EmbeddedData _ed;

        inline static float sqr(float x) {
            return x * x;
        }
        inline static void norm(fltvec & v) {
            float x = 0;
            for (auto & item : v)
                x += sqr(item);
            x = sqrt(x);
            if (x>1)
                for (auto & item : v)
                    item /= x;
        }

        inline static EmbeddedData emptyED() {
            return EmbeddedData(fltmat(), fltmat());
        }
        static EmbeddedData init(unsigned dim, unsigned relNum, unsigned entNum);
    
        inline const fltvec & vh(const Triple & t) const {
            return _ed.second[t.h];
        }
        inline const fltvec & vr(const Triple & t) const {
            return _ed.first[t.r];
        }
        inline const fltvec & vt(const Triple & t) const {
            return _ed.second[t.t];
        }
        inline fltvec & vh(const Triple & t) {
            return _ed.second[t.h];
        }
        inline fltvec & vr(const Triple & t) {
            return _ed.first[t.r];
        }
        inline fltvec & vt(const Triple & t) {
            return _ed.second[t.t];
        }


    public:
        EmbeddingModel(const DataSet & ds, unsigned dim,
                    const EmbeddedData & ed = emptyED());

        inline unsigned dim() const {
            return _dim;
        }
        virtual std::string methodName() const = 0;

        virtual float calc_sum(const Triple & t) const = 0;
        virtual float update(const Triple & pos, const Triple & neg, float rate, float margin) = 0;
        virtual void output(const std::string & ext) const;
    };
}

#endif
