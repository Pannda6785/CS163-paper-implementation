#include "tests.h"

int main() {    
    // visualTest(false, true, true, true, false);
    
    int ITE;
    std::cin >> ITE;
    for (int ite = 0; ite < ITE; ite++) {
        stressTestCheckPath(true, true, true, false);
    }
    for (int ite = 0; ite < ITE; ite++) {
        strestTestCheckTime(true, true, true, false);
    }
    std::cout << "all okay\n";

    // testOnDataTest(std::filesystem::path("./datasets/wikipedia-growth/out.wikipedia-growth"), false, false, true, false, 10, false, false); // big

    // testOnDataTest(std::filesystem::path("./datasets/ca-cit-HepPh/out.ca-cit-HepPh"), true, true, true, false);
    // testOnDataTest(std::filesystem::path("./datasets/dblp_coauthor/out.dblp_coauthor"), true, true, true, false); // big
    // // testOnDataTest(std::filesystem::path("./datasets/elec/out.elec"), true, true, true, false); // negative lambdas
    // testOnDataTest(std::filesystem::path("./datasets/enron/out.enron"), true, true, true, false);
    // // testOnDataTest(std::filesystem::path("./datasets/epinions/out.epinions"), true, true, true, false); // negative lambdas
    // testOnDataTest(std::filesystem::path("./datasets/facebook-wosn-links/out.facebook-wosn-links"), true, true, true, false);
    // testOnDataTest(std::filesystem::path("./datasets/flickr-growth/out.flickr-growth"), true, true, true, false); // kinda big
    // testOnDataTest(std::filesystem::path("./datasets/munmun_digg_reply/out.munmun_digg_reply"), true, true, true, false);
    // testOnDataTest(std::filesystem::path("./datasets/slashdot-threads/out.slashdot-threads"), true, true, true, false);
    // // testOnDataTest(std::filesystem::path("./datasets/wikiconflict/out.wikiconflict"), true, true, true, false); // negative and real lambda's
    // testOnDataTest(std::filesystem::path("./datasets/wikipedia-growth/out.wikipedia-growth"), true, true, true, false); // big
    // testOnDataTest(std::filesystem::path("./datasets/youtube-u-growth/out.youtube-u-growth"), true, true, true, false);
}
