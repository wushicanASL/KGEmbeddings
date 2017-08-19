#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <cstring>
#include <vector>

#include "DataSet.h"
#include "Triple.h"

using namespace sysukg;

typedef std::set<std::string> strset;
typedef std::vector<bool> boolvec;

const strset AvailableDataSets = {
    "WN11", "FB13", "WN18", "FB15k", "FB40k"
};

inline void outputSymm(std::ostream & os, const std::string & r) {
    os << "Symmetric\t" << r << std::endl;
}
inline void outputTran(std::ostream & os, const std::string & r) {
    os << "Transitive\t" << r << std::endl;
}
inline void outputRefl(std::ostream & os, const std::string & r) {
    os << "Reflexive\t" << r << std::endl;
}
inline void outputAsym(std::ostream & os, const std::string & r) {
    os << "Asymmetric\t" << r << std::endl;
}
inline void outputIrre(std::ostream & os, const std::string & r) {
    os << "Irreflexive\t" << r << std::endl;
}
inline void outputFunc(std::ostream & os, const std::string & r) {
    os << "Functional\t" << r << std::endl;
}
inline void outputIfun(std::ostream & os, const std::string & r) {
    os << "Inverse functional\t" << r << std::endl;
}
inline void outputInve(std::ostream & os, const std::string & r, const std::string & s) {
    os << "Inverse\t" << r << '\t' << s << std::endl;
}
inline void outputDisj(std::ostream & os, const std::string & r, const std::string & s) {
    os << "Disjoint\t" << r << '\t' << s << std::endl;
}
inline void outputSubr(std::ostream & os, const std::string & r, const std::string & s) {
    os << "Subproperty\t" << r << '\t' << s << std::endl;
}
inline void outputSubc(std::ostream & os, const std::string & r, const std::string & s, const std::string & t) {
    os << "Subproperty of Chain\t" << r << '\t' << s << '\t' << t << std::endl;
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
            const Triple * rset, * sset, * tset;
            unsigned short * th = new unsigned short[ds.entityNum()],
                           * ht = new unsigned short[ds.entityNum()];
            do {
                std::cout << --r << std::endl;
                memset(ht, 0, ds.entityNum() * sizeof(unsigned short));
                memset(th, 0, ds.entityNum() * sizeof(unsigned short));
                isSymm = isTran = isRefl = isAsym = isIrre = isFunc = isIfun = true;
                rset = ds.getIndex_r(r);
                for (unsigned i = 0; i < ds.rcount(r); ++i) {
                    if (isSymm) {
                        if (look_for(rset, rset + ds.rcount(r), rset[i].symm(), Triple_rht_less) == nullptr)
                            isSymm = false;
                    }
                    if (isTran) {
                        flag0 = false;
                        flag1 = true;
                        for (unsigned j = 0; j < ds.rcount(r); ++j) {
                            flag0 = true;
                            if (look_for(rset, rset + ds.rcount(r), Triple(rset[i].h, r, rset[j].t, true), Triple_rht_less) == nullptr)
                                flag1 = true;
                        }
                        if (flag0 && flag1)
                            isTran = false;
                    }
                    if (isRefl) {
                        if (rset[i].h != rset[i].t)
                            isRefl = false;
                    }
                    if (isAsym) {
                        if (look_for(rset, rset + ds.rcount(r), rset[i].symm(), Triple_rht_less) != nullptr)
                            isSymm = false;
                    }
                    if (isIrre) {
                        if (rset[i].h == rset[i].t)
                            isIrre = false;
                    }
                    if (isFunc) {
                        if (++ht[rset[i].h] > 1)
                            isFunc = false;
                    }
                    if (isIfun) {
                        if (++th[rset[i].t] > 1)
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
                for (s = 0; s < r; ++s) {
                    isInve = isDisj = true;
                    sset = ds.getIndex_r(s);
                    for (unsigned i = 0; i < ds.rcount(r); ++i) {
                        if (isInve) {
                            if (look_for(sset, sset + ds.rcount(s), Triple(rset[i].t, s, rset[i].h, true), Triple_rht_less) == nullptr)
                                isInve = false;
                        }
                        if (isDisj) {
                            if (look_for(sset, sset + ds.rcount(s), Triple(rset[i].h, s, rset[i].t, true), Triple_rht_less) != nullptr)
                                isDisj = false;
                        }
                    }
                    if (isInve)
                        for (unsigned i = 0; i < ds.rcount(s); ++i)
                            if (look_for(rset, rset + ds.rcount(r), Triple(sset[i].t, r, sset[i].h, true), Triple_rht_less) == nullptr) {
                                isInve = false;
                                break;
                            }
                    if (isInve) outputInve(fout, ds.getRelationName(r), ds.getRelationName(s));
                    if (isDisj) outputDisj(fout, ds.getRelationName(r), ds.getRelationName(s));
                }
                s = ds.relationNum();
                do {
                    if (--s != r) {
                        isSubr = true;
                        sset = ds.getIndex_r(s);
                        for (unsigned i = 0; i < ds.rcount(r); ++i)
                            if (look_for(sset, sset + ds.rcount(s), Triple(rset[i].h, s, rset[i].t, true), Triple_rht_less) == nullptr) {
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
                        for (unsigned i = 0; i < ds.rcount(r); ++i) {
                            sset = ds.getIndex_r(s);
                            tset = ds.getIndex_r(t);
                            for (unsigned j = 0; j < ds.rcount(s); ++j) {
                                flag0 = true;
                                if (look_for(tset, tset + ds.rcount(t), Triple(rset[i].h, t, sset[j].t), Triple_rht_less) == nullptr) {
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
