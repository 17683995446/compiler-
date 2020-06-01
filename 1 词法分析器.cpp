#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
char token[50];
char ch;

char *keyword[]={"if","else","while","for","int","float"};
char program[100];
int current=0,q=0;int row=1,sign;
int number=0;
void run(){
	sign=0;
	number=0;
	ch=program[current++];
	q=0;
	while(ch==' ')
	{
		ch=program[current++];
	}
	//字母开头（可能是标识符或者关键字） 
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
	{
		q=0;
		while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
			{
				token[q++]=ch;
				ch=program[current++];
					//含有数字，标识符 
					if((ch>='0'&&ch<='9'))
					{
						sign=1;
					}
			}
			current--;
		//不含数字，可能是关键字 
		if(sign!=1){
			for(int i=0;i<6;i++)
			{
				if(strcmp(token,keyword[i])==0)
				{
					sign=i+3;
				}
			}
		}
		if(sign==0)
		sign=1;
		}else if((ch>='0'&&ch<='9'))
		{
			number=0;
			while((ch>='0'&&ch<='9')){
				number=number*10+ch-'0';
				ch=program[current++];
			}
			current--;
			sign=2;
			if(number>65535)sign=-1;
		}else switch(ch)
		{
			case '+':sign=35;token[q++]=ch;break;
			case '-':sign=36;token[q++]=ch;break;
			case '(':sign=55;token[q++]=ch;break;
			case ')':sign=56;token[q++]=ch;break;
			case ';':sign=57;token[q++]=ch;break;
			case '=':
				ch=program[current++];
				if(ch=='='){
					sign=38;token[q++]=ch;token[q++]=ch;
					break;
				}else{
					sign=37;
					token[q++]='=';
					current--;
					break;
				}
			case '<':
				ch=program[current++];
				if(ch=='='){
					sign=40;token[q++]='<';token[q++]='=';
					break; 
				}else{
					sign=39;
					token[q++]='<';
					current--;
					break;
				}
			case '>':
				ch=program[current++];
				if(ch=='=')
				{
					sign=42;
					token[q++]='>';token[q++]='=';
					break;
				}else{
					sign=41;
					token[q++]='>';
					current--;
					break;
				}
			case '{':sign=58;token[q++]=ch;break;
			case '}':sign=59;token[q++]=ch;break;
			case ',':sign=60;token[q++]=ch;break;
			case '\n':sign=-3;token[q++]=ch;break;
			default:sign=-1;break;
		}
		token[q++]='\0';
}


int main()
{
	
	FILE *fp;
	if((fp=fopen("input.txt","r"))==NULL)
	{
		cout<<"ERROR"<<endl;
		exit(1);
	}
	current=0;
	while(EOF!=(ch=fgetc(fp))){
		program[current++]=ch;
	}
	int total=current; 
	current=0;
	while(current<total){
		run();
		switch(sign)
		{
			case 1:cout<<"("<<sign<<",指向"<<token<<"在符号表的入口)"<<endl;break;
			case 2:cout<<"("<<sign<<",指向"<<number<<"在符号表的入口)"<<endl;break;
			case -1:cout<<row<<"行输入"<<ch<<"错误"<<endl;break;
			case -3:row++;break;
			default:cout<<"("<<sign<<","<<'-'<<")"<<endl;break;
		}
	}	
	return 0;
}
