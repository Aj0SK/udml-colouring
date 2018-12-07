#include "./sat-solve/cnf.cpp"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include <sstream>  

#include "./cryptominisat/build/include/cryptominisat5/cryptominisat.h"
#include <assert.h>

#include <thread>
#include <atomic>
#include <unistd.h>

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
        cout << task_name << ": " << (chrono::duration_cast<chrono::milliseconds>(time-startingTime).count()) << " ms" << endl;
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
        ohodnotenie.resize(variable_count);
        
        for(int i=0;i<variable_count;++i)
            ohodnotenie[i] = solver.get_model()[i];
        
        return true;
    }
    
    return false;
}

vector<vector<int> > ofarbenie(const Graph& G, int k, const vector<lbool>& ohodnotenie)
{
    std::map<std::pair<int, int>, int> UM;
    vector<pair<int, int> > back;
    
    int m = 0;
    
    for(size_t i=0;i<G.size();++i)
        for(const int v : G[i]) if( UM.find({i, v}) == UM.end() )
        {
            UM[{i, v}] = UM[{v, i}] = m;
            back.push_back({i, v});
            ++m;
        }
    
    vector<vector<int> > ofarbenie;
    
    for(int i=0;i<m;++i)
        for(int j=0;j<k;++j)
            if( ohodnotenie[i*k+j] == l_True)
            {
                ofarbenie.push_back({back[i].first, back[i].second, j});
            }
    
    return ofarbenie;
}

bool over(const Graph& G, const int k, const vector<vector<int> >& ofarbenie)
{
    std::map<std::pair<int, int>, int> farba_hrany;
    
    for(int i=0;i<ofarbenie.size();++i)
    {
        int a, b, farba;
        a = ofarbenie[i][0];
        b = ofarbenie[i][1];
        farba = ofarbenie[i][2];
        
        if(farba_hrany.find({a, b}) != farba_hrany.end()) return false;
        farba_hrany[{a, b}] = farba;
        farba_hrany[{b, a}] = farba;
    }
    
    for(size_t i=0;i<G.size();++i)
        for(size_t j=0;j<G[i].size();++j)
        {
            int a = i, b = G[i][j];
            int farba = farba_hrany[{a, b}];
            
            int pocet = 0;
            
            for(size_t m : G[a]) if(farba_hrany[{m, a}] == farba) pocet++; 
            for(size_t m : G[b]) if(farba_hrany[{m, b}] == farba) pocet++;
            
            if(pocet != 2) return false;
        }
    
    return true;
}

int main()
{
    int batch_size;
    cin >> batch_size;
    
    for(int f=0;f<batch_size;++f)
    {
        int id, n;
        cin >> id >> n;
        
        vector<vector<int> > G(n);
        
        string odpad; getline(cin, odpad);
        
        for(int i=0;i<n;++i)
        {
            string input_line;
            getline(cin, input_line);
            auto susedia = line_to_ints(input_line);
            
            for(const int x : susedia) G[i].push_back(x);
        }
        
        cout << id << endl;
        
        // vypocet
        
        bool satisfable = false;
        string sat_formula;
        vector<lbool> ohodnotenie;
        
        {
            TimeProfiler x("SAT");
            {
                TimeProfiler spe("Formula");
                sat_formula = cnf_colouring(G, 3);
            }
            satisfable = sat_solve(sat_formula, ohodnotenie);
        }
        
        if(satisfable)
        {
            cout << "1\n";
            auto kk = ofarbenie(G, 3, ohodnotenie);
            if(!over(G, 3, kk))
            {
                cout << "Overenie prebehlo NEuspesne\n";
                return 0;
            }
        }
        else cout << "0\n";
        
        
        backtrack_solver BS(G, 3, 3.0);
        
        bool zaf = BS.je_zafarbitelny();
            
        if(zaf && !satisfable)
        {
            cout << "Chyba\n";
            return 0;
        }
        //BS.print_outcome();
        cout << "Backtrack: " << BS.total_time() << " ms\n";
        
    }
    
    return 0;
}
