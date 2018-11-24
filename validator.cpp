#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

string err(const string& message)
{
    cerr << message << endl;
    exit(1);
}

vector<int> line_to_ints(const string& input_line)
{
    std::stringstream stream(input_line);
    vector<int> cisla;
    
    int x;
    while(stream >> x) cisla.push_back(x);
    
    return cisla;
}

int main()
{
    int number_of_graphs;
    if(!(cin >> number_of_graphs) ) err("Chyba pocet grafov!");
    
    for(int f=0;f<number_of_graphs;++f)
    {
        int id, n;
        if(!(cin >> id >> n)) err("Chyba id a pocet vrcholov!");
        
        vector<vector<int> > G(n);
        
        string odpad; getline(cin, odpad);
        
        for(int i=0;i<n;++i)
        {
            string input_line;
            getline(cin, input_line);
            auto susedia = line_to_ints(input_line);
            
            for(const int x : susedia) G[i].push_back(x);
        }
        
        map<pair<int, int>, bool> M;
        
        for(int i=0;i<n;++i)
            for(int j=0;j<G[i].size();++j)
                M[{i, G[i][j]}] = true;
            
        
        for(auto x : M) if( M.find({x.first.second, x.first.first}) == M.end()) err("Hrana nie je pridana obojsmerne!");
    }
    
    
    
    return 0;
}
