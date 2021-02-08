#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 100
#define QUE_SIZE 9
#define TRUE 1

/*파일에서 읽어온 데이터 관련 저장 구조체*/
typedef struct process_info
{
	int pid;          //프로세스 아이디
	int arrive_time;  //도착시간
	int service_time; //총서비스시간
	int remain_time; //실제 실행한 시간
	int execut_time;
}Process_info;

/*노드 구조체 --> 노드에는 Process_info의 정보가 들어간다*/
typedef struct struct_node
{
	Process_info data;
	struct struct_node* next;
}Node;

/*큐의 head,tail 표시 구조체*/
typedef struct struct_queue
{
	Node* rear;
	Node* front;
	int time_quantum;
}Queue;

/*Node형 이중포인터를 이용하여 각 큐의 head를 담는 이중포인터배열 구조체*/
typedef struct group_node
{
	Queue* node_head[QUE_SIZE];
}Groupque;

typedef struct head
{
	Groupque* h;
}Head;

/*파일내용 읽어오는 함수*/
void read_data();

/*읽어온 파일내용을 도착시간 순으로 정렬하는 알고리즘*/
void sort_data(Process_info p[],int );

/*큐초기화*/
void QueInit(Head*);

/*큐 관리 포인터 배열 함수*/
void Group_que(Groupque);

/*큐에 데이터 삽입하는 함수*/
void Insertqueue(Head* head,Groupque* group,Queue* q,Process_info* p,int,int );

/*프로세스 정보 탐색 함수*/
void SearchProcess(int process_amount);

/*각 큐의 시간할당량 지정 함수*/
int Squre_time_quantum(int,int);

/*큐삭제함수()*/
Process_info* Dequeque(Queue q[]);

/*CPU실행 함수*/
void ExecuteProcess();

/*fcfs 함수*/
void Fcfs();

/*라운드 로빈 함수*/
void RoundRobin(Process_info*);

/*종료 및 출력함수()*/
void CPU_finish(Process_info*);

/*강등함수()*/
void Degrade(Process_info*);

/*강등 조건확인 함수()*/
int Degrade_Condition();


Process_info p[BUF_SIZE]; //프로세스 정보를 담는 구조체배열
Process_info p_temp; //Dequeue 하고 잠시 저장

 Head head;
 Groupque group;
 Queue q[QUE_SIZE];

int select_num;
int time=0;
int num=2;
int process_amount=0;
int index=0;

int k=0;

int main()
{
	int k=0;

	head.h=&group;

	read_data();//파일로부터 데이터 가져오기
	QueInit(&head); //각 큐 초기화
	
	printf("================================================다단계 피드백 큐 스케줄링===================================================\n");
	printf("||                                                                                                                        ||\n");
	printf("||                                             [     1. 시작 하기        ]                                                ||\n");
	printf("||                                                                                                                        ||\n");
	printf("============================================================================================================================\n");

	//printf("\n\n\n                                    ""시작하려면 1번을 누르세요""  :  \n");
	//scanf("%d",&select_num);

	while(1)
	{
		if(time==21)
			break;
		time++;

		SearchProcess(process_amount); //프로세스의 도착시간을 확인 

		ExecuteProcess(); //CPU 실행

		
		//모든 프로세스의 남아있는 시간이 0이면 while문 빠져나오는 함수();
		
	}

	return 0;
}

void read_data()
{
	
	FILE* stream;
	int i=0;
	
	stream=fopen("input.txt","r");
	

	while(1)
	{
		
		fscanf(stream,"%d,%d,%d",&p[i].pid,&p[i].arrive_time,&p[i].service_time);
		p[i].remain_time=p[i].service_time;
		p[i].execut_time=0;
		process_amount++; //들어온 프로세스의 개수
		if(feof(stream))
		{
			break;
		}
		i++;
	}
	//p=(Process_info*)malloc(sizeof(Process_info)*process_amount);
	sort_data(p,process_amount);   // 도착시간 순으로 정렬하기

	fclose(stream);
}

void sort_data(Process_info p[],int process_amount)
{
	/*선택정렬 알고리즘*/

	int maxTime;
	int temp;
	int i,j;

	for(i=0;i<process_amount-1;i++)
	{
		maxTime=i;
		for(j=i+1;j<process_amount;j++)
		{
			if(p[j].arrive_time<p[maxTime].arrive_time)
				maxTime=j;
		}
		temp=p[i].arrive_time;
		p[i].arrive_time=p[maxTime].arrive_time;
		p[maxTime].arrive_time=temp;
	}
}

void QueInit(Head* h)
{
	int i=0;

	for(i=0;i<QUE_SIZE;i++)
	{
		Queue* qq;
		
		group.node_head[i]=&q[i];
		q[i].time_quantum=Squre_time_quantum(i,num);
		//q[i].front=(Node*)malloc(sizeof(Node));
		q[i].front=NULL;
		q[i].rear=NULL;
	}
}

void SearchProcess(int process_amount)
{
	int i=0;

	for(i=0;i<process_amount;i++)
	{
		if(time-1==p[i].arrive_time)
		{
			Insertqueue(&head,&group,&q[0],&p[i],time,i);//현재시간-1 이 도착시간과 같으면 큐에 삽입
		}
	}
}

void Insertqueue(Head* head,Groupque* group,Queue* q,Process_info* pp,int time,int k)
{
	int i=0;

	Node* newnode=(Node*)malloc(sizeof(Node));
	newnode->next=NULL;
	newnode->data=p[k];
	
	//head->h->node_head[0]->rear->next=newnode;
	//head->h->node_head[0]->rear=newnode;
	
	if(q[0].front==NULL)
	{
		q[0].front=newnode;
		q[0].rear=newnode;
	}
	else
	{
		q[0].rear->next=newnode;
		q[0].rear=newnode;
	}
}

int Squre_time_quantum(int n,int num)
{
	num=num*2;
	n--;
	if(n==-1)
	{
		return 1;
	}
	else if(n==0)
	{
		return num/2;
	}
	Squre_time_quantum(n,num);
}

void ExecuteProcess()
{
	index=0;

	while(1)
	{
		if(q[index].front)//큐가 비어있지 않으면
			break;
		else  //큐가 비어있으면
			index++; //수행할 큐 인덱스 선택	
	}

	if(index==8) //맨 마지막 큐면
	{
		RoundRobin(&p_temp);
	}
	else 
	{
		Fcfs();
	}
}

void Fcfs()
{
	if(q[index].front->data.execut_time==q[index].time_quantum)
	{
		 // 시간할당량이랑 수행한 시간이랑 같다는 것의 의미는 더이상 수행이 안되는데
		 // 여기서 만약 남아있는시간이 없으면 종료고 남아있으면 강등할지 말지를 결정해야한다.

		if(q[index].front->data.remain_time!=0)
		{
			if(Degrade_Condition()) //강등조건에 맞으면
			{
				q[index].front->data.execut_time=0;// 실행한 시간 초기화
				p_temp=*(Dequeque(q)); //해당 노드 디큐
				Degrade(&p_temp);//저장해뒀던 노드를 강등 
			}
			else //강등 조건에 맞지 않으면 
			{
				q[index].front->data.execut_time=0;// 실행한 시간 초기화
				q[index].front->data.remain_time--;//프로세스 남은 시간 --
				q[index].front->data.execut_time++;//프로세스 수행한 횟수++

				if(q[index].front->data.remain_time==0) //남아있는 시간이 없으면
				{
					p_temp=*Dequeque(q);  //해당 프로세스 노드에서 제거한후
					CPU_finish(&p_temp);  // 해당 노드의 프로세스 정보 출력
				}
				else //남아있는 시간이 있으면
				{
					dfdf;
				}
			}
		}
		else if(q[index].front->data.remain_time==0)
		{
			p_temp=*(Dequeque(q));//해당 프로세스 노드에서 제거한후
			CPU_finish(&p_temp); // 해당 노드의 프로세스 정보 출력
		}
	}
	else if(q[index].front->data.execut_time<q[index].time_quantum) //시간 할당량 보다 수행한 시간이 작을 때
	{
		q[index].front->data.remain_time--;//프로세스 남은 시간 --
		q[index].front->data.execut_time++;//프로세스 수행한 횟수++

		if(q[index].front->data.remain_time==0) //남아있는 시간이 없으면
		{
			p_temp=*(Dequeque(q));//해당 프로세스 노드에서 제거한후
			CPU_finish(&p_temp); // 해당 노드의 프로세스 정보 출력
		}

		if(q[index].front->data.execut_time==q[index].time_quantum)
		{
			//만약 수행하고나서 시간만료가 되면 
			//강등을 할껀지
			//안할껀지
		}

	}
}

void RoundRobin(Process_info* roundrobin_p)
{
	Node* roundrobin_node=(Node*)malloc(sizeof(Node));
	roundrobin_node->next=NULL;
	roundrobin_node->data=*roundrobin_p;

	q[index].rear->next=roundrobin_node;
	q[index].rear=roundrobin_node;
}

Process_info* Dequeque(Queue* q)
{
	Node* delNode;
	Process_info store;
	delNode=q[index].front;
	store=delNode->data;
	q[index].front=q[index].front->next;
	free(delNode);

	return &store;
}

void Degrade(Process_info* degrade_p)
{
	Node* degrade_node=(Node*)malloc(sizeof(Node));
	degrade_node->next=NULL;
	degrade_node->data=p_temp;

	if(q[index+1].front==NULL)
	{
		q[index+1].front=degrade_node;
		q[index+1].rear=degrade_node;
	}
	else
	{
		q[index+1].rear->next=degrade_node;
		q[index+1].rear=degrade_node;
	}
}

int Degrade_Condition()
{
	
}

void CPU_finish(Process_info* finish_p)
{
	printf("프로세스 id: %d\n",p_temp.pid);
	printf("프로세스 도착시간 : %d초\n",p_temp.arrive_time);
	printf("프로세스 서비스 시간: %d초\n",p_temp.service_time);
	printf("프로세스 종료 시간: %d초\n",time);
	printf("프로세스 반환 시간 : %d초\n",time-(p_temp.arrive_time));
	printf("프로세스 정규화된 반환 시간: %03f초\n\n",(double)(time-(p_temp.arrive_time))/(p_temp.service_time));
}
