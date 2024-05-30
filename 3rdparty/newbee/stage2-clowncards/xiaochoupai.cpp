#include <ctime>
#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
#include<algorithm>
#define CARD_COUNT 52

#define pb push_back

using namespace std;

typedef struct cards {
	int num; //当前牌堆中牌的数量 
	bool vis[CARD_COUNT];//游戏进行的状态 
	int chu,qi; //出牌/弃牌次数 
	int score; //当前分数 
	int target; //目标分数 
	int* choose;//牌的序号和花色 
}CARDS;

int new_card(CARDS* cur);
char* calc_card(int id);
void display_menu(CARDS* cur);
void dfs(int* cur,CARDS* bur);

int main() {
	srand(time(NULL));
	CARDS* now=(CARDS*)malloc(sizeof(CARDS));
	now->choose=(int*)malloc(sizeof(int)*64);
	for(int i=0;i<CARD_COUNT;i++) {
		now->vis[i]=0;
	}
	now->num=CARD_COUNT;
	now->chu=now->qi=3;
	now->score=0;
	now->target=300;
	now->choose[0]=0;
	for(int i=0;i<8;i++) {
		now->choose[++now->choose[0]]=new_card(now);
	}
	while(true) {
		display_menu(now);
		char ch=' ';
		int opt=0; //0出牌 1弃牌 
		int* use=(int*)malloc(sizeof(int)*64);
		use[0]=0;
		while(ch!='\n') {
			ch=getchar();
			if(isdigit(ch)) {
				use[++use[0]]=(int)(ch-'0');
			} else if(ch=='-') {
				opt=1;
			}
		}
		if(opt==1) {
			printf("你的有效输入是: ");
			for(int i=1;i<=use[0];i++) {
				printf("%d ",-use[i]);
			}
			printf("\n");
			if(now->qi==0) {
				printf("已经没有弃牌次数了\n");
				continue;
			}
			printf("你弃置了这些牌: ");
			for(int i=1;i<=use[0];i++) {
				printf("%s ",calc_card(now->choose[use[i]]));
			}
			printf("\n");
			for(int i=1;i<=use[0];i++) {
				now->choose[use[i]]=new_card(now);
			}
			now->qi--;
		} else {
			printf("你的有效输入是: ");
			for(int i=1;i<=use[0];i++) {
				printf("%d ",use[i]);
			}
			printf("\n");
			int* pai=(int*)malloc(sizeof(int)*64);
			pai[0]=use[0];
			for(int i=1;i<=use[0];i++) {
				pai[i]=now->choose[use[i]];
			}
			sort(pai+1,pai+pai[0]+1);
			dfs(pai,now);
			for(int i=1;i<=use[0];i++) {
				now->choose[use[i]]=new_card(now);
			}
			now->chu--;
		}
		if(now->chu==0&&now->score<now->target) {
			printf("失败了是否重新开始y/n: ");
			char ch=' ';
			while(!islower(ch)) ch=getchar();
//			getchar();
			if(ch=='y') {
				for(int i=0;i<CARD_COUNT;i++) {
					now->vis[i]=0;
				}
				now->num=CARD_COUNT;
				now->chu=now->qi=3;
				now->score=0;
				now->target=300;
				now->choose[0]=0;
				for(int i=0;i<8;i++) {
					now->choose[++now->choose[0]]=new_card(now);
				}
			} else {
				break;
			}
		} else if(now->score>=now->target) {
			printf("当前回合获胜了\n");
			printf("是否进入下一回合y/n: ");
			char ch=' ';
			while(!islower(ch)) ch=getchar();
//			getchar();
			if(ch=='y') {
				for(int i=0;i<CARD_COUNT;i++) {
					now->vis[i]=0;
				}
				now->num=CARD_COUNT;
				now->chu=now->qi=3;
				now->score=0;
				now->target=300;
				now->choose[0]=0;
				for(int i=0;i<8;i++) {
					now->choose[++now->choose[0]]=new_card(now);
				}
			} else {
				break;
			}
		}
		getchar();
		system("cls");
	}
	return 0;
}

int new_card(CARDS* cur) {
	int oo=rand()%cur->num;
	for(int i=0;i<CARD_COUNT;i++) {
		if(cur->vis[i]==1) {
			continue;
		} else {
			if(oo==0) {
				cur->vis[i]=1;
				cur->num--;
				return i;
			}
			oo--;
		}
	}
	return 0;
}

