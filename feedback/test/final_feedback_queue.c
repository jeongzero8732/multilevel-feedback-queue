#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 100
#define QUE_SIZE 1
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

Process_info* Dequeque2(Queue q[]);

/*CPU실행 함수*/
void ExecuteProcess();

/*fcfs 함수*/
int Fcfs();

/*라운드 로빈 함수*/
void RoundRobin();

/*종료 및 출력함수()*/
void CPU_finish();

/*강등함수()*/
void Degrade(Process_info*);

void Round_Robin_insert(Process_info*);

/*강등 조건확인 함수()*/
int Degrade_Condition();

/*서비스 시간이 끝난 프로세스 찾기*/
int SearchEndProcess();

void Change_index();

Process_info* Fcfs_Dequeque(Queue* q);

Process_info p[BUF_SIZE]; //프로세스 정보를 담는 구조체배열
Process_info p_temp; //Dequeue 하고 잠시 저장
Process_info p_temp2; //Dequeue 하고 잠시 저장

 Queue q[QUE_SIZE];


int time=-1;
int num=2;
int process_amount=0;
int index=0;
int fcfs_index=0;
int flag=0;
int flag3=0;

int k=0;

int main()
{
	int select_num;
	int k=0;


	read_data();//파일로부터 데이터 가져오기
	QueInit(); //각 큐 초기화
	
	while(1)
	{
		time++;

		if(time==4000)
			break;
		ExecuteProcess(); //CPU 실행

		SearchProcess(process_amount); //프로세스의 도착시간을 확인 

		//if(SearchEndProcess())
		//	break;
		
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
		if(feof(stream))
		{
			break;
		}

		fscanf(stream,"%d,%d,%d",&p[i].pid,&p[i].arrive_time,&p[i].service_time);
		p[i].remain_time=p[i].service_time;
		p[i].execut_time=0;
		process_amount++; //들어온 프로세스의 개수
	
		i++;
	}
	//p=(Process_info*)malloc(sizeof(Process_info)*process_amount);
	//sort_data(p,process_amount);   // 도착시간 순으로 정렬하기

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

	Node* newnode=(Node*)malloc(sizeof(Node));
	newnode->next=NULL;
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
			p_temp2=*(Dequeque(q));

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
	{//내일 학교가서 여기서부터 수정하면됨 큐가 1개이면 나머지를 보면 안됨.
		if(k==QUE_SIZE-1)//모든 큐에 아무것도 없으면 수행하면 안됨
			return;

		if(q[k].front==NULL)
		{
			if(k==QUE_SIZE-1)//모든 큐에 아무것도 없으면 수행하면 안됨
				return;

			k++;

			continue;
		}
		else
		{
			break; //큐에 머라도 있다면
		}
		
	}
	
	while(1) //인덱스 결정
	{
		if(q[index].front!=NULL)//큐가 비어있지 않으면
		{
			if(index==QUE_SIZE)
				break;

			if(flag==1) //fcfs 수행을 위한 인덱스 세팅을 위한
			{
				index=fcfs_index;
				break;
			}
			else
			{
				break;
			}
		}
		else  //큐가 비어있으면
			index++; //수행할 큐 인덱스 선택	
	}

	if(index==QUE_SIZE-1) //맨 마지막 큐면
	{
		RoundRobin();
	}
	else 
	{
		Fcfs();
	}
}

int Fcfs()
{
		flag=0;

		q[index].front->data.remain_time--;//프로세스 남은 시간 --
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

		q[QUE_SIZE-1].front->data.remain_time--;//프로세스 남은 시간 --
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
	q[index].front=q[index].front->next;
	free(&delNode2);

	return &store2;
}

void Round_Robin_insert(Process_info* insert_p)
{
	Node* roundrobin_node=(Node*)malloc(sizeof(Node));
	roundrobin_node->next=NULL;
	roundrobin_node->data=*insert_p;

	if(q[QUE_SIZE-1].front==NULL)
	{
		q[QUE_SIZE-1].front=roundrobin_node;
		q[QUE_SIZE-1].rear=roundrobin_node;
	}
	else
	{
		q[QUE_SIZE-1].rear->next=roundrobin_node;
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
		if(index_temp==8) // 끝까지 다돌았다는뜻
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

void CPU_finish()
{
	printf("수행 큐: %d\n",index);
	printf("프로세스 id: %d\n",p_temp.pid);
	printf("프로세스 도착시간 : %d초\n",p_temp.arrive_time);
	printf("프로세스 서비스 시간: %d초\n",p_temp.service_time);
	printf("프로세스 종료 시간: %d초\n",time);
	printf("프로세스 반환 시간 : %d초\n",time-(p_temp.arrive_time));
	printf("프로세스 정규화된 반환 시간: %2f초\n\n",(time-(p_temp.arrive_time))/(double)(p_temp.service_time));
}

int SearchEndProcess()
{
	int i=0;
	int flag=1;

	for(i=0;i<process_amount;i++)
	{
		if(p[i].remain_time==0)
		{
			flag=2;
			continue;
		}
		else
			return FALSE;
	}

	return TRUE;
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