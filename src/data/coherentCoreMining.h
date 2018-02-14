#ifndef __COHERENTCOREMINING__ 
#define __COHERENTCOREMINING__ 

#include "graph.h"

using namespace std;

void core_init(){
    sharingEdges.clear();
    sharingEdges.resize(node_id2label.size());
    for ( auto & m : sharingEdges){
        m.clear(); m.resize(0);
    }
    sharingNodeWeights.clear();
    sharingNodeWeights.resize(node_id2label.size());
    sharingNodeDegrees.clear();
    sharingNodeDegrees.resize(node_id2label.size());
    for ( auto & m : sharingNodeWeights ) m = 0;
    for ( auto & m : sharingNodeDegrees ) m = 0;
}

//A B are the graphs we are interested in 
//seeds are the nodes that we want to include in our solution
set<int> find_core_graph_with_seeds(int graph_A, int graph_B, set<int> seed, int max_d){
    //clear all global variables
    core_init();

    using namespace denseSubgraph;
    init();
    //The two list represented graphs A and B
    auto & A = graph_edges[graph_A];
    auto & B = graph_edges[graph_B];


    set<int> connect;
    if ( max_d == -1 || !seed.size() ) {
        for ( int i = 0 ; i < node_id2label.size() ; i ++ ) {
            connect.insert(i);
        }
    }
    else {
        vector<int> visited;
        visited.resize(node_id2label.size());
        set<int> c1,c2;
        for ( auto & m : seed ) {
            for ( auto & n : visited ) n = 0; 
            search(c1, A, visited, m, max_d);
            for ( auto & n : visited ) n = 0; 
            search(c2, B, visited, m, max_d);
        }
        for ( auto & m : c1 ) connect.insert(m);
        for ( auto & m : c2 ) connect.insert(m);
        for ( auto & m : seed ) connect.insert(m);
    }

    edge_count = 0;
    node_count = 0;
    double epsContrast = 1e100, epsPenalty = 1e100;
    for ( int i = 0 ; i < node_id2label.size() ; i ++ ) {
        if ( !mfind(connect, i) ) continue;
        int ptr1, ptr2;
        ptr1 = ptr2 = 0;
        sort(A[i].begin(), A[i].end());
        sort(B[i].begin(), B[i].end());
        while ( ptr1 < A[i].size() && ptr2 < B[i].size() ){
            if ( A[i][ptr1].first < B[i][ptr2].first ) {
                ptr1 ++;
            }
            else if ( A[i][ptr1].first == B[i][ptr2].first){
                if ( mfind(connect, A[i][ptr1].fi) ) {
                    epsContrast = min(epsContrast, min(A[i][ptr1].se , B[i][ptr2].se));
                    sharingEdges[i].pb(mp(A[i][ptr1].fi, min(A[i][ptr1].se , B[i][ptr2].se)));
                }
                ptr1 ++; ptr2 ++;
            }
            else {
                ptr2 ++;
            }
        }
        double cnt = 0;
        for ( auto & m : A[i] ) 
            if ( mfind(connect, m.fi) ) 
                cnt += m.se;
        for ( auto & m : B[i] ) 
            if ( mfind(connect, m.fi) ) 
                cnt += m.se; 
        cnt = abs(cnt);
        sharingNodeWeights[i] = cnt;
        cnt = 0.0;
        for ( auto & m : sharingEdges[i] ) {
            cnt += m.se;
        }
        sharingNodeDegrees[i] = cnt;
    }
    for ( int i = 0 ; i < sharingEdges.size() ; i ++ ){
        for ( auto & m : sharingEdges[i]){
            edge_count += m.se;
        }
    }

    s = 0; t = node_id2label.size() + 1;
    for ( int i = 0 ; i < node_id2label.size() ; i ++ ) {
        dup[i + 1] = pow(sharingNodeWeights[i], NORM_CONST);
        node_count += dup[i + 1];
        epsPenalty = min(epsPenalty, dup[i + 1]);
        du[i + 1] = sharingNodeDegrees[i];
    }

    l = epsContrast / node_count ; r = edge_count / epsPenalty ; delta = epsContrast / node_count /node_count;
    while ( r - l > delta ) {
        mid = (l + r ) /2;
        solve(mid, seed, sharingEdges);
        // dfs(s);
        // int cnt_node = 0;
        // for ( int i = 1 ; i <= node_count ; i ++  ){
        //     cnt_node += v[i];
        // }
        if (( (double)edge_count * node_count - maxflow)  > EPSILON_1 /*&& cnt_node > seed.size()*/) l = mid; else r = mid;

    }
    cerr << "Final coherent score " << l << endl;
    solve(l, seed, sharingEdges);
    dfs(s);

    set<int> sharingSubgraph;
    int ret_cnt = 0;
    for ( int i = 1 ; i <= node_count ; i ++ ) {
        if ( v[i] ) { ret_cnt += 1; sharingSubgraph.insert(i - 1); }
    }
    cerr << "Found " << ret_cnt << " nodes" << endl;
    stringstream filebuf;
    filebuf << "output/coherent.out";
    FILE *    matrixOut = tryOpen(filebuf.str(), "w");
    fprintf(matrixOut, (graph_names[graph_A] + "," + graph_names[graph_B] + "\n").c_str()); 
    fprintf(matrixOut, "%lld\n", sharingSubgraph.size());
    fprintf(matrixOut, "seeds:");
    for ( auto & i : seed ) {
        // fprintf(matrixOut, "%d,", i);
        fprintf(matrixOut, "%s,", (node_id2label[i]).c_str());
    }
    fprintf(matrixOut, "\n");
    for ( auto & i : sharingSubgraph ) {
        // fprintf(matrixOut, "%d,", i);
        fprintf(matrixOut, "%s,", (node_id2label[i]).c_str());
    }
    fprintf(matrixOut, "\n");
    printMatrix(A, sharingSubgraph, matrixOut);
    printMatrix(B, sharingSubgraph, matrixOut);
    return sharingSubgraph;
}

#endif

