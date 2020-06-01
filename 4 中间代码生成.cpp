#include<bits/stdc++.h>

using namespace std; 
struct Quaternary		//四元式结构
{
	int serial;
	string op;
	string a;
	string b;
	string result;
}Q;
string content[100];
struct Token{
	string value;
	int mode;
}token;
stack<string> num_stack;
stack<string> operator_stack;
vector<Quaternary> Q_;
string temp_op,temp_a,temp_b,temp_result;
string text="";
int number;
int total;
int tag=0;
string keyword[]={"if","else","then"};
string boolword[]={"not","and","or"}; 	
string arithmetic[10]={"+" , "-" , "*" , "/" ,"j<" , "j<=" , "=" , "j>" , "j>="};  
string newtemp[]={"T1","T2","T3","T4","T5","T6","T7","T8","T9","T10","T11"};
int newtemp_index = 0;
int serial=1;
int p=0,i=0;
int sign;

Quaternary Q_generator(int serial,string op,string a,string b,string result){
	Q.serial = serial;
	Q.op = op;
	Q.a = a;
	Q.b = b;
	Q.result = result;
	return Q;
}

void get_Q()
{
	while((!num_stack.empty())&&(!operator_stack.empty()))
	{
		temp_op = operator_stack.top();operator_stack.pop();
		temp_b = num_stack.top();num_stack.pop();
		temp_a = num_stack.top();num_stack.pop();
		if(temp_op=="=")
		{
			temp_result = temp_a;
			temp_a=temp_b;
			temp_b="-";
		}else{
			temp_result=newtemp[newtemp_index++];
			num_stack.push(temp_result);
		}
		Q = Q_generator(serial,temp_op,temp_a,temp_b,temp_result);serial++;
		Q_.push_back(Q);
		//找到真出口
		
		for(int i=0;i<Q_.size();i++)
		{
			if(Q_[i].result=="NULL")
			{
				stringstream ss;
				ss<<serial-1;
				Q_[i].result = ss.str();
				break;
			}
		} 
		
	}
 
}


void get_Output()
{
	while(!Q_.empty())
	{
		cout<<Q_.front().serial<<"("<<Q_.front().op<<","<<Q_.front().a<<","<<Q_.front().b<<","<<Q_.front().result<<")"<<endl;
		Q_.erase(Q_.begin());
	}
}


void scan2(string s)
{
	int flag=1;
	text="";
	while(s[p]==' ')p++;
	if((s[p]>='A'&&s[p]<='Z')||(s[p]>='a'&&s[p]<='z'))
	{
		text+=s[p];p++;
		while((s[p]>='A'&&s[p]<='Z')||(s[p]>='a'&&s[p]<='z')||(s[p]>='0'&&s[p]<='9'))
		{
			if(s[p]>='0'&&s[p]<='9')flag=0;
			text+=s[p];p++;
		}
		if(flag)
		{
			for(int i=0;i<3;i++)
			{
				if(keyword[i]==text)
				{
					sign=3+i;
					return;
				}
			}
			for(int i=0;i<3;i++)
			{
				if(boolword[i]==text)
				{
					sign=20+i;
					return;
				}
			}
		}
		sign=1;
	}else if(s[p]>='0'&&s[p]<='9'){
		number=0;
		while(s[p]>='0'&&s[p]<='9')
		{
			number=number*10+s[p]-'0';
			p++;
		}
		stringstream ss;
		ss<<number;
		text=ss.str();
		sign=2;
	}else{
		text+=s[p];
		switch(s[p])
		{
			case '+':sign=10;break;
			case '-':sign=11;break;
			case '*':sign=12;break;
			case '/':sign=13;break;
			case '<':if(s[p+1]=='=')sign=15;
			        else sign=14;text=arithmetic[sign-10];break;
			case '>':if(s[p+1]=='=')sign=18;
					else sign=17;text=arithmetic[sign-10];break;
			case '=':sign = 16;break;
			case '(':sign = 99;break;
			case ')':sign = 100;break;
		}
		p++; 
	}
}

Token getToken(int i)
{
	while(p!=content[i].length())
	{
		scan2(content[i]);
		token.value = text;
		token.mode = sign;
		return token;
	}
	token.value="END";
	token.mode = -1;
	return token;
}


void run()
{
//	cout<<token.value<<"haha"<<token.mode<<endl;
	if(token.mode<=2){
		num_stack.push(token.value);
	}else if(token.mode>=10&&token.mode<20)
	{
		operator_stack.push(token.value);	
	}else if(token.mode>=20&&token.mode<30)
	{
		operator_stack.push(token.value);
	}else if(token.value=="=")
	{
		operator_stack.push(token.value);
	}else if(token.mode>2&&token.mode<10)
	{
		if(token.mode==3)
		{
			while(token.value!="END")
			{
//				cout<<"haha";
				token = getToken(tag);
				if(token.value=="END")break;
				if(token.value!="then")
				{
					run();
				}else{
					temp_op = operator_stack.top();operator_stack.pop();
					temp_a = num_stack.top();num_stack.pop();
					temp_b = num_stack.top();num_stack.pop();
				//	cout<<temp_a<<" "<<temp_b<<endl;
					Q=Q_generator(serial,temp_op,temp_b,temp_a,"NULL");serial++; 
					Q_.push_back(Q);
				}
				
			}	
		}	
	}
}

int main()
{
	ifstream infile("input2.txt",ios::in);
	int t=0;
	cout<<"待翻译语句："<<endl;
	while(getline(infile,content[t]) )
	{
		cout << content[t] << endl;
		t++;
	}
	total=t;
	cout<<"四元式序列："<<endl; 
	while(tag<total)
	{
		p=0;
		token = getToken(tag);
		while(token.value!="END")
		{
			run();
			token = getToken(tag);
		}
		get_Q();
		get_Output();
		while(!operator_stack.empty())operator_stack.pop();
		while(!num_stack.empty())num_stack.pop();
		tag++;
	}
	return 0;
} 
