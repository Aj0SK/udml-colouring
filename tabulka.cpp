#include <iostream>
#include <vector>

using namespace std;

int main()
{
    cout << "\\begin{center}\n\\begin{tabular}{| c | c | c | c | c |} \n \\hline ID & SAT & Backtrack & Vysledok & SAT formula \\\\ \n \\hline\\hline \n";
    
    int batch_size, opakuj;
    cin >> batch_size >> opakuj;
    
    for(int i=0;i<batch_size;++i)
    {
        vector<int> kum(5, 0);
        
        for(int j=0;j<opakuj;++j)
        {
            string junk;
            int id, cas1, casF, existencia, casB;
        
            cin >> id;
            cin >> junk >> cas1 >> junk;
            cin >> junk >> casF >> junk;
            cin >> existencia;
            cin >> junk >> casB >> junk;
            
            kum[0] += id;
            kum[1] += cas1;
            kum[2] += casF;
            kum[3] += existencia;
            kum[4] += casF;
        }
        
        cout << kum[0]/5 << " & " << kum[1]/5 << " & " << kum[2]/5 << " & " << (((kum[3]/5) == 1)? "Existuje" : "NEexistuje") << " & " << kum[4]/5 << " \\\\ " << endl;
        cout << "\\hline \n";
    }
    
    cout << "\\end{tabular} \n \\end{center}";
    return 0;
}
