/* Apolagies for how un-readable this code is

The solution is basically to first go through the routes that the patrolling
guards take and mark everywhere that they can see as a invalid location at
whichever times they can see you (check out the valid_time method)

After you've done that do a breadth first search to find the fridge. If you
get stuck in the same place for long enough then report that it's impossible

Stefan Kennedy, Ireland, DCU
*/

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <utility>
#include <cstring>
#include <set>
#include <vector>

using namespace std;

typedef struct{
    short a;
    short stuck;
    vector<pair<int, int>> b;
} space;

space grid[62][62];
queue<pair<pair<int, int>, int>> q;

int valid_time(vector<pair<int, int>> a, int actual){
    for(int i = 0; i < a.size(); i++){
        if(a[i].second == 0) return 0;
        if (((actual - a[i].first) % a[i].second) == 0) return 0;
    }
    return 1;
}

int bfs(int targetx, int targety){
    set<int> visited;
    while(!q.empty()){
        pair<int, int> pare = q.front().first;
        int time = q.front().second;
        q.pop();
        int x = pare.first, y = pare.second;
        if(x == targetx && y == targety){
            return time;
        }
        if(!valid_time(grid[x][y].b, time+1) && (((x-targetx==1 || targetx-x==1) && y == targety) || ((y-targety==1 || targety-y==1) && x==targetx))){
            return time+1;
        }
        if(grid[x][y].stuck >= 720) continue;
        if(!valid_time(grid[x][y].b, time)) {
            visited.erase(x*100+y);
            continue;
        }
        bool stuck = 1;
        if(grid[x+1][y].a && (visited.find((x+1)*100+y) == visited.end()) && valid_time(grid[x+1][y].b, time+1)){
            q.push(make_pair(make_pair(x+1, y), time+1));
            visited.insert((x+1)*100+y);
            stuck = 0;
        }
        if(grid[x-1][y].a && (visited.find((x-1)*100+y) == visited.end()) && valid_time(grid[x-1][y].b, time+1)){
            q.push(make_pair(make_pair(x-1, y), time+1));
            visited.insert((x-1)*100+y);
            stuck = 0;
        }
        if(grid[x][y-1].a && (visited.find(x*100+y-1) == visited.end()) && valid_time(grid[x][y-1].b, time+1)){
            q.push(make_pair(make_pair(x, y-1), time+1));
            visited.insert(x*100+y-1);
            stuck = 0;
        }
        if(grid[x][y+1].a && (visited.find(x*100+y+1) == visited.end()) && valid_time(grid[x][y+1].b, time+1)){
            q.push(make_pair(make_pair(x, y+1), time+1));
            visited.insert(x*100+y+1);
            stuck = 0;
        }
        if(stuck) grid[x][y].stuck++;
        else grid[x][y].stuck == 0;
        q.push(make_pair(make_pair(x, y), time+1));
    }
    return -1;
}

int main()
{

    memset(grid, 0, sizeof grid);
    int i;

    int r, c;
    scanf("%d %d\n", &r, &c);

    int bedx, bedy, fridgex, fridgey;
    scanf("(%d %d) (%d %d)\n", &bedy, &bedx, &fridgey, &fridgex);

    for(i = 1; i <= r; i++){
        int j;
        for(j = 1; j <= c; j++){
            char c;
            scanf("%c", &c);
            grid[j][i].a = c == '.';
        }
        scanf("\n");
    }

    int p;
    scanf("%d\n", &p);

    for(i = 0; i < p; i++){
        int size;
        scanf("%d ", &size);
        size = size;
        int mod = size*2-2;
        int j;
        for(j = 0; j < size; j++){
            int walkx, walky;
            scanf("(%d %d) ", &walky, &walkx);
            for(int y = walky; y <= 60; y++){
                if(!grid[walkx][y].a) break;
                grid[walkx][y].b.push_back(make_pair(j, mod));
            }
            for(int y = walky-1; y > 0; y--){
                if(!grid[walkx][y].a) break;
                grid[walkx][y].b.push_back(make_pair(j, mod));
            }
            for(int x = walkx+1; x <= 60; x++){
                if(!grid[x][walky].a) break;
                grid[x][walky].b.push_back(make_pair(j, mod));
            }
            for(int x = walkx-1; x > 0; x--){
                if(!grid[x][walky].a) break;
                grid[x][walky].b.push_back(make_pair(j, mod));
            }

            for(int y = walky; y <= 60; y++){
                if(!grid[walkx][y].a) break;
                grid[walkx][y].b.push_back(make_pair(size*2 - j - 2, mod));
            }
            for(int y = walky-1; y > 0; y--){
                if(!grid[walkx][y].a) break;
                grid[walkx][y].b.push_back(make_pair(size*2 - j - 2, mod));
            }
            for(int x = walkx+1; x <= 60; x++){
                if(!grid[x][walky].a) break;
                grid[x][walky].b.push_back(make_pair(size*2 - j - 2, mod));
            }
            for(int x = walkx-1; x > 0; x--){
                if(!grid[x][walky].a) break;
                grid[x][walky].b.push_back(make_pair(size*2 - j - 2, mod));
            }
        }
    }

    if(valid_time(grid[bedx][bedy].b, 0)){
        q.push(make_pair(make_pair(bedx, bedy), 0));
    }
    int blarg = bfs(fridgex, fridgey);
    if(blarg >= 0){
        printf("%d", blarg);
    }else{
        printf("IMPOSSIBLE");
    }

    return 0;
}
