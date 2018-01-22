#include<bits/stdc++.h>

using namespace std;

class trie{
public:
    class node{
    public:
        bool end_word;
        map<int, node*> children;
        node():end_word(false){};
        ~node(){for(pair<const int, node*>& child : children) delete child.second;}
        int capacity(){
            int sz = sizeof(bool);
            sz += ((sizeof(int)+sizeof(node*))*children.size())+sizeof(children);
            for(pair<const int, node*>& child : children)
                sz += child.second->capacity();
            return sz;
        }   
    };

    trie(){
        root = new node();
    }

    ~trie(){
        delete root;
    }

    void insert(string str){
        node* r = root;
        int t;
        for(int i=0; i<str.length(); ++i){
            t = str[i]-'a';
            if(r->children.find(t) == r->children.end()){
                node* n = new node();
                r->children.insert(pair<int, node*>(t, n));
                r = n;
            }else{
                r = r->children[t];
            }
        }
        r->end_word = true;
    }

    vector<string> search(string q, int tol){
        vector<string> matches;
        vector<int> row(q.length()+1);
        for(int i=0; i<=q.length(); ++i)
            row[i] = i;
        for(pair<const int, node*>& p : root->children)
            _search(p.second, string(1, p.first+'a'), q, tol, row, matches);
        
        return matches;
    }

    void remove(string str){
        _remove(root, str, 0);
    }

    void print(){
        _print(root, 0);
    }

    int capacity(){
        int sz = sizeof(node*);
        sz += root->capacity();
        return sz;
    }

private:
    node* root;

    void _search(node* r, string w, string q, int tol, const vector<int>& prev_row, vector<string>& vec){
        int _min = INT_MAX;
        vector<int> curr_row(q.length()+1, 0);
        
        curr_row[0] = prev_row[0]+1;
        for(int j=1; j<=q.length(); ++j){
            curr_row[j] = min(prev_row[j-1], min(prev_row[j], curr_row[j-1]));
            if(q[j-1] != w[w.length()-1]){
                curr_row[j]++;
            }
            if(curr_row[j] < _min)
                _min = curr_row[j];
        }

        if(_min > tol)
            return;
        if(r->end_word)
            vec.push_back(w);
        for(pair<const int, node*>& p : r->children)
            _search(p.second, w+string(1, p.first+'a'), q, tol, curr_row, vec);
    }

    bool _remove(node* r, string str, int depth){
        if(r == nullptr)
            return false;
        if(depth == str.length()){
            if(r->end_word){
                r->end_word = false;
                if(r->children.size() > 0)
                    return false;
                return true;
            }
            return false;
        }
        int index = (int)(str[depth]-'a');
        map<int, node*>::iterator it = r->children.find(index);
        if(it == r->children.end())
            return false;
        node* n = it->second;
        if(_remove(n, str, depth+1)){
            r->children.erase(index);
            delete n;
            if(r->end_word || r->children.size() > 0)
                return false;
            return true;
        }
        return false;
    }

    void _print(node* r, const int depth){
        for(pair<const int, node*>& p : r->children){
            for(int i=0; i<depth; ++i){
                printf(" ");
            }
            printf("%c", char(p.first+'a'));
            if(p.second->end_word)
                printf("*");
            printf("\n");
            _print(p.second, depth+1);
        }
    }
};