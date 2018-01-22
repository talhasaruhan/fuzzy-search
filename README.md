# Fuzzy String Search

Implementations of 3 data structures that help to accelerate approximate string matching.

## JS Implementations

### BK-Tree
* **BKTree(function metric, string[] items)** : Creates the tree with given items and metric function (Levenshtein or Hamming)
* **add(string S)** : Inserts given string S into the tree
* **delete(string S)** : Marks the string S as deleted
* **query(string Q, int T)** : Returns strings within T distance of given string Q
* **entries()** : Returns entries inside the tree
* **has(string S)** : Checks for if given string S, equivalent to bool : query(S, 0).length > 0
* **print()** : Prints the tree
* **merge(BKTree T)** : Imports entries from another BKTree
* **toJSON()** : Converts tree into JSON format
* **parse(JSON J)** : Parses given JSON data, overrides current tree


## C++ Implementations

### BK-Tree
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

### Trie
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

### VP-Tree
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


