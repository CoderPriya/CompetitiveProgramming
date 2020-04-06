/*
Paint by Letters
Andi Qu
- F = E - V + C + 1
- Every corner of a cell is a node
- Every side of a cell is an edge if it is between 2 different colours
- DFS to find connected components
- To check if a component is completely inside a query rectangle, traverse
  the perimeter of the rectangle
- Complexity: O(MN + Q(M + N))
*/

#include <bits/stdc++.h>
#define FOR(i, x, y) for (int i = x; i < y; i++)
using namespace std;

int n, m, q;

char grid[2001][2001];
pair<int, int> special[2002][2002], d[4]{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
int s_pref[2002][2002], v_pref[2002][2002], h_pref[2002][2002];
bool deleted[2002][2002];

bool inside(int x, int y, int nx, int ny) {
    if (!(~nx && ~ny && nx <= n + 1 && ny <= m + 1)) return false;
    if (nx == x + 1) return grid[x][y] != grid[x][y - 1];
    if (nx == x - 1) return grid[nx][y] != grid[nx][y - 1];
    if (ny == y + 1) return grid[x][y] != grid[x - 1][y];
    return grid[x][ny] != grid[x - 1][ny];
}

bool outside(int x, int y, int x1, int y1, int x2, int y2) {
    int a = special[x][y].first, b = special[x][y].second;
    if (a > x1 && a <= x2 && b > y1 && b <= y2 && !deleted[a][b]) {
        deleted[a][b] = true;
        return true;
    }
    return false;
}

void deactivate(int x, int y) {
    int a = special[x][y].first, b = special[x][y].second;
    deleted[a][b] = false;
}

void dfs(int x, int y) {
    FOR(i, 0, 4) {
        int nx = x + d[i].first, ny = y + d[i].second;
        if (inside(x, y, nx, ny)) {
            if (nx == x + 1) v_pref[x][y] = 1;
            else if (nx == x - 1) v_pref[nx][y] = 1;
            else if (ny == y + 1) h_pref[x][y] = 1;
            else h_pref[x][ny] = 1;

            if (!special[nx][ny].first) {
                special[nx][ny] = special[x][y];
                dfs(nx, ny);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // ifstream cin("paint.in");
    // ofstream cout("paint.out");

    cin >> n >> m >> q;
    FOR(i, 1, n + 1) FOR(j, 1, m + 1) cin >> grid[i][j];
    FOR(i, 1, n + 2) FOR(j, 1, m + 2) if (!special[i][j].first) {
        s_pref[i][j] = 1;
        special[i][j] = {i, j};
        dfs(i, j);
    }
    FOR(i, 1, n + 2) FOR(j, 1, m + 2) {
        s_pref[i][j] += s_pref[i - 1][j] + s_pref[i][j - 1] - s_pref[i - 1][j - 1];
        v_pref[i][j] += v_pref[i - 1][j] + v_pref[i][j - 1] - v_pref[i - 1][j - 1];
        h_pref[i][j] += h_pref[i - 1][j] + h_pref[i][j - 1] - h_pref[i - 1][j - 1];
    }

    while (q--) {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);

        int E = v_pref[x2][y2] - v_pref[x2][y1] - v_pref[x1 - 1][y2] + v_pref[x1 - 1][y1] +
                h_pref[x2][y2] - h_pref[x2][y1 - 1] - h_pref[x1][y2] + h_pref[x1][y1 - 1];
        int V = (x2 - x1) * (y2 - y1);
        int C = s_pref[x2][y2] - s_pref[x2][y1] - s_pref[x1][y2] + s_pref[x1][y1];

        // Delete connected components not entirely inside the query rectangle
        FOR(i, x1, x2 + 2) {
            if (outside(i, y2 + 1, x1, y1, x2, y2)) C--;
            if (outside(i, y1, x1, y1, x2, y2)) C--;
        }
        FOR(i, y1, y2 + 2) {
            if (outside(x2 + 1, i, x1, y1, x2, y2)) C--;
            if (outside(x1, i, x1, y1, x2, y2)) C--;
        }
        FOR(i, x1, x2 + 2) {
            deactivate(i, y2 + 1);
            deactivate(i, y1);
        }
        FOR(i, y1, y2 + 2) {
            deactivate(x2 + 1, i);
            deactivate(x1, i);
        }

        cout << E - V + C + 1 << '\n';
    }
    return 0;
}