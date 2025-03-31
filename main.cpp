#include "tests.h"

int main() {    
    // testOnDataTest(std::filesystem::path("./datasets/ca-cit-HepPh/out.ca-cit-HepPh"));
    // visualTestOnRandomGraph(false, true, true, true, false);
    int ITE;
    std::cin >> ITE;
    for (int ite = 0; ite < ITE; ite++) {
        stressTestCheckPath(true, true, false, false);
    }
    std::cout << "all okay\n";
}