char* calc_card(int id) {
	char* buff=(char*)malloc(sizeof(char)*64);
	switch(id/13) {
		case 0:
			strcpy(buff,"红桃");
			break;
		case 1:
			strcpy(buff,"方块");
			break;
		case 2:
			strcpy(buff,"黑桃");
			break;
		case 3:
			strcpy(buff,"梅花");
			break; 
	}
	switch(id%13) {
		case 0:
			buff=strcat(buff,"A");
			break;
		case 1:
			buff=strcat(buff,"2");
			break;
		case 2:
			buff=strcat(buff,"3");
			break;
		case 3:
			buff=strcat(buff,"4");
			break;
		case 4:
			buff=strcat(buff,"5");
			break;
		case 5:
			buff=strcat(buff,"6");
			break;
		case 6:
			buff=strcat(buff,"7");
			break;
		case 7:
			buff=strcat(buff,"8");
			break;
		case 8:
			buff=strcat(buff,"9");
			break;
		case 9:
			buff=strcat(buff,"10");
			break;
		case 10:
			buff=strcat(buff,"J");
			break;
		case 11:
			buff=strcat(buff,"Q");
			break;
		case 12:
			buff=strcat(buff,"K");
			break;
	}
	return buff;
}

void display_menu(CARDS* cur) {
	printf("===================================\n");
	printf("目前拥有的小丑牌: 无\n");
	printf("剩余出牌次数:%d          剩余弃牌次数:%d\n",cur->chu,cur->qi);
	printf("当前分数:%d       目标分数:%d\n",cur->score,cur->target);
	printf("当前有的手牌:");
	for(int i=1;i<=8;i++) {
		printf("%s  ",calc_card(cur->choose[i]));
	}
	printf("\n");
	printf("序号:          1      2      3      4      5      6      7      8\n");
	return;
}

int calc_Q(int tmp) {
	if(tmp%13==0) return 11;
	if(tmp%13>=10) return 10;
	return tmp%13+1;
}

bool cmp1(int pp,int qq) {
	return pp%13<qq%13;
}

