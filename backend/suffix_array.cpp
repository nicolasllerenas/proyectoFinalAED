#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class SuffixArray {
private:
    vector<int> SA;
    string text;

    // Construir SA con método naive
    void build() {
        vector<pair<int, string>> suffixes;
        int n = text.size();
        for (int i = 0; i < n; i++) {
            suffixes.push_back({i, text.substr(i)});
        }
        
        sort(suffixes.begin(), suffixes.end(), 
            [](const pair<int, string>& a, const pair<int, string>& b) {
                return a.second < b.second;
            });
        
        SA.clear();
        for (const auto& p : suffixes) {
            SA.push_back(p.first);
        }
    }

public:
    SuffixArray(const string& str) : text(str) {
        build();
    }

    void print() {
        cout << "\nSuffix Array para \"" << text << "\":" << endl;
        cout << "Indice\tPosicion\tSufijo" << endl;
        for (int i = 0; i < SA.size(); i++) {
            cout << i << "\t" << SA[i] << "\t\t" << text.substr(SA[i]) << endl;
        }
    }

    void searchPattern(const string& pattern) {
        int n = SA.size();
        int m = pattern.size();
        int l = 0, r = n - 1;
        int found = -1;

        while (l <= r) {
            int mid = (l + r) / 2;
            string suffix = text.substr(SA[mid], m);
            
            if (pattern == suffix) {
                found = SA[mid];
                break;
            }
            
            if (pattern < suffix) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }

        if (found != -1) {
            cout << "Patron \"" << pattern << "\" encontrado en posicion: " << found << endl;
            cout << "Texto: " << text << endl;
            cout << "        ";
            for (int i = 0; i < found; i++) cout << " ";
            cout << "^";
            if (pattern.size() > 1) {
                for (int i = 0; i < pattern.size()-1; i++) cout << "~";
            }
            cout << endl;
        } else {
            cout << "Patron \"" << pattern << "\" no encontrado." << endl;
        }
    }
};

int main() {
    string inputText = "banana$";
    SuffixArray sa(inputText);
    int choice;

    while (true) {
        cout << "\nTexto actual: " << inputText << endl;
        cout << "\nMenu Suffix Array:" << endl;
        cout << "1. Cambiar texto" << endl;
        cout << "2. Mostrar Suffix Array" << endl;
        cout << "3. Buscar patron" << endl;
        cout << "4. Salir" << endl;
        cout << "Elija una opcion: ";
        cin >> choice;
        cin.ignore(); // Para consumir el salto de línea

        switch (choice) {
            case 1: {
                cout << "Ingrese el nuevo texto (debe terminar con $): ";
                getline(cin, inputText);
                if (inputText.back() != '$') {
                    inputText += '$';
                }
                sa = SuffixArray(inputText);
                break;
            }
            case 2: {
                sa.print();
                break;
            }
            case 3: {
                string pattern;
                cout << "Ingrese el patron a buscar: ";
                getline(cin, pattern);
                sa.searchPattern(pattern);
                break;
            }
            case 4: {
                cout << "Saliendo..." << endl;
                return 0;
            }
            default: {
                cout << "Opcion invalida. Intente de nuevo." << endl;
                break;
            }
        }
    }
    return 0;
}