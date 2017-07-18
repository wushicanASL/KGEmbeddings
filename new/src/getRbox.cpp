#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <unordered_map>

#include "DataSet.h"

typedef std::set<std::string> strset;

const strset AvailableDataSets = {
    "WN11", "FB13", "WN18", "FB15k", "FB40k"
};

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    if (argc != 2) {
        std::cout << "Usage: " << std::static_cast<std::string>(argv[0]) << " [Name of data set]" << std::endl
                  << "Example: " << std::static_cast<std::string>(argv[0]) << " FB15k" << std::endl;
    } else {
        const std::string dataset = argv[1];
        if (AvailableDataSets.find(dataset) == AvailableDataSets.end()) {
            std::cout << "Data set not Available!" << std::endl
                      << "Available data sets:";
            for (auto const & item : AvailableDataSets)
                std::cout << ' ' << item;
            std::cout << std::endl;
        } else {
            DataSet ds(dataset);
            for ()
        }
    }
    return 0;
}
