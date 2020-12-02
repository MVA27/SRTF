#include<stdio.h>
#include<stdlib.h>

struct node {
int Process;
int ArrivalTime;
int BurstTime;
int TurnAroundTime;
int WaitingTime;
int CompletionTime;
struct node *next;
};
struct node *start = NULL;

int timeQuantum = 0;
int saveIndividualCompletionTime = 0;
int burstTimeStatus = 0;


int totalBurstTime;
int minimumArrivalTime;

int startTime = 0;
int currentTime = 0;

int recoredrAray[100];
struct node *listQueue = NULL;


void deletionAtTheEnd()
{
    struct node *temp = listQueue;
    struct node *previousNode;

    while(temp->next != NULL)
    {
        previousNode = temp;
        temp = temp->next;
    }
    previousNode->next=NULL;
    free(temp);
}

void deletionOfSpecificValue(int value)
{
    struct node *temp = listQueue;
    struct node *nextNode;
    while(temp->next->Process != value )
    {
        temp = temp->next;
    }
    nextNode = temp->next;
    temp->next = nextNode->next;
    nextNode->next=NULL;
    free(nextNode);
}

void core(int arrayIndex){


    int i,m;
    int sizeOfArray = 0;

    struct node *check = start;
    for(i=0;i<arrayIndex;i++)
    {
        while(check != NULL)
        {
            if(check->Process == recoredrAray[i])
            {
                struct node *p = (struct node *)malloc(sizeof(struct node));
                p->Process = check->Process;
                p->ArrivalTime = check->ArrivalTime;
                p->BurstTime = check->BurstTime;
                p->TurnAroundTime = 0;
                p->WaitingTime = 0;
                p->CompletionTime = 0;
                p->next=NULL;

                if(listQueue == NULL){
                    listQueue = p;
                }

                else
                {
                    struct node *temp = listQueue;
                    while(temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = p;
                }

            }
            check = check->next;
        }
        check = start;
    }

    check = listQueue;
    int minimumBurstTime = check->BurstTime;
    int minimumBurstTimeProcessNo = check->Process;
    while(check != NULL)
    {
        if(check->BurstTime < minimumBurstTime)
        {
            minimumBurstTime = check->BurstTime;
            minimumBurstTimeProcessNo = check->Process;
        }
        check = check->next;
    }


    check = listQueue;
    while(check != NULL)
    {
        if(check->Process == minimumBurstTimeProcessNo)
        {
            currentTime = currentTime + timeQuantum;
            check->BurstTime = check->BurstTime - timeQuantum;


            if(check->BurstTime <= 0)
            {
                check->BurstTime = 0; 
				burstTimeStatus = 1; 
				saveIndividualCompletionTime = currentTime;	
            }
        }
        check = check->next;
    }

    printf(" < %d > %d ",minimumBurstTimeProcessNo,currentTime);

	check = start;
    while(check != NULL)
    {
        if(check->Process == minimumBurstTimeProcessNo && burstTimeStatus == 1) 
        {
			check->CompletionTime = saveIndividualCompletionTime;
			burstTimeStatus = 0;
        }
        check = check->next;
    }


    check = listQueue;
    while(check != NULL)
    {
        if(check->BurstTime == 0)
        {
            if(check == listQueue && check->next == NULL)
            {
                //THIS IS FIRST NODE AND LAST NODE
                struct node *deletionPointer;
                deletionPointer = check;
                listQueue = NULL;
                free(deletionPointer);
            }

            else if(check == listQueue)
            {
                //THIS IS FIRST NODE
                struct node *deletionPointer;
                deletionPointer = check;
                listQueue = listQueue->next;
                free(deletionPointer);
            }

            else if(check->next == NULL)
            {
                //THIS IS LAST NODE
                deletionAtTheEnd();
            }

            else{
                //THIS IS INTERMEDIATE NODE
                deletionOfSpecificValue(check->Process);
            }

        }
        check = check->next;
    }

}

void createNode()
{
    struct node *temp = start;
    struct node *p = (struct node *)malloc(sizeof(struct node));
    printf("\nEnter Process Number : ");
    scanf("%d",&p->Process);

    printf("\nEnter Arrival Time : ");
    scanf("%d",&p->ArrivalTime);

    printf("\nEnter Burst Time : ");
    scanf("%d",&p->BurstTime);

    p->TurnAroundTime = 0;
    p->WaitingTime = 0;
    p->CompletionTime = 0;
    p->next=NULL;

    if(start == NULL){
        start = p;
    }

    else{
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = p;
    }
}


void main()
{
    int n,i;
    printf("\nENTER THE NUMBER OF PROCESS : ");
    scanf("%d",&n);

    timeQuantum = 1;

    for(i=1;i<=n;i++){
        createNode();
    }

    printf("\nTOTAL PROCESSES IN THE SYSTEM : ");
    struct node *x = start;
    while(x != NULL){
        printf(" %d ",x->Process);
        totalBurstTime = totalBurstTime + x->BurstTime;
        x = x->next;
    }

    printf("\ntotalBurstTime = %d",totalBurstTime);

    x = start;
    minimumArrivalTime = start->ArrivalTime;
    while(x != NULL)
    {
        if(x->ArrivalTime < minimumArrivalTime)
            {
                minimumArrivalTime = x->ArrivalTime;
            }
            x = x->next;
    }
    printf("\nMINIMUM Arrival Time is = %d \n",minimumArrivalTime);

    printf("NOTE : Output format is : TIME <PROCESS NO> TIME <PROCESS NO.> ..... \n");
    printf("\nGantt Chart:\n");

	int arrayIndex = 0;
    startTime = minimumArrivalTime;
    currentTime = startTime;


    printf("\n %d ",startTime);

	
    for(i=minimumArrivalTime;;i++)
    {
        x=start;
        while(x != NULL)
        {
            if(x->ArrivalTime == i)
            {
                recoredrAray[arrayIndex] = x->Process;
                arrayIndex++;
            }

            else{
                //NO PROCESS FOUND
            }

            x = x -> next;
        }

        core(arrayIndex);
        arrayIndex = 0 ;

        if(currentTime == totalBurstTime)
        {
			break;
		}

    }

    printf("\n");
    printf("\n DETAILS OF EACH PROCESS : ");
    int totalTurnAroundTime = 0;
    int totalWaitingTime = 0;
    struct node *m = start;
    while(m != NULL)
    {
		m->TurnAroundTime = m->CompletionTime - m->ArrivalTime;
		m->WaitingTime = m->TurnAroundTime - m->BurstTime;

        printf("\n Process No = %d : Arrival Time = %d : Burst Time = %d : Turn Around Time = %d : Waiting Time = %d : Completion Time = %d ",m->Process,m->ArrivalTime,m->BurstTime,m->TurnAroundTime,m->WaitingTime,m->CompletionTime);

		totalTurnAroundTime = totalTurnAroundTime + m->TurnAroundTime;
        totalWaitingTime = totalWaitingTime + m->WaitingTime;
        m = m->next;
    }

    printf("\n average waiting time = %f ",(float)totalWaitingTime/n);
    printf("\n average turn around time = %f ",(float)totalTurnAroundTime/n);
}
