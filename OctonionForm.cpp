#include "OctonionForm.h"

template<size_t N> array<int,N> conj(array<int,N> x)
    {
        for (int i=1; i < N;  i++) { x[i] *= -1; }
        return x;
    }

template<typename T, size_t N> array<T,N> CDMult(array<T,N> x, array<T,N> y)
    {
        array<int,N/2> a, b, c, d;
        int m = N / 2;
        for (int i=0; i < m;  i++) {
            a[i] = x[i];
            b[i] = x[i+m];
            c[i] = y[i];
            d[i] = y[i+m];
        }
        if (N > 1) {
            array<T,N> z;
            array<T,N/2> z1 = CDMult(a, c) - CDMult(conj(d), b);
            array<T,N/2> z2 = CDMult(d, a) + CDMult(b, conj(c));
            for (int i=0; i < m;  i++) {
                z[i] = z1[i];
                z[i+m] = z2[i];
            }
            return z;
        }
        else if (N == 1) {
            array<T,N> z;
            z[0] = x[0] * y[0];
            return z;
        }
    }


// multiplication table of positive basis elements e_i e_j, returning k such that e_k = e_i e_j up to the idea that -e_k is represented as e_{-k})
unordered_map<pair<int,int>, int, hash_pair> getHashList()
    {
        for (int i=0; i < algebraDim;  i++)
        {
            for (int j=0; j < algebraDim;  j++)
            {
            array<int,algebraDim> x = {};
            array<int,algebraDim> y = {};
            array<int,algebraDim> z = {};
            x[i] = y[j] = 1;
            z = CDMult(x,y);
            multTable[make_pair(i+1,j+1)] = firstNonzeroIndex(z);
            //shifting each by 1 moves us from 0-indexing to 1-indexing, important so that -e_1 = e_{-1} since -0 = 0. Note the shifting on RHS of last line happens in firstNonzeroIndex definition in header to account for sign
            }
        }
        return multTable;
    }

int basisMult(int x, int y) {
    int z = multTable[make_pair(abs(x),abs(y))];
    if ((x < 0 && y > 0) || (x > 0 && y < 0)) {return -z;} // negative product
    else {return z;}
}

int octonionForm(array<int,4> x) {
    int a = x[0];
    int b = x[1];
    int c = x[2];
    int d = x[3];
    int z1 = basisMult(basisMult(a, basisMult(b,c)),d);
    int w1 = basisMult(a,basisMult(basisMult(c,b),d));
    int z2 = basisMult(basisMult(d, basisMult(b,c)),a);
    int w2 = basisMult(d,basisMult(basisMult(c,b),a));
    int z3 = basisMult(basisMult(d,c), basisMult(b,a));
    int w3 = basisMult(basisMult(d,b), basisMult(c,a));
    int z4 = basisMult(basisMult(a,c), basisMult(b,d));
    int w4 = basisMult(basisMult(a,b), basisMult(c,d));
    return (z1 - w1 + z2 - w2 + z3 - w3 + z4 - w4);
    // this is technically returning the sum of the indices, e.g. e_1 + e_3 becomes 1+3 = 4. Since e_{-k} = -e_k, I'm pretty sure the sum of the vectors is zero iff the indices sum to zero since this holds for each index k. This just cuts out recasting to vectors and then doing 8 vector additions, instead doing 8 int additions.
}

bool octonionFormVanishes() {
    for (int x1=1; x1 <= algebraDim;  x1++) {
    for (int x2=1; x2 <= algebraDim;  x2++) {
    for (int x3=1; x3 <= algebraDim;  x3++) {
    for (int x4=1; x4 <= algebraDim;  x4++) {
        if (octonionForm({x1,x2,x3,x4}) != 0){
            cout << x1 << ", " << x2 << ", " << x3 << ", " << x4 << endl;
            return false;
        }
    }}} cout << "heartbeat" << endl;}
    cout << "vanishes" << endl;
    return true;
}


int main() {
    unordered_map<pair<int,int>, int, hash_pair> multTable = getHashList();
    steady_clock::time_point begin = steady_clock::now();
    octonionFormVanishes();
    steady_clock::time_point end = steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<microseconds>(end - begin).count() << "[µs]" << std::endl;
}