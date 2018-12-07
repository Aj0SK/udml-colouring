#include "cnf.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <chrono>

using namespace std;

std::string cnf_colouring(Graph G, int k)
{
    std::map<std::pair<int, int>, int> UM;
    std::vector<std::vector<int> > clauses;
    
    int n = G.size(), m = 0;
    
    for(size_t i=0;i<G.size();++i)
        for(const int v : G[i])
            if( UM.find({i, v}) == UM.end() )
            {
                UM[{i, v}] = UM[{v, i}] = m;
                ++m;
            }
    
    for(int i=0;i<m;++i)
    {
        std::vector<int>clause;
        
        for(int j = i*k;j<((i+1)*k);++j)
            clause.push_back(j+1);
        
        clauses.push_back(clause);
    }
    
    // p->q = !p or q
    for(int f=0;f<m;++f)
        for(int i = f*k; i<(f+1)*k; ++i)
            for(int j = i+1; j<(f+1)*k; ++j)
            {
                std::vector<int>clause;
                
                clause.push_back(-(i+1));
                clause.push_back(-(j+1));
                
                clauses.push_back(clause);
            }
    
    for(int f=0;f<n;++f)
        for(int i=0;i<G[f].size();++i)
            for(int j=i+1;j<G[f].size();++j)
                for(int l=0;l<k;++l)
                {
                    std::vector<int>clause;
                
                    int a, b;
                    
                    a = UM[{f, G[f][i]}];
                    b = UM[{f, G[f][j]}];
                    
                    clause.push_back(-(a*k+l+1));
                    clause.push_back(-(b*k+l+1));
                
                    clauses.push_back(clause);
                }
    
    std::string out = "p cnf " + std::to_string(m*k) + " " + std::to_string(clauses.size()) + "\n";
    
    for(const auto& klauzula : clauses)
    {
        for(size_t i=0;i<klauzula.size();++i)
        {
            if(i != 0) out += " ";
            out += std::to_string(klauzula[i]);
        }
        out += " 0\n";
    }
    
    return out;
}

class backtrack_solver
{
private:
    Graph G;
    int K;
    double TIMEOUT;
    
    chrono::high_resolution_clock::time_point start, end;
    
    bool zafarbitelnost;
    
public:

    double total_time()
    {
        int x = chrono::duration_cast<chrono::milliseconds>(end-start).count();
        return x;
    }
    
    void print_outcome()
    {
        if(total_time() > TIMEOUT) cout << "Backtrack skoncil timeoutom po " << TIMEOUT << " sekundach\n";
        else if(zafarbitelnost) cout << "Graf JE backtrackom zafarbitelny.\n";
        else cout << "Graf NIE JE backtrackom zafarbitelny.\n";
    }
    
    backtrack_solver(const Graph& g, int k, double timeout)
    {
        G = g;
        K = k;
        TIMEOUT = timeout;
    }
    
    bool zafarbi(const vector<vector<int> >& s_G, const vector<pair<int, int> >& to_nodes, vector<int>& ofarbene, int dalsi)
    {
        if(dalsi == to_nodes.size()) return true;
    
        auto isend = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = isend-start;
        
        if(diff.count() > TIMEOUT) return false;
        
        for(int i=0;i<K;++i)
        {
            ofarbene[dalsi] = i;
        
            int a, b, counter=0;
            a = to_nodes[dalsi].first;
            b = to_nodes[dalsi].second;

            for(int x : s_G[a]) if(ofarbene[x] == i) counter++;
            for(int x : s_G[b]) if(ofarbene[x] == i) counter++;
        
            if( counter == 2 && zafarbi(s_G, to_nodes, ofarbene, dalsi+1) ) return true;
        
            ofarbene[dalsi] = -1;
        }
    
        return false;
    }

    bool je_zafarbitelny()
    {
        int m = 0;
    
        std::map<std::pair<int, int>, int> UM;
        vector< pair<int, int> > to_nodes;
    
        for(size_t i=0;i<G.size();++i)
            for(const int v : G[i]) if( UM.find({i, v}) == UM.end() )
            {
                UM[{i, v}] = UM[{v, i}] = m;
                to_nodes.push_back({v, i});
                ++m;
            }
        
        vector<int> ofarbene(m, -1);

        vector<vector<int> > s_G(G.size());
     
        for(size_t i=0;i<G.size();++i)
            for(const int v : G[i])
                s_G[i].push_back(UM[{v, i}]);
    
        start = chrono::high_resolution_clock::now();
        
        zafarbitelnost = false;
        if( zafarbi(s_G, to_nodes, ofarbene, 0) ) zafarbitelnost = true;
            
        end = std::chrono::high_resolution_clock::now();
        return (zafarbitelnost);
    }
};
