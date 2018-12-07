#include <bits/stdc++.h>

#define ff first
#define ss second

using namespace std;

using ll = long long;
using pll = pair<long long, long long>;

int generuj(int a, int b)
{
    int x = rand()%(b-a);
    x += a;
    return x;
}

void vypis_maticu (const int n, const vector<vector<bool> >& G)
{
    for(int i=0;i<n;++i)
    {
        vector<int> susedia;
        for(int j=0;j<n;++j) if(G[i][j]) susedia.push_back(j);
                
        for(int j=0;j<susedia.size();++j)
        {
            if(j!=0) cout << " ";
            cout << susedia[j];
        }
        cout << "\n";
    }
}

int main()
{
    srand(time(NULL));
    
    int typ;
    cin >> typ;
    
    int pocet_grafov = generuj(1, 20);
    cout << pocet_grafov << "\n";
    
    if(typ == 1)
    {
        for(int i=0;i<pocet_grafov;++i)
        {
            int n = generuj(4, 21);
            
            vector<vector<bool> > G(n, vector<bool>(n, false));
            
            for(int j=0;j<n;++j)
            {
                int pripoj = generuj(0, n);
                if(pripoj == j)
                {
                    --j;
                    continue;
                }
                
                G[j][pripoj] = true;
            }
            
            for(int j=0;j<((n*n)/4);++j)
            {
                int a, b;
                
                a = generuj(0, n);
                b = generuj(0, n);
                
                if(G[a][b]) 
                {
                    --j;
                    continue;
                }
                
                G[a][b] = G[b][a] = true;
            }
            
            cout << "\n" << i+1 << "\n";
            cout << n << "\n";
            
            vypis_maticu(n, G);
        }
    }
    
    return 0;
}
