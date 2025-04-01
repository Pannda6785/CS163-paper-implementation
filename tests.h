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

void stressTestCheckPath(bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // check if paths are valid and have the claimed time
    const int LOWER_N = 1, UPPER_N = 7, LOWER_M = 0, UPPER_M = 14, T = 50, L = 5;
    auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, T, L);
    streamPresentation_ize(n, edges);

    auto checkPath = [&](std::string problem, int ta, int tw, int source, int target, const std::pair<int, std::vector<Edge>> timepath) -> void {
        if (abs(timepath.first) == INF) return; // no path reported;
        bool pathStructureCorrect = isPathValid(n, edges, timepath.second, ta, tw, source, target);
        int time_found = computeTimeOfPath(n, edges, ta, tw, timepath.second, problem);
        if (!pathStructureCorrect || time_found != timepath.first) {
            std::cout << "COUNTERTEST FOUND ON PROBLEM " << problem << '\n';
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
            for (const Edge &e : edges) {
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
        std::vector<int> time1 = ft.getAllFastestTime();
        for (int u = 1; u <= n; u++) {
            checkPath("FASTEST", ta, tw, x, u, ft.getFastestPath(u));
        }
    }

    if (shortest) {
        
    }
}

void strestTestCheckTime(bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // check if time is indeed optimal
    const int LOWER_N = 1, UPPER_N = 5, LOWER_M = 0, UPPER_M = 8, T = 30, L = 6;
    auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, T, L);
    streamPresentation_ize(n, edges);

    auto outputCounterTest = [&](std::string problem, int ta, int tw, int x, std::vector<int> time1, std::vector<int> time2) -> void {
        std::cout << "COUNTERTEST FOUND ON PROBLEM " << problem << '\n';
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
        
    }
}

void visualTestOnRandomGraph(bool use_random_over_manual, bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // for empirical/human validation
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
        }
        std::cout << "specification: " << x << ' ' << ta << ' ' << tw << '\n';
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
        }
        std::cout << "specification: " << x << ' ' << ta << ' ' << tw << '\n';
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
        }
        std::cout << "specification: " << x << ' ' << ta << ' ' << tw << '\n';
        Fastest ft(n, edges, ta, tw, x);
        std::vector<int> time = ft.getAllFastestTime(); 
        std::cout << "fastest: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }

    if (shortest) {
        
    }
}

void testOnDataTest(std::filesystem::path filepath, bool foremost = true, bool revforemost = true, bool fastest = true, bool shortest = true) { // run on external datasets
    // loading of input
    std::cout << "\nTesting on " << filepath.string() << '\n';
    Timer timer;
    auto [n, edges] = loadGraph(filepath);
    std::cout << "loading graph took " << timer.elapsed(true) << " seconds\n";
    streamPresentation_ize(n, edges);
    std::cout << "sorting graph took " << timer.elapsed(true) << " seconds\n";

    if (foremost) {
        int x = 1;
        int ta = 0, tw = INF;

        timer.reset();
        Foremost fm(n, edges, ta, tw, x);
        std::cout << "foremost computation took " << timer.elapsed(true) << " seconds\n";
        std::vector<int> time = fm.getAllForemostTime();
        
        std::filesystem::path output_path = filepath.parent_path() / "foremost.txt";
        std::ofstream out(output_path);
        for (int u = 1; u <= n; u++) out << time[u] << '\n';
        out.close();
    }

    if (revforemost) {
        int x = n;
        int ta = 0, tw = INF;
        
        timer.reset();
        ReverseForemost rfm(n, edges, ta, tw, x);
        std::cout << "revforemost computation took " << timer.elapsed(true) << " seconds\n";
        std::vector<int> time = rfm.getAllReverseForemostTime();

        std::filesystem::path output_path = filepath.parent_path() / "revforemost.txt";
        std::ofstream out(output_path);
        for (int u = 1; u <= n; u++) out << time[u] << '\n';
        out.close();
    }

    if (fastest) {
        int x = 1;
        int ta = 0, tw = INF;

        timer.reset();
        Fastest ft(n, edges, ta, tw, x);
        std::cout << "fastest computation took " << timer.elapsed(true) << " seconds\n";
        std::vector<int> time = ft.getAllFastestTime();
        
        std::filesystem::path output_path = filepath.parent_path() / "fastest.txt";
        std::ofstream out(output_path);
        for (int u = 1; u <= n; u++) out << time[u] << '\n';
        out.close();
    }

    if (shortest) {
        
    }
}