#include <iostream>

using namespace std;

int main()
{
    cout << "\\begin{center}\n\\begin{tabular}{| c | c | c | c | c |} \n \\hline ID & SAT & Backtrack & Vysledok & SAT formula \\\\ \n \\hline\\hline \n";
    
    int id;
    while(cin >> id)
    {
        string junk;
        int cas1, cas2, existencia, cas3;
        
        cin >> junk >> cas1 >> junk;
        cin >> junk >> cas2 >> junk;
        cin >> existencia;
        cin >> junk >> cas3 >> junk;
        
        cout << id << " & " << cas1 << " & " << cas2 << " & " << ((existencia == 0)?"Existuje":"NEexistuje") << " & " << cas3 << " \\\\ " << endl;
        cout << "\\hline \n";
    }
    
    cout << "\\end{tabular} \n \\end{center}";
    return 0;
}
