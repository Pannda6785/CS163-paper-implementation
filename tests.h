#include "utility.h"
#include "timer.h"
#include "naive.h"
#include "foremost.h"
#include "revforemost.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <vector>

void stressTestCheckPath(int ITE) { // check validity (if the path is that claimed). NOT DONE
    // const int LOWER_N = 1, UPPER_N = 10, LOWER_M = 0, UPPER_M = 10, C = 20, L = 20;
    
    // for (int ite = 0; ite < ITE; ite++) {
    //     auto [n, edges] = randomGraph(LOWER_N, UPPER_N, LOWER_M, UPPER_M, C, L);
    //     streamPresentation_ize(n, edges);
    //     int ta = 0;
    //     int tw = C;
    //     int x = 1;
        
    //     Foremost fm(n, edges, ta, tw, x);
    //     std::vector<int> t1 = fm.getAllForemostTime();

    //     if (t1 != t2) {
    //         std::cout << "COUNTERTEST FOUND!\n";
    //         std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
    //         std::cout << "specification: " << x << ' ' << ta << ' ' << tw << '\n';
    //         for (auto [u, v, t, lambda] : edges) {
    //             std::cout << u << ' ' << v << ' ' << t << '_' << lambda << '\n';
    //         }
    //         std::cout << "output:\n";
    //         for (int u = 1; u <= n; u++) {
    //             std::cout << t1[u] << " \n"[u == n];
    //         }
    //         for (int u = 1; u <= n; u++) {
    //             std::cout << t2[u] << " \n"[u == n];
    //         }
    //         exit(0);
    //     }
    // }

    // for (int u = 1; u <= n; u++) {
    //     out << u << ": " << 1 << ' ';
    //     std::vector<Edge> path = fm.getForemostPath(u).second;
    //     if (!isPathValid(n, edges, path)) {
    //         std::cout << "Path not valid!\n";
    //         exit(0);
    //     }
    //     for (auto [u, v, t, lambda] : path) {
    //         out << v << ' ';
    //     }
    //     out << '\n';
    // }
}

void strestTestCheckTime() { // check for time optimality
    const int LOWER_N = 1, UPPER_N = 10, LOWER_M = 0, UPPER_M = 10, T = 20, L = 20;
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
        for (int u = 1; u <= n; u++) {
            std::cout << time2[u] << " \n"[u == n];
        }
        exit(0);

    };

    /* foremost test */ {
        int x = 1;
        int ta = 0, tw = T;
        Foremost fm(n, edges, ta, tw, x);
        std::vector<int> time1 = fm.getAllForemostTime();
        std::vector<int> time2 = naive::getAllForemostTime(n, edges, ta, tw, x);
        if (time1 != time2) outputCounterTest("FOREMOST", ta, tw, x, time1, time2);
    }
    
}

void visualTestOnRandomGraph() { // just for empirical validation
    auto [n, edges] = randomGraph();
    streamPresentation_ize(n, edges);
    
    std::cout << "size of graph: " << n << ' ' << edges.size() << '\n';
    for (auto [u, v, t, lambda] : edges) {
        std::cout << u << ' ' << v << ' ' << t << '_' << lambda << '\n';
    }
    
    /* foremost test */ {
        int x = 1;
        int ta = 10, tw = 90;
        std::cout << "specification: " << x << ' ' << ta << ' ' << tw << ' ';
        Foremost fm(n, edges, ta, tw, x);
        std::vector<int> time = fm.getAllForemostTime();
        std::cout << "foremost: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }

    /* foremost test */ {
        int x = 1;
        int ta = 10, tw = 90;
        std::cout << "specification: " << x << ' ' << ta << ' ' << tw << ' ';
        ReverseForemost rfm(n, edges, ta, tw, x);
        std::vector<int> time = rfm.getAllReverseForemostTime(); 
        std::cout << "revforemost: ";
        for (int u = 1; u <= n; u++) {
            std::cout << time[u] << " \n"[u == n];
        }
    }
}

void testOnDataTest(std::filesystem::path filepath) { // run on external datasets
    // loading of input
    Timer timer;
    auto [n, edges] = loadGraph(filepath);
    std::cout << "loading graph took " << timer.elapsed(true) << " seconds\n";
    streamPresentation_ize(n, edges);
    std::cout << "sorting graph took " << timer.elapsed(true) << " seconds\n";
    
    /* foremost test */ {
        int x = 1;
        int ta = 0, tw = INF;
        std::filesystem::path output_path = filepath.parent_path() / "foremost.txt";
        std::ofstream out(output_path);
        timer.reset();
        Foremost fm(n, edges, ta, tw, x);
        std::cout << "foremost computation took " << timer.elapsed(true) << " seconds\n";
        std::vector<int> time = fm.getAllForemostTime();
        for (int u = 1; u <= n; u++) out << time[u] << '\n';
        out.close();
    }

    /* reverse foremost test */ {
        int x = n;
        int ta = 0, tw = INF;
        std::filesystem::path output_path = filepath.parent_path() / "revforemost.txt";
        std::ofstream out(output_path);
        timer.reset();
        ReverseForemost rfm(n, edges, ta, tw, x);
        std::cout << "revforemost computation took " << timer.elapsed(true) << " seconds\n";
        std::vector<int> time = rfm.getAllReverseForemostTime();
        for (int u = 1; u <= n; u++) out << time[u] << '\n';
        out.close();
    }

    /* shortest test */ {
        
    }

    /* fastest test */ {
        
    }
}