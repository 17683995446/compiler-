#include<iostream>
#include<set>
#include<algorithm>
#include<vector>
#include <iomanip>
#include<fstream>
#define maxn 100

using namespace std;
struct G{
	char left;//产生式左部 
	set<char> first;//每个候选式的first集合 
	string right;//候选式
};
int total;//产生式总数 
G g[maxn];//产生式 
set<char> First[maxn];
set<char> Follow[maxn];
string Terminal="";//终结符 
string NotTerminal="";//非终结符 
vector<char> stack; 
vector<char> input;
int haveNone[maxn]={0};//有空 
int hadFollow[maxn]={0};//已经求过follow 
int hadFirst[maxn]={0};//已经求过first 
int table[maxn][maxn]={0};//分析表 
void getFirst(char X)
{
	int flag=0;
	int index=NotTerminal.find(X);
	if(hadFirst[index]>total)return;
	else hadFirst[index]++;
	for(int i=0;i<total;i++)
	{
		if(g[i].left==X)
		{
			if(haveNone[index]) {
				
				First[index].insert('@');
				if(g[i].right[0]=='@'){
					g[i].first.insert('@');
					continue;
				}	
			}
			//第一个为终结符 
			if(Terminal.find(g[i].right[0])!=string::npos)
				{
					g[i].first.insert(g[i].right[0]);
					First[index].insert(g[i].right[0]);
				}
			else{
			//第一个为非终结符 
				for(int j=0;j<g[i].right.length();j++)
				 {
				 	//直到找到终结符或者不能推出空串的非终结符才停止
					if(Terminal.find(g[i].right[j])!=string::npos)
					{
						g[i].first.insert(g[i].right[j]);
						First[index].insert(g[i].right[j]);	
						break;
					}else{
						int index_temp = NotTerminal.find(g[i].right[j]);
						getFirst(g[i].right[j]);
						set<char>::iterator iter;
						for(iter=First[index_temp].begin();iter!=First[index_temp].end();iter++)
						{
							g[i].first.insert(*iter);
								First[index].insert(*iter);		
						} 
				 		if(!haveNone[index_temp])break;
				 }
				 
			}
		}
		}
	}
}

void getFollow(char X)
{
	int index = NotTerminal.find(X);
	if(hadFollow[index]>total)return;
	else hadFollow[index]++;
	for(int i=0;i<total;i++)
	{
		if(g[i].right.find(X)!=string::npos){
			int index_temp = g[i].right.find(X);
			for(int j=index_temp;j<g[i].right.length();j++)
			{
				if(j+1<g[i].right.length()){
					if(Terminal.find(g[i].right[j+1])!=string::npos){
						Follow[index].insert(g[i].right[j+1]);
						break;
					}else{
						set<char>::iterator iter;
						int temp = NotTerminal.find(g[i].right[j+1]);
						for(iter=First[temp].begin();iter!=First[temp].end();iter++)
						{
							if(*iter!='@')
							Follow[index].insert(*iter);
						}
						if(haveNone[temp])continue;
						else break;	
					}
				}else{
					getFollow(g[i].left);
					int temp=NotTerminal.find(g[i].left);
					set<char>::iterator iter;
					for(iter=Follow[temp].begin();iter!=Follow[temp].end();iter++)
					{
						Follow[index].insert(*iter);
					}
					
				}
			}
		}
	}
} 

void getTable()
{
	for(int i=0;i<total;i++)
	{
		int p = NotTerminal.find(g[i].left);
		if(Terminal.find(g[i].right[0])!=string::npos){
			int q=Terminal.find(g[i].right[0]);
			table[p][q]=i+1;//第一种情况 
		}else if(g[i].right[0]=='@'){
			//第三种情况 
			set<char>::iterator iter;
			for(iter=Follow[p].begin();iter!=Follow[p].end();iter++)
			{
				int temp = Terminal.find(*iter);
				table[p][temp]=i+1; 
			}
		}else{//可能是第一种或者第二种 
				set<char>::iterator iter;
				for(iter=g[i].first.begin();iter!=g[i].first.end();iter++)
				{
					if(*iter=='@')continue;
					int temp = Terminal.find(*iter);
					table[p][temp]=i+1; 
					cout<<p<<" "<<temp<<" "<<i+1<<endl;
				}
				if(g[i].first.find('@')==g[i].first.end()) continue;//不能推出空串	
				//第一种 
				
				// 执行到这里就是第二种 
						for(iter=Follow[p].begin();iter!=Follow[p].end();iter++)
						{
							int temp = Terminal.find(*iter);
							table[p][temp]=i+1; 
						}
		}
	}
}

void getAnalysis(string str)
{
	stack.push_back('#');
    stack.push_back(NotTerminal[0]);
    input.push_back('#');
    for(int i=str.length()-1;i>=0;i--){input.push_back(str[i]);}
	cout<<setw(20)<<"分析栈"<<setw(20)<<"输入串"<<setw(20)<<"所用产生式"<<endl;
    //如果剩余输入串长度不为0，就一直循环
    while(input.size()>0){
        string outputs = "";
        for(int i=0;i<stack.size();i++){
            outputs+=stack[i];
        }
        cout<<setw(20)<<outputs;
        outputs = "";
        for(int i=input.size()-1;i>=0;i--){
            outputs+=input[i];
        }
        cout<<setw(20)<<outputs;
        char a = stack[stack.size()-1];
        char b = input[input.size()-1];
        //如果可以匹配，并且都为# 
        if(a==b&&a=='#'){
            cout<<setw(20)<<"分析成功!"<<endl;
            return;
        }
        //可以匹配
        if(a==b){
            stack.pop_back();
            input.pop_back();
            cout<<setw(20)<<a<<" 匹配"<<endl;
        }else if(table[NotTerminal.find(a)][Terminal.find(b)]>0){
            //如果有值
            int index = table[NotTerminal.find(a)][Terminal.find(b)]-1;
            stack.pop_back();
            if(g[index].right!="@"){
                for(int i=g[index].right.length()-1;i>=0;i--){
                    stack.push_back(g[index].right[i]);
                }
            }
            cout<<setw(20)<<g[index].left<<"->"<<g[index].right<<endl;
        }else{
            cout<<setw(20)<<"错误，它不是LL(1)型文法！"<<endl;
            return;
        }
    }
}

void getOutput()
{
	cout<<"终结符："<<Terminal<<endl;
	cout<<"非终结符："<<NotTerminal<<endl;
	set<char>::iterator iter;
	cout<<"First集合："<<endl;
	for(int i=0;i<NotTerminal.length();i++)
	{
		getFirst(NotTerminal[i]);
		cout<<"First("<<NotTerminal[i]<<")={";
		for(iter=First[i].begin();iter!=First[i].end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
	cout<<"单个候选式First集："<<endl; 
	for(int i=0;i<total;i++)
	{
		cout<<"First("<<g[i].left<<"->"<<g[i].right<<")={";
		for(iter=g[i].first.begin();iter!=g[i].first.end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
	Follow[0].insert('#');
	cout<<"Follow集合"<<endl;
	for(int i=0;i<NotTerminal.length();i++)
	{
		getFollow(NotTerminal[i]);
		cout<<"Follow("<<NotTerminal[i]<<")={";
		for(iter=Follow[i].begin();iter!=Follow[i].end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
} 


void getOutput2()
{
		cout<<setw(10)<<"分析表:"<<endl; 
	cout<<setw(10);
	for(int i=0;i<Terminal.length();i++)
	cout<<Terminal[i]<<setw(5);
	cout<<endl; 
	for(int i=0;i<NotTerminal.length();i++)
	{
		cout<<NotTerminal[i]<<setw(5);
		for(int j=0;j<Terminal.length();j++)
		{
			cout<<table[i][j]<<setw(5);
		}
		cout<<endl;
	}
}

int main()
{
	ifstream read;
	read.open("LL(1)input.txt",ios::app);
	read>>total;
	for(int i=0;i<total;i++)
	{
		//getchar();
		read>>g[i].left>>g[i].right;
		cout<<g[i].left<<"->"<<g[i].right<<endl;
		if(NotTerminal.find(g[i].left)==string::npos)
		{
			NotTerminal+=g[i].left;
		}
		for(int j=0;j<g[i].right.length();j++)
		{
			if(g[i].right[j]=='@'){
				int temp=NotTerminal.find(g[i].left);
				haveNone[temp]=1;
			}
			else if(g[i].right[j]>='A'&&g[i].right[j]<='Z')
				{
					if(NotTerminal.find(g[i].right[j])==string::npos)
					{
					NotTerminal+=g[i].right[j];
					}
				}
			else if(Terminal.find(g[i].right[j])==string::npos)
				{
					Terminal+=g[i].right[j];
				}
			
		}
	}
	getOutput();
	Terminal+='#';
	getTable();
	getOutput2();
	getAnalysis("i+i*i");
	//getAnalysis("vi,i:r") ;
	return 0;
} 
