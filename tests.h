#include "graphUtil.h"
#include "timer.h"
#include "naive.h"
#include "foremost.h"
#include "revforemost.h"
#include "fastest.h"
#include "shortest.h"

#include <iostream>
#include <fstream>
#include <iomanip>      
#include <filesystem>
#include <string>
#include <vector>
#include <random>

class Tests {
public:
    void visualTest(bool use_manual, bool also_get_path, bool foremost, bool revforemost, bool fastest, bool shortest);

    void stressTestCheckPath(bool foremost, bool revforemost, bool fastest, bool shortest);

    void strestTestCheckTime(bool foremost, bool revforemost, bool fasteste, bool shortest);

    int number_of_x = 1;
    bool only_use_first_x = true;
    bool output_to_file = false;
    void testOnDataTest(std::filesystem::path filepath, bool foremost, bool revforemost, bool fastest, bool shortest);
};

// =====================================================================================================================================================================
// IMPLEMENTATION OF TESTS =============================================================================================================================================
// =====================================================================================================================================================================

void Tests::visualTest(bool use_manual, bool also_get_path = false, bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // for empirical/human validation
    int n;
    std::vector<Edge> edges;
    int ta, tw, x;
    std::vector<int> targets; // for when also_get_path is true
    if (!use_manual) {
        tie(n, edges) = randomGraph();
        x = 1;
        ta = 10, tw = 90;
        std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
        for (auto [u, v, t, lambda] : edges) {
            std::cout << u << ' ' << v << ' ' << t << ' ' << lambda << '\n';
        }
        std::cout << "x and time interval: " << x << ' ' << ta << ' ' << tw << '\n';
        targets = {n};
    } else {
        int m;\
        std::cout << "input n and m: ";
        std::cin >> n >> m;
        std::cout << "input x, ta and tw: ";
        std::cin >> x >> ta >> tw;
        for (int i = 0; i < m; i++) {
            int u, v, t, lambda;
            std::cout << "input the edge (u, v, t, lambda): "; 
            std::cin >> u >> v >> t >> lambda;
            edges.push_back({u, v, t, lambda});
        }
        if (also_get_path) {
            int target_cnt;
            std::cout << "input number of path queries: ";
            std::cin >> target_cnt;
            std::cout << "input " << target_cnt << " nodes: ";
            for (int i = 0; i < target_cnt; i++) {
                int u;
                std::cin >> u;
                targets.push_back(u);
            }
        }
    }
    streamPresentation_ize(n, edges);
    
    if (foremost) {
        Foremost fm(n, edges, x, ta, tw);
        std::vector<int> time = fm.getAllForemostTime();
        std::cout << "foremost times: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
        if (also_get_path) {
            for (int u : targets) {
                int time; 
                std::vector<Edge> path;
                tie(time, path) = fm.getForemostPath(u);
                std::cout << "path to " << u << " is:\n";
                if (abs(time) == INF) std::cout << "no path\n";
                for (const Edge &e : path) {
                    std::cout << "(" << e.u << ", " << e.v << ", " << e.t << ", " << e.lambda << ")\n";
                }
            }
        }
    }

    if (revforemost) {
        ReverseForemost rfm(n, edges, x, ta, tw);
        std::vector<int> time = rfm.getAllReverseForemostTime(); 
        std::cout << "revforemost times: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
        if (also_get_path) {
            for (int u : targets) {
                int time; 
                std::vector<Edge> path;
                tie(time, path) = rfm.getReverseForemostPath(u);
                std::cout << "path from " << u << " is:\n";
                if (abs(time) == INF) std::cout << "no path\n";
                for (const Edge &e : path) {
                    std::cout << "(" << e.u << ", " << e.v << ", " << e.t << ", " << e.lambda << ")\n";
                }
            }
        }
    }
    
    if (fastest) { 
        Fastest ft(n, edges, x, ta, tw);
        std::vector<int> time = ft.getAllFastestTime(); 
        std::cout << "fastest times: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
        if (also_get_path) {
            for (int u : targets) {
                int time; 
                std::vector<Edge> path;
                tie(time, path) = ft.getFastestPath(u);
                std::cout << "path to " << u << " is:\n";
                if (abs(time) == INF) std::cout << "no path\n";
                for (const Edge &e : path) {
                    std::cout << "(" << e.u << ", " << e.v << ", " << e.t << ", " << e.lambda << ")\n";
                }
            }
        }
    }

    if (shortest) {
        Shortest st(n, edges, x, ta, tw);
        std::vector<int> time = st.getAllShortestTime(); 
        std::cout << "shortest times: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
        if (also_get_path) {
            for (int u : targets) {
                int time; 
                std::vector<Edge> path;
                tie(time, path) = st.getShortestPath(u);
                std::cout << "path to " << u << " is:\n";
                if (abs(time) == INF) std::cout << "no path\n";
                for (const Edge &e : path) {
                    std::cout << "(" << e.u << ", " << e.v << ", " << e.t << ", " << e.lambda << ")\n";
                }
            }
        }
    }
}

