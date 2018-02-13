#ifndef __CONTRASTING_SUBGRAPH__
#define __CONTRASTING_SUBGRAPH__

#include "graph.h"

using namespace std;

void contrast_init(){
    contrast_graph_edges.clear();
    contrast_graph_edges.resize(node_id2label.size());
    for ( auto & m : contrast_graph_edges){
        m.clear(); m.resize(0);
    }
    contrast_graph_node_weights.clear();
    contrast_graph_node_weights.resize(node_id2label.size());
    contrast_graph_node_degrees.clear();
    contrast_graph_node_degrees.resize(node_id2label.size());
    for ( auto & m : contrast_graph_node_weights ) m = 0;
    for ( auto & m : contrast_graph_node_degrees ) m = 0;
}

//A B are the graphs we are interested in 
//seeds are the nodes that we want to include in our solution
set<int> find_contrast_graph_with_seeds(int graph_A, int graph_B, set<int> seed, int max_d){
    //clear all global variables
    denseSubgraph::init();
    contrast_init();

    auto & A = graph_edges[graph_A];
    auto & B = graph_edges[graph_B];

    using namespace denseSubgraph;
    //edge count m node count n
    edge_count = 0;
    node_count = 0;
    double epsContrast = 1e100, epsPenalty = 1e100;
    set<int> connect;
    connect.clear();
    vector<int> visited;
    visited.resize(node_id2label.size());
    set<int> c1, c2;
    if ( max_d != -1 && seed.size() )  {
        for ( auto & m : seed ) {
            for ( auto & m : visited ) m = 0;
            search(c1, A, visited, m, max_d); 
            for ( auto & m : visited ) m = 0;
            search(c2, B, visited, m, max_d); 
        }
    }
    else {
        for ( int i = 0 ; i < node_id2label.size() ; i ++ ) {
            connect.insert(i);
        }
    }
    for ( auto & m : c1 ) connect.insert(m);
    for ( auto & m : c2 ) connect.insert(m);
    for ( auto & m : seed ) connect.insert(m);

    cerr << connect.size() << endl;

    for ( int i = 0 ; i < node_id2label.size() ; i ++ ) {
        if ( !mfind(connect, i) ) continue;
        int ptr1, ptr2;
        ptr1 = ptr2 = 0;
        //graph A minus graph B
        sort(A[i].begin(), A[i].end());
        sort(B[i].begin(), B[i].end());
        while ( ptr1 < A[i].size() && ptr2 < B[i].size() ){
            if ( A[i][ptr1].first < B[i][ptr2].first ) {
                epsContrast = min(epsContrast, A[i][ptr1].se);
                contrast_graph_edges[i].pb(mp(A[i][ptr1].fi, A[i][ptr1].se ));
                ptr1 ++;
            }
            else if ( A[i][ptr1].first == B[i][ptr2].first){
                if ( !mfind(connect, A[i][ptr1].fi) ) {ptr1 ++; ptr2 ++; continue;}
                if ( abs(A[i][ptr1].se - B[i][ptr2].se) == 0 ) { ptr1 ++; ptr2 ++; continue; }
                epsContrast = min(epsContrast, abs(A[i][ptr1].se - B[i][ptr2].se));
                contrast_graph_edges[i].pb(mp(A[i][ptr1].fi, abs(A[i][ptr1].se - B[i][ptr2].se)));
                ptr1 ++; ptr2 ++;
            }
            else {
                if ( !mfind(connect, B[i][ptr2].fi) ){ptr2 ++; continue;}
                epsContrast = min(epsContrast, B[i][ptr2].se);
                contrast_graph_edges[i].pb(mp(B[i][ptr2].fi, B[i][ptr2].se ));
                ptr2 ++;
            }
        }
        while ( ptr1 < A[i].size() ) {
            if ( !mfind(connect, A[i][ptr1].fi) ) {ptr1 ++; continue;}
            epsContrast = min(epsContrast, A[i][ptr1].se);
            contrast_graph_edges[i].pb(mp(A[i][ptr1].fi, A[i][ptr1].se ));
            ptr1 ++;
        }
        while ( ptr2 < B[i].size() ){
            if ( !mfind(connect, B[i][ptr2].fi) ) {ptr2 ++; continue;}
            epsContrast = min(epsContrast, B[i][ptr2].se);
            contrast_graph_edges[i].pb(mp(B[i][ptr2].fi, B[i][ptr2].se ));
            ptr2 ++;
        }
        //node weight by adding degrees in graph A and B
        double cnt = 0;
        for ( auto & m : A[i] ) 
            if ( mfind(connect, m.fi) ) 
                cnt += m.se;
        for ( auto & m : B[i] ) 
            if ( mfind(connect, m.fi) ) 
                cnt += m.se; 
        cnt = abs(cnt);
        contrast_graph_node_weights[i] = cnt;
        //new graph node degree
        cnt = 0.0;
        for ( auto & m : contrast_graph_edges[i] ) {
            cnt += m.se;
        }
        contrast_graph_node_degrees[i] = cnt;
    }
    for ( int i = 0 ; i < contrast_graph_edges.size() ; i ++ ){
        for ( auto & m : contrast_graph_edges[i]){
            edge_count += m.se;
        }
    }
    //mincut graph weight on edge (i,t) : 2*g*dup[i] - du[v]

    s = 0; t = node_id2label.size() + 1;
    for ( int i = 0 ; i < node_id2label.size() ; i ++ ) {
        dup[i + 1] = pow(contrast_graph_node_weights[i], NORM_CONST);
        node_count += dup[i + 1];
        epsPenalty = min(epsPenalty, dup[i + 1]);
        du[i + 1] = contrast_graph_node_degrees[i];
    }
    cerr << node_count << ' ' << edge_count << endl;
    l = epsContrast / node_count ; r = edge_count / epsPenalty ; delta = epsContrast / node_count /node_count;
    while ( r - l > delta ) {
        mid = (l + r ) /2;
        solve(mid, seed, contrast_graph_edges);
        // dfs(s);
        // int cnt_node = 0;
        // for ( int i = 1 ; i <= node_count ; i ++  ){
        //     cnt_node += v[i];
        // }
        if (( (double)edge_count * node_count - maxflow) > EPSILON_1) l = mid; else r = mid;

    }
    cerr << "Final contrast score " << l << endl;
    solve(l, seed, contrast_graph_edges);
    dfs(s);


    int ret_cnt = 0;
    set<int> contrast_set;
    for ( int i = 1 ; i <= node_count ; i ++ ) {
        if ( v[i] ){ cerr << i - 1 << ' ';  ret_cnt ++; contrast_set.insert(i - 1);}
    }
    cerr << "Found " << ret_cnt << " nodes" << endl;
    for (int u : seed) {
        myAssert(contrast_set.count(u), "[Error] Seeds are not in the contrast set!");
    }


    stringstream filebuf;
    filebuf << "output/contrast.out";
    FILE *    matrixOut = tryOpen(filebuf.str(), "w");
    fprintf(matrixOut, (graph_names[graph_A] + "," + graph_names[graph_B] + "\n").c_str()); 
    fprintf(matrixOut, "%lld\n", contrast_set.size());
    fprintf(matrixOut, "seeds:");
    for ( auto & i : seed ) {
        // fprintf(matrixOut, "%d,", i);
        fprintf(matrixOut, "%s,", (node_id2label[i]).c_str());
    }
    fprintf(matrixOut, "\n");
    for ( auto & i : contrast_set ) {
        // fprintf(matrixOut, "%d,", i);
        fprintf(matrixOut, "%s,", (node_id2label[i]).c_str());
    }
    fprintf(matrixOut, "\n");
    printMatrix(A, contrast_set, matrixOut);
    printMatrix(B, contrast_set, matrixOut);

    return contrast_set;
}

#endif

