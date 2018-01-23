#include<bits/stdc++.h>

using namespace std;

class bk_tree{
public:
    class node{
    public:
        int index;
        unordered_map<int, node*> children;
        node(int i):index(i){}
        ~node(){
            for(const pair<int, node*>&  p : children)
                delete p.second;
        }
        int capacity(){
            int sz = sizeof(index);
            sz += ((sizeof(int)+sizeof(node*))*children.size())+sizeof(children);
            for(pair<const int, node*>& child : children)
                sz += child.second->capacity();
            return sz;
        }
    };

    int size;

    bk_tree(string str):size(1){
        items.push_back(str);
        root = new node(0);
    }

    bk_tree():size(0){
        root = nullptr;
    }

    bk_tree(const vector<string>& v){
        items = v;
        size = v.size();
        _build_from_items(v);
    }

    void insert(string str){
        size++;
        if(deleted.find(str) != deleted.end()){
            deleted.erase(str);
            return;
        }

        int index = items.size();
        items.push_back(str);
        if(!_add_from_items(index)){
            size--;
            items.pop_back();
        }
    }

    void remove(string str){
        if(deleted.find(str) != deleted.end())
            return;
        deleted.insert(str);
        size--;
        if(deleted.size() >= size/2)
            _rebuild();
    }

    vector<string> search(string str, int tol){
        if(size <= 0)
            return vector<string>(0);
        vector<string> vec;
        _search(str, tol, root, vec);
        return vec;
    }

    bool has(string str, int tol=0){
        if(size <= 0)
            return false;
        return _has(str, tol, root);
    }

    void print(node* r = nullptr, node* n = nullptr, int depth=0){
        cout << size << "\n";
        _print(root, root, 0);
    }

    int capacity(){
        int sz = sizeof(size) + sizeof(node*) + sizeof(items) + sizeof(deleted);
        for(string& s : items)
            sz += s.size() * sizeof(char);
        for(const string& s : deleted)
            sz += s.size() * sizeof(char);
        sz += root->capacity();
        return sz;
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
    vector<string> items;
    node* root;
    unordered_set<string> deleted;

    void _build_from_items(const vector<string>& v){
        if(size < 1){
            root = nullptr;
            return;
        }else if(size == 1){
            root = new node(0);
            return;
        }else{
            root = new node(0);
            for(int i=1; i<size; ++i){
                if(!_add_from_items(i)){
                    swap(items[i], items[size-1]);
                    size--;
                    i--;
                }
            }     
        }
    }

    void _rebuild(){
        int i=0, j = items.size()-1;
        while(i <= j){
            if(deleted.find(items[i]) != deleted.end()){
                swap(items[i], items[j--]);
            }else{
                i++;
            }
        }

        items.resize(i);
        deleted.clear();
        delete root;
        size = i;
        _build_from_items(items);
    }

    bool _add_from_items(int index){
        string str = items[index];

        if(root == nullptr){
            root = new node(index);
        }else{
            node* t = root;
            node* new_node = new node(index);
            int dist = levenshtein_distance(items[t->index], str);
            while(t->children.find(dist) != t->children.end()){
                t = t->children.find(dist)->second;
                dist = levenshtein_distance(items[t->index], str);
            }
            if(dist > 0){
                t->children.insert(pair<int, node*>(dist, new_node));
                return true;
            }else{
                return false;
            }
        }
    }

    void _search(string str, int tol, node* r, vector<string>& vec){
        string r_val = items[r->index];
        int dist = levenshtein_distance(r_val, str);
        
        if(dist <= tol){
            if(deleted.find(r_val) == deleted.end())
                vec.push_back(r_val);
        }

        int gte = dist-tol, lte = dist+tol;
        
        for(const pair<int, node*>& p : r->children){
            if(p.first >= gte && p.first <= lte)
                _search(str, tol, p.second, vec);
        }
    }

    bool _has(string str, int tol, node* r){
        string r_val = items[r->index];
        int dist = levenshtein_distance(r_val, str);
        
        if(dist <= tol){
            if(deleted.find(r_val) == deleted.end())
                return true;
        }

        int gte = dist-tol, lte = dist+tol;
        
        for(const pair<int, node*>& p : r->children){
            if(p.first >= gte && p.first <= lte)
                if(_has(str, tol, p.second))
                    return true;
        }

        return false;
    }

    void _print(node* r = nullptr, node* n = nullptr, int depth=0){
        if(n == nullptr)
            n = root;
        if(r == nullptr)
            r = root;
        
        for(int i=0; i<depth; ++i)
            cout << "\t";
        
        string n_val = items[n->index], r_val = items[r->index];
        if(deleted.find(n_val) != deleted.end())
            cout << "D*";
        cout << n_val << ", " << levenshtein_distance(r_val, n_val) << endl;
        for(const pair<int, node*>& x : n->children){
            _print(n, x.second, depth+1);
        }
    }
};