void Tests::stressTestCheckPath(bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // check if paths are valid and have the claimed time
    const int LOWER_N = 1, UPPER_N = 15, LOWER_M = 0, UPPER_M = 40, T = 100, L = 10;
    auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, T, L);
    streamPresentation_ize(n, edges);
    int x = 1;
    int ta = T / 4, tw = T - ta;

    auto checkPath = [&](std::string problem, int source, int target, const std::pair<int, std::vector<Edge>> timepath) -> void {
        if (abs(timepath.first) == INF) return; // no path reported;

        bool pathStructureCorrect = isPathValid(n, edges, timepath.second, ta, tw, source, target);
        int time_found = computeTimeOfPath(n, edges, ta, tw, timepath.second, problem);

        if (!pathStructureCorrect || time_found != timepath.first) {
            std::cout << "COUNTERTEST (PATH) FOUND ON PROBLEM " << problem << '\n';
            std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
            std::cout << "time interval: " << ta << ' ' << tw << '\n';
            std::cout << "source and target: " << source << ' ' << target << '\n';
            for (auto [u, v, t, lambda] : edges) {
                std::cout << u << ' ' << v << ' ' << t << ' ' << lambda << '\n';
            }
            std::cout << "time reported: " << timepath.first << '\n';
            if (time_found != timepath.first) {
                std::cout << "time is not consistenct! actual time of reported path is: " << time_found << '\n';
            }
            std::cout << "path reported:\n";
            for (const Edge &e : timepath.second) {
                std::cout << "(" << e.u << ", " << e.v << ", " << e.t << ", " << e.lambda << ")\n";
            }
            if (!pathStructureCorrect) {
                std::cout << "path structure is invalid!\n";
            }
            exit(-1);
        }
    };

    if (foremost) {
        Foremost fm(n, edges, x, ta, tw);
        for (int u = 1; u <= n; u++) {
            checkPath("FOREMOST", x, u, fm.getForemostPath(u));
        }
    }
    
    if (revforemost) {
        ReverseForemost rfm(n, edges, x, ta, tw);
        for (int u = 1; u <= n; u++) {
            checkPath("REVFOREMOST", u, x, rfm.getReverseForemostPath(u));
        }
    }
    
    if (fastest) {
        Fastest ft(n, edges, x, ta, tw);
        for (int u = 1; u <= n; u++) {
            checkPath("FASTEST", x, u, ft.getFastestPath(u));
        }
    }

    if (shortest) {
        Shortest st(n, edges, x, ta, tw);
        for (int u = 1; u <= n; u++) {
            checkPath("SHORTEST", x, u, st.getShortestPath(u));
        }
    }
}

