#ifndef _FEED
#define _FEED

typedef struct node
{
	int id;
	int serv;
	int avr;
	Node* next;
}Node;

typdef struct Que
{
	Node* link;
}

void Insert(RR* R,char* hong)
{
	int i=0;
	int j=0;
	char jun[20];
	char* mong=NULL;

	a=a+1;
	mong=strtok(hong,",");

	while(mong!=NULL)
	{
		if(i==0){
			R->id=atoi(mong);
			mong=strtok(NULL,",");
			i++;
		}
		if(i==1)
		{
			R->avr_time=atoi(mong);
			mong=strtok(NULL,",");
			i++;
		}
		if(i==2)
		{
			R->serv_time=atoi(mong);
			break;
		}
		
	}
}



#endif