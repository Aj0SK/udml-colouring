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

bool sat_solve(const string& input)
{
    std::stringstream inp_stream (input);
    
    SATSolver solver;
    solver.set_num_threads(4);
    
    string format1, format2;
    int clauses_count, variable_count;
    
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
        cout << "Riesenie existuje a je:" << endl;
        
        //for(int i=0;i<variable_count;++i)
        //cout << i << " is " << solver.get_model()[i] << endl;
        
        return true;
    }
    else
    {
        cout << "Riesenie neexistuje!" << endl;
        return false;
    }
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
        
        // vypocet
        
        {
            //TimeProfiler x("Vypocet" + to_string(f));
            
            string sat_formula = cnf_colouring(G, 3);
            //sat_solve(sat_formula);
        }
        
        return 0;
    }
    
    return 0;
}