void Tests::strestTestCheckTime(bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // check if time is indeed optimal
    const int LOWER_N = 1, UPPER_N = 8, LOWER_M = 0, UPPER_M = 8, T = 30, L = 6;
    auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, T, L);
    streamPresentation_ize(n, edges);
    int x = 1;
    int ta = T / 4, tw = T - ta;

    auto outputCounterTest = [&](std::string problem, std::vector<int> time1, std::vector<int> time2) -> void {
        std::cout << "COUNTERTEST (TIME) FOUND ON PROBLEM " << problem << '\n';
        std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
        std::cout << "x and time interval: " << x << ' ' << ta << ' ' << tw << '\n';
        for (auto [u, v, t, lambda] : edges) {
            std::cout << u << ' ' << v << ' ' << t << ' ' << lambda << '\n';
        }
        std::cout << "output:\n";
        for (int u = 1; u <= n; u++) {
            std::cout << time1[u] << " \n"[u == n];
        }
        std::cout << "should be:\n";
        for (int u = 1; u <= n; u++) {
            std::cout << time2[u] << " \n"[u == n];
        }
        exit(-1);
    };

    if (foremost) {
        Foremost fm(n, edges, x, ta, tw);
        std::vector<int> time1 = fm.getAllForemostTime();
        std::vector<int> time2 = naive::getAllForemostTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("FOREMOST", time1, time2);
    }
    
    if (revforemost) {
        ReverseForemost rfm(n, edges, x, ta, tw);
        std::vector<int> time1 = rfm.getAllReverseForemostTime();
        std::vector<int> time2 = naive::getAllReverseForemostTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("REVFOREMOST", time1, time2);
    }
    
    if (fastest) {
        Fastest ft(n, edges, x, ta, tw);
        std::vector<int> time1 = ft.getAllFastestTime();
        std::vector<int> time2 = naive::getAllFastestTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("FASTEST", time1, time2);
    }

    if (shortest) {
        Shortest st(n, edges, x, ta, tw);
        std::vector<int> time1 = st.getAllShortestTime();
        std::vector<int> time2 = naive::getAllShortestTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("SHORTEST", time1, time2);
    }
}

void Tests::testOnDataTest(std::filesystem::path filepath, bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // run on external datasets
    // loading of input
    std::cout << "\nTesting " << number_of_x << (only_use_first_x ? " first" : " random") << " sources on " << filepath.string() << '\n';
    Timer timer;
    auto [n, edges] = loadGraph(filepath);
    std::cout << "loading graph took " << timer.elapsed(true) << " seconds\n";
    streamPresentation_ize(n, edges);
    std::cout << "sorting graph took " << timer.elapsed(true) << " seconds\n";

    int ta = 0, tw = INF;
    vector<int> xs; // the list of x's to run on
    number_of_x = std::min(number_of_x, n);
    if (only_use_first_x) {
        xs.resize(number_of_x);
        iota(xs.begin(), xs.end(), 1);
    } else {
        xs.resize(n);
        iota(xs.begin(), xs.end(), 1);
        std::shuffle(xs.begin(), xs.end(), rng);
        xs.resize(number_of_x);
    }

    if (foremost) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            Foremost fm(n, edges, x, ta, tw);
            sum += timer.elapsed();
            
            if (output_to_file) {
                std::vector<int> time = fm.getAllForemostTime();
                std::filesystem::path output_path = filepath.parent_path() / "foremost.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "foremost computation took " << sum << " seconds on average\n";
    }

    if (revforemost) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            ReverseForemost rfm(n, edges, x, ta, tw);
            sum += timer.elapsed();
            
            if (output_to_file) {
                std::vector<int> time = rfm.getAllReverseForemostTime();
                std::filesystem::path output_path = filepath.parent_path() / "revforemost.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "revforemost computation took " << sum << " seconds on average\n";
    }

    if (fastest) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            Fastest ft(n, edges, x, ta, tw);
            sum += timer.elapsed();
            
            if (output_to_file) {
                std::vector<int> time = ft.getAllFastestTime();
                std::filesystem::path output_path = filepath.parent_path() / "fastest.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "fastest computation took " << sum << " seconds on average\n";
    }

    if (shortest) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            Shortest st(n, edges, x, ta, tw);
            sum += timer.elapsed();
            
            if (output_to_file) {
                std::vector<int> time = st.getAllShortestTime();
                std::filesystem::path output_path = filepath.parent_path() / "shortest.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "shortest computation took " << sum << " seconds on average\n";
    }
}