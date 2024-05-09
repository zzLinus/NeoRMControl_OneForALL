#include <iostream>
#include <stack>

#include <vector>
using namespace std;
const int N = 1000;

vector<int> g1[N], g2[N];
bool vis1[N], vis2[N];
stack<int> st;

void addEdge(int x, int y) {
  g1[x].push_back(y);
  g2[y].push_back(x);
}

void dfs1(int x) {
  if (vis1[x]) {
    return;
  }
  vis1[x] = 1;
  for (auto y : g1[x]) {
    dfs1(y);
  }
  st.push(x);
}

void dfs2(int x) {
  if (vis2[x]) {
    return;
  }
  cout << x << ' ';
  vis2[x] = 1;
  for (auto y : g2[x]) {
    dfs2(y);
  }
}

int main() {
  int n = 7;
  addEdge(1, 0);
  addEdge(2, 1);
  addEdge(2, 5);
  addEdge(3, 2);
  addEdge(0, 3);
  addEdge(0, 4);
  addEdge(4, 3);
  addEdge(5, 7);
  addEdge(7, 6);
  addEdge(6, 5);
  for (int i = 0; i < n; i++) {
    dfs1(i);
  }
  while (!st.empty()) {
    dfs2(st.top());
    st.pop();
    std::cout << '\n';
  }
  std::cout << "hi" << std::endl;
}

