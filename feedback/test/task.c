#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 100
#define QUE_SIZE 9
#define TRUE 1
#define FALSE 0

/*���Ͽ��� �о�� ������ ���� ���� ����ü*/
typedef struct process_info
{
	int pid;          //���μ��� ���̵�
	int arrive_time;  //�����ð�
	int service_time; //�Ѽ��񽺽ð�
	int remain_time; //���� ������ �ð�
	int execut_time;
}Process_info;

/*��� ����ü --> ��忡�� Process_info�� ������ ����*/
typedef struct struct_node
{
	Process_info data;
	struct struct_node* next;
}Node;

/*ť�� head,tail ǥ�� ����ü*/
typedef struct struct_queue
{
	Node* rear;
	Node* front;
	int time_quantum;
}Queue;

/*Node�� ���������͸� �̿��Ͽ� �� ť�� head�� ��� ���������͹迭 ����ü*/
typedef struct group_node
{
	Queue* node_head[QUE_SIZE];
}Groupque;

typedef struct head
{
	Groupque* h;
}Head;

/*���ϳ��� �о���� �Լ�*/
void read_data();

/*�о�� ���ϳ����� �����ð� ������ �����ϴ� �˰���*/
void sort_data(Process_info p[],int );

/*ť�ʱ�ȭ*/
void QueInit(Head*);

/*ť ���� ������ �迭 �Լ�*/
void Group_que(Groupque);

/*ť�� ������ �����ϴ� �Լ�*/
void Insertqueue(Head* head,Groupque* group,Queue* q,Process_info* p,int,int );

/*���μ��� ���� Ž�� �Լ�*/
void SearchProcess(int process_amount);

/*�� ť�� �ð��Ҵ緮 ���� �Լ�*/
int Squre_time_quantum(int,int);

/*ť�����Լ�()*/
Process_info* Dequeque(Queue q[]);

/*CPU���� �Լ�*/
void ExecuteProcess();

/*fcfs �Լ�*/
int Fcfs();

/*���� �κ� �Լ�*/
void RoundRobin();

/*���� �� ����Լ�()*/
void CPU_finish();

/*�����Լ�()*/
void Degrade(Process_info*);

/*���� ����Ȯ�� �Լ�()*/
int Degrade_Condition();

/*���� �ð��� ���� ���μ��� ã��*/
int SearchEndProcess();

int Q_search();

Process_info* Fcfs_Dequeque(Queue* q);

Process_info p[BUF_SIZE]; //���μ��� ������ ��� ����ü�迭
Process_info p_temp; //Dequeue �ϰ� ��� ����

 Head head;
 Groupque group;
 Queue q[QUE_SIZE];


int time=-1;
int num=2;
int process_amount=0;
int index=0;
int fcfs_index=0;
int flag=0;

int k=0;

int main()
{
	int select_num;
	int k=0;

	head.h=&group;

	read_data();//���Ϸκ��� ������ ��������
	QueInit(&head); //�� ť �ʱ�ȭ
	
	while(1)
	{
		time++;

		if(time==21)
			break;
		ExecuteProcess(); //CPU ����

		SearchProcess(process_amount); //���μ����� �����ð��� Ȯ�� 

		if(SearchEndProcess())
			break;
		
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
		process_amount++; //���� ���μ����� ����
		if(feof(stream))
		{
			break;
		}
		i++;
	}
	//p=(Process_info*)malloc(sizeof(Process_info)*process_amount);
	sort_data(p,process_amount);   // �����ð� ������ �����ϱ�

	fclose(stream);
}

