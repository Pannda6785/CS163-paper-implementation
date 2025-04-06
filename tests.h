#include "utility.h"
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

void stressTestCheckPath(bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // check if paths are valid and have the claimed time
    const int LOWER_N = 1, UPPER_N = 10, LOWER_M = 0, UPPER_M = 30, T = 100, L = 10;
    auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, T, L);
    streamPresentation_ize(n, edges);

    auto checkPath = [&](std::string problem, int ta, int tw, int source, int target, const std::pair<int, std::vector<Edge>> timepath) -> void {
        if (abs(timepath.first) == INF) return; // no path reported;
        bool pathStructureCorrect = isPathValid(n, edges, timepath.second, ta, tw, source, target);
        int time_found = computeTimeOfPath(n, edges, ta, tw, timepath.second, problem);
        if (!pathStructureCorrect || time_found != timepath.first) {
            std::cout << "COUNTERTEST (PATH) FOUND ON PROBLEM " << problem << '\n';
            std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
            std::cout << "specification: " << ta << ' ' << tw << '\n';
            std::cout << "source target: " << source << ' ' << target << '\n';
            for (auto [u, v, t, lambda] : edges) {
                std::cout << u << ' ' << v << ' ' << t << '_' << lambda << '\n';
            }
            std::cout << "time reported: " << timepath.first << '\n';
            if (time_found != timepath.first) {
                std::cout << "time of path should be: " << time_found << '\n';
            }
            if (!pathStructureCorrect) {
                std::cout << "path structure is invalid\n";
            }
            std::cout << "path reported:\n";
            for (const Edge &e : timepath.second) {
                std::cout << "(" << e.u << ", " << e.v << ", " << e.t << ", " << e.lambda << ")\n";
            }
            exit(-1);
        }
    };

    if (foremost) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        Foremost fm(n, edges, ta, tw, x);
        for (int u = 1; u <= n; u++) {
            checkPath("FOREMOST", ta, tw, x, u, fm.getForemostPath(u));
        }
    }
    
    if (revforemost) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        ReverseForemost rfm(n, edges, ta, tw, x);
        for (int u = 1; u <= n; u++) {
            checkPath("REVFOREMOST", ta, tw, u, x, rfm.getReverseForemostPath(u));
        }
    }
    
    if (fastest) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        Fastest ft(n, edges, ta, tw, x);
        for (int u = 1; u <= n; u++) {
            checkPath("FASTEST", ta, tw, x, u, ft.getFastestPath(u));
        }
    }

    if (shortest) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        Shortest st(n, edges, ta, tw, x);
        for (int u = 1; u <= n; u++) {
            checkPath("SHORTEST", ta, tw, x, u, st.getShortestPath(u));
        }
    }
}

void strestTestCheckTime(bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // check if time is indeed optimal
    const int LOWER_N = 1, UPPER_N = 5, LOWER_M = 0, UPPER_M = 8, T = 30, L = 6;
    auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, T, L);
    streamPresentation_ize(n, edges);

    auto outputCounterTest = [&](std::string problem, int ta, int tw, int x, std::vector<int> time1, std::vector<int> time2) -> void {
        std::cout << "COUNTERTEST (TIME) FOUND ON PROBLEM " << problem << '\n';
        std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
        std::cout << "specification: " << x << ' ' << ta << ' ' << tw << '\n';
        for (auto [u, v, t, lambda] : edges) {
            std::cout << u << ' ' << v << ' ' << t << '_' << lambda << '\n';
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
        int x = 1;
        int ta = T / 4, tw = T - ta;
        Foremost fm(n, edges, ta, tw, x);
        std::vector<int> time1 = fm.getAllForemostTime();
        std::vector<int> time2 = naive::getAllForemostTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("FOREMOST", ta, tw, x, time1, time2);
    }
    
    if (revforemost) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        ReverseForemost rfm(n, edges, ta, tw, x);
        std::vector<int> time1 = rfm.getAllReverseForemostTime();
        std::vector<int> time2 = naive::getAllReverseForemostTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("REVFOREMOST", ta, tw, x, time1, time2);
    }
    
    if (fastest) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        Fastest ft(n, edges, ta, tw, x);
        std::vector<int> time1 = ft.getAllFastestTime();
        std::vector<int> time2 = naive::getAllFastestTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("FASTEST", ta, tw, x, time1, time2);
    }

    if (shortest) {
        int x = 1;
        int ta = T / 4, tw = T - ta;
        Shortest st(n, edges, ta, tw, x);
        std::vector<int> time1 = st.getAllShortestTime();
        std::vector<int> time2 = naive::getAllShortestTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("SHORTEST", ta, tw, x, time1, time2);
    }
}

void visualTest(bool use_random_over_manual, bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // for empirical/human validation
    int n;
    std::vector<Edge> edges;
    int ta, tw, x;
    if (use_random_over_manual) {
        tie(n, edges) = randomGraph();
        std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
        for (auto [u, v, t, lambda] : edges) {
            std::cout << u << ' ' << v << ' ' << t << '_' << lambda << '\n';
        }
    } else {
        int m;
        std::cin >> n >> m;
        std::cin >> ta >> tw >> x;
        for (int i = 0; i < m; i++) {
            int u, v, t, lambda;
            std::cin >> u >> v >> t >> lambda;
            edges.push_back({u, v, t, lambda});
        }
    }
    streamPresentation_ize(n, edges);
    
    if (foremost) {
        if (use_random_over_manual) {
            x = 1;
            ta = 10, tw = 90;
            std::cout << "foremost specification: " << x << ' ' << ta << ' ' << tw << '\n';
        }
        Foremost fm(n, edges, ta, tw, x);
        std::vector<int> time = fm.getAllForemostTime();
        std::cout << "foremost: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }

    if (revforemost) {
        if (use_random_over_manual) {
            x = 1;
            ta = 10, tw = 90;
            std::cout << "revforemost specification: " << x << ' ' << ta << ' ' << tw << '\n';
        }
        ReverseForemost rfm(n, edges, ta, tw, x);
        std::vector<int> time = rfm.getAllReverseForemostTime(); 
        std::cout << "revforemost: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }
    
    if (fastest) { 
        if (use_random_over_manual) {
            x = 1;
            ta = 10, tw = 90;
            std::cout << "fastest specification: " << x << ' ' << ta << ' ' << tw << '\n';
        }
        Fastest ft(n, edges, ta, tw, x);
        std::vector<int> time = ft.getAllFastestTime(); 
        std::cout << "fastest: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }

    if (shortest) {
        if (use_random_over_manual) {
            x = 1;
            ta = 10, tw = 90;
            std::cout << "shortest specification: " << x << ' ' << ta << ' ' << tw << '\n';
        }
        Shortest st(n, edges, ta, tw, x);
        std::vector<int> time = st.getAllShortestTime(); 
        std::cout << "shortest: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }
}

void testOnDataTest(std::filesystem::path filepath, bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true, int number_of_x = 1, bool only_use_first_x = true, bool output = true) { // run on external datasets
    // loading of input
    std::cout << "\nTesting " << number_of_x << " sources on " << filepath.string() << '\n';
    Timer timer;
    auto [n, edges] = loadGraph(filepath);
    std::cout << "loading graph took " << timer.elapsed(true) << " seconds\n";
    streamPresentation_ize(n, edges);
    std::cout << "sorting graph took " << timer.elapsed(true) << " seconds\n";

    int ta = 0, tw = INF;
    vector<int> xs;
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
            Foremost fm(n, edges, ta, tw, x);
            sum += timer.elapsed();
            
            if (output) {
                std::vector<int> time = fm.getAllForemostTime();
                std::filesystem::path output_path = filepath.parent_path() / "foremost.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "foremost computation took " << sum << " seconds\n";
    }

    if (revforemost) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            ReverseForemost rfm(n, edges, ta, tw, x);
            sum += timer.elapsed();
            
            if (output) {
                std::vector<int> time = rfm.getAllReverseForemostTime();
                std::filesystem::path output_path = filepath.parent_path() / "revforemost.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "revforemost computation took " << sum << " seconds\n";
    }

    if (fastest) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            Fastest ft(n, edges, ta, tw, x);
            sum += timer.elapsed();
            
            if (output) {
                std::vector<int> time = ft.getAllFastestTime();
                std::filesystem::path output_path = filepath.parent_path() / "fastest.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "fastest computation took " << sum << " seconds\n";
    }

    if (shortest) {
        double sum = 0;
        for (int x : xs) {
            timer.reset();
            Shortest st(n, edges, ta, tw, x);
            sum += timer.elapsed();
            
            if (output) {
                std::vector<int> time = st.getAllShortestTime();
                std::filesystem::path output_path = filepath.parent_path() / "shortest.txt";
                std::ofstream out(output_path);
                for (int u = 1; u <= n; u++) out << time[u] << '\n';
                out.close();
            }
        }
        sum /= number_of_x;
        std::cout << "shortest computation took " << sum << " seconds\n";
    }
}