#include <iostream>
#include <cstring>
#include <cstdlib>

class SuffixArray {
private:
    int* SA;
    char* text;
    int n;

    // Función de comparación para qsort
    static int cmp(void* context, const void* a, const void* b) {
        char* text = static_cast<char*>(context);
        int i = *(const int*)a;
        int j = *(const int*)b;
        return strcmp(text + i, text + j);
    }

public:
    SuffixArray(const char* str, int len) : n(len) {
        text = new char[n + 1];
        SA = new int[n];
        for (int i = 0; i < n; i++) {
            text[i] = str[i];
            SA[i] = i;
        }
        text[n] = '\0';
        
        qsort_s(
            SA, 
            n, 
            sizeof(int), 
            cmp,
            text
        );
    }

    ~SuffixArray() {
        delete[] SA;
        delete[] text;
    }

    // Búsqueda de patrón
    int search(const char* pattern) {
        int m = strlen(pattern);
        int l = 0, r = n - 1;
        
        while (l <= r) {
            int mid = (l + r) / 2;
            int cmp_res = strncmp(pattern, text + SA[mid], m);
            
            if (cmp_res == 0) return SA[mid];    // Encontrado
            if (cmp_res < 0) r = mid - 1;
            else l = mid + 1;
        }
        return -1;  // No encontrado
    }

    // Obtener SA como string
    char* get_sa_string() {
        char* buffer = new char[n * 12];  // Suficiente espacio
        buffer[0] = '\0';
        
        for (int i = 0; i < n; i++) {
            char num[12];
            sprintf(num, "%d ", SA[i]);
            strcat(buffer, num);
        }
        return buffer;
    }
};

// Interfaz para Python
extern "C" {
    SuffixArray* create_sa(const char* text, int len) {
        return new SuffixArray(text, len);
    }
    
    void free_sa(SuffixArray* sa) {
        delete sa;
    }
    
    char* get_sa(SuffixArray* sa) {
        return sa->get_sa_string();
    }
    
    int search_pattern(SuffixArray* sa, const char* pattern) {
        return sa->search(pattern);
    }
}