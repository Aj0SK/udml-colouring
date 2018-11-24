#include "cnf.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

std::string cnf_colouring(Graph G, int k)
{
    std::map<std::pair<int, int>, int> UM;
    std::vector<std::pair<int, int> > help;
    
    std::vector<std::vector<int> > clauses;
    
    int n = G.size(), m = 0;
    
    for(size_t i=0;i<G.size();++i)
    {
        for(size_t j=0;j<G[i].size();++j) if( UM.find({i, j}) == UM.end() )
        {
            UM[{i, j}] = UM[{j, i}] = m;
            help.push_back({i, j});
            ++m;
        }
    }
    
    for(int i=0;i<m;++i)
    {
        std::vector<int>clause;
        for(int j = i*k;j<((i+1)*k);++j) clause.push_back(j+1);
        clauses.push_back(clause);
    }
    
    // p->q = !p or q
    for(int i=0;i<m;++i)
    {
        for(int nn = i*k;nn<((i+1)*k);++nn)
        {
            for(int j = nn+1;j<((i+1)*k);++j)
            {
                std::vector<int>clause;
                
                clause.push_back(-(nn+1));
                clause.push_back(-(j+1));
                
                clauses.push_back(clause);
            }
        }
    }
    
    for(int f=0;f<n;++f)
    {
        for(int i=0;i<G[f].size();++i)
            for(int j=i+1;j<G[f].size();++j)
                for(int l=0;l<k;++l)
                {
                    std::vector<int>clause;
                
                    int a, b;
                    
                    a = UM[{f, G[f][i]}];
                    b = UM[{f, G[f][j]}];
                    
                    clause.push_back(-(a*k+l));
                    clause.push_back(-(b*k+l));
                
                    clauses.push_back(clause);
                }
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
    
    std::cout << out << std::endl;
    
    return out;
}
