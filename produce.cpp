#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>
#include<map>
using namespace std;
char part[20];
char key[15][15]={
"program","const","var","procedure","function","begin",
"end","read","write","if","then","else","for","do","not"
};
char type[4][15]={"integer","real","boolean","char"};
int nowway=0;
int begins=1;
int funflags=0;
map <string,int> funcn; //用来记录函数名 
map <string,int> typen; //用来记录类型 
void checksentence(int start,char s[]);
void printitle(){
	cout<<"#include<stdio.h>"<<endl;
}
int keyword(char pre[]){
	for(int i=0;i<=14;i++){
		if(strcmp(pre,key[i])==0){
			return i;
		}
	}
	return -1;
}
int whichtype(char pre[]){
	for(int i=0;i<=3;i++){
		if(strcmp(pre,type[i])==0){
			return i;
		}
	}
	return -1;
}
int getype (int start,char s[]){
	int zs=0;
		char zsh[20];
		while(s[start]!=';'&&s[start]!=')'){
			if(s[start]==' '){
				start++;
				continue;
			}
			zsh[zs++]=s[start];
			start++;
		}
		zsh[zs]='\0';
		
		return whichtype(zsh);
}
string factor(int start,int endt,char s[]){
//	cout<<"???"<<endl;
	string voidfactor="";
	string funfactor="";
	int voidstart=start;
	int funstart=start;
	int voidpots[10];
	int pots=0;
	int funpots[10][2];
	int fpots=0;
//	cout<<"voidst: "<<voidstart<<" "<<endt<<" "<<endl;
	while(voidstart<=endt&&s[voidstart]!='\0'){
		voidfactor="";
		while(isalpha(s[voidstart])||isdigit(s[voidstart]))
		voidfactor+=s[voidstart++];
		if(funcn[voidfactor]==2){
			pots++;
			voidpots[pots]=voidstart;
		}
		voidstart++;
	}

	while(funstart<=endt&&s[funstart]!='\0'){
		funfactor="";
		if(isalpha(s[funstart])||isdigit(s[funstart]))funpots[fpots][0]=funstart;
		while((isalpha(s[funstart])||isdigit(s[funstart])))
			funfactor+=s[funstart++];
		if(funcn[funfactor]==1&&s[funstart]==':'&&s[funstart+1]=='='){//有返回值的函数改为return
			
			funpots[fpots][1]=funstart+2;
			fpots++;
		}
		funstart++; 
		
	}
//	cout<<"!!!"<<endl;
	//cout<<start<<endl;
	string factors="";
	while(s[start]!='\0'&&start<=endt){
	//	cout<<"have been joined"<<endl;
		for(int i=1;i<=pots;i++){
			if(start==voidpots[i]){
				if(s[start]!='(')factors+="()";
				break;
			}//如果是无返回值的函数 
		}
		for(int i=0;i<fpots;i++){
			if(start==funpots[i][0]){
				factors+="return ";
				start=funpots[i][1];
				break;
			}//如果是有返回值的函数赋值 
		}
		if(s[start]==' '){
			start++;
			continue;
		}
		else if((s[start-1]==' '||start==0)&&s[start]=='n'&&s[start+1]=='o'&&s[start+2]=='t'&&(s[start+3]==' '||s[start+3]=='\0')){
			factors+='!';
			start+=3;
		}//取反 
		else if((s[start-1]==' '||start==0)&&s[start]=='d'&&s[start+1]=='i'&&s[start+2]=='v'&&(s[start+3]==' '||s[start+3]=='\0')){
			factors+='/';
			start+=3;
		}//取整 
		else if((s[start-1]==' '||start==0)&&s[start]=='m'&&s[start+1]=='o'&&s[start+2]=='d'&&(s[start+3]==' '||s[start+3]=='\0')){
			factors+='%';
			start+=3;
		}//取余 
		else if((s[start-1]==' '||start==0)&&s[start]=='a'&&s[start+1]=='n'&&s[start+2]=='d'&&(s[start+3]==' '||s[start+3]=='\0')){
			factors+="&&";
			start+=3;
		}//逻辑与 
		else if((s[start-1]==' '||start==0)&&s[start]=='o'&&s[start+1]=='r'&&(s[start+2]==' '||s[start+2]=='\0')){
			factors+="||";
			start+=2;
		}//逻辑或 
		else if(s[start]=='<'&&s[start+1]=='>'){
			factors+="!=";
			start+=2;
		}//不等
		else if(s[start]=='='&&s[start-1]!=':'){
			factors+="==";
			start++;
		}//恒等号 
		else if(s[start]==':'&&s[start+1]=='='){
			factors+='=';
			start+=2;
		}//赋值号 
		else if(s[start]=='['){
			factors+='[';
			start++;
			while(s[start]!=']'){
				if(s[start]==','){
					factors+=']';
					start++;
				}
				else factors+=s[start++];
			}
			factors+=']';
			start++;
		}//数组变量如a[x,y,z]=a[x][y][z]
		else if(s[start]==';'){
			factors.append(";\n");
			start++;
		}
		else factors+=s[start++];
	}

	return factors;
	
}
void transparam(int start,char s[]){

	cout<<"(";
	start++;
	string votype="";
	string voparam="";
	int endflag=0;
	queue <string> q;
	while(s[start]!=')'){
		if(s[start]==' '){
			start++;
			continue;
		}
		else if(s[start-1]==' '&&s[start]=='v'&&s[start+1]=='a'&&s[start+2]=='r'&&s[start+3]==' '){
			start+=3;
			continue;
		}	
		else if(s[start]==':'){
			q.push(voparam);
			voparam="";
			start++;
		//	while(s[start]==' ')start++;
			int zs=0;
			char zsh[20];
			while(s[start]!=';'&&s[start]!=')'){

				if(s[start]==' '){
					start++;
					continue;
				}
				zsh[zs++]=s[start];
				start++;
			}
			if(s[start]==')')endflag=1;
			zsh[zs]='\0';
			if(s[start]!=')')start++;
			int vtypea=whichtype(zsh);
			if(vtypea==0){
				
				while(!q.empty()){
					cout<<"int "<<q.front();
					q.pop();
					if(!q.empty())cout<<",";
				}
				if(endflag==0)cout<<",";
				
			}
			else if(vtypea==1){
				while(!q.empty()){
					cout<<"float "<<q.front();
					q.pop();
					if(!q.empty())cout<<",";
				}
				if(endflag==0)cout<<",";
			}
			else if(vtypea==3){
				while(!q.empty()){
					cout<<"char "<<q.front();
					q.pop();
					if(!q.empty())cout<<",";
				}
				if(endflag==0)cout<<",";
			}				
		}
		else if(s[start]==','){
			q.push(voparam);
			voparam="";
			start++;
		}
		else voparam+=s[start++];
		
	}
	
	cout<<")";
}
void checkconst(int start,char s[]){
//	queue <string> q;
	string cname="";
	int ctype=1;
	while(s[start]!='='){
		if(ctype==1&&s[start]=='\'')break;//字符串类型，最后没" 
		cname+=s[start++];
		
	}
//	q.push(cname);
	start++;
	if(s[start]=='\'')ctype=0;//表示常量是字符 
	string content="";
	while(s[start]!=';'){
		if(s[start]=='.')ctype=2;
		content+=s[start++];
	}
	
	if(ctype==1){
		cout<<"const int "<<cname<<"="<<content<<";"<<endl;
	}
	else if(ctype==0){
		cout<<"const char "<<cname<<"="<<content<<";"<<endl;
	}
	else if(ctype==2){
		cout<<"const float "<<cname<<"="<<content<<";"<<endl;
	}
}
void checkvar(int start,char s[]){
//	cout<<start;

	queue <string> q;
//	queue <string> arr;
	string vname="";
	int ctype=1;
	while(s[start]!=':'){
	//	cout<<s[start]<<endl;
		if(s[start]==' '){
			start++;
			continue;	
		}
		else if(s[start]==','){
			q.push(vname);
			start++;
		//	cout<<vname<<endl;
			vname="";
		}//字符串类型，最后没" 
		else vname+=s[start++];
		
		
	}
		q.push(vname);//漏了最后一个。 
	start++;//指针指向：后面, 
	while(s[start]==' ')start++;//过滤空格
	int c=0;
	char ch[20];
	int arryflag=0;
	while(s[start]!=';'&&s[start]!='['){
		if(s[start]==' '){
			start++;
			continue;
		}
		
		ch[c++]=s[start];
		start++;
			
		if(s[start]=='['){
			arryflag=1;
			break;
		}
	}
	if(arryflag==0){//不是数组类型
		ch[c]='\0';
		int vtype=whichtype(ch);
		if(vtype==0){
			cout<<"int ";
			while(!q.empty()){
				vname=q.front();
				if(typen.find(vname)==typen.end()){
					typen[vname]=1;
				}
				q.pop();
				cout<<vname;
				if(!q.empty())cout<<",";
			}
		}
		else if(vtype==1){
			cout<<"float ";
			while(!q.empty()){
				vname=q.front();
				if(typen.find(vname)==typen.end()){
					typen[vname]=2;
				}
				q.pop();
				cout<<vname;
				if(!q.empty())cout<<",";
			}
		}
		else if(vtype==3){
			cout<<"char ";
			while(!q.empty()){
				vname=q.front();
				if(typen.find(vname)==typen.end()){
					typen[vname]=3;
				}
				q.pop();
				cout<<vname;
				if(!q.empty())cout<<",";
			}
		}
	}
	else{
		while(s[start]!='.')start++;
		
		start+=2;
		
		string indexa="";//记录下标范围，pascal有负数下标，先不管 
		int dimen=0;//记录数组维度 
		string alldimen[5];
		while(s[start]!=']'){
			if(s[start]==' '){
				start++;
				continue;
			}
			else if(s[start]==','){
				dimen++;
				alldimen[dimen]="";
				alldimen[dimen]+=indexa;
				indexa="";
				while(s[start]!='.')start++;
				start+=2; 
			}
			else{
				indexa+=s[start++];
			}
		}
		dimen++;
		alldimen[dimen]="";
		alldimen[dimen]+=indexa;
		start++;
		while(s[start]==' ')start++;
		if(s[start]=='o')start+=2;
		while(s[start]==' ')start++;
		int zs=0;//查询arrary的类型 
		char zsh[20];
		while(s[start]!=';'){

			if(s[start]==' '){
				start++;
				continue;
			}
			zsh[zs++]=s[start];
			start++;
		}
		zsh[zs]='\0';
	//	cout<<zsh<<endl;
		int vtypea=whichtype(zsh);
		if(vtypea==0){
			cout<<"int ";
			while(!q.empty()){
				vname=q.front();
				q.pop();
				cout<<vname;
				if(typen.find(vname)==typen.end()){
					typen[vname]=1;
				}
				for(int i=1;i<=dimen;i++){
					cout<<"["<<alldimen[i]<<"]";
				}
				if(!q.empty())cout<<",";
			}
		}
		else if(vtypea==1){
			cout<<"float ";
			while(!q.empty()){
				vname=q.front();
				if(typen.find(vname)==typen.end()){
					typen[vname]=2;
				}
				q.pop();
				cout<<vname;
				
				for(int i=1;i<=dimen;i++){
					cout<<"["<<alldimen[dimen]<<"]";
				}
				if(!q.empty())cout<<",";
			}		
		}
		else if(vtypea==3){
			cout<<"char ";
			while(!q.empty()){
				vname=q.front();
				if(typen.find(vname)==typen.end()){
					typen[vname]=3;
				}
				q.pop();
				cout<<vname;

				for(int i=1;i<=dimen;i++){
					cout<<"["<<alldimen[dimen]<<"]";
				}
				if(!q.empty())cout<<",";
			}
		}
	}
	cout<<";";
	cout<<endl;
} 
void checkvoid(int start,char s[]){
	cout<<"void ";
	string voname="";
	while(s[start]!='('&&s[start]!=';'){
		if(s[start]==' '){
			start++;
			continue;
		}
		else voname+=s[start++];
	}
	if(funcn.find(voname)==funcn.end()){
			funcn[voname]=2;
		}
	cout<<voname;//输出函数名称 
	if(s[start]=='('){
		
		transparam(start,s);
		cout<<"{"<<endl;
	}
	else cout<<"(){"<<endl;
	
}
void checkfunction(int start,char s[]){
	string funame="";
	int parambeg;
	int funtype;
	while(s[start]!='('&&s[start]!=';'){
		if(s[start]==' '){
			start++;
			continue;
		}
		else funame+=s[start++];
	}
	
//	cout<<funame;//输出函数名称 
	if(s[start]=='('){
		parambeg=start;
	//	transparam(start,s);
		while(s[start]!=')')start++;
		start+=2;

	}
	else {
		parambeg=-1;//没有参数表 
		start++;
	}
	funtype=getype(start,s);
	if(funtype==0){
		cout<<"int "<<funame;

		if(funcn.find(funame)==funcn.end()){
			funcn[funame]=1;
		}
	}
	else if(funtype==1){
		cout<<"float "<<funame;

		if(funcn.find(funame)==funcn.end()){
			funcn[funame]=2;
		}
	}
	else if(funtype==3){
		cout<<"char "<<funame;

		if(funcn.find(funame)==funcn.end()){
			funcn[funame]=3;
		}
	}
	if(parambeg==-1)cout<<"()";
	else {
		transparam(parambeg,s);
	}
	cout<<"{"<<endl;
	
} 
int checkfor(int start,char s[]){
	
	string fname="";
	string fsvalue="";
	string fevalue="";
	int doflag=0;
	while(s[start]!=':'){
		if(s[start]==' ')start++;
		else fname+=s[start++];
	}
	cout<<"for("<<fname<<"=";
	start+=2;
	while(s[start]!='\0'){
		if(s[start]==' ')start++;
		else if(s[start-1]==' '&&s[start]=='t'&&s[start+1]=='o'&&s[start+2]==' '){
			start+=3;
			break;
		}
		else fsvalue+=s[start++];
	}
	cout<<fsvalue<<";";
	while(s[start]!='\0'){
		if(s[start]==' ')start++;
		else if(s[start-1]==' '&&s[start]=='d'&&s[start+1]=='o'&&s[start+2]==' '){
			start+=3;
			doflag=1;
			break;
		}
		else fevalue+=s[start++];
	}
	cout<<fname<<"!="<<fevalue<<";";
	cout<<fname<<"++)";
	if(doflag){
		//cout<<"{";
		return start;
	}
}
int checkread(int start,char s[]){
	cout<<"scanf(\"";
	string readname="";
	queue <string> q;
	int typ[20],typnum=0;
	int arryflag=0; 
	//start+=4;
	while(s[start]!='(')start++;
		start++;
	//	cout<<"pre: "<<start<<endl;
		while(1){
			if(s[start]==' ')start++;
			else if(s[start]==','||s[start]==')'){		
				if(arryflag){
					arryflag=0;
					if(s[start]==')')break;
					start++;
					continue;
				}
				q.push(readname);
				typnum++;
				typ[typnum]=typen[readname];
				readname="";
				if(s[start]==')')break;
				start++;
				
			}//输入多个变量时 
			else if(s[start]=='['){
				typnum++;
				typ[typnum]=typen[readname];
				while(s[start]!=']'){
					if(s[start]==','){
						readname+="][";
						start++;
					}
					else readname+=s[start++];
				}
				readname+=']';
				q.push(readname);
				readname="";
				start++;
				arryflag=1;
			}//输入数组类型时 
			else readname+=s[start++];
		}
		//q.push(readname);
		//	typnum++;
		//	typ[typnum]=typen[readname];
		//	cout<<"typnum: "<<typnum<<endl;
		for(int i=1;i<=typnum;i++){
			if(typ[i]==1)cout<<"%d";
			else if(typ[i]==2)cout<<"%f";
			else if(typ[i]==3)cout<<"%c";
		}
		cout<<"\",";
		while(!q.empty()){
			cout<<"&"<<q.front();
			q.pop();
			if(!q.empty())cout<<",";
		}
	//	cout<<");"<<endl;
		return start;
}
int checkwrite(int start,char s[]){
	cout<<"printf(\"";
	string outa="";
	string outype="";
//	start+=5;
	int fuarflag=0;
	while(s[start]==' ')start++;
	if(s[start]=='(')start++;
	while(s[start]==' ')start++;
	int prestart=start;//其他东西输出时候 
	//cout<<"start: "<<start<<endl;
	while(1){
		if(s[start]==' ')start++;
		else if(s[start]=='('){//函数 
			if(funcn[outa]==1)cout<<"%d";
			else if(funcn[outa]==2)cout<<"%f";
			else if(funcn[outa]==3)cout<<"%c";
			outa="";
			while(s[start]!=')')start++;
			start++;
			fuarflag=1;
		}
		else if(s[start]=='['){//数组 
			if(typen[outa]==1)cout<<"%d";
				else if(typen[outa]==2)cout<<"%f";
				else if(typen[outa]==3)cout<<"%c";
				outa="";
			while(s[start]!=']'){
				start++;
			}
			start++;
			fuarflag=1;
		}
		else if(s[start]==','||s[start]==')'){
			if(fuarflag){
				fuarflag=0;
				if(s[start]==')')break;
				start++;
				continue;
			}
			if(typen[outa]==1)cout<<"%d";
			else if(typen[outa]==2)cout<<"%f";
			else if(typen[outa]==3)cout<<"%c";
			outa="";
			if(s[start]==')')break;
			start++;
			
		}
		else outa+=s[start++];
	}
	cout<<"\",";
	return prestart;
}
int checkif(int start,char s[]){
	int prestart=start;
	cout<<"if( ";
	while(1){
		start++;
		if(s[start-1]==' '&&s[start]=='t'&&s[start+1]=='h'&&s[start+2]=='e'&&s[start+3]=='n'&&(s[start+4]==' '||s[start+4]=='\0')){
			start+=4;
			break;
		}	
	}
	cout<<factor(prestart,start-5,s);
	cout<<" )";
	while(s[start]==' ')start++;
//	cout<<start<<" 111111111111";
	return start;
}
void checkfirst(int front,char s[]){//翻译过程体 
	int count=0;
	int flagfirst=front;
	while(s[front]==' ')front++;
	if(isalpha(s[front])){
		while( isalpha(s[front]) || isdigit(s[front]) ){
			part[count++]=s[front];
			front++;
		}
		part[count]='\0';//0结束符，分隔单词 
		if(flagfirst==0){
			int mate=keyword(part);
			while(s[front]==' ')front++;
			if(mate==0) return;
			
			else if(mate==1||(mate==-1&&nowway==1)){
				nowway=1;
				if(mate==-1)front=0;
				checkconst(front,s);
			}
			else if(mate==2||(mate==-1&&nowway==2)){
				nowway=2;
				if(mate==-1)front=0;
				checkvar(front,s);
			}
			else if(mate==3&&nowway!=3){
			//	nowway=3;
				funflags=1;
				checkvoid(front,s); 
			}
			else if(mate==4&&nowway!=3){
				funflags=1;
				checkfunction(front,s);
			}
			else if(mate==5){//begin
			if(funflags==0){
				cout<<endl;
				cout<<"int main(){"<<endl;	
			}
				begins*=2;
			//	cout<<begins<<endl;
				checksentence(front,s);
			}
		}
	}	
}
int main(){
	freopen("pascal_test.in","r",stdin);
	freopen("pascal_to_c.c","w",stdout); 
	char s[100];
	int id,end=0;
	printitle();
	while(gets(s)!=NULL){
	//	cout<<s<<endl; 
		//cout<<id<<" "<<end<<endl;
	//	cout<<begins<<endl;
		if(begins==1) checkfirst(end,s);
		else checksentence(end,s);

	}
	return 0;
} 
void checksentence(int start,char s[]){//翻译begin下的语句 
//	cout<<begins<<" 11111111111111111"<<endl;
	while(s[start]!='\0'){
		while(s[start]==' ')start++;
		if((start==0||s[start-1]==' ')&&s[start]=='b'&&s[start+1]=='e'&&s[start+2]=='g'&&s[start+3]=='i'&&s[start+4]=='n'&&(s[start+5]==' '||s[start+5]=='\0')){
			start+=5;
			begins*=2;
			cout<<"{"<<endl;
			checksentence(start,s);
		}
		else if((start==0||s[start-1]==' ')&&s[start]=='e'&&s[start+1]=='n'&&s[start+2]=='d'&&(s[start+3]==' '||s[start+3]=='\0')){
			start+=3;
			begins/=2;
			if(funflags==1)funflags=0;
			while(s[start]==' ')start++;
			if(s[start]==';'){
				cout<<";"<<endl;
				cout<<"}"<<endl;
				
			}
			else if(s[start]=='.'){
				
				cout<<";"<<endl;
				cout<<"return 0;"<<endl;
				cout<<"}"<<endl;
			}
			//checksentence(start,s);
			start++;
		}
		else if((start==0||s[start-1]==' ')&&s[start]=='i'&&s[start+1]=='f'&&(s[start+2]==' '||s[start+2]=='\0')){
			start+=2;
			start=checkif(start,s);//待补充 
			//cout<<";";
		}
		else if((start==0||s[start-1]==' ')&&s[start]=='e'&&s[start+1]=='l'&&s[start+2]=='s'&&s[start+3]=='e'&&(s[start+4]==' '||s[start+4]=='\0')){
			cout<<";"<<endl;
			start+=4;
			cout<<"else ";
			//checksentence(start,s);
		}
		else if((start==0||s[start-1]==' ')&&s[start]=='f'&&s[start+1]=='o'&&s[start+2]=='r'&&(s[start+3]==' '||s[start+3]=='\0')){
			start+=3;
			
			start=checkfor(start,s);
		}
		else if((s[start-1]==' '||start==0)&&s[start]=='r'&&s[start+1]=='e'&&s[start+2]=='a'&&s[start+3]=='d'&&(s[start+4]==' '||s[start+4]=='\0')){
			start+=4;
			start=checkread(start,s);//待补充 
		}
		else if((s[start-1]==' '||start==0)&&s[start]=='w'&&s[start+1]=='r'&&s[start+2]=='i'&&s[start+3]=='t'&&s[start+4]=='e'&&(s[start+5]==' '||s[start+5]=='\0')){
			start+=5;
			start=checkwrite(start,s);//待补充 
		}
		else {//函数调用的判断 
	//	cout<<"111111111";
			int funss=start;
			while(s[start]!='\0')start++;
		//	cout<<"funss: "<<funss<<" "<<"start: "<<start<<endl;
			cout<<factor(funss,start,s);

		}
	} 
}
