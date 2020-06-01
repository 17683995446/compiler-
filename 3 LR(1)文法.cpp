#include<iostream>
#include<vector>
#include<set> 
#include<sstream>
#include<algorithm>
#include<iomanip>
#include<fstream>
#define maxn 100
using namespace std;
struct G{
	char left;
	int dot=0;
	string search; 
	string right;
	bool operator == (const G &t);//比较去重 
};

bool G::operator == (const G &t)
{
    return ((search==t.search)&&(left == t.left) && (right == t.right) && (dot==t.dot));
}

string Action[maxn][maxn];
int Goto[maxn][maxn];
vector<char> tmp_index[maxn];//第i个状态的全部首字母 
string Terminal="";
string NotTerminal="";
set<char> First[maxn];
int book[maxn]={0};
G g[maxn];
int flag=0;
int total;
vector<G> NotTerminal_closure[maxn];
vector<G> I[maxn];
vector<char> stack;
vector<char> input;
int hadFirst[maxn]={0};
int haveNone[maxn]={0};
void getFirst(char X)
{
	int index=NotTerminal.find(X);
	//避免死循环 
	if(hadFirst[index]>total)return;
	else hadFirst[index]++;
	for(int i=0;i<total;i++)
	{
		if(g[i].left==X)
		{
			if(haveNone[index]){//这个非终结符能推出空串 
				First[index].insert('@');
				if(g[i].right[0]=='@'){
					continue;
				}	
			}
			//第一个为终结符 
			if(Terminal.find(g[i].right[0])!=string::npos)
				{
					First[index].insert(g[i].right[0]);
				}
			else{
			//第一个为非终结符 
				for(int j=0;j<g[i].right.length();j++)
				 {
				 	//直到找到终结符或者不能推出空串的非终结符才停止
					if(Terminal.find(g[i].right[j])!=string::npos)
					{
						First[index].insert(g[i].right[j]);	
						break;
					}else{
						int index_temp = NotTerminal.find(g[i].right[j]);
						getFirst(g[i].right[j]);
						set<char>::iterator iter;
						for(iter=First[index_temp].begin();iter!=First[index_temp].end();iter++)
						{
								First[index].insert(*iter);		
						} 
				 		if(!haveNone[index_temp])break;
				 }
				 
			}
		}
		}
	}
}

string getsearchFirst(string str){
	string temp_str="";
	for(int i=0;i<str.length();i++)
	{
		if(Terminal.find(str[i])!=string::npos)
		{
			temp_str+=str[i];
			break;
		}else{
			int index = NotTerminal.find(str[i]);
			set<char>::iterator iter;
			for(iter=First[index].begin();iter!=First[index].end();iter++)
			{
				if(*iter!='@')
				temp_str+=(*iter);
			}
			if(!haveNone[index])break;
		}	
	}

	return temp_str;
}

void getAnalysis(string str)
{
	stack.push_back('#');
	stack.push_back(0);
	input.push_back('#');
	for(int i=str.length()-1;i>=0;i--) input.push_back(str[i]);
	cout<<setw(20)<<"分析栈"<<setw(20)<<"输入串"<<setw(20)<<"动作"<<endl;
	do{
		string output_str="";
		for(int i=0;i<stack.size();i++){
			if(i%2==0)
			 output_str+=stack[i];
			 else
			 {
			 	stringstream ss;
             	ss<<int(stack[i]);
             	output_str+=ss.str();
			 }
		}
		cout<<setw(20)<<output_str;
		output_str="";
		for(int i=input.size()-1;i>=0;i--) output_str+=input[i];
		cout<<setw(20)<<output_str; 
		int stack_top = stack[stack.size()-1];
		char in_top = input[input.size()-1];
		int input_top = Terminal.find(in_top);
		string output = Action[stack_top][input_top];
		cout<<setw(20)<<output<<endl;
		if(output[0]=='S') 
		{
			int move=0;
			for(int i=1;i<output.length();i++)
			move=move*10+int(output[i]-'0');
			stack.push_back(in_top);
			stack.push_back(move);
			input.pop_back();
		}else if(output[0]=='r')
		{
		    int num=0;
			for(int i=1;i<output.length();i++)
			num=num*10+(output[i]-'0');
			int p=g[num-1].right.length()-1;//要规约的产生式的最后一个字符 
			while(p>=0&&stack.size()>=2&&stack[stack.size()-2]==g[num-1].right[p--]){
				//将分析栈中要规约的字符串移除分析栈 
				stack.pop_back();
				stack.pop_back();
			}
			
			int go_to_2 = NotTerminal.find(g[num-1].left);
			int go_to_1 = stack[(stack.size()-1)];
			int go_to = Goto[go_to_1][go_to_2];
			stack.push_back(g[num-1].left);
			stack.push_back(go_to);
		}else if(output=="acc"){
			return;
		}	
	}while(true);
}



void getTable(G item,char X,int num)
{
	if(Terminal.find(X)!=string::npos){
        int t=Terminal.find(X);
        if(item.right.length()==item.dot){
           if(item.left==g[0].left){
            	Action[num][(Terminal.length()-1)]="acc";
			}else{
				stringstream ss;
             	ss<<(flag+1);
				Action[num][t]="S"+ss.str();
				for(int j=0;j<total;j++){
            	if(item.left==g[j].left&&item.right==g[j].right){
            		stringstream ss;
            		ss<<(j+1);
            		for(int k=0;k<=Terminal.length();k++)
					for(int w=0;w<item.search.length();w++)
					if(Terminal[k]==item.search[w]) 
					Action[(flag+1)][k]="r"+ss.str();;
            		break;
				}
				}
			}
            
		}else{
			stringstream ss;
             ss<<(flag+1);
			Action[num][t]="S"+ss.str();
			
			}
	}else{
		
		int t=NotTerminal.find(X);
		Goto[num][t]=flag+1;
		if(item.right.length()==item.dot){
			if(item.left==g[0].left)
			Action[(flag+1)][(Terminal.length()-1)]="acc";
			else{
            		for(int j=0;j<total;j++)
					if(item.left==g[j].left&&item.right==g[j].right){
            		stringstream ss;
            		ss<<(j+1);
            		for(int k=0;k<=Terminal.length();k++) 
					for(int w=0;w<item.search.length();w++)
					if(Terminal[k]==item.search[w])
					Action[(flag+1)][k]="r"+ss.str();;
            		break;
				}
			}
		}			
	}
}



vector<G> go(vector<G> item,char X,int num)
{
	vector<G> tmp;
    for(int i=0;i<item.size();i++)
    {
    	if(item[i].right.length()!=item[i].dot)
            if(item[i].right[(item[i].dot)] == X){
            	//cout<<item[i].right<<"haha"<<endl;	
            	item[i].dot++;
            	getTable(item[i],X,num);
            	
            	string temp_search="";
    			if(item[i].right.length()>item[i].dot+1)
    			temp_search+=getsearchFirst(item[i].right.substr(item[i].dot+1,item[i].right.length()));
				if(temp_search=="")
				temp_search+=item[i].search;
            	
                tmp.push_back(item[i]);
		}
	}
         
//	for(int i=0;i<tmp.size();i++)
//	  cout<<"tmpbegin   "<<tmp[i].left<<"->"<<tmp[i].right<<" "<<tmp[i].dot<<" "<<X<<tmp.empty()<<endl;
    int book[maxn]={0};
	if(!tmp.empty()){
    	for(int i=0;i<tmp.size();i++)
    	{
    		if(NotTerminal.find(tmp[i].right[(tmp[i].dot)])!=string::npos)
    		{
    			string temp_search="";
    			if(tmp[i].right.length()>tmp[i].dot+1)
    			temp_search+=getsearchFirst(tmp[i].right.substr(tmp[i].dot+1,tmp[i].right.length()));
				if(temp_search=="")
				temp_search+=tmp[i].search;
				
    			int t=NotTerminal.find(tmp[i].right[(tmp[i].dot)]);
    			if(!book[t])
    			for(int j=0;j<NotTerminal_closure[t].size();j++)
    			{
    				G temp_g = NotTerminal_closure[t][j];
    				temp_g.search = temp_search;
    			    tmp.push_back(temp_g);
					
					if(NotTerminal.find(temp_g.right[(temp_g.dot)])!=string::npos) 
					if(temp_g.right.length()>temp_g.dot+1)
    			temp_search+=getsearchFirst(temp_g.right.substr(temp_g.dot+1,temp_g.right.length()));	
				 
				}
				book[t]=1;
			}
		}
//		for(int i=0;i<tmp.size();i++)
//	  cout<<"tmpend   "<<tmp[i].left<<"->"<<tmp[i].right<<" "<<tmp[i].dot<<" "<<X<<tmp.empty()<<endl;
	}
	
	for(int i=0;i<=flag;i++)
	{
		
		if(I[i].size()==tmp.size())
		{
			int point=0;
			for(int j=0;j<I[i].size();j++)
			{
			  if(I[i][j]==tmp[j])continue;
			  else {point=1;break;}
			}
		    if(!point){tmp.clear();break;}
		}
	}
	
	return tmp;
}
//求状态规范族 
void get_I()
{
	cout<<"项目规范族："<<endl;
	I[0].push_back(g[0]);
		//非终结符 
		int book[maxn]={0};
		for(int i=0;i<I[0].size();i++)
    	{
    		if(NotTerminal.find(I[0][i].right[(I[0][i].dot)])!=string::npos)
    		{
    			string temp_search="";
    			if(I[0][i].right.length()>I[0][i].dot+1)
    			temp_search+=getsearchFirst(I[0][i].right.substr(I[0][i].dot+1,I[0][i].right.length()));
				if(temp_search=="")
				temp_search+=I[0][i].search;
    			int t=NotTerminal.find(I[0][i].right[(I[0][i].dot)]);
    			if(!book[t])
    			for(int j=0;j<NotTerminal_closure[t].size();j++)
    			{
    			   G temp_g = NotTerminal_closure[t][j];
    			 	temp_g.search=temp_search;
				    I[0].push_back(temp_g);	
				    
				    if(NotTerminal.find(temp_g.right[(temp_g.dot)])!=string::npos) 
				    if(temp_g.right.length()>temp_g.dot+1)
    			temp_search+=getsearchFirst(temp_g.right.substr(temp_g.dot+1,temp_g.right.length()));
    			 
				}
    			book[t]=1;
			}
		}
		tmp_index[0].push_back(g[0].right[0]);
		for(int i=0;i<=flag;i++)
		{
			for(int j=0;j<I[i].size();j++)
			{
				//if(!tmp_index.empty())
				if(I[i][j].dot<I[i][j].right.length()&&I[i][j].right[(I[i][j].dot)]!='@'&&find(tmp_index[i].begin(),tmp_index[i].end(),I[i][j].right[(I[i][j].dot)])==tmp_index[i].end())
				{
					tmp_index[i].push_back(I[i][j].right[(I[i][j].dot)]);
				}
			}
			for(int it=0;it<tmp_index[i].size();it++)
			{
				vector<G> temp=go(I[i],tmp_index[i][it],i);
				if(!temp.empty())
				I[++flag]=temp;
			}
		}
		
		for(int i=0;i<=flag;i++)
		{
			cout<<"I"<<i<<":"<<endl;
			for(int j=0;j<I[i].size();j++){
			 cout<<I[i][j].left<<"->"<<I[i][j].right.substr(0,I[i][j].dot)<<"."<<I[i][j].right.substr(I[i][j].dot,I[i][j].right.length()-I[i][j].dot)<<" "<<I[i][j].dot<<I[i][j].search<<endl; 
		}
	}
	
}    




int main()
{
	ifstream read;
	read.open("LR(1)input.txt",ios::app);
	read>>total;
	cout<<"产生式："<<endl;
	for(int i=0;i<total;i++)
	{
		//getchar();
		read>>g[i].left>>g[i].right;
		cout<<g[i].left<<"-->"<<g[i].right<<endl;
		g[i].dot=0;
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
		NotTerminal_closure[NotTerminal.find(g[i].left)].push_back(g[i]);
	}
	for(int i=0;i<NotTerminal.length();i++)
	{
		getFirst(NotTerminal[i]);
		cout<<"First("<<NotTerminal[i]<<")={";
		set<char>::iterator iter;
		for(iter=First[i].begin();iter!=First[i].end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
	g[0].search="#";
	cout<<"终结符："<<Terminal<<endl;
	cout<<"非终结符："<<NotTerminal<<endl;

	Terminal+="#";
	get_I();
	cout<<"              ";
	for(int i=0;i<Terminal.length();i++)
	{
		cout<<Terminal[i]<<setw(10);
	}
	cout<<endl;
	for(int i=0;i<=flag;i++)
	{
		cout<<i<<setw(5);
		for(int j=0;j<=Terminal.length();j++)
		{
			if(!Action[i][j].empty())cout<<Action[i][j]<<setw(10);
			else cout<<""<<setw(10);
		}
		cout<<endl;
	}
	cout<<" ";
	for(int i=0;i<NotTerminal.length();i++)
	cout<<" "<<NotTerminal[i];
	cout<<setw(10)<<endl;
	for(int i=0;i<=flag;i++)
	{
		cout<<i<<" "; 
		for(int j=0;j<NotTerminal.length();j++)
		cout<<Goto[i][j]<<" ";
		cout<<setw(10)<<endl;
	}
	//getAnalysis("i+i*i");
	//getAnalysis("vi,i:r");
	getAnalysis("vi,i:r");
	return 0;
} 


/*
5
G S
S vI:T
I I,i
I i
T r
*/
