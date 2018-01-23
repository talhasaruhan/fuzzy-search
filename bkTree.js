function levenshteinDistance(str1, str2){
    var m = str1.length;
    var n = str2.length;
    var dn = new Array(m+1);

    for(var i=0; i<=m; ++i){
        dn[i] = new Array(n+1);
        for(var j=0; j<=n; ++j){
            if(i==0)
                dn[0][j] = j;
            else if(j==0)
                dn[i][0] = i;
            else if(str1[i-1] == str2[j-1])
                dn[i][j] = dn[i-1][j-1];
            else
                dn[i][j] = 1 + Math.min(
                    dn[i-1][j],
                    Math.min(dn[i][j-1],
                    dn[i-1][j-1])
                    );
        }
    }

    return dn[m][n];
}

function hammingDistance(str1, str2){
    var dist = 0;
    if(str1.length != str2.length)
        return Math.max(str1.length, str2.length);
    for(var i=0; i<str1.length; ++i){
        if(str1[i] != str2[i]){
            dist = dist + 1;
        }
    }
    return dist;
}

class BKTreeNode{
    constructor(index=0){
        this.index = index;
        this.children = null;
    }

    toJSON(){
        if(this.children != null)
            var childrenJSON = Array.from(this.children);
        else
            var childrenJSON = null;

        return {'index' : this.index, 'children' : childrenJSON};
    }

    parse(json){
        if(typeof json === "string" || json instanceof String)
            json = JSON.parse(json);
        this.index = json["index"];
        if(json["children"] == null)
            this.children = null;
        else{
            // if(this.children == null)
            this.children = new Map();
            for(var i=0; i<json["children"].length; ++i){
                var entry = json["children"][i];
                var node = new BKTreeNode();
                node.parse(entry[1]);
                this.children.set(entry[0], node);
            }
        }
    }
}

class BKTree{
    constructor(items=null, metric=levenshteinDistance){
        this.metric = metric;
        this.deleted = new Set();
        this.size = 0;
        this.items = [];
        console.log(items);
        if(items == null)
            this._constructEmptyTree();
        else if(typeof items === "string" || items instanceof String){
            this.items.push(items);
            this._constructFromRoot(items);
        }else if(Array.isArray(items)){
            this.items = items;
            this._contructFromDict();
        }
    }

    _constructEmptyTree(){
        this.root = null;
    }

    _constructFromRoot(str){
        this.root = new BKTreeNode(0);
    }

    _contructFromDict(){
        this.root = new BKTreeNode(0);
        this.size = this.items.length;
        var temp;
        for(var i=1; i<this.size; ++i){
            if(!this._addFromItems(i)){
                temp = this.items[i];
                this.items[i] = this.items[this.size-1];
                this.items[this.size-1] = temp;
                this.items.pop();
                this.size = this.size-1;
                i = i-1;
            }
        }
    }

    add(str){
        this.size = this.size + 1;
        if(this.deleted.delete(str)){
            return;
        }

        var index = this.items.length;
        this.items.push(str);
        if(this.root == null){
            this.root = new BKTreeNode(0);
            return;
        }
        if(!this._addFromItems(index)){
            this.size = this.size - 1;
            this.items.pop();
        }
    }

    _addFromItems(index){
        var t = this.root;
        var dist = this.metric(this.items[t.index], this.items[index]);
        while(dist > 0){
            if(t.children == null){
                t.children = new Map();
                break;
            }
            if(!t.children.has(dist))
                break;
            t = t.children.get(dist);
            dist = this.metric(this.items[t.index], this.items[index]);
        }
        if(dist > 0){
            t.children.set(dist, new BKTreeNode(index));
            return true;
        }else{
            return false;
        }
    }

    delete(str){
        if(this.has(str)){
            this._unsafeDelete(str);
        }
    }

    _unsafeDelete(str){
        this.size = this.size - 1;
        this.deleted.add(str);
    }    

    query(str, tol){
        var matches = new Array();
        this._query(str, tol, this.root, matches);
        return matches;
    }

    _query(str, tol, n, vec){
        if(this.deleted.has(this.items[n.index]))
            return;

        var dist = this.metric(str, this.items[n.index]);
        if(dist <= tol)
            if(vec != null)
                vec.push({[dist] : this.items[n.index]});

        var gte = dist-tol;  
        var lte = dist+tol;

        if(n.children == null)
            return;
        for(const entry of n.children.entries()){
            if(entry[0] <= lte && entry[0] >= gte)
                this._query(str, tol, entry[1], vec);
        }  
    }

    entries(){
        var items = new Array();
        this._entries(this.root, items);
        return items;
    }

    _entries(n=this.root, vec){
        if(vec != null)
            vec.push(n.value);
        if(n.children == null)
            return;
        for(const entry of n.children.entries()){
            this._entries(entry[1], vec);
        }
    }

    has(str){
        if(this.query(str, 0).length > 0)    
            return true;
        return false;
    }

    _getMetricName(){
        var metricName;
        if(this.metric == hammingDistance)
            metricName = "hammingDistance";
        else if(this.metric == levenshteinDistance)
            metricName = "levenshteinDistance";
        else
            metricName = null;
        return metricName;
    }

    print(){
        console.log(this.size);
        console.log(this._getMetricName());
        this._print();
    }
    
    _print(r=this.root, n=this.root, depth=0){        
        var str = "";
        for(var i=0; i<depth; ++i)
            str = str + "\t";

        if(this.deleted.has(this.items[n.index]))
            str = str + "*";

        str = str + this.items[n.index] + ", " + this.metric(this.items[r.index], this.items[n.index]);
        console.log(str);
        
        if(n.children == null)
            return;
        for(const entry of n.children.entries()){
            this._print(n, entry[1], depth+1);
        }
    }

    merge(bk){
        var items = bk.entries();
        for(var i=0; i<items.length; ++i){
            this.add(items[i]);
        }
    }

    toJSON(){
        return {'metric' : this._getMetricName(),'root' : this.root};
    }

    parse(json){
        if(typeof json === "string" || json instanceof String)
            json = JSON.parse(json);
        if(this.root == null)
            this.root = new BKTreeNode();

        var metricName = json["metric"];
        if(metricName == "hammingDistance")
            this.metric = hammingDistance;
        else if(metricName == "levenshteinDistance")
            this.metric = levenshteinDistance;
        else
            this.metric = null;

        this.root.parse(json["root"]);
    }

    stringify(){
        return JSON.stringify(this);
    }

}