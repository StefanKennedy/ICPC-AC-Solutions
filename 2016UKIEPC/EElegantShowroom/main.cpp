#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

struct stuff{
    int x;
    int y;
    int depth;
    bool operator<(const stuff& rhs) const {return depth > rhs.depth;}

};

int index(int x, int y){

    return (x*402)+y;
}

int hashh(int a, int b){
    return a*1000 + b;
}

char grid[402*402];


bool compare2(stuff i, stuff j){ i.depth > j.depth; cout << i.depth << " < " << j.depth << endl; }

int bfs(priority_queue<stuff> q, int xl, int yl){

    int visited[500000] = {0};

    while(!q.empty()){
        stuff info = q.top();
        pair<int, int> loc;
        loc.first = info.x;
        loc.second = info.y;
        //cout << "Popping with depth: " << info.depth << endl;
        q.pop();
        if(grid[index(loc.first, loc.second)] == '#'){
            continue;
        }
        if(grid[index(loc.first, loc.second)] == 'D' && (loc.second == 1 || loc.first == 1 || loc.second == xl || loc.first == yl)){
            cout << /*"FOUND IT! at " << loc.first << " " << loc.second << " depth: " << */info.depth - 1<< endl;
            return 0;
        }
        //cout << loc.second << " " << loc.first << " depth " << info.depth << endl;
        if(visited[hashh(loc.first-1, loc.second)] == 0) visited[hashh(loc.first-1, loc.second)] = 1601;
        if(visited[hashh(loc.first+1, loc.second)] == 0) visited[hashh(loc.first+1, loc.second)] = 1601;
        if(visited[hashh(loc.first, loc.second+1)] == 0) visited[hashh(loc.first, loc.second+1)] = 1601;
        if(visited[hashh(loc.first, loc.second-1)] == 0) visited[hashh(loc.first, loc.second-1)] = 1601;
        if(info.depth+(grid[index(loc.first, loc.second)] != 'D') < visited[hashh(loc.first-1, loc.second)] && loc.first-1>0){

            stuff s;
            s.x = loc.first-1;
            s.y = loc.second;
            s.depth = info.depth;
            if(grid[index(loc.first, loc.second)] != 'D'){
               s.depth += 1;
               //cout << "Increasing depth" << endl;
            }else{
                //cout << "Not increasing depth" << endl;
            }

            q.push(s);
            //cout << "Adding with depth: " << s.depth << endl;
            visited[hashh(loc.first-1, loc.second)] = s.depth;
        }
        //cout << "To go right: " << info.depth+(grid[index(loc.first, loc.second)] != 'D') << " < " << visited[hashh(loc.first+1, loc.second)] << endl;
        if(info.depth+(grid[index(loc.first, loc.second)] != 'D') < visited[hashh(loc.first+1, loc.second)] && loc.first+1<=xl+1){
            stuff s;
            s.x = loc.first+1;
            s.y = loc.second;
            s.depth = info.depth;
            if(grid[index(loc.first, loc.second)] != 'D'){
                s.depth += 1;
                //cout << "Increasing depth" << endl;
            }else{
                //cout << "Not increasing depth" << endl;
            }

            q.push(s);
            //cout << "Adding with depth: " << s.depth << endl;
            visited[hashh(loc.first+1, loc.second)] = s.depth;
        }
        if(info.depth+(grid[index(loc.first, loc.second)] != 'D') < visited[hashh(loc.first, loc.second-1)] && loc.second-1>0){

            stuff s;
            s.x = loc.first;
            s.y = loc.second-1;
            s.depth = info.depth;
            if(grid[index(loc.first, loc.second)] != 'D'){
                s.depth += 1;
                //cout << "Increasing depth" << endl;
            }else{
                //cout << "Not increasing depth" << endl;
            }

            q.push(s);
            //cout << "Adding with depth: " << s.depth << endl;
            visited[hashh(loc.first, loc.second-1)] = s.depth;
        }
        if(info.depth+(grid[index(loc.first, loc.second)] != 'D') < visited[hashh(loc.first, loc.second+1)] && loc.second+1<=xl){
            stuff s;
            s.x = loc.first;
            s.y = loc.second+1;
            s.depth = info.depth;
            if(grid[index(loc.first, loc.second)] != 'D'){
                s.depth  += 1;
                //cout << "Increasing depth" << endl;
            }else{
                //cout << "Not increasing depth" << endl;
            }

            q.push(s);
            //cout << "Adding with depth: " << s.depth << endl;
            visited[hashh(loc.first, loc.second+1)] = s.depth;
        }

    }
}

int main()
{
/*
    for(int x = 0; x < 400; x++){cout << "#";}
    cout << endl;
    for(int x = 0; x < 398/2; x++){
        cout << "#";
        for(int y = 0; y < 398; y++){ cout << "D";}
        cout << "#" << endl;
    }
    cout << "#";
    for(int x = 0; x < 398; x++){cout << "D";}
    cout << "#" << endl;
    for(int x = 0; x < 398/2-1; x++){
        cout << "#";
        for(int y = 0; y < 398; y++){ cout << "D";}
        cout << "#" << endl;
    }
    for(int x = 0; x < 400; x++){cout << "#";}
    cout << endl;
    return 0;*/
    int i;
    int j;

    int x, y;
    cin >> y >> x;

    for(i=1; i <= y;i++){
        string s;
        cin >> s;
        for(j=1;j <= x;j++){
            grid[index(i, j)] = s[j-1];
        }
    }

    int sx, sy;
    cin >> sx >> sy;

    /*cout << x << " " << y << endl;
    for(i=1; i<= y;i++){
        for(j=1; j <= x; j++){
            cout << grid[index(i, j)];
        }
        cout << endl;
    }
    cout << sx << " " << sy << endl;
*/
    priority_queue<stuff> q;
    stuff s;
    s.x = sx;
    s.y = sy;
    s.depth = 1;
    q.push(s);

    bfs(q, x, y); // I think the far right walls might be un-traversed
    return 0;
}
