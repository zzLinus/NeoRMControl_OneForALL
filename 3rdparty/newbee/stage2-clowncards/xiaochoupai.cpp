#include <ctime>
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cstdio>

#define CARD_COUNT 52

#define pb push_back

using namespace std;

class CARDS {
public:
	int num; //当前牌堆中牌的数量 
	bool vis[CARD_COUNT];//当前卡片是否被访问
	int chu,qi; //出牌/弃牌次数 
	int score; //当前分数 
	int target; //目标
	int* choose;//用来储存选中卡片的索引和总数量
};

int new_card(CARDS* cur);
char* calc_card(int id);
void display_menu(CARDS* cur);
void dfs(int* cur,CARDS* bur);

int main() {
	srand(time(NULL));
	CARDS *now = new CARDS[sizeof(CARDS)];
	now->choose = new int[sizeof(int) * 64];
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
		int *use = new int[sizeof(int) * 64];
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
			cout << "你的有效输入是:";
			for(int i=1;i<=use[0];i++) {
				cout << -use[i];
			}
			cout << endl;
			if(now->qi==0) {
				cout << "已经没有弃牌次数了" << endl;
				continue;
			}
			cout << "你弃掉了这些牌：";
			for(int i=1;i<=use[0];i++) {
				cout << calc_card(now->choose[use[i]]);
			}
			cout << endl;
			for(int i=1;i<=use[0];i++) {
				now->choose[use[i]]=new_card(now);
			}
			now->qi--;
		} else {
			cout << "你的有效输入是";
			for(int i=1;i<=use[0];i++) {
				cout << use[i];
			}
			cout << endl;
			int *pai = new int[sizeof(int)*64];
			pai[0]=use[0];
			for(int i=1;i<=use[0];i++) {
				pai[i]=now->choose[use[i]];
			}
			sort(pai+1,pai+pai[0]+1);
			dfs(pai,now);
			delete[] pai;
			for(int i=1;i<=use[0];i++) {
				now->choose[use[i]]=new_card(now);
			}
			now->chu--;
		}
		if(now->chu==0&&now->score<now->target) {
			cout << "失败了是否重新开始y/n:";
			cout << endl;
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
			cout << "当前回合获胜了" << endl;
			cout << "是否进入下一回合y/n";
			cout << endl;
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
		delete[] use;
	}
	delete[] now;
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
	char *buff = new char[sizeof(char) * 64];
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
	if(buff!=NULL){
		return buff;
		delete[] buff;
	}
}

void display_menu(CARDS* cur) {
	cout << "===================================" << endl;
	cout << "目前拥有的小丑牌: 无" << endl;
	cout << "剩余出牌次数:" << cur->chu <<"       "<< "剩余弃牌次数:" << cur->qi << endl;
	cout << "当前分数:" << cur->score<<"       "<< "目标分数:" << cur->target << endl;
	cout << "当前有的手牌:";
	for(int i=1;i<=8;i++) {
		cout << calc_card(cur->choose[i]) << "  ";
	}
	cout << endl;
	cout<<"序号:          1      2      3      4      5      6      7      8";
	cout << endl;
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
	//同花顺
	if(cur[0]==5) {
		bool flg=0;
		for(int i=2;i<=cur[0];i++) {
			if(cur[i]/13!=cur[1]/13) {
				flg=1;
				break;
			}
		}
		if(flg==0&&cur[5]-cur[1]==4) {
			int sum=100;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			cout << "你打出了同花顺    ";
			cout << "有效牌是: ";
			for(int i=1;i<=cur[0];i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*8" << "=" << sum * 8;
			cout << endl;
			bur->score+=sum*8;
			return;
		}
	}
	//四条 
	sort(cur+1,cur+cur[0]+1,cmp1);
	int *cnt = new int[sizeof(int) * 64];
	for(int i=0;i<13;i++) cnt[i]=0;
	for(int i=1;i<=cur[0];i++) cnt[cur[i]%13]++;
	if(cur[0]>=4) {
		if(cnt[cur[1]%13]==4) {
			int sum=60;
			sum+=calc_Q(cur[1])*4;
			cout << "你打出了四条    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*7" << "=" << sum * 7;
			cout << endl;
			bur->score+=sum*7;
			return;
		} else if(cnt[cur[2]%13]==4) {
			int sum=60;
			sum+=calc_Q(cur[1])*4;
			cout << "你打出了四条    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			delete[] cnt;
			cout << "此次得分是: " << sum << "*7" << "=" << sum * 7;
			cout << endl;
			bur->score+=sum*7;
			return;
		}
	}
	//葫芦
	if(cur[0]==5) {
		if(cnt[cur[1]%13]==3&&cnt[cur[4]%13]==2
			||cnt[cur[1]%13]==2&&cnt[cur[3]%13]==3) {
			int sum=40;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			cout << "你打出了葫芦    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*4" << "=" << sum * 4;
			cout << endl;
			bur->score+=sum*4;
			return;
		}
	}
	//同花
	if(cur[0]==5) {
		bool flg=1;
		for(int i=2;i<=cur[0];i++) {
			if(cur[i]/13!=cur[1]/13) {
				flg=0;
				break;
			}
		}
		if(flg) {
			int sum=35;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			cout << "你打出了同花    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*4" << "=" << sum * 4;
			cout << endl;
			bur->score+=sum*4;
			return;
		}
	}
	//顺子
//	SHUNZI:;
	if(cur[0]==5) {
		bool flg=1;
		if(cur[5]%13-4!=cur[1]%13) {
			flg=0;
		}
		if(flg) {
			int sum=30;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			cout << "你打出了顺子    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*4" << "=" << sum * 4;
			cout << endl;
			bur->score+=sum*4;
			return;
		}
	}
	//三条
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
			cout << "你打出了三条    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*3" << "=" << sum * 3;
			cout << endl;
			bur->score+=sum*3;
			return;
		}
	}
	//对子
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
			cout << "你打出了对子    ";
			cout << "有效牌是: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "此次得分是: " << sum << "*2" << "=" << sum * 2;
			cout << endl;
			bur->score+=sum*2;
			return;
		}
	}
	//高牌
	if(1) {
		int p1=-1;
		if(cur[1]%13==0) {
			p1=1;
		} else p1=cur[0];
		int sum=5;
		sum+=calc_Q(cur[p1]);
		cout << "你打出了高牌    ";
		cout << "有效牌是: ";
		cout << calc_card(cur[p1]);
		cout << endl;
		cout << "此次得分是: " << sum << "*1" << "=" << sum * 1;
		cout << endl;
		bur->score+=sum*1;
		return;
	}
	delete[] cnt;
}