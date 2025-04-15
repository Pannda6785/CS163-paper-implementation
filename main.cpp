#include "tests.h"

int main() {   
    
    Tests tests;
    
    tests.visualTest(true, false, true, true, true, true);
    
    int ITE;
    std::cin >> ITE;
    for (int ite = 0; ite < ITE; ite++) {
        tests.strestTestCheckTime(true, true, true, true);
    }
    for (int ite = 0; ite < ITE; ite++) {
        tests.stressTestCheckPath(true, true, true, true);
    }
    std::cout << "stress test all okay\n";

    tests.testOnDataTest(std::filesystem::path("./datasets/ca-cit-HepPh/out.ca-cit-HepPh"), true, true, true, true);

    // tests.testOnDataTest(std::filesystem::path("./datasets/ca-cit-HepPh/out.ca-cit-HepPh"), true, true, true, true);
    // tests.testOnDataTest(std::filesystem::path("./datasets/dblp_coauthor/out.dblp_coauthor"), true, true, true, true); // big
    // // tests.testOnDataTest(std::filesystem::path("./datasets/elec/out.elec"), true, true, true, true); // negative lambdas
    // tests.testOnDataTest(std::filesystem::path("./datasets/enron/out.enron"), true, true, true, true);
    // // tests.testOnDataTest(std::filesystem::path("./datasets/epinions/out.epinions"), true, true, true, true); // negative lambdas
    // tests.testOnDataTest(std::filesystem::path("./datasets/facebook-wosn-links/out.facebook-wosn-links"), true, true, true, true);
    // tests.testOnDataTest(std::filesystem::path("./datasets/flickr-growth/out.flickr-growth"), true, true, true, true); // kinda big
    // tests.testOnDataTest(std::filesystem::path("./datasets/munmun_digg_reply/out.munmun_digg_reply"), true, true, true, true);
    // tests.testOnDataTest(std::filesystem::path("./datasets/slashdot-threads/out.slashdot-threads"), true, true, true, true);
    // // tests.testOnDataTest(std::filesystem::path("./datasets/wikiconflict/out.wikiconflict"), true, true, true, true); // negative and real lambda's
    // tests.testOnDataTest(std::filesystem::path("./datasets/wikipedia-growth/out.wikipedia-growth"), true, true, true, true); // big
    // tests.testOnDataTest(std::filesystem::path("./datasets/youtube-u-growth/out.youtube-u-growth"), true, true, true, true);
}

