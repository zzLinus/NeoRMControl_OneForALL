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
	int num; //��ǰ�ƶ����Ƶ����� 
	bool vis[CARD_COUNT];//��ǰ��Ƭ�Ƿ񱻷���
	int chu,qi; //����/���ƴ��� 
	int score; //��ǰ���� 
	int target; //Ŀ��
	int* choose;//��������ѡ�п�Ƭ��������������
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
		int opt=0; //0���� 1����
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
			cout << "�����Ч������:";
			for(int i=1;i<=use[0];i++) {
				cout << -use[i];
			}
			cout << endl;
			if(now->qi==0) {
				cout << "�Ѿ�û�����ƴ�����" << endl;
				continue;
			}
			cout << "����������Щ�ƣ�";
			for(int i=1;i<=use[0];i++) {
				cout << calc_card(now->choose[use[i]]);
			}
			cout << endl;
			for(int i=1;i<=use[0];i++) {
				now->choose[use[i]]=new_card(now);
			}
			now->qi--;
		} else {
			cout << "�����Ч������";
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
			cout << "ʧ�����Ƿ����¿�ʼy/n:";
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
			cout << "��ǰ�غϻ�ʤ��" << endl;
			cout << "�Ƿ������һ�غ�y/n";
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
			strcpy(buff,"����");
			break;
		case 1:
			strcpy(buff,"����");
			break;
		case 2:
			strcpy(buff,"����");
			break;
		case 3:
			strcpy(buff,"÷��");
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
	cout << "Ŀǰӵ�е�С����: ��" << endl;
	cout << "ʣ����ƴ���:" << cur->chu <<"       "<< "ʣ�����ƴ���:" << cur->qi << endl;
	cout << "��ǰ����:" << cur->score<<"       "<< "Ŀ�����:" << cur->target << endl;
	cout << "��ǰ�е�����:";
	for(int i=1;i<=8;i++) {
		cout << calc_card(cur->choose[i]) << "  ";
	}
	cout << endl;
	cout<<"���:          1      2      3      4      5      6      7      8";
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
	//ͬ��˳
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
			cout << "������ͬ��˳    ";
			cout << "��Ч����: ";
			for(int i=1;i<=cur[0];i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*8" << "=" << sum * 8;
			cout << endl;
			bur->score+=sum*8;
			return;
		}
	}
	//���� 
	sort(cur+1,cur+cur[0]+1,cmp1);
	int *cnt = new int[sizeof(int) * 64];
	for(int i=0;i<13;i++) cnt[i]=0;
	for(int i=1;i<=cur[0];i++) cnt[cur[i]%13]++;
	if(cur[0]>=4) {
		if(cnt[cur[1]%13]==4) {
			int sum=60;
			sum+=calc_Q(cur[1])*4;
			cout << "����������    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*7" << "=" << sum * 7;
			cout << endl;
			bur->score+=sum*7;
			return;
		} else if(cnt[cur[2]%13]==4) {
			int sum=60;
			sum+=calc_Q(cur[1])*4;
			cout << "����������    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			delete[] cnt;
			cout << "�˴ε÷���: " << sum << "*7" << "=" << sum * 7;
			cout << endl;
			bur->score+=sum*7;
			return;
		}
	}
	//��«
	if(cur[0]==5) {
		if(cnt[cur[1]%13]==3&&cnt[cur[4]%13]==2
			||cnt[cur[1]%13]==2&&cnt[cur[3]%13]==3) {
			int sum=40;
			for(int i=1;i<=cur[0];i++) {
				sum+=calc_Q(cur[i]);
			}
			cout << "�����˺�«    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*4" << "=" << sum * 4;
			cout << endl;
			bur->score+=sum*4;
			return;
		}
	}
	//ͬ��
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
			cout << "������ͬ��    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*4" << "=" << sum * 4;
			cout << endl;
			bur->score+=sum*4;
			return;
		}
	}
	//˳��
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
			cout << "������˳��    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*4" << "=" << sum * 4;
			cout << endl;
			bur->score+=sum*4;
			return;
		}
	}
	//����
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
			cout << "����������    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*3" << "=" << sum * 3;
			cout << endl;
			bur->score+=sum*3;
			return;
		}
	}
	//����
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
			cout << "�����˶���    ";
			cout << "��Ч����: ";
			for(int i=1;i<=4;i++) {
				cout << calc_card(cur[i]);
			}
			cout << endl;
			cout << "�˴ε÷���: " << sum << "*2" << "=" << sum * 2;
			cout << endl;
			bur->score+=sum*2;
			return;
		}
	}
	//����
	if(1) {
		int p1=-1;
		if(cur[1]%13==0) {
			p1=1;
		} else p1=cur[0];
		int sum=5;
		sum+=calc_Q(cur[p1]);
		cout << "�����˸���    ";
		cout << "��Ч����: ";
		cout << calc_card(cur[p1]);
		cout << endl;
		cout << "�˴ε÷���: " << sum << "*1" << "=" << sum * 1;
		cout << endl;
		bur->score+=sum*1;
		return;
	}
	delete[] cnt;
}