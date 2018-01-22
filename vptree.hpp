#include<bits/stdc++.h>

using namespace std;

class vp_tree{
public:
    class node{
    public:
        int index;
        double mu;
        node* inside;
        node* outside;
        node():index(0), mu(0.), inside(nullptr), outside(nullptr){}
        ~node(){
            delete inside;
            delete outside;
        }
    };

    int size;

    vp_tree(vector<string> v):items(v){
        size = items.size();
        root = _build(0, size);
    }

    vector<string> search(string str, int k){
        vector<string> vec;
        _search(root, str, k, vec);
        return vec;
    }

    void print(){
        _print(root, 0);
    }

    static int levenshtein_distance(string str1, string str2){
        int m = str1.length(), n = str2.length();
        int dn[m+1][n+1];

        for(int i=0; i<=m; ++i){
            for(int j=0; j<=n; ++j){
                if(i==0)
                    dn[0][j] = j;
                else if(j==0)
                    dn[i][0] = i;
                else if(str1[i-1] == str2[j-1])
                    dn[i][j] = dn[i-1][j-1];
                else
                    dn[i][j] = 1 + min(
                        dn[i-1][j],
                        min(dn[i][j-1],
                        dn[i-1][j-1])
                        );
            }
        }

        return dn[m][n];
    }

    inline vector<string>::iterator begin() noexcept { return items.begin(); }
    inline vector<string>::const_iterator cbegin() const noexcept { return items.cbegin(); }
    inline vector<string>::iterator end() noexcept { return items.end(); }
    inline vector<string>::const_iterator cend() const noexcept { return items.end(); }
    
private:
    node* root;
    vector<string> items;

    node* _build(int lower, int upper){
        if(lower == upper)
            return nullptr;
        
        node* n = new node();
        n->index = lower;
        
        if(upper-lower <= 1)
            return n;

        int median = (lower+upper)/2;
        vector<string>::iterator beg = items.begin();
        nth_element(beg+lower+1, beg+median, beg+upper,
            [&](const string str1, const string str2) -> bool 
            {return 
                levenshtein_distance(items[lower], str1) < levenshtein_distance(items[lower], str2);
            });
        
        n->index = lower;
        n->mu = levenshtein_distance(items[lower], items[median]);
        n->inside = _build(lower+1, median);
        n->outside = _build(median, upper);
        
        return n;
    }

    void _search(node* n, string str, int k, vector<string>& vec){
        if(n == nullptr)
            return;

        string r = items[n->index];
        int d = levenshtein_distance(r, str);

        if(d <= k)
            vec.push_back(r);

        if(d > k + n->mu)
            _search(n->outside, str, k, vec);
        else if(d < n->mu - k)
            _search(n->inside, str, k, vec);
        else{
            _search(n->inside, str, k, vec);
            _search(n->outside, str, k, vec);
        }
    }

    void _print(node* r, const int depth){
        if(r == nullptr)
            return;
        for(int i=0; i<depth; ++i)
            cout << " ";
        cout << items[r->index] << endl;
        _print(r->inside, depth+1);
        _print(r->outside, depth+1);
    }
};