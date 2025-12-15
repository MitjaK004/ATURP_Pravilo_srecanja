#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>

using namespace std;

static inline int id(int x, int y, int M) {
    return x * M + y;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << -1 << "\n";
        return 0;
    }

    ifstream in(argv[1]);
    if (!in) {
        cout << -5 << "\n";
        return 0;
    }

    int N, M, E;
    in >> N >> M >> E;

    int V = N * M;
    vector<unsigned char> mask(V, 0);

    auto add_edge = [&](int x1, int y1, int x2, int y2) {
        int a = id(x1, y1, M);
        int b = id(x2, y2, M);
        if (x2 == x1 + 1) { mask[a] |= 4; mask[b] |= 8; }
        else if (x2 == x1 - 1) { mask[a] |= 8; mask[b] |= 4; }
        else if (y2 == y1 + 1) { mask[a] |= 1; mask[b] |= 2; }
        else if (y2 == y1 - 1) { mask[a] |= 2; mask[b] |= 1; }
        };

    for (int i = 0; i < E; i++) {
        int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        add_edge(x1, y1, x2, y2);
    }

    int px, py, cx, cy, fx, fy;
    in >> px >> py >> cx >> cy >> fx >> fy;

    if (cx == fx && cy == fy) {
        cout << 0 << "\n";
        return 0;
    }

    int dx = cx - px;
    int dy = cy - py;
    int dir = -1;
    if (dx == 0 && dy == 1) dir = 0;
    else if (dx == 0 && dy == -1) dir = 1;
    else if (dx == 1 && dy == 0) dir = 2;
    else if (dx == -1 && dy == 0) dir = 3;
    else {
        cout << -1 << "\n";
        return 0;
    }

    const int INF = 1000000000;
    vector<int> dist(V * 4, INF);
    deque<pair<int, int>> q;

    int start = id(cx, cy, M);
    dist[start * 4 + dir] = 0;
    q.push_back({ start, dir });

    int dvec[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
    int bitOf[4] = { 1, 2, 4, 8 };

    auto rightDir = [&](int d) {
        if (d == 0) return 2;
        if (d == 2) return 1;
        if (d == 1) return 3;
        return 0;
        };
    auto leftDir = [&](int d) {
        if (d == 0) return 3;
        if (d == 3) return 1;
        if (d == 1) return 2;
        return 0;
        };

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop_front();

        int u = cur.first;
        int d = cur.second;
        int du = dist[u * 4 + d];

        int x = u / M;
        int y = u % M;
        unsigned char m = mask[u];

        int rdir = rightDir(d);
        int ldir = leftDir(d);

        bool hasStraight = (m & bitOf[d]) != 0;

        auto tryMove = [&](int ndir, bool allow) {
            if (!allow) return;
            if ((m & bitOf[ndir]) == 0) return;
            int nx = x + dvec[ndir][0];
            int ny = y + dvec[ndir][1];
            if (nx < 0 || nx >= N || ny < 0 || ny >= M) return;
            int v = id(nx, ny, M);
            int ns = v * 4 + ndir;
            if (dist[ns] > du + 1) {
                dist[ns] = du + 1;
                q.push_back({ v, ndir });
            }
            };

        tryMove(d, true);
        tryMove(rdir, true);
        tryMove(ldir, !hasStraight);

        if (x == fx && y == fy) {
            cout << du << "\n";
            return 0;
        }
    }

    int finish = id(fx, fy, M);
    int ans = INF;
    for (int d = 0; d < 4; d++)
        ans = min(ans, dist[finish * 4 + d]);

    cout << (ans == INF ? -1 : ans) << "\n";
    return 0;
}
