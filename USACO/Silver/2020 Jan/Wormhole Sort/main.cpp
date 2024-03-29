// USACO 2020 January Contest, Silver Problem 3. Wormhole Sort

/*
 Binary search on the answer x. For all wormholes i such that x≤wi, draw an edge between barns ai and bi. It is possible to sort
 the cows with wormholes of width at least x if and only if pi is in the same connected component as i for all i.

 To see that this is true, consider the case when the resulting edges form a tree. We can always perform swaps until one of the
 barns that is a leaf in the tree contains the correct cow. Then remove this leaf from the tree and continue the process.
 */

/*
 import java.io.*;
 import java.util.*;
 public class wormsort {
   public static void main(String[] args) throws IOException{
     BufferedReader br = new BufferedReader(new FileReader("wormsort.in"));
     StringTokenizer st = new StringTokenizer(br.readLine());
     int n = Integer.parseInt(st.nextToken());
     int m = Integer.parseInt(st.nextToken());
     loc = new int[n];
     component = new int[n];
     edges = new LinkedList[n];
     for(int i = 0; i < n; i++) edges[i] = new LinkedList<>();
     lhs = new int[m];
     rhs = new int[m];
     weight = new int[m];
     st = new StringTokenizer(br.readLine());
     for(int i = 0; i < n; i++) loc[i] = Integer.parseInt(st.nextToken())-1;
     for(int i = 0; i < m; i++) {
       st = new StringTokenizer(br.readLine());
       int a = Integer.parseInt(st.nextToken())-1;
       int b = Integer.parseInt(st.nextToken())-1;
       int w = Integer.parseInt(st.nextToken());
       edges[a].add(new Edge(b, w));
       edges[b].add(new Edge(a, w));
     }
     br.close();
     int minW = 0;
     int maxW = 1000000001;
     while(minW != maxW) {
       int mid = (minW + maxW + 1) / 2;
       if(valid(mid)) minW = mid;
       else maxW = mid-1;
     }
     if(minW > 1e9) minW = -1;
     PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter("wormsort.out")));
     pw.println(minW);
     pw.close();
   }
   static int[] loc, lhs, rhs, weight;
   static LinkedList<Edge>[] edges;
   static int[] component;
   private static void dfs(int curr, int label, int minW) {
     if(component[curr] == label) return;
     component[curr] = label;
     for(Edge child: edges[curr]) if(child.w >= minW) dfs(child.d, label, minW);
   }
   private static boolean valid(int minW) {
     Arrays.fill(component, -1);
     int numcomps = 0;
     for(int i = 0; i < component.length; i++) {
       if(component[i] < 0) {
         dfs(i, numcomps++, minW);
       }
     }
     for(int i = 0; i < loc.length; i++) {
       if(component[i] != component[loc[i]]) return false;
     }
     return true;
   }
   static class Edge {
     int d, w;
     public Edge(int d, int w) {
       this.d = d;
       this.w = w;
     }
   }
 }
 */
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;
#define GET_ARRAY_LEN(array, len)                 \
    {                                             \
        len = (sizeof(array) / sizeof(array[0])); \
    }

struct Edge
{
    int d, w;
};
int n, m;
int* loc;
int* component;
vector<Edge>* edges;
void dfs(int curr, int label, int minW)
{
    if (component[curr] == label)
        return;
    component[curr] = label;
    for (Edge child : edges[curr])
        if (child.w >= minW)
            dfs(child.d, label, minW);
}
bool valid(int minW)
{
    fill(component, component + n, -1);
    int numcomps = 0;
    for (int i = 0; i < n; i++)
    {
        if (component[i] < 0)
        {
            dfs(i, numcomps++, minW);
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (component[i] != component[loc[i]])
            return false;
    }
    return true;
}
int main()
{
    cin >> n >> m;
    loc = new int[n];
    component = new int[n];
    edges = new vector<Edge>[n];

    for (int i = 0; i < n; i++)
    {
        int index;
        cin >> index;
        loc[i] = --index;
    }
    for (int i = 0; i < m; i++)
    {
        int a, b, w;
        cin >> a >> b >> w;
        Edge e1, e2;
        e1.d = b - 1;
        e1.w = w;
        e2.d = a - 1;
        e2.w = w;
        edges[a].push_back(e1);
        edges[b].push_back(e2);
    }
    int minW = 0;
    int maxW = 1000000001;
    while (minW != maxW)
    {
        int mid = (minW + maxW + 1) / 2;
        if (valid(mid))
            minW = mid;
        else
            maxW = mid - 1;
    }
    if (minW > 1e9)
        minW = -1;
    cout << minW;
    delete (loc);
    delete (component);
    delete (edges);
}
