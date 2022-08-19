#include<bits/stdc++.h>
using namespace std;

#define MAX 24
#define INIFINITE 201201//定义无限大 
typedef char DataType;//定义数据类型 
typedef int KeyType;//定义关键字类型 
typedef struct node{
	int key;//储存关键字信息 
	struct node *next; 
}HashListNode; 
typedef struct{
	DataType data; 
	KeyType key;
	HashListNode *firstare;//指向的第一片区域 
}HashTable[MAX],Hash;

int H(int key,int n){//除留余数 
	return key%n;
}
void Init(Hash test[],int n){//初始化 
	for(int i=0;i<n;i++){
		test[i].key=INIFINITE;
		test[i].firstare=NULL;//注意这的初始化 
	}
}
void CreatHashList(HashListNode *&link,int key){//创建链地址 （尾插法） 
	HashListNode *p;
	p=new HashListNode;
	p->key=key;
	p->next=NULL;
	if(!link){
		link=p;
	}
	else{
		while(link->next){
			
			link=link->next;
		}
		link->next=p;
	}
		
}
void CreateHash(Hash test[],int num,int n){//创建哈希表 
	bool flag[MAX]={false};//冲突标记 
	for(int i=0;i<num;i++){
		int temp,k;
		cin>>temp;//关键字信息 
		k=H(temp,n);
		if(!flag[k]){//若未冲突 
			test[k].key=temp;
			flag[k]=true; 
		}
		else{//处理冲突 
			CreatHashList(test[k].firstare,temp); // 冲突则采用头插法创建hash表
		}
	}
} 

void SearchNode(Hash test[],int n){//查找结点，查找失败输出-1
	int aim,count=0;
	cin>>aim;//目标 
	int k=H(aim,n);
	bool flag=false;//查找到的标记 
	if(test[k].key==INIFINITE){
		cout<<"-1";
		return;
	}
	
	if(test[k].key==aim){
		flag=true;
		count++;
	}
	else{
		count++;//下面直接从下一片区域找起  这要加一次   
		while(test[k].firstare){
			count++;
			if(test[k].firstare->key==aim){
				flag=true;
				break;
			}
			test[k].firstare=test[k].firstare->next;
		}
	}
	if(!count||!flag){
		cout<<"-1";
	}
	else{
		cout<<k<<","<<count;
	}
	
}
int main(){
	HashTable test;
	int n,num;
	cin>>n>>num;
	Init(test,n);
	CreateHash(test,num,n);
	SearchNode(test,n);
	
	return 0;
}

/*
13
13
16 74 60 43 54 90 46 31 29 88 77 78 79
16
3,1
*/