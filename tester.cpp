#include "./sat-solve/cnf.cpp"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include <sstream>  

#include "./cryptominisat/build/include/cryptominisat5/cryptominisat.h"
#include <assert.h>

using namespace CMSat;

using namespace std;

class TimeProfiler
{
    
private:
    string task_name;
    chrono::high_resolution_clock::time_point startingTime;
    
public:
    TimeProfiler(const string &name) : task_name(name)
    {
        startingTime = chrono::high_resolution_clock::now();
    }

    ~TimeProfiler()
    {
        auto time = chrono::high_resolution_clock::now();
        cerr << task_name << " trval " << (chrono::duration_cast<chrono::nanoseconds>(time-startingTime).count() / 1000000.0) << "ms" << endl;
    }
};


vector<int> line_to_ints(const string& input_line)
{
    std::stringstream stream(input_line);
    
    vector<int> cisla;
    
    int x;
    while(stream >> x) cisla.push_back(x);
    
    return cisla;
}

bool sat_solve(const string& input, vector<lbool>& ohodnotenie)
{
    std::stringstream inp_stream (input);
    SATSolver solver;
    string format1, format2;
    int clauses_count, variable_count;
    
    solver.set_num_threads(4);
    
    inp_stream >> format1 >> format2 >> variable_count >> clauses_count;
    
    solver.new_vars(variable_count);
    
    for(int i=0;i<clauses_count;++i)
    {
        vector<Lit> clause;
        
        int x;
        while(inp_stream >> x && x != 0)
        {
            if(x > 0) clause.push_back(Lit(x-1, false));
            else clause.push_back(Lit(-x-1, true));
        }
        
        solver.add_clause(clause);
    }
    
    lbool ret = solver.solve();
    
    if(ret == l_True)
    {
        cout << "Riesenie existuje:" << endl;
        
        ohodnotenie.resize(variable_count);
        
        for(int i=0;i<variable_count;++i)
            ohodnotenie[i] = solver.get_model()[i];
        
        return true;
    }
    else
    {
        cout << "Riesenie neexistuje!" << endl;
        return false;
    }
}

bool over(const Graph& G, const vector<lbool>& ohodnotenie, int k)
{
    std::map<std::pair<int, int>, int> UM;

    int n = G.size(), m = 0;
    
    for(size_t i=0;i<G.size();++i)
        for(const int v : G[i]) if( UM.find({i, v}) == UM.end() )
        {
            UM[{i, v}] = UM[{v, i}] = m;
            ++m;
        }
    
    for(int i=0;i<m;++i)
    {
        int pocet_ofarbeni = 0;
        
        for(int j=0;j<k;++j)
            if( ohodnotenie[i*k+j] == l_True)
                pocet_ofarbeni++;
        
        if(pocet_ofarbeni != 1)
        {
            cerr << "Zly pocet ofarbeni.\n";
            return false;
        }
    }
    
    for(int f=0;f<n;++f)
        for(size_t i=0;i<G[f].size();++i)
            for(size_t j=i+1;j<G[f].size();++j)
            {
                int a, b;
                a = UM[{f, G[f][i]}];
                b = UM[{f, G[f][j]}];
                
                for(int l=0;l<k;++l) if(ohodnotenie[a*k+l] == l_True && ohodnotenie[b*k+l] == l_True)
                {
                    cerr << "Dve hrany ofarbene rovnako.\n";
                    return false;
                }
            }
            
    cout << "Overenie prebehlo uspesne!\n";
    return true;
}

int cc;

bool zafarbi(const vector<vector<int> >& s_G, const vector<pair<int, int> >& to_nodes, const int k, vector<int>& ofarbene, int dalsi)
{
    if(dalsi == to_nodes.size()) return true;
    
    if(++cc && cc > 100000000)
    {
        if(cc == 100000001)cout << "Vypocet trval pridlho\n";
        return false;
    }
    
    for(int i=0;i<k;++i)
    {
        ofarbene[dalsi] = i;
        
        int a, b, counter=0;
        a = to_nodes[dalsi].first;
        b = to_nodes[dalsi].second;

        for(int x : s_G[a]) if(ofarbene[x] == i) counter++;
        for(int x : s_G[b]) if(ofarbene[x] == i) counter++;
        
        if( counter == 2 && zafarbi(s_G, to_nodes, k, ofarbene, dalsi+1) ) return true;
        
        ofarbene[dalsi] = -1;
    }
    
    return false;
}

bool je_zafarbitelny(const Graph& G, int k)
{
    cc = 0;
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
    
    if( zafarbi(s_G, to_nodes, k, ofarbene, 0) ) return true;
    return false;
}

int main()
{
    int batch_size;
    cin >> batch_size;
    
    for(int f=0;f<batch_size;++f)
    {
        int id, n;
        cin >> id >> n;
        
        cout << id << endl;
        
        vector<vector<int> > G(n);
        
        string odpad; getline(cin, odpad);
        
        for(int i=0;i<n;++i)
        {
            string input_line;
            getline(cin, input_line);
            auto susedia = line_to_ints(input_line);
            
            for(const int x : susedia) G[i].push_back(x);
        }
        
        // vypocet
        
        bool vysledok = false;
        
        {
            TimeProfiler x("Vypocet " + to_string(f));
            
            string sat_formula = cnf_colouring(G, 3);
            
            vector<lbool> ohodnotenie;
            if(sat_solve(sat_formula, ohodnotenie))
            {
                vysledok = true;
                over(G, ohodnotenie, 3);
            }
        }
        
        {
            TimeProfiler x("Vypocet " + to_string(f));
            
            if (je_zafarbitelny(G, 3))
            {
                cout << "Kontrola : Je zafarbitelny.";
                if(vysledok == false)
                {
                    cout << "Red flag!\n";
                    char xx;
                    cin >> xx;
                    return 0;
                }
            }
            else cout << "Kontrola : Nie je zafarbitelny.";
        }
        
    }
    
    return 0;
}