void sort_data(Process_info p[],int process_amount)
{
	/*�������� �˰���*/

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
		if(time==p[i].arrive_time)
		{
			
			//&&(q[index].front->data.execut_time==q[index].time_quantum)
			if(q[0].front!=NULL) //Q_search()//ť�� �Ӱ� ������
			{
				if(q[index].front->data.remain_time!=0)//�����ִ� �ð��� ������
				{
					if(q[index].front->data.execut_time>=q[index].time_quantum)//�ð��Ҵ緮�� �����ٸ�
					{
						q[index].front->data.execut_time=0;// ������ �ð� �ʱ�ȭ
						p_temp=*(Dequeque(q)); //�ش� ��� ��ť
						Degrade(&p_temp);//�����ص״� ��带 ����
						Insertqueue(&head,&group,&q[0],&p[i],time,i);//����ð�-1 �� �����ð��� ������ ť�� ����
					}
					else//�ð� �Ҵ緮�� �ȳ����ٸ�
					{
						Insertqueue(&head,&group,&q[0],&p[i],time,i);
					}
				}
			}
			else
			{
				Insertqueue(&head,&group,&q[0],&p[i],time,i);//����ð�-1 �� �����ð��� ������ ť�� ����
			}
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
	int k=0;
	index=0;

	while(1)
	{
		if(k==9)//��� ť�� �ƹ��͵� ������ �����ϸ� �ȵ�
			return;

		if(q[k].front==NULL)
		{
			k++;
			continue;
		}
		else
		{
			break; //ť�� �Ӷ� �ִٸ�
		}
		
	}
	
	while(1)
	{
		if(q[index].front!=NULL)//ť�� ������� ������
		{
			if(flag==1) //fcfs ������ ���� �ε��� ������ ����
			{
				
				index=fcfs_index;
				break;
			}
			else
			{
				break;
			}
		}
		else  //ť�� ���������
			index++; //������ ť �ε��� ����	
	}

	if(q[8].front!=NULL) //�� ������ ť��
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

		q[index].front->data.remain_time--;//���μ��� ���� �ð� --
		q[index].front->data.execut_time++;//���μ��� ������ Ƚ��++

		if(q[index].front->data.execut_time>=q[index].time_quantum) //�ð������
		{
			if(q[index].front->data.remain_time==0) //�����ִ� �ð��� ������ �� ����Ǹ�
			{
				p_temp=*(Dequeque(q));//�ش� ���μ��� ��忡�� ��������
				CPU_finish(); // �ش� ����� ���μ��� ���� ���
			}
			else if(q[index].front->data.remain_time!=0)//���� �ð��� ������
			{
				if(Degrade_Condition()) //�������ǿ� ������
				{
					q[index].front->data.execut_time=0;// ������ �ð� �ʱ�ȭ
					p_temp=*(Dequeque(q)); //�ش� ��� ��ť
					Degrade(&p_temp);//�����ص״� ��带 ���� 
				}
				else //���� ���ǿ� ���� ������ ������ ����
				{
				
				}
			}
			
		}
		else //�ð� ���ᰡ �ƴϸ�
		{

			if(q[index].front->data.remain_time==0) //�����ִ� �ð��� ������ �� ����Ǹ�
			{
				p_temp=*(Dequeque(q));//�ش� ���μ��� ��忡�� ��������
				CPU_finish(); // �ش� ����� ���μ��� ���� ���
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
	Node* roundrobin_node=(Node*)malloc(sizeof(Node));
	roundrobin_node->next=NULL;
	roundrobin_node->data=p_temp;

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

	/*�ڱ� �ڽ��� ������ ��� ť���� ���μ����� ������ �����ϸ� �ȵǰ�*/
	/*�ڱ� �ڽ��� ������ ��� ť���� ���μ����� �ϳ��� ������ ������*/
	while(1)
	{
		if(index_temp==8) // ������ �ٵ��Ҵٴ¶�
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
		else //�Ӱ� �������
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CPU_finish()
{
	printf("���μ��� id: %d\n",p_temp.pid);
	printf("���μ��� �����ð� : %d��\n",p_temp.arrive_time);
	printf("���μ��� ���� �ð�: %d��\n",p_temp.service_time);
	printf("���μ��� ���� �ð�: %d��\n",time);
	printf("���μ��� ��ȯ �ð� : %d��\n",time-(p_temp.arrive_time));
	printf("���μ��� ����ȭ�� ��ȯ �ð�: %03f��\n\n",(double)(time-(p_temp.arrive_time))/(p_temp.service_time));
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
