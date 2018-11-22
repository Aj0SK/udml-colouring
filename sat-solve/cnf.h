#ifndef CNF_H
#define CNF_H

#include <string>
#include <vector>

typedef std::vector<std::vector<int>> Graph;

std::string cnf_colouring(Graph G, int k);

#endif
