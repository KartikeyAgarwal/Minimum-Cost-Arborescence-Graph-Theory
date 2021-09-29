#include<bits/stdc++.h>
using namespace std;
#define max 10000

int visited[max]={0};
int cycle[max]={0};

void dfs(vector<vector<pair<int,int>>> next,int n,int src)      //dfs to find the vertices that are unreachable
{
    visited[src]=1;
    for(auto it=next[src].begin();it!=next[src].end();it++)
    {
        if(visited[it->first]!=1)
        {
            // cout<<it->first;
            dfs(next,n,it->first);
        }
    }
}

//to remove unreachable vertices from the adjlist
vector<vector<pair<int,int>>> remove_unreachable(vector<vector<pair<int,int>>> adjlist,int n,int src)   
{
    for(int i=1;i<=n;i++)
    {
        if(visited[i]!=1)
        {
            // cout<<i;
            adjlist[i].clear();
            for(int j=1;j<=n;j++)
            {
                // it=find_if(adjlist[j].begin(),adjlist[j].end(),call);
                // adjlist[j].erase(it);
                for(auto it=adjlist[j].begin();it!=adjlist[j].end();it++)
                {
                    if(it->first==i)
                    {
                        adjlist[j].erase(it);
                        break;
                    }
                }
            }
        }
    }
    return adjlist;
}


// contraction is the function to connect all the vertices outside to the minlabel vertex of the cycle
vector<vector<pair<int,int>>> contraction(vector<vector<pair<int,int>>> adjlist,int parent[],int cycle[],int n)
{
    int minlabel=1;
    for(int i=1;i<=n;i++)
    {
        if(cycle[i]!=0)
        {
            minlabel=i;
            break;
        }
    }
    // cout<<"/"<<minlabel<<"/";
    for(int i=1;i<=n;i++)
    {
        if(cycle[i]==0)
        {
            for(int j=1;j<=n;j++)
            {
                if(cycle[j]!=0)
                {
                    for(auto it=adjlist[j].begin();it!=adjlist[j].end();it++)
                    {
                        if(it->first==i && j!=minlabel)
                        {
                            adjlist[minlabel].push_back(make_pair(it->first,it->second));
                            adjlist[j].erase(it);break;
                        }
                    }
                }
            }
            for(int j=1;j<=n;j++)
            {
                if(cycle[j]!=0)
                { 
                    for(auto it=adjlist[i].begin(); it!=adjlist[i].end();it++)
                    {
                        if(it->first==j && it->first!=minlabel)
                        {
                            // cout<<"*"<<j<<"*";
                            adjlist[i].erase(it);
                            adjlist[i].push_back(make_pair(minlabel,it->second));
                        }
                    }
                }
            }
        }
    }
    // cout<<"hello";
    return adjlist;
}

