#ifndef __DATASET_H__
#define __DATASET_H__

#include <string>
#include <map>
#include <utility>
#include <set>
#include <list>
#include <vector>

#include "Triple.h"

namespace sysukg {
    inline bool operator<(const std::pair<unsigned, unsigned> & left,
                        const std::pair<unsigned, unsigned> & right) {
        if (left.first < right.first) return true;
        if (left.first > right.first) return false;
        return left.second < right.second;
    }

    class DataSet {
    public:
        typedef std::set<Triple> tplset;
        typedef std::map<unsigned, tplset> tplsetmap;
        typedef std::map<unsigned, tplset> index_r;
        typedef std::map<std::string, unsigned> dictionary;
        typedef std::list<tplset> setlst;

    private:
        const std::string _NAME;

        dictionary _entity2id, _relation2id;
        std::vector<std::string> _id2entity, _id2relation;
        tplset _trainset, _testset, _validset, _updateset;
        setlst _testsets;

        index_r _index_r;

    protected:
        tplset readTriples(const std::string & filename, bool have_flag = true);
        inline void makeIndex(const Triple & t) {
            _index_r[t.r].insert(t);
        }

    public:
        explicit DataSet(const std::string & name, unsigned short testnum = 0);

        inline const tplset & getIndex_r(unsigned r) const {
            return _index_r.at(r);
        }

        inline unsigned entityNum() const {
            return _entity2id.size();
        }
        inline unsigned relationNum() const {
            return _relation2id.size();
        }

        inline const std::string & getEntityName(unsigned id) const {
            return _id2entity[id];
        }
        inline const std::string & getRelationName(unsigned id) const {
            return _id2relation[id];
        }

        tplset allPosTriples() const;
        inline const tplset & trainset() const {
            return _trainset;
        }
        inline const tplset & updateset() const {
            return _updateset;
        }
    };
}

#endif
