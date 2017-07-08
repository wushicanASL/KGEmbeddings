#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <string>
#include <random>
#include <algorithm>
#include <cstring>

const int D = 50;

std::vector< std::vector<double> > e2vec, r2vec, rtvec;

struct triple{
    int h, r, t;
    triple() {}
    triple(int _h, int _r, int _t) {
        h = _h;
        r = _r;
        t = _t;
    }
    bool operator<(const triple & other) const {
        if (h < other.h)
            return true;
        if (h > other.h)
            return false;
        if (r < other.r)
            return true;
        if (r > other.r)
            return false;
        if (t < other.t)
            return true;
        return false;
    }
    bool operator!=(const triple & other) const {
        return h != other.h || r != other.r || t != other.t;
    }
};
struct item {
    triple x;
    double y;
    bool f;
    item() {}
    item(triple t, bool _f) {
        x = t;
        f = _f;
        y = 0;
        double tmph = 0, tmpt = 0;
        for (int i = 0; i < D; ++i) {
            tmph += rtvec[t.r][i] * e2vec[t.h][i];
            tmpt += rtvec[t.r][i] * e2vec[t.t][i];
        }
        for (int i = 0; i < D; ++i)
            y += fabs(  (e2vec[t.h][i] - rtvec[t.r][i] * tmph)
                      + r2vec[t.r][i]
                      - (e2vec[t.t][i] - rtvec[t.r][i] * tmpt));
    }
};

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    bool flag;
    if (strcmp(argv[2], "rel") == 0)
        flag = false;
    else
        flag = true;

    int entnum, relnum;

    std::ifstream fin("../entity2id.txt");
    fin >> entnum;
    fin.close();
    fin.open("../relation2id.txt");
    fin >> relnum;
    fin.close();
    e2vec.resize(entnum);
    r2vec.resize(relnum);
    rtvec.resize(relnum);

    std::set<triple> S;
    fin.open("../triple2id.txt");
    int tnum;
    triple temp;
    for (fin >> tnum; tnum > 0; --tnum) {
        fin >> temp.h >> temp.t >> temp.r;
        S.insert(temp);
    }
    fin.close();

    fin.open("./entity2vec.vec");
    for (int i = 0; i < entnum; ++i) {
        e2vec[i].resize(D);
        for (int j = 0; j < D; ++j)
            fin >> e2vec[i][j];
    }
    fin.close();

    fin.open("./relation2vec.vec");
    for (int i = 0; i < relnum; ++i) {
        r2vec[i].resize(D);
        for (int j = 0; j < D; ++j)
            fin >> r2vec[i][j];
    }
    fin.close();

    fin.open("./A.vec");
    for (int i = 0; i < relnum; ++i) {
        rtvec[i].resize(D);
        for (int j = 0; j < D; ++j)
            fin >> rtvec[i][j];
    }
    fin.close();

    int hitnum = 0, missnum = 0, h3 = 0, h1 = 0, rank;
    double sum = 0;
    std::random_device rd;
    std::vector<item> a;
    fin.open((std::string("../") + std::string(argv[1])).c_str());
    while (fin >> temp.h >> temp.r >> temp.t) {
        a.push_back(item(temp, true));
        if (flag) {
            if (rd() % 2)
                for (int e = 0; e < entnum; ++e)
                    if (S.find(triple(e, temp.r, temp.t)) == S.end())
                        a.push_back(item(triple(e, temp.r, temp.t), false));
                    else
                        continue;
            else
                for (int e = 0; e < entnum; ++e)
                    if (S.find(triple(temp.h, temp.r, e)) == S.end())
                        a.push_back(item(triple(temp.h, temp.r, e), false));
        } else {
            for (int r = 0; r < relnum; ++r)
                if (S.find(triple(temp.h, r, temp.t)) == S.end())
                    a.push_back(item(triple(temp.h, r, temp.t), false));
        }
        std::sort(a.begin(), a.end(), [](const item & l, const item r)->bool {
            return l.y < r.y;
        });
        for (rank = 0; a[rank].x != temp; ++rank)
            continue;
        ++rank;
        sum += rank;
        if (rank <= 10) {
            ++hitnum;
            if (rank <= 3) {
                ++h3;
                if (rank <= 1)
                    ++h1;
            }
        } else {
            ++missnum;
        }
        a.clear();
    }

    std::cout << sum/(hitnum+missnum) << '\t' << hitnum/static_cast<double>(hitnum+missnum) << '\t'
              << h3/static_cast<double>(hitnum+missnum) << '\t' << h1/static_cast<double>(hitnum+missnum) << std::endl;
    return 0;
}

