#include <bits/stdc++.h>

using namespace std;

const int PRIMALITY_TEST_ACCURACY = 20;
long long cp, cq;

long long fast_exp(long long base, long long exponent, long long mod){
    if(!exponent)
        return 1;
    if(exponent == 1)
        return base;
    long long ret = fast_exp(base * base % mod, exponent >> 1, mod);
    if(exponent & 1)
        ret = ret * base % mod;
    return ret;
}

bool pass_test(int r, int p){
    int a = rand() % (p - 4) + 2;
    int rem = fast_exp(a, r, p);
    if(rem == 1 || rem == p - 1)
        return true;
    while(r != p - 1){
        rem = 1ll * rem * rem % p;
        r <<= 1;
        if(rem == 1)
            return false;
        if(rem == p - 1)
            return true;
    }
    return false;
}

bool miller_rabin_test(int p){
    if(p == 2 || p == 3)
        return true;
    int r = p - 1;
    int u = 0;
    while((r & 1) == 0){
        ++u;
        r >>= 1;
    }
    for(int i = 0; i < PRIMALITY_TEST_ACCURACY; i++)
        if(!pass_test(r, p))
            return false;
    return true;
}

int get_random_prime(){
    while(true){
        int p = rand() % 10000 * 2 + 1;
        if(miller_rabin_test(p))
            return p;
    }
}

long long gcd(long long a, long long b){
    return b ? gcd(b, a % b) : a;
}

void EEA(long long a, long long b, long long &x, long long &y){
    if(a == 0){
        x = 0;
        y = 1;
        return;
    }
    long long x1, y1;
    EEA(b % a, a, x1, y1);
    x = y1 - b / a * x1;
    y = x1;
}


long long getDecrypted(long long y, long long &p, long long &q, long long &n, long long &phi, long long &e, long long &d){
    long long yp = y % p;
    long long yq = y % q;
    long long dp = d % (p - 1);
    long long dq = d % (q - 1);
    long long xp = fast_exp(yp, dp, p);
    long long xq = fast_exp(yq, dq, q);
    long long x = (1ll * q * cp * xp + 1ll * p * cq * xq) % n;
    return x;
}

long long getEncrypted(long long x, long long &p, long long &q, long long &n, long long &phi, long long &e, long long &d){
    long long xp = x % p;
    long long xq = x % q;
    long long ep = e % (p - 1);
    long long eq = e % (q - 1);
    long long yp = fast_exp(xp, ep, p);
    long long yq = fast_exp(xq, eq, q);
    long long y = (1ll * q * cp * yp + 1ll * p * cq * yq) % n;
    return y;
}

void Encrypt(long long &p, long long &q, long long &n, long long &phi, long long &e, long long &d){
    cin.ignore();
    string x = "";
    do{
        printf("\033c");
        puts("Enter text to be Encrypted:");
        getline(cin, x);
    }while(x.empty());
    vector<long long> v;
    for(char c: x)
        v.push_back(c);
    for(long long &c: v)
        c = getEncrypted(c, p, q, n, phi, e, d);
    puts("Your Encrypted text is:");
    for(int i = 0; i < v.size(); i++)
        printf("%lld%c", v[i], "#\n"[i == v.size() - 1]);
}

void Decrypt(long long &p, long long &q, long long &n, long long &phi, long long &e, long long &d){
    cin.ignore();
    string x = "";
    do{
        printf("\033c");
        puts("Enter text to be Decrypted:");
        getline(cin, x);
    }while(x.empty());
    vector<long long> v;
    v.push_back(0);
    for(char c: x){
        if(c == '#')
            v.push_back(0);
        else
            v.back() = v.back() * 10 + c - '0';
    }
    for(long long &c: v)
        c = getDecrypted(c, p, q, n, phi, e, d);
    puts("Your Decrypted text is:");
    for(int i = 0; i < v.size(); i++)
        printf("%c", v[i]);
    puts("");
}


void RUN(long long &p, long long &q, long long &n, long long &phi, long long &e, long long &d){
    while(true){
        int operation;
        do{
            printf("\033c");
            puts("Welcome to the RSA algorithm, What do you want to do?");
            puts("1 => To Encrypt");
            puts("2 => To Decrypt");
            puts("3 => To Exit");
            scanf("%d", &operation);
        }while(operation < 1 || operation > 3);
        if(operation == 1)
            Encrypt(p, q, n, phi, e, d);
        else if(operation == 2)
            Decrypt(p, q, n, phi, e, d);
        else
            return;
        puts("\nPress any key to continue...");
        cin.get();
    }
}

long long inv(long long a, long long m){
    long long x, y;
    EEA(a, m, x, y);
    return (x % m + m) % m;
}

int main()
{
    long long p, q;
    p = get_random_prime();
    q = p;
    while(q == p)
        q = get_random_prime();
    long long n = 1ll * p * q;
    long long phi = (p - 1ll) * (q - 1ll);
    long long e;
    do{
        e = rand() % (phi - 2) + 1;
    }while(e == 1 || gcd(e, phi) > 1);
    long long d;
    d = inv(e, phi);
    cp = inv(q, p);
    cq = inv(p, q);
    RUN(p, q, n, phi, e, d);
    return 0;
}
