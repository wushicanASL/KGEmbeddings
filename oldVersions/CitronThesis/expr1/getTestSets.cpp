#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <random>

struct triple {
    int h, r, t;
    triple() {}
    triple(int _h, int _r, int _t) : h(_h), r(_r), t(_t) {}
    bool operator<(const triple & other) const {
        if (h < other.h) return true;
        if (h > other.h) return false;
        if (r < other.r) return true;
        if (r > other.r) return false;
        if (t < other.t) return true;
        return false;
    }
};

int main() {
    int relnum;
    std::ifstream fin("relation2id.txt");
    fin >> relnum;
    fin.close();

    triple temp;
    std::vector< std::set<triple> > S(relnum, std::set<triple>()), T(relnum, std::set<triple>());
    std::set<triple> Aset, Bset, Cset;

    fin.open("./alltriple2id.txt");
    fin >> temp.h;
    while (fin >> temp.h >> temp.t >> temp.r)
        S[temp.r].insert(temp);
    fin.close();

    fin.open("../FB15K/test2id.txt");
    while (fin >> temp.h >> temp.t >> temp.r) {
        Aset.insert(temp);
        T[temp.r].insert(temp);
    }
    fin.close();

    bool isRefl, isSymm;
    for (int i = 0; i < relnum; ++i) {
        isRefl = isSymm = true;
        for (auto & item : S[i]) {
            if (item.h != item.t)
                isRefl = false;
            if (S[i].find(triple(item.t, item.r, item.h)) == S[i].end())
                isSymm = false;
            if (!isRefl && !isSymm)
                break;
        }
        if (isRefl || isSymm)
            for (auto & item : T[i]) {
                Bset.insert(item);
                Cset.insert(item);
            }
    }
    bool isSubr;
    for (int i = 0; i < relnum; ++i)
        for (int j = 0; j < relnum; ++j)
            if (j != i) {
                isSubr = true;
                for (auto & item : T[i])
                    if (S[j].find(triple(item.h, j, item.t)) == S[j].end()) {
                        isSubr = false;
                        break;
                    }
                if (isSubr) {
                    for (auto & item : S[i])
                        Bset.insert(item);
                    break;
                }
            }

    std::cout << Aset.size()  << '\t' << Bset.size() << '\t' << Cset.size() << std::endl;
    std::ofstream fout("Atest.txt");
    for (auto & item : Aset)
        fout << item.h << '\t' << item.r << '\t' << item.t << std::endl;
    fout.close();
    fout.open("Btest.txt");
    for (auto & item : Bset)
        fout << item.h << '\t' << item.r << '\t' << item.t << std::endl;
    fout.close();
    fout.open("Ctest.txt");
    for (auto & item : Cset)
        fout << item.h << '\t' << item.r << '\t' << item.t << std::endl;
    fout.close();

    return 0;
}

