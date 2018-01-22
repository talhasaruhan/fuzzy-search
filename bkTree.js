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
    constructor(value=""){
        this.value = value;
        this.children = null;
    }

    toJSON(){
        if(this.children != null)
            var childrenJSON = Array.from(this.children);
        else
            var childrenJSON = null;

        return {'value' : this.value, 'children' : childrenJSON};
    }

    parse(json){
        if(typeof json === "string" || json instanceof String)
            json = JSON.parse(json);
        this.value = json["value"];
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
    constructor(metric=levenshteinDistance, items=null){
        this.metric = metric;
        this.deleted = new Set();
        this.size = 0;
        if(items == null)
            this._constructEmptyTree();
        else if(typeof items === "string" || items instanceof String)
            this._constructFromRoot(items);
        else if(Array.isArray(items))
            this._contructFromDict(items);
    }

    _constructEmptyTree(){
        this.root = null;
    }

    _constructFromRoot(str){
        this.root = new BKTreeNode(str);
    }

    _contructFromDict(items){
        this.root = new BKTreeNode(items[0]);
        for(var i=1; i<items.length; ++i){
            this.add(items[i]);
        }
    }

    add(str){
        this.size = this.size + 1;
        if(this.deleted.delete(str))
            return;
        if(this.root == null)
            this.root = new BKTreeNode(str);
        else{
            var t = this.root;
            var dist = this.metric(t.value, str);
            while(dist > 0){
                if(t.children == null){
                    t.children = new Map();
                    break;
                }
                if(!t.children.has(dist))
                    break;
                t = t.children.get(dist);
                dist = this.metric(t.value, str);
            }
            if(dist > 0)
                t.children.set(dist, new BKTreeNode(str));
        }
    }

    delete(str){
        if(this.has(str)){
            this._unsafeDelete();
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
        if(this.deleted.has(n.value))
            return;

        var dist = this.metric(str, n.value);
        if(dist <= tol)
            if(vec != null)
                vec.push({[dist] : n.value});

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
        console.log(this._getMetricName());
        this._print();
    }
    
    _print(r=this.root, n=this.root, depth=0){        
        var str = "";
        for(var i=0; i<depth; ++i)
            str = str + "\t";

        str = str + n.value + ", " + this.metric(r.value, n.value);
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