void dfs(int* cur,CARDS* bur) {
	for(int i=1;i<=cur[0];i++) {
		cout<<cur[i]<<" "<<calc_card(cur[i])<<endl;
	}
	//同花顺
	TONGHUASHUN:;
	if(cur[0]==5) {
		for(int i=2;i<=cur[0];i++) {
			if(cur[i]/13!=cur[1]/13) {
				goto SITIAO;
			}
		}
		if(cur[5]-cur[1]==4) {
			int sum=100;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			printf("你打出了同花顺    ");
			printf("有效牌是: ");
			for(int i=1;i<=cur[0];i++) {
				printf("%s ",calc_card(cur[i]));
			}
			printf("\n");
			printf("此次得分是: %d * 8 = %d\n",sum,sum*8);
			bur->score+=sum*8;
			return;
		}
	}
	//四条 
	SITIAO:;
//	sort(cur+1,cur+cur[0]+1,[](int pp,int qq) {
//		return pp%13<qq%13;	
//	});
	sort(cur+1,cur+cur[0]+1);
	int* cnt=(int*)malloc(sizeof(int)*13);
	for(int i=0;i<13;i++) cnt[i]=0;
	for(int i=1;i<=cur[0];i++) cnt[cur[i]%13]++;
	if(cur[0]>=4) {
		if(cnt[cur[1]%13]==4) {
			int sum=60;
			sum+=calc_Q(cur[1])*4;
			printf("你打出了四条    ");
			printf("有效牌是: ");
			for(int i=1;i<=4;i++) {
				printf("%s ",calc_card(cur[i]));
			}
			printf("\n");
			printf("此次得分是: %d * 7 = %d\n",sum,sum*7);
			bur->score+=sum*7;
			return;
		} else if(cnt[cur[2]%13]==4) {
			int sum=60;
			sum+=calc_Q(cur[1])*4;
			printf("你打出了四条    ");
			printf("有效牌是: ");
			for(int i=2;i<=5;i++) {
				printf("%s ",calc_card(cur[i]));
			}
			printf("\n");
			printf("此次得分是: %d * 7 = %d\n",sum,sum*7);
			bur->score+=sum*7;
			return;
		}
	}
	//葫芦
	HULU:;
	if(cur[0]==5) {
		if(cnt[cur[1]%13]==3&&cnt[cur[4]%13]==2
			||cnt[cur[1]%13]==2&&cnt[cur[3]%13]==3) {
			int sum=40;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			printf("你打出了葫芦    ");
			printf("有效牌是: ");
			for(int i=1;i<=cur[0];i++) {
				printf("%s ",calc_card(cur[i]));
			}
			printf("\n");
			printf("此次得分是: %d * 4 = %d\n",sum,sum*4);
			bur->score+=sum*4;
			return;
		}
	}
	//同花
	TONGHUA:;
	if(cur[0]==5) {
		for(int i=2;i<=cur[0];i++) {
			if(cur[i]/13!=cur[1]/13) {
				goto SHUNZI;
			}
		}
		int sum=35;
		for(int i=1;i<=cur[0];i++) {
			sum+=calc_Q(cur[i]);
		}
		printf("你打出了同花    ");
		printf("有效牌是: ");
		for(int i=1;i<=cur[0];i++) {
			printf("%s ",calc_card(cur[i]));
		}
		printf("\n");
		printf("此次得分是: %d * 4 = %d\n",sum,sum*4);
		bur->score+=sum*4;
		return;
	}
	//顺子
	SHUNZI:;
	if(cur[0]==5) {
		if(cur[5]%13-4!=cur[1]%13) {
			goto SANTIAO;
		}
		int sum=30;
		for(int i=1;i<=cur[0];i++) {
			sum+=calc_Q(cur[i]);
		}
		printf("你打出了顺子    ");
		printf("有效牌是: ");
		for(int i=1;i<=cur[0];i++) {
			printf("%s ",calc_card(cur[i]));
		}
		printf("\n");
		printf("此次得分是: %d * 4 = %d\n",sum,sum*4);
		bur->score+=sum*4;
		return;
	}
	//三条
	SANTIAO:;
	if(cur[0]>=3) {
		int p1=-1,p2=-1;
		for(int i=1;i<=cur[0]-2;i++) {
			if(cnt[cur[i]%13]==3) {
				p1=i; p2=i+2; break;
			}
		}
		if(p1!=-1) {
			int sum=30;
			for(int i=p1;i<=p2;i++) {
				sum+=calc_Q(cur[i]);
			}
			printf("你打出了三条    ");
			printf("有效牌是: ");
			for(int i=p1;i<=p2;i++) {
				printf("%s ",calc_card(cur[i]));
			}
			printf("\n");
			printf("此次得分是: %d * 3 = %d\n",sum,sum*3);
			bur->score+=sum*3;
			return;
		}
	}
	//对子
	DUIZI:;
	if(cur[0]>=2) {
		int p1=-1,p2=-1;
		for(int i=1;i<=cur[0]-1;i++) {
			if(cnt[cur[i]%13]==2) {
				p1=i; p2=i+1; break;
			}
		}
		if(p1!=-1) {
			int sum=20;
			for(int i=p1;i<=p2;i++) {
				sum+=calc_Q(cur[i]);
			}
			printf("你打出了对子    ");
			printf("有效牌是: ");
			for(int i=p1;i<=p2;i++) {
				printf("%s ",calc_card(cur[i]));
			}
			printf("\n");
			printf("此次得分是: %d * 2 = %d\n",sum,sum*2);
			bur->score+=sum*2;
			return;
		}
	}
	//高牌
	GAOPAI:;
	if(1) {
		int p1=-1;
		if(cur[1]%13==0) {
			p1=1;
		} else p1=cur[0];
		int sum=5;
		sum+=calc_Q(cur[p1]);
		printf("你打出了高牌    ");
		printf("有效牌是: ");
		printf("%s ",calc_card(cur[p1]));
		printf("\n");
		printf("此次得分是: %d * 1 = %d\n",sum,sum*1);
		bur->score+=sum*1;
		return;
	}
}
//一看到这个题目就很懵不知道从何开始下手，自己又去学了一下算法和数据结构，由于目前写c的代码写的比较好，所以就用c来写了，目前只实现了第一阶段，后面的小丑牌还在思考怎么实现 