//detect_cycle is the recursive function that detects cycle in th adjlist and contracts it
//then returns contracted adjlist
int tempo=1;
vector<vector<pair<int,int>>> detect_cycle(vector<vector<pair<int,int>>> adjlist,int parent[],int n,int src,int tempo)
{
    int j,flag;
    // cout<<"yes";
    while(tempo<=n)
    {
        flag=0;
        j=tempo;
        // for(auto it=adjlist[i].begin();it!=adjlist[i].end();it++)
        // {
            int temp=n;
            while(parent[j]!=-1 && parent[j]==0 && temp--)
            {
                // cout<<"chill";
                j=parent[j];
                if(j==tempo)
                {
                    flag=1;
                    break;
                }
                // cout<<"why"<<j;
            }            
            // cout<<"no";
            j=tempo;

            for(int i=1;i<=n;i++)   cycle[i]=0;
            
            if(flag==1)
            {
                // cout<<"#";
                while(parent[j]!=tempo)
                {
                    cycle[j]=parent[j];
                    j=parent[j];
                }
                cycle[j]=parent[j];
                // break;
                //contraction 
            }
            // for(int i=1;i<=n;i++)
            //     cout<<cycle[i]<<' ';
            if(flag==1)
            {
                // cout<<"$";
                adjlist=contraction(adjlist,parent,cycle,n);cout<<"^";
                tempo++;
                for(int i=1;i<=n;i++)
                    cycle[i]=0;
                detect_cycle(adjlist,parent,n,src,tempo);
                // break;
            }
        // }
        if(flag==0)
            tempo++;
        // cout<<"____"<<tempo<<"/";
        // cout<<"/"<<tempo<<"/";

        // tempo++;
    }
    // cout<<parent[6];
    /*int cnt=0;
    int father=999999;int ptr=0;
    for(int i=1;i<=n;i++)
    {ptr=0;
        for(auto it=adjlist[i].begin();it!=adjlist[i].end();it++)
        {    // cout<<"%"<<adjlist[6].size()<<"/";
            cnt++;
            if(i==2)
                cout<<it->first<<"?";
            if(it->first==src)
            {ptr=1;
                if(it->second<father)
                    father=it->second;
                parent[i]=src;
            }
        }
        if(ptr==1)          //ptr is to find src is pointing to which vertex
        {
            parent[i]=src;
            cout<<"yes"<<i;
            for(auto it=adjlist[i].begin();it!=adjlist[i].end();it++)
            {
                cout<<"/"<<it->first<<"/";
                // if(it->first==src)
                // {
                //     adjlist[i].erase(it);
                //     cout<<"up";
                //     // break;
                // }
            }
            adjlist[i].push_back(make_pair(src,father));
            cout<<"yes"<<i;
        }
    } */

// cout<<cnt;
    return adjlist;
}


int main()
{
    // _GLIBCXX_USE_GETTIMEOFDAY();
    int t;
    int n,m,s;
    cin>>t;
    while(t--)
    {
        cin>>n>>s;
        cin>>m;
        for(int i=0;i<=2*n;i++)
        {
            visited[i]=0;
            cycle[i]=0;
        }
        vector<vector<pair<int,int>>> original(n+1);    //original graph

        vector<vector<pair<int,int>>> adjlist(n+1);        
        int u,v,w;
        if(n==0)
            continue;
        if(n==1)
        {
            cout<<"0 0"<<"#"<<"0"<<endl;
            continue;
        }
        vector<vector<pair<int,int>>> next(n+1);
        for(int i=0;i<m;i++)
        {
            cin>>u>>v>>w;

            original[v].push_back(make_pair(u,w));      //adjacency adjlist of child pointing to parent
            next[u].push_back(make_pair(v,w));
        }
        
        for(int i=1;i<=n;i++)
        {
            for(auto it=original[i].begin();it!=original[i].end();it++)
            {
                adjlist[i].push_back(make_pair(it->first,it->second));     //copy original graph to another 
            }
        }
        // fill(visited,visited+n,0);
        // cout<<s;
        for(int i=1;i<=n;i++)
        {
                for(auto it=adjlist[s].begin();it!=adjlist[s].end();it++)
                {
                    adjlist[s].erase(it);
                    // cout<<it->first<<' ';
                    break;
                }
        }
        // for(auto it=adjlist[s].begin();it!=adjlist[s].end();it++)
        //     cout<<it->first;

// cout<<"yeo";
        dfs(next,n,s);
        adjlist=remove_unreachable(adjlist,n,s);
// cout<<endl;
        int parent[n+1]; 
        for(int i=0;i<=n;i++)
            parent[i]=0;
        for(int i=1;i<=n;i++)
        {
            int min=999999;
            int minindex=999999;
            for(auto it=adjlist[i].begin(); it!=adjlist[i].end();it++)
            {
                if(it->second < min)
                {
                    min=it->second;
                    minindex=it->first;
                }
            }
            
            parent[i]=minindex;
            parent[s]=0;
            for(auto it=adjlist[i].begin(); it!=adjlist[i].end();it++)
            {
                it->second=it->second-min;  
                // if(i==6)
                // cout<<"/"<<it->first<<"/";        //reducing each incoming edge of a vertex by its minimum incoming edge
            }

        }

        // detect_cycle(adjlist,parent,n,s);

     /*   for(int i=1;i<=n;i++)
        {
            for(auto it=adjlist[i].begin();it!=adjlist[i].end();it++)
            {
                cout<<it->first<<' '<<it->second;
            }
            cout<<endl;
        }
*/  
        for(int i=1;i<=n;i++)
        {
            if(parent[i]==999999)
                parent[i]=-1;
        }
        // for(int i=1;i<=n;i++)
        //     cout<<parent[i]<<' ';

        tempo=1;
        adjlist=detect_cycle(adjlist,parent,n,s,tempo);

        // cout<<"yo";
// cout<<endl;
        // for(auto it=adjlist[2].begin();it!=adjlist[2].end();it++)
        //     cout<<it->first<<"/";
parent[s]=0;
        int cost_sum=0;
        vector<vector<pair<int,int>>> new_graph(n+1);
        int indegree[n+1]={0};
        for(int i=1;i<=n;i++)
        {
            int min=999999;
            int min_vertex=999999;
            if(i==s)
            {
                for(auto it=next[i].begin();it!=next[i].end();it++)
                {
                    // cout<<it->first;
                    if(it->second<min)
                    {
                        min_vertex=it->first;
                        min=it->second;
                    }
                }
                    // parent[min_vertex]=i;
                    if(indegree[min_vertex]==0)
                    {
                        // cout<<min_vertex;
                        new_graph[i].push_back(make_pair(min_vertex,min));
                        cost_sum +=min;
                        parent[min_vertex]=i;
                        indegree[min_vertex]=1;
                    }
                    
                    // cout<<it->first;                                         
            }
            else if(visited[i]==1)
            {
                int flag=0;
                for(auto it=adjlist[i].begin();it!=adjlist[i].end();it++)
                {
                    if(it->second<min)
                    {
                        min=it->second;
                        min_vertex=it->first;
                    }
                }
                for(auto it=new_graph[min_vertex].begin();it!=new_graph[min_vertex].end();it++)
                {
                    if(it->first==i)
                        flag=1;
                }
                if(flag==1)
                    continue;
                else
                {
                    if(indegree[i]==0 && visited[min_vertex]==1)
                    {
                        for(auto it2=next[min_vertex].begin();it2!=next[min_vertex].end();it2++)
                        {
                            if(it2->first==i)
                            {
                                cost_sum +=it2->second;
                                min=it2->second;
                            }
                        }
                        new_graph[min_vertex].push_back(make_pair(i,min));
                        parent[i]=min_vertex;
                        indegree[i]=1;
                    }
                }
                    
            }
        }
        // cout<<"hello";
        
        // cout<<endl;
        int distance[n+1];
        for(int i=0;i<=n;i++)
            distance[i]=-1;
        distance[s]=0;
        int calculated[n+1];
        calculated[s]=0;
        for(int i=1;i<=n;i++)
        {
            for(auto it=new_graph[i].begin();it!=new_graph[i].end();it++)
            {
                // calculated[it->first]=calculated[i]+it->second;
                if(distance[i]==-1)
                    continue;
                distance[it->first] =distance[i]+ it->second;
                // cout<<distance[it->first]<<' ';
            }
        }
        // cout<<endl;
        for(int i=1;i<=n;i++)
        {
            for(auto it=new_graph[i].begin();it!=new_graph[i].end();it++)
            {
                // cout<<i<<' '<<it->first<<endl;
                if(distance[it->first]==-1)
                {
                    distance[it->first] =distance[i]+ it->second;
                    // cout<<distance[it->first]<<' ';
                }
            }
            // cout<<endl;
        }
        
        // cout<<endl;
        cout<<cost_sum<<' ';

        for(int i=1;i<=n;i++)
            cout<<distance[i]<<' ';
    
        cout<<"# ";
        for(int i=1;i<=n;i++)
            cout<<parent[i]<<' ';

        cout<<endl;

    }
    // _GLIBCXX_USE_GETTIMEOFDAY();
}