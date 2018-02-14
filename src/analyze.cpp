#include "data/contrastingSubgraph.h"
#include "data/coherentCoreMining.h"
const int iter =200;

bool loadArgs(int argc, char * argv[], int & graph_num_A, int & graph_num_B, bool & core, bool & grow, set<int> & seeds, int & step_core, int & step_contrast){
    if ( argc != 10 && argc != 9 ) {
        cerr << "Please give command as following " << endl;
        cerr << "./bin/analyze data_folder_name(under data folder) Graph_A_num(int) Graph_B_num(int) use_core(bool) use_neighbor(bool) use_log(bool) step_core(int) step_contrast(int) seed(list of int)" << endl;
        cerr << "e.g. ./analyze Test 0 1 1 1 1 5 5 1,2,3,4,5,6" << endl;
        cerr << "For general contrast without seeds " << endl;
        cerr << "e.g. ./analyze Test 0 1 1 0 1 0 0" << endl;
        return false;
    }

    DIR_PREFIX = argv[1];
    GRAPH_DIR = "data/" + DIR_PREFIX + "/";

    graph_num_A = atoi(argv[2]), graph_num_B = atoi(argv[3]);
    core = (bool)atoi(argv[4]);
    grow = (bool)atoi(argv[5]);
    LOG_EDGE = (bool)atoi(argv[6]);
    step_core = atoi(argv[7]);
    step_contrast = atoi(argv[8]);
    cerr << "=== Graph Information ===" << endl;
    loadGraphs();
    int i = 0;
    if ( argc == 10 ){
        string s = argv[9];
        while ( i < s.length() ) {
            int c = 0;
            while ( s[i] != ','&& i < s.length() ){
                c *= 10;
                c += s[i] - '0';
                i ++;
            }
            seeds.insert(c);
            i++;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {

    int graph_num_A, graph_num_B, step_core, step_contrast;
    bool core, grow;
    set<int> seeds;
    if ( ! loadArgs(argc, argv, graph_num_A, graph_num_B, core, grow, seeds, step_core, step_contrast) )
        return 0;

    cerr << "=== Settings ===" << endl;
    cerr << "  Contrsting graph " << graph_names[graph_num_A] << " and " << graph_names[graph_num_B] << endl;
    if ( core ) cerr << "  Coherent core is ON" << endl;
    else cerr << "  Coherent core is OFF" << endl;
    if ( grow ) cerr << "  Neighbor constraint ON, " << "with core neighbor r = " << step_core << " and contrast neighbor r = " << step_contrast << endl;
    else cerr << "  Neighbor constraint OFF" << endl;
    if ( seeds.size() ) {
        cerr << "  Seeds are given:" << endl;
        cerr << "    ";
        for ( auto m = seeds.begin() ; m != seeds.end() ; ++m ) if ( m != seeds.begin() ) cerr << ',' << node_id2label[*m]; else cerr << node_id2label[*m];
        cerr << endl;
    }

    if ( ! core ){
        cerr << "=== Contrast Subgraph ===" << endl;
        find_contrast_graph_with_seeds(graph_num_A, graph_num_B, seeds, step_contrast);
    }
    else {
        cerr << "=== Coherent Core ===" << endl;
        set<int> core = find_core_graph_with_seeds(graph_num_A, graph_num_B, seeds, step_core);
        if ( grow ) {
            cerr << "=== Contrast Subgraph ===" << endl;
            find_contrast_graph_with_seeds(graph_num_A, graph_num_B, core, step_contrast);
        } 
        else {
            cerr << "=== Contrast Subgraph ===" << endl;
            find_contrast_graph_with_seeds(graph_num_A, graph_num_B, core, -1);
        }
    }
    cerr << "=== Done ===" << endl;
}

