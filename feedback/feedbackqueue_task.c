#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define BUF_SIZE 100
#define QUE_SIZE 9
#define TRUE 1
#define FALSE 0

/*파일에서 읽어온 데이터 관련 저장 구조체*/
typedef struct process_info
{
	int pid;          //프로세스 아이디d
	int arrive_time;  //도착시간
	int service_time; //총서비스시간
	int remain_time; //실제 실행한 시간
	int execut_time;
	double turnaround_time;
	int End_time;
	double normalization_turnaround_time;
}Process_info;

/*노드 구조체 --> 노드에는 Process_info의 정보가 들어간다*/
typedef struct struct_node
{
	Process_info data;
	struct struct_node* next;
	struct struct_node* prev;
}Node;

/*큐의 head,tail 표시 구조체*/
typedef struct struct_queue
{
	Node* rear;
	Node* front;
	int time_quantum;
}Queue;

/*파일내용 읽어오는 함수*/
void read_data();

/*읽어온 파일내용을 도착시간 순으로 정렬하는 알고리즘*/
void sort_data(Process_info p[],int );

/*큐초기화*/
void QueInit();

/*큐에 데이터 삽입하는 함수*/
void Insertqueue(int );

/*프로세스 정보 탐색 함수*/
void SearchProcess(int process_amount);

/*각 큐의 시간할당량 지정 함수*/
int Squre_time_quantum(int,int);

/*큐삭제함수()*/
Process_info* Dequeque(Queue q[]);

/*라운드로빈 큐삭제함수*/
Process_info* Dequeque2(Queue q[]);

/*CPU실행 함수*/
void ExecuteProcess();

/*fcfs 함수*/
int Fcfs();

/*라운드 로빈 함수*/
void RoundRobin();

/*종료 및 출력함수()*/
void CPU_finish();
void CPU_END();
void SearchEndProcess();

/*강등함수()*/
void Degrade(Process_info*);

void Round_Robin_insert(Process_info*);

/*강등 조건확인 함수()*/
int Degrade_Condition();

void Change_index(); // 인덱스 변경 함수

Process_info* Fcfs_Dequeque(Queue* q);
Process_info p[BUF_SIZE]; //프로세스 정보를 담는 구조체배열
Process_info print[BUF_SIZE];
Process_info p_temp; //Dequeue 하고 잠시 저장
Process_info p_temp2; //Dequeue 하고 잠시 저장

Queue q[QUE_SIZE];

double printf_data[BUF_SIZE]={0};

int time=-1;
int num=2;
int process_amount=0;
int index=0;
int fcfs_index=0;
int flag=0;
int flag3=0;
int flag4=0;
int rr_index=0;
int end=0;
int k=0;
int P_index;

int main()
{
	int number;

	

	read_data();//파일로부터 데이터 가져오기
	QueInit(); //각 큐 초기화
	
	printf("=====================================다단계 피드백 큐 알고리즘========================================\n\n");
	printf("                                    1. 프로세스 정보 보기\n\n");
	printf("                                    2. 결과보기\n\n");
	printf("                                    3. 종료\n\n");
	printf("======================================================================================================\n\n");
	
	while(1)
	{
		time++;

		if(time==200)
			break;
		ExecuteProcess(); //CPU 실행

		SearchProcess(process_amount); //프로세스의 도착시간을 확인 

	}

	while(1)
	{
		printf("원하시는 수행 번호를 입력하세요([3]종료): ");
		scanf("%d",&number);

		if(number==1)  //프로세스 정보보기
		{
			SearchEndProcess();
		}
		else if(number==2)//결과 출력
		{
			CPU_END();
		}
		else //종료
		{
			break;
		}
	}

	//getch();
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

		if(p[i].pid==0)
		{
			break;
		}

		p[i].remain_time=p[i].service_time;
		p[i].execut_time=0;
		process_amount++; //들어온 프로세스의 개수
	
		i++;
	}

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

void QueInit()
{
	int i=0;

	for(i=0;i<QUE_SIZE;i++)
	{
		q[i].time_quantum=Squre_time_quantum(i,num);
		q[i].front=NULL;
		q[i].rear=NULL;
	}
}

void SearchProcess(int process_amount)
{
	int i=0;

	for(i=0;i<process_amount;i++)
	{
		if(time==p[i].arrive_time)
		{
			if(q[0].front!=NULL) //Q_search()//큐에 머가 있으면
			{
				if(q[index].front->data.remain_time!=0)//남아있는 시간이 있으면
				{
					if(q[index].front->data.execut_time>=q[index].time_quantum)//시간할당량이 끝났다면
					{
						q[index].front->data.execut_time=0;// 실행한 시간 초기화
						p_temp=*(Dequeque(q)); //해당 노드 디큐
						Degrade(&p_temp);//저장해뒀던 노드를 강등
						Insertqueue(i);//현재시간-1 이 도착시간과 같으면 큐에 삽입
					}
					else//시간 할당량이 안끝났다면
					{
						Insertqueue(i);
					}
				}
			}
			else
			{
				Insertqueue(i);//현재시간-1 이 도착시간과 같으면 큐에 삽입
			}
		}
	}
}

