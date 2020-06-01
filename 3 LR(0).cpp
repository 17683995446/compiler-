#include<iostream>
#include<vector>
#include<set>
#include<fstream> 
#include<sstream>
#include<algorithm>
#include<iomanip>
#define maxn 100
using namespace std;
struct G{
	char left;
	int dot=0;
	string right;
};
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
	if(hadFirst[index]>total)return;
	else hadFirst[index]++;
	for(int i=0;i<total;i++)
	{
		if(g[i].left==X)
		{
			if(haveNone[index]) {
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
		cout<<input_top;
		string output = Action[stack_top][input_top];
		cout<<setw(20)<<output<<endl;
		if(output[0]=='S') 
		{
			int move=0;
			for(int i=1;i<output.length();i++)
			move=move*10+(output[i]-'0');
			stack.push_back(in_top);
			stack.push_back(move);
			input.pop_back();
		}else if(output[0]=='r')
		{
		    int num=0;
			for(int i=1;i<output.length();i++)
			num+=num*10+(output[i]-'0');
			int p=g[num-1].right.length()-1;
			
			while(p>=0&&stack.size()>=2&&stack[stack.size()-2]==g[num-1].right[p--]){
				stack.pop_back();
				stack.pop_back();
			}
			
			int go_to_2 = NotTerminal.find(g[num-1].left);
			int go_to_1 = stack[(stack.size()-1)];
			int go_to = Goto[go_to_1][go_to_2];
			stack.push_back(g[num-1].left);
			stack.push_back(go_to);
		}else if(output=="acc"){
			//cout<<setw(20)<<"配对成功！"; 
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
    			
				
    			int t=NotTerminal.find(tmp[i].right[(tmp[i].dot)]);
    			if(!book[t])
    			for(int j=0;j<NotTerminal_closure[t].size();j++)
    			{
    				G temp_g = NotTerminal_closure[t][j];
    			    tmp.push_back(temp_g);	
				}
				book[t]=1;
			}
		}
//		for(int i=0;i<tmp.size();i++)
//	  cout<<"tmpend   "<<tmp[i].left<<"->"<<tmp[i].right<<" "<<tmp[i].dot<<" "<<X<<tmp.empty()<<endl;
	}
	
	return tmp;
}
//求状态规范族 
void get_I()
{
	I[0].push_back(g[0]);
		//非终结符 
		int book[maxn]={0};
		for(int i=0;i<I[0].size();i++)
    	{
    		if(NotTerminal.find(I[0][i].right[(I[0][i].dot)])!=string::npos)
    		{
    			
    			int t=NotTerminal.find(I[0][i].right[(I[0][i].dot)]);
    			if(!book[t])
    			for(int j=0;j<NotTerminal_closure[t].size();j++)
    			{
    			   G temp_g = NotTerminal_closure[t][j];
				    I[0].push_back(temp_g);	
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
				tmp_index[i].push_back(I[i][j].right[(I[i][j].dot)]);
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
			 cout<<I[i][j].left<<"->"<<I[i][j].right.substr(0,I[i][j].dot)<<"."<<I[i][j].right.substr(I[i][j].dot,I[i][j].right.length()-I[i][j].dot)<<" "<<I[i][j].dot<<endl; 
		}
	}
	
}    




int main()
{
	ifstream read;
	read.open("LR(0)input.txt",ios::app);
	read>>total;
	for(int i=0;i<total;i++)
	{
		//getchar();
		read>>g[i].left>>g[i].right;
		cout<<g[i].left<<"->"<<g[i].right<<endl;
		g[i].dot=0;
		if(NotTerminal.find(g[i].left)==string::npos)
		{
			NotTerminal+=g[i].left;
		}
		for(int j=0;j<g[i].right.length();j++)
		{
			if(g[i].right[j]=='@'){
				int temp=NotTerminal.find(g[i].left);
				book[temp]=1;
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
	cout<<"终结符："<<Terminal<<endl;
	cout<<"非终结符："<<NotTerminal<<endl;
	for(int i=0;i<NotTerminal.length();i++)
	{
		cout<<NotTerminal_closure[i].size();
		for(int j=0;j<NotTerminal_closure[i].size();j++)
		cout<<NotTerminal_closure[i][j].left<<"->"<<NotTerminal_closure[i][j].right<<" ";
		cout<<endl;
	}
	Terminal+="#";
	get_I();
	for(int i=0;i<=flag;i++)
	{
		for(int j=0;j<=Terminal.length();j++)
		{
			if(!Action[i][j].empty())cout<<"i="<<i<<",j="<<j<<","<<Action[i][j]<<endl;
		}
	}
	for(int i=0;i<NotTerminal.length();i++)
	cout<<NotTerminal[i]<<" ";
	cout<<"#"<<endl;
	for(int i=0;i<=flag;i++)
	{
		for(int j=0;j<=NotTerminal.length();j++)
		cout<<Goto[i][j]<<" ";
		cout<<endl;
	}
	//getAnalysis("aedaec") ;
	getAnalysis("vi,i:r") ;
	return 0;
} 
