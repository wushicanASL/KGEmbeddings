#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "DataSet.h"

using namespace sysukg;

typedef std::set<std::string> strset;
typedef std::vector<bool> boolvec;

const strset AvailableDataSets = {
    "WN11", "FB13", "WN18", "FB15k", "FB40k"
};

inline void outputSymm(std::ostream & os, const std::string & r) {
    os << "symm\t" << r << std::endl;
}
inline void outputTran(std::ostream & os, const std::string & r) {
    os << "tran\t" << r << std::endl;
}
inline void outputRefl(std::ostream & os, const std::string & r) {
    os << "refl\t" << r << std::endl;
}
inline void outputAsym(std::ostream & os, const std::string & r) {
    os << "asym\t" << r << std::endl;
}
inline void outputIrre(std::ostream & os, const std::string & r) {
    os << "irre\t" << r << std::endl;
}
inline void outputFunc(std::ostream & os, const std::string & r) {
    os << "func\t" << r << std::endl;
}
inline void outputIfun(std::ostream & os, const std::string & r) {
    os << "ifun\t" << r << std::endl;
}
inline void outputInve(std::ostream & os, const std::string & r, const std::string & s) {
    os << "inve\t" << r << '\t' << s << std::endl;
}
inline void outputDisj(std::ostream & os, const std::string & r, const std::string & s) {
    os << "disj\t" << r << '\t' << s << std::endl;
}
inline void outputSubr(std::ostream & os, const std::string & r, const std::string & s) {
    os << "subr\t" << r << '\t' << s << std::endl;
}
inline void outputSubc(std::ostream & os, const std::string & r, const std::string & s, const std::string & t) {
    os << "subc\t" << r << '\t' << s << '\t' << t << std::endl;
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    if (argc != 2) {
        std::cout << "Usage: " << std::string(argv[0]) << " [Name of data set]" << std::endl
                  << "Example: " << std::string(argv[0]) << " FB15k" << std::endl;
    } else {
        const std::string dataset = argv[1];
        if (AvailableDataSets.find(dataset) == AvailableDataSets.end()) {
            std::cout << "Data set not Available!" << std::endl
                      << "Available data sets:";
            for (auto & item : AvailableDataSets)
                std::cout << ' ' << item;
            std::cout << std::endl;
        } else {
            std::ofstream fout(dataset + ".rbox");
            DataSet ds(dataset);
            bool isSymm, isTran, isRefl, isInve, isSubr, isSubc,
                 isAsym, isIrre, isFunc, isIfun, isDisj, flag0, flag1;
            unsigned r = ds.relationNum(), s, t;
            const DataSet::tplset * rset, * reset, * sset, * tset;
            do {
                --r;
                isSymm = isTran = isRefl = isAsym = isIrre = isFunc = isIfun = true;
                rset = &ds.getIndex_r(r);
                for (auto & item : *rset) 
                    if (item.f) {
                        if (isSymm) {
                            if (rset->find(Triple(item.symm())) == rset->end())
                                isSymm = false;
                        }
                        if (isTran) {
                            flag0 = false;
                            flag1 = true;
                            reset = &ds.getIndex_r(r);
                            for (auto & jtem: *reset) {
                                flag0 = true;
                                if (rset->find(Triple(item.h, r, jtem.t, true)) == rset->end())
                                    flag1 = true;
                            }
                            if (flag0 && flag1)
                                isTran = false;
                        }
                        if (isRefl) {
                            if (item.h != item.t)
                                isRefl = false;
                        }
                        if (isAsym) {
                            if (rset->find(Triple(item.symm())) != rset->end())
                                isSymm = false;
                        }
                        if (isIrre) {
                            if (item.h == item.t)
                                isIrre = false;
                        }
                        if (isFunc) {
                            if (ds.getIndex_r(r).size() > 1)
                                isFunc = false;
                        }
                        if (isIfun) {
                            if (ds.getIndex_r(r).size() > 1)
                                isIfun = false;
                        }
                    }
                if (isSymm) outputSymm(fout, ds.getRelationName(r));
                if (isTran) outputTran(fout, ds.getRelationName(r));
                if (isRefl) outputRefl(fout, ds.getRelationName(r));
                if (isAsym) outputAsym(fout, ds.getRelationName(r));
                if (isIrre) outputIrre(fout, ds.getRelationName(r));
                if (isFunc) outputFunc(fout, ds.getRelationName(r));
                if (isIfun) outputIfun(fout, ds.getRelationName(r));
                s = r;
                do {
                    --s;
                    isInve = isDisj = true;
                    sset = &ds.getIndex_r(s);
                    for (auto & item : *rset)
                        if (item.f) {
                            if (isInve) {
                                if (sset->find(Triple(item.t, s, item.h, true)) == sset->end())
                                    isInve = false;
                            }
                            if (isDisj) {
                                if (sset->find(Triple(item.h, s, item.t, true)) != sset->end())
                                    isDisj = false;
                            }
                        }
                    if (isInve) outputInve(fout, ds.getRelationName(r), ds.getRelationName(s));
                    if (isDisj) outputDisj(fout, ds.getRelationName(r), ds.getRelationName(s));
                } while (s > 0);
                s = ds.relationNum();
                do {
                    if (--s != r) {
                        isSubr = true;
                        sset = &ds.getIndex_r(s);
                        for (auto & item : *rset)
                            if (item.f && sset->find(Triple(item.h, s, item.t, true)) == sset->end()) {
                                isSubr = false;
                                break;
                            }
                        if (isSubr) outputSubr(fout, ds.getRelationName(r), ds.getRelationName(s));
                    }
                } while (s > 0);

                s = ds.relationNum();
                do {
                    --s;
                    t = ds.relationNum();
                    do {
                        --t;
                        flag0 = false;
                        flag1 = true;
                        for (auto & item : *rset)
                            if (item.f) {
                                sset = &ds.getIndex_r(s);
                                tset = &ds.getIndex_r(t);
                                for (auto & jtem : *sset)
                                    if (jtem.f) {
                                        flag0 = true;
                                        if (tset->find(Triple(item.h, t, jtem.t)) == tset->end()) {
                                            flag1 = false;
                                            break;
                                        }
                                    }
                                if (!flag1)
                                    break;
                            }
                        isSubc = flag0 && flag1;
                        if (isSubc)
                            outputSubc(fout, ds.getRelationName(r),
                                             ds.getRelationName(s),
                                             ds.getRelationName(t));
                    } while (t > 0);
                } while (s > 0);
            } while (r > 0);
            fout.close();
        }
    }
    return 0;
}
