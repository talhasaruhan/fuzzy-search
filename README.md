# Fuzzy String Search

Implementations of 3 data structures that help to accelerate approximate string matching.

## BK-Tree
* **bk_tree()** : Constructs empty BK-Tree
* **bk_tree(string S)** : Constructs the tree with S as root node
* **bk_tree(vector<string> S)** : Constructs the tree with given dictionary
* **void insert(string S)** : Inserts the given string into the tree
* **void remove(string S)** : Marks the string as deleted, and rebuilds the tree after number of deleted items exceeds a point.
* **vector<string> search(string Q, int T)** : Searches the tree for strings within T distance from given string Q.
* **int capacity()** : Returns the size of the tree in bytes
* **(static) int levenshtein_distance(string S, string R)** : Returns the Levenshtein Distance of two strings
* **void print()** : Prints the tree in the following format
```
hell, 0
	yelp, 2
	D*help, 1
		yell, 2
```
* **Provides iterator to it's elements(insert order) and allows C++ 11 style range based for loops**
```
bk_tree t(words);
for(string& s : t)
  s;
```

## Trie
* **trie()** : Constructs empty Trie.
* **void insert(string S)** : Inserts string into Trie
* **void search(string Q, int T)** : Searches for string Q within tolerance of T, uses Levenshtein Distance,
* **void remove(string S)** : Removes string S from the Trie
* **int capacity()** : Returns the size of the Trie in bytes
* **void print()** : Prints the Trie in the following format 
```
h
 a
  m*
 e
  l
   l
    o*
   p*
    i
     n
      g*

```

## VP-Tree
* **vp_tree(vector<string> S)** : Constructs the tree with given set of strings
* **vector<string> search(string Q, int T)** : Searches the tree for strings within T distance from given string Q.
* **(static) int levenshtein_distance(string S, string R)** : Returns the Levenshtein Distance of two strings
* **void print()** : Prints the tree in the following format
```
hell
 help
 yell
  yelp
```
* **Provides iterator to it's elements and allows C++ 11 style range based for loops**
```
vp_tree t(words);
for(string& s : t)
  s;
```


