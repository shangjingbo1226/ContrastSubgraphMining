#ifndef __GRAPH_H__
#define __GRAPH_H__

#define ll long long
#define MSET(x) memset((x), 0, sizeof((x)))
#define MMSET(x) memset((x), -1, sizeof((x)))
#define rep(i, a, b) for ( int (i) = a; (i) < (b) ; (i) ++ )
#define repA(i, a, b) for ( int (i) = a ;  (i) <= (b) ; (i) ++ )
#define repD(i, a, b) for ( int (i) = a; (i) >= (b) ; (i) -- )
#define fi first
#define se second
#define pb push_back
#define mfind(m, item) ((m).find((item)) != (m).end())
#define mp(a,b) make_pair((a),(b))
#define mt(a,b,c) make_pair(make_pair((a),(b)), (c))
#define tfi first.first
#define tse first.second
#define tth second
#define tri pair<pair<double, double>, int>
#include "../utils/utils.h"

double NORM_CONST = 0.0;
bool LOG_EDGE = false;
const double epsilon = 1e-10;
const double EPSILON_1 = 1e-4;

vector<vector<pair<int, double>>> contrast_graph_edges;
vector<double> contrast_graph_node_weights;
vector<double> contrast_graph_node_degrees;
vector<int> contrasting_subgraph;

vector<vector<pair<int, double>>> sharingEdges;
vector<double> sharingNodeWeights;
vector<double> sharingNodeDegrees;

string DIR_PREFIX;
string GRAPH_DIR;
vector<string> node_id2label;
vector<string> graph_names;
vector<vector<vector<pair<int, double>>>> graph_edges;

using namespace std;

namespace denseSubgraph{
    const int inf=0x3fffffff,u=500000, w=10000000;
    int head[u],ver[w],next[w],q[u],d[u],v[u];
    int s,t,tot,i,ans;
    double node_count,edge_count;
    double edge[w],maxflow,k,l,r,mid,delta, dup[u], du[u];

    void init(){
        MSET(dup); MSET(du); MSET(head); MSET(ver); MSET(next); MSET(q); MSET(d); MSET(v); MSET(edge);
        node_count = edge_count = s = t = tot = i = ans = 0;
        maxflow = k = l = r = mid = delta = 0.0;
    }

    void add(int x,int y,double z)
    {
        ver[++tot]=y;
        edge[tot]=z;
        next[tot]=head[x];
        head[x]=tot;
        ver[++tot]=x;
        edge[tot]=0;
        next[tot]=head[y];
        head[y]=tot;
    }

    bool bfs()
    {
        memset(d,0,sizeof(d));
        int l,r;
        l=r=1; q[1]=s; d[s]=1;
        while(l<=r)
        {
            for(int i=head[q[l]];i;i=next[i])
                if(edge[i]>epsilon && !d[ver[i]])
                {
                    q[++r]=ver[i];
                    d[ver[i]]=d[q[l]]+1;
                    if(ver[i]==t) return 1;
                }
            l++;
        }
        return 0;
    }

    double dinic(int x,double f)
    {
        if(x==t) return f;
        double temp=f,k;
        for(int i=head[x];i;i=next[i])
            if(edge[i]>epsilon && temp>epsilon && d[ver[i]]==d[x]+1)
            {
                k=dinic(ver[i],min(temp,edge[i]));
                if(k<epsilon) d[ver[i]]=0;
                edge[i]-=k;
                edge[i^1]+=k;
                temp-=k;   
            }
        return f-temp;
    }

    //Need to set node_count and edge_count before using this function
    void solve(double g, set<int> & seeds, vector<vector<pair<int, double>>> & edges)
    {
        memset(head,0,sizeof(head));
        tot=1; maxflow=0;
        for(i=1;i<=node_count;i++)
        {
            if ( !mfind(seeds, i - 1) ) 
                add(s,i,edge_count);
            else  add(s, i, std::numeric_limits<double>::max());
            add(i,t,edge_count+ 2*g * dup[i]-du[i]);
        }
        for ( int i = 0 ; i < edges.size() ; i ++ ) {
            int u = i + 1;
            for ( int j = 0 ; j < edges[i].size() ; j ++ ) {
                auto & m = edges[i][j];
                int v = m.fi + 1;
                add(u,v,m.se);
            }
        }
        while(bfs())
            while((k=dinic(s,inf))>epsilon) maxflow+=k;
    }

    void dfs(int x)
    {
        v[x]=1; ans++;
        for(int i=head[x];i;i=next[i])
            if(!v[ver[i]] && edge[i]>epsilon) dfs(ver[i]);
    }


}

void printMatrix(vector<vector<pair<int, double> > > & A, set<int> contrast_set, FILE * matrixOut){
    
    // cerr << "Adj Matrix :" << endl;
    // cerr << "--------------------------" << endl;
    size_t max_l = 0;
    for ( auto & i : contrast_set ) {
        max_l = max(max_l, node_id2label[i].length());
    }
    
    // cerr << padding_sentence("", max_l) << " | ";
    // for ( auto & i : contrast_set ) {
    //     cerr << padding_sentence(node_id2label[i], max_l) << " | ";   
    // }

    // cerr << endl;
    for ( auto & i : contrast_set ) {
        // cerr << padding_sentence(node_id2label[i], max_l) << " | ";
        for ( auto & j : contrast_set ) {
            stringstream buf;
            double cnt = 0;
            for ( auto & m : A[i] ) {
                if ( m.fi == j ){
                    cnt = m.se;
                }
            }
            buf << cnt;
            // cerr << padding_sentence(buf.str(), max_l) << " | ";
            fprintf(matrixOut, "%s,", buf.str().c_str());
        }
        fprintf(matrixOut, "\n");
        // cerr << endl;
    }
    // cerr << endl;
}

void loadNodeLabels()
{
    node_id2label.resize(0);
    FILE* in = tryOpen(GRAPH_DIR + "NodeName.csv", "r");
    while (getLine(in)) {
        vector<string> tokens = splitBy(line, ',');
        int id;
        string label;
        fromString(tokens[0], id);
        label = strip(tokens[1]);
        node_id2label.push_back(label);
    }
    fclose(in);

    cerr << "# of total nodes = " << node_id2label.size() << endl;
}

void loadEdgeList(){
    graph_edges.resize(graph_names.size());
    for ( auto & m : graph_edges ) {
        m.resize(node_id2label.size());
    }
    int count = 0;
    FILE* in = tryOpen(GRAPH_DIR + "GraphData.csv", "r");
    while ( getLine(in)) {
        vector<string> tokens = splitBy(line, ',');
        int graph_id, u, v;
        double w;
        fromString(tokens[0], graph_id);
        fromString(tokens[1], u);
        fromString(tokens[2], v);
        fromString(tokens[3], w);
        // w = (int)(w / 100.0);
        if ( u == v ) continue;
        if ( w == 0 ) continue;
        if ( LOG_EDGE ) w = 1.0 + log(w);
        graph_edges[graph_id][u].push_back(make_pair(v,w));
        graph_edges[graph_id][v].push_back(make_pair(u,w));
        count += 1;
    }
    fclose(in);
    cerr << "# of edges in total = " << count << endl;
}

void loadEdges()
{
    graph_names.resize(0);
    FILE* in = tryOpen(GRAPH_DIR + "GraphName.csv", "r");
    while (getLine(in)) {
        vector<string> tokens = splitBy(line, ',');
        int id;
        string name;
        fromString(tokens[0], id);
        name = tokens[1];
        graph_names.push_back(name);
    }
    fclose(in);
    cerr << "# of graphs = " << graph_names.size() << endl;

    loadEdgeList();
}

void loadGraphs()
{
    srand(1);
    loadNodeLabels();
    loadEdges();
}

void search(set<int> & c, vector<vector<pair<int, double> > > & e, vector<int> & v, int cur, int max_d){
    queue<pair<int,int> > q;
    q.push(mp(cur,0));
    while (!q.empty() ) {
        auto n = q.front();
        q.pop();
        if ( n.se > max_d ) continue;
        c.insert(n.fi);
        if ( v[n.fi] ) {
            continue;
        }
        v[n.fi] = 1;
        for ( auto & m : e[n.fi] ) {
            q.push(mp(m.fi,n.se + 1 ));; 
        }
    }
}


#endif