void Insertqueue(int k)
{
	int i=0;

	Node* newnode=(Node*)malloc(sizeof(Node)); //새로운 노드 생성
	newnode->next=NULL;
	newnode->prev=NULL;
	newnode->data=p[k];
	
	if(q[0].front==NULL)
	{
		q[0].front=newnode;
		q[0].rear=newnode;
	}
	else
	{
		if(flag3==1)
		{
			p_temp2=*(Dequeque2(q)); //비선점은 시간할당량이 끝날때까지 수행 가능하므로 다시 큐에 넣어준다

			if(q[QUE_SIZE-1].front==NULL)
			{
				q[QUE_SIZE-1].front=newnode;
				q[QUE_SIZE-1].rear=newnode;
			}
			else
			{
				q[QUE_SIZE-1].rear->next=newnode;
				q[QUE_SIZE-1].rear=newnode;
			}
			
			Round_Robin_insert(&p_temp2);
		}
		else
		{
			q[0].rear->next=newnode;
			newnode->prev=q[0].rear;
			q[0].rear=newnode;
		}
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
	int k=0;
	index=0;

	while(1)
	{
		if(k==QUE_SIZE)//모든 큐에 아무것도 없으면 수행하면 안됨
			return;

		if(q[k].front==NULL)
		{
			k++;
			continue;
		}
		else
		{
			break; //큐에 머라도 있다면
		}
		
	}
	
	while(1)
	{
		if(q[index].front!=NULL)//큐가 비어있지 않으면
		{
			if(index==QUE_SIZE-1)
				break;

			if(flag==1) //fcfs 수행을 위한 인덱스 세팅을 위한
			{
				index=fcfs_index;
				break;
			}
			else if(flag4==1)
			{
				index=rr_index;
				break;
			}
			else
				break;
		}
		else  //큐가 비어있으면
			index++; //수행할 큐 인덱스 선택	
	}

	if(index==QUE_SIZE-1) 
	{
		RoundRobin();//맨 마지막 큐면
	}
	else 
	{
		Fcfs();//맨 마지막 큐가 아니면
	}
}

int Fcfs()
{
		flag=0;

		q[index].front->data.remain_time--;//프로세스 남은 시간 --
		p[P_index].remain_time--;
		q[index].front->data.execut_time++;//프로세스 수행한 횟수++

		if(q[index].front->data.execut_time>=q[index].time_quantum) //시간만료시
		{
			if(q[index].front->data.remain_time==0) //남아있는 시간이 없으면 즉 종료되면
			{
				p_temp=*(Dequeque(q));//해당 프로세스 노드에서 제거한후
				CPU_finish(); // 해당 노드의 프로세스 정보 출력
			}
			else if(q[index].front->data.remain_time!=0)//남은 시간이 있으면
			{
				if(Degrade_Condition()) //강등조건에 맞으면
				{
					q[index].front->data.execut_time=0;// 실행한 시간 초기화
					p_temp=*(Dequeque(q)); //해당 노드 디큐
					Degrade(&p_temp);//저장해뒀던 노드를 강등
					Change_index();//인덱스를 다시 세팅
				}
				else //강등 조건에 맞지 않으면 현상태 유지
				{
				
				}
			}
			
		}
		else //시간 만료가 아니면
		{

			if(q[index].front->data.remain_time==0) //남아있는 시간이 없으면 즉 종료되면
			{
				p_temp=*(Dequeque(q));//해당 프로세스 노드에서 제거한후
				CPU_finish(); // 해당 노드의 프로세스 정보 출력
				flag=0;

			}
			else
				{
				flag=1;
				fcfs_index=index;
			}
		}
	

	return 0;
}

void RoundRobin()
{
		flag=0;
		flag3=0;
		flag4=0;

		q[QUE_SIZE-1].front->data.remain_time--;//프로세스 남은 시간 --
		p[P_index].remain_time--;
		q[QUE_SIZE-1].front->data.execut_time++;//프로세스 수행한 횟수++

		if(q[QUE_SIZE-1].front->data.execut_time>=q[QUE_SIZE-1].time_quantum) //시간만료시
		{
			
			if(q[QUE_SIZE-1].front->data.remain_time==0) //남아있는 시간이 없으면 즉 종료되면
			{
				p_temp=*(Dequeque(q));//해당 프로세스 노드에서 제거한후
				CPU_finish(); // 해당 노드의 프로세스 정보 출력
			}
			else if(q[QUE_SIZE-1].front->data.remain_time!=0)//남은 시간이 있으면
			{
				flag3=1;
				q[QUE_SIZE-1].front->data.execut_time=0;// 실행한 시간 초기화
				p_temp2=*(Dequeque(q)); //해당 노드 디큐
				Round_Robin_insert(&p_temp2);//저장해뒀던 노드를 다시 큐에 집어넣기 
			}
			
		}
		else //시간 만료가 아니면
		{
			if(q[QUE_SIZE-1].front->data.remain_time==0) //남아있는 시간이 없으면 즉 종료되면
			{
				p_temp=*(Dequeque(q));//해당 프로세스 노드에서 제거한후
				CPU_finish(); // 해당 노드의 프로세스 정보 출력
			}
			else//계속 수행
			{
				flag4=1;
				rr_index=index;
			}
		}
	
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

Process_info* Dequeque2(Queue* q)
{
	Node* delNode2;
	Process_info store2;
	delNode2=q[index].rear;
	store2=delNode2->data;

	if(q[index].rear->prev==NULL)
	{
		q[index].front=q[index].front->next;
	}
	else
	{
		q[index].rear=q[index].rear->prev;
		q[index].rear->next=NULL;
	}

	free(delNode2);

	return &store2;
}

void Round_Robin_insert(Process_info* insert_p)
{
	Node* roundrobin_node=(Node*)malloc(sizeof(Node));
	roundrobin_node->next=NULL;
	roundrobin_node->prev=NULL;
	roundrobin_node->data=*insert_p;

	if(q[QUE_SIZE-1].front==NULL)
	{
		q[QUE_SIZE-1].front=roundrobin_node;
		q[QUE_SIZE-1].rear=roundrobin_node;
	}
	else
	{
		q[QUE_SIZE-1].rear->next=roundrobin_node;
		roundrobin_node->prev=q[QUE_SIZE-1].rear;
		q[QUE_SIZE-1].rear=roundrobin_node;
	}
}

Process_info* Fcfs_Dequeque(Queue* q)
{
	Node* delNode;
	Process_info store;
	delNode=q[fcfs_index].front;
	store=delNode->data;
	q[index].front=q[fcfs_index].front->next;
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
	int index_temp=0;

	/*자기 자신을 제외한 모든 큐에서 프로세스가 없으면 강등하면 안되고*/
	/*자기 자신을 제외한 모든 큐에서 프로세스가 하나라도 있으면 강등함*/
	while(1)
	{
		if(index_temp==QUE_SIZE) // 끝까지 다돌았다는뜻
		{
			break;
		}
		
		if(index_temp==index)
		{
			if(q[index_temp].rear==q[index_temp].front)
			{
				index_temp++;
				continue;
			}
		}

		if(q[index_temp].front==NULL) 
		{
			index_temp++;
			continue;
		}
		else //머가 들어있음
		{
			return TRUE;
		}
	}

	return FALSE;
}

void Change_index()
{
	int j=0;

	for(j=0;j<process_amount;j++)
	{
		if(q[j].front!=NULL)
		{
			index=j;
			return;
		}
		j++;
	}
}

void CPU_finish()
{
	print[end].pid=p_temp.pid;
	print[end].arrive_time=p_temp.arrive_time;
	print[end].service_time=p_temp.service_time;
	print[end].End_time=time;
	print[end].turnaround_time=(time-(p_temp.arrive_time));
	print[end].normalization_turnaround_time=(time-(p_temp.arrive_time))/(double)(p_temp.service_time);

	end++;

}

void CPU_END()
{
	int i=0;
	printf("\n======================================================================================================\n");
	printf("    프로세스 :  id    도착시간   서비스시간   종료시간      반환시간         정규화된 반환시간\n\n");
	for(i=0;i<process_amount;i++)
	{
	printf("             %5d    %5d   %8d    %9d         %2f           %2f       \n\n",print[i].pid,print[i].arrive_time,print[i].service_time,print[i].End_time,print[i].turnaround_time,print[i].normalization_turnaround_time);
	}
	printf("======================================================================================================\n\n");
}

void SearchEndProcess()
{
	int i=0;

	for(i=0;i<process_amount;i++)
	{
		printf("\n\n==================================들어온 프로세스 정보================================================\n\n");
		printf("                     프로세스 :  id    도착시간   서비스시간   \n\n");
		for(i=0;i<process_amount;i++)
		{
		printf("                %19d    %5d    %9d\n\n",p[i].pid,p[i].arrive_time,p[i].service_time);
		}
		printf("======================================================================================================\n\n");
	}

}
