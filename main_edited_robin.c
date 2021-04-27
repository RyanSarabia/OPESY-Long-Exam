/*************************************************************** 
Names:  Loquinte, Kenneth
        Sarabia, Ryan
Group: 64
Section: S15
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linked List

typedef struct timeNodeTag TimeNode;
struct timeNodeTag
{
    int start;
    int end;
    TimeNode *pNext;
};

typedef struct nodeTag ListNode;
struct nodeTag
{
    int id;
    int arrival_time;
    int execution_time;
    int wait_time;
    int turnaround_time;
    int io_length;
    int io_interval;
    int ready;
    int time_allotment;
    TimeNode *pTime;
    ListNode *pNext;
};

typedef struct IONodeTag IONode;

struct IONodeTag
{
    int fulfill_time;
    ListNode *pProcess;
    IONode *pNext;
};

typedef struct queue Queue;
struct queue
{
    int queueID;
    int priority;
    int timeQuantum;
    ListNode *pCurr;
};

ListNode *
initializeListNode()
{
    return NULL;
}

ListNode *createListNode(int id, int arrival_time, int execution_time, int io_length, int io_interval, int ready)
{
    ListNode *pTemp;

    if ((pTemp = (ListNode *)malloc(sizeof(ListNode))) == NULL)
    {
        printf("Not enough memory.");
        return NULL;
    }
    pTemp->id = id;
    pTemp->arrival_time = arrival_time;
    pTemp->execution_time = execution_time;
    pTemp->io_length = io_length;
    pTemp->io_interval = io_interval;
    pTemp->ready = ready;
    pTemp->wait_time = 0;
    pTemp->turnaround_time = 0;
    pTemp->time_allotment = 0;
    pTemp->pTime = NULL;
    pTemp->pNext = NULL;

    return pTemp;
}

ListNode *addListNode(ListNode *pFirst, ListNode *pTemp)
{
    pTemp->pNext = pFirst;
    pFirst = pTemp;
    return pFirst;
}

ListNode *getLastNode(ListNode *cur)
{
    while (cur != NULL && cur->pNext != NULL)
        cur = cur->pNext;
    return cur;
}

ListNode *freeNode(ListNode *pFirst, int id)
{
    ListNode *pCurr;
    ListNode *pPrev;
    TimeNode *pCurrTime;

    pCurr = pFirst;

    if (pCurr->id == id)
    {
        pFirst = pFirst->pNext;
        pCurrTime = pCurr->pTime;
        freeTimeList(pCurrTime);
        free(pCurr);

        return pFirst;
    }

    else
    {
        while (pCurr != NULL && pCurr->pNext->id != id)
        {
            pCurr = pCurr->pNext;
        }

        pPrev = pCurr;        //previous node
        pCurr = pCurr->pNext; //node to be deleted
        pPrev->pNext = pCurr->pNext;
        pCurrTime = pCurr->pTime;
        freeTimeList(pCurrTime);
        free(pCurr);
        return pFirst;
    }
}

ListNode *reorderFrontArrival(ListNode *pFirst, int limit)
{
    ListNode *pTemp;
    ListNode *pCurr;
    int moveCount = 0;

    if (pFirst->pNext != NULL)
    {
        pTemp = pFirst;
        pCurr = pFirst;
        pFirst = pFirst->pNext;

        while (pCurr->pNext != NULL && pCurr->pNext->arrival_time <= limit)
        {
            pCurr = pCurr->pNext;
            moveCount++;
        }

        if (pCurr->pNext == NULL)
        {
            pTemp->pNext = NULL;
            pCurr->pNext = pTemp;
        }

        else if (moveCount == 0)
        {
            pFirst = pTemp;
        }

        else
        {
            pTemp->pNext = pCurr->pNext;
            pCurr->pNext = pTemp;
        }
    }

    else
    {
        pFirst = pFirst;
    }
    return pFirst;
}

ListNode *reorderFrontQueue(ListNode *pFirst, int IO_fulfill_time)
{
    ListNode *pTemp;
    ListNode *pCurr;
    int moveCount = 0;

    if (pFirst->pNext != NULL)
    {
        pTemp = pFirst;
        pCurr = pFirst;
        pFirst = pFirst->pNext;

        while (pCurr->pNext != NULL && pCurr->pNext->arrival_time <= IO_fulfill_time)
        {
            pCurr = pCurr->pNext;
            moveCount++;
        }

        if (pCurr->pNext == NULL)
        {
            pTemp->pNext = NULL;
            pCurr->pNext = pTemp;
        }

        else if (moveCount == 0)
        {
            pFirst = pTemp;
        }

        else
        {
            pTemp->pNext = pCurr->pNext;
            pCurr->pNext = pTemp;
        }
    }

    else
    {
        pFirst = pFirst;
    }
    return pFirst;
}

ListNode *moveNodeToQueue(ListNode **pDest, ListNode *pID, int curr_time)
{
    ListNode *pNext;

    pNext = pID->pNext;
    pID->pNext = NULL;

    pID->time_allotment = 0; //not sure if nassave to... 90% sure
    *pDest = addListNode(*pDest, pID);
    *pDest = reorderFrontArrival(*pDest, curr_time);

    return pNext;
}

TimeNode *initializeTimeNode()
{
    return NULL;
}

TimeNode *createTimeNode(int start, int end)
{
    TimeNode *pTemp;

    if ((pTemp = (TimeNode *)malloc(sizeof(TimeNode))) == NULL)
    {
        printf("Not enough memory.");
        return NULL;
    }
    pTemp->start = start;
    pTemp->end = end;
    pTemp->pNext = NULL;
    return pTemp;
}

TimeNode *addTimeNode(TimeNode *pFirst, TimeNode *pTemp)
{

    TimeNode *pCurr = pFirst;

    while (pCurr->pNext)
    {
        pCurr = pCurr->pNext;
    }

    pCurr->pNext = pTemp;
    return pFirst;
}

TimeNode *getLastTimeNode(TimeNode *cur)
{
    while (cur != NULL && cur->pNext != NULL)
        cur = cur->pNext;
    return cur;
}

IONode *createIONode(int fulfill_time, ListNode *pProcess)
{
    IONode *pTemp;

    if ((pTemp = (IONode *)malloc(sizeof(IONode))) == NULL)
    {
        printf("Not enough memory.");
        return NULL;
    }

    pTemp->fulfill_time = fulfill_time;
    pTemp->pProcess = pProcess;
    pTemp->pNext = NULL;
    return pTemp;
}

IONode *addIONode(IONode *pFirst, IONode *pTemp)
{

    IONode *pCurr = pFirst;

    while (pCurr->pNext)
    {
        pCurr = pCurr->pNext;
    }

    pCurr->pNext = pTemp;
    return pFirst;
}

IONode *processIO(IONode *pFirst, int curr_time)
{

    int isFront; //indicates whether current IONode is in front of list
    IONode *pCurr;
    IONode *pTemp;
    ListNode *pProcess;

    pCurr = pFirst;
    isFront = 1;

    while (pCurr != NULL)
    {
        if (pCurr->fulfill_time >= curr_time)
        {

            pProcess = pCurr->pProcess;
            pProcess->ready = 1; //NOT SURE PA IF THIS IS ACTUALLY MODIFYING THE LISTNODE! NOTE THIS.

            pTemp = pCurr;
            pCurr = pCurr->pNext;

            if (isFront)
            {
                pFirst = pCurr;
            }

            free(pTemp);
        }

        else
        {
            isFront = 0;
            pCurr = pCurr->pNext;
        }
    }
}

void freeList(ListNode *pFirst)
{
    ListNode *pCurr;

    while (pFirst != NULL)
    {
        pCurr = pFirst;
        pFirst = pFirst->pNext;
        freeTimeList(pCurr->pTime);
        free(pCurr);
    }
}

void freeTimeList(TimeNode *pFirst)
{
    TimeNode *pCurr;

    while (pFirst->pNext != NULL)
    {
        pCurr = pFirst;
        pFirst = pFirst->pNext;
        free(pCurr);
    }
}

int countNodes(ListNode *cur)
{
    int count = 0;
    while (cur != NULL)
    {
        count++;
        cur = cur->pNext;
    }
    return count;
}

int getIdleTime(ListNode *pCurr, int time)
{

    if (pCurr->arrival_time > time)
    {
        return pCurr->arrival_time - time;
    }

    else
        return 0;
}

//Sorts top queue based on arrival time
void boost(Queue *priorityQueue, int num_queues, int curr_time)
{
    ListNode *pCurr;
    ListNode *pFirst;
    ListNode *pTemp;
    ListNode *pTop;
    int isNodeFront = 1;

    pTop = priorityQueue->pCurr;

    for (int i = 1; i < num_queues; i++)
    {
        pCurr = (priorityQueue + i)->pCurr;

        while (pCurr != NULL)
        {

            if (pCurr->ready)
            {
                pTemp = moveNodeToQueue(&pTop, pCurr, curr_time);

                if (isNodeFront)
                {
                    (priorityQueue + i)->pCurr = pTemp;
                }
            }

            else
            {
                isNodeFront = 0;
                pCurr = pCurr->pNext;
            }
        }
    }

    //go through each queue except highest queue.
    // do the ff:
    // 1. check if process is in IO mode (just check its "ready" state), if NOT then do not modify the node
    // 2. reset time allotment to 0
    // 3. move each node to list nung highest queue and sort it while doing that (movefronttoback na function?)
}

// Scheduling functions

void display(ListNode *pId)
{
    printf("P[%d]\n", pId->id);

    TimeNode *pCurrTime = pId->pTime;
    do
    {
        printf("Start time: %d  ", pCurrTime->start);
        printf("End time: %d\n", pCurrTime->end);
        pCurrTime = pCurrTime->pNext;
    } while (pCurrTime);

    printf("Waiting time: %d\n", pId->wait_time);
    printf("Turnaround time: %d\n\n", pId->turnaround_time);
    printf("**********************\n\n");
}

int robin(ListNode **pId, IONode **pIOList, int *curr_time, int *total_waiting_time, int time_quantum, int priorityBoost, int interval_num)
{
    int *wait_time = total_waiting_time;
    int prev_end;
    int curr_id;
    int inside_time;
    int *time = curr_time;
    int idleTime = 0;
    int fulfill_time = 0;
    IONode *pIONode;
    ListNode *pCurr = *pId;
    TimeNode *pCurrTime;

    // Return
    // 0 -- finished all processes
    // 1 -- priority boost
    // 2 -- idle time > 0
    // 3 -- process expended its time allotment
    while (countNodes(*pId) > 0)
    {
        if (*pIOList != NULL)
        {
            processIO(*pIOList, curr_time);
        }

        // if ((idleTime = getIdleTime(pCurr, time)) > 0)
        // {
        //     time += idleTime;
        // }

        if (pCurr->ready == 1)
        {

            if (pCurr->pTime == NULL)
            {
                pCurr->pTime = createTimeNode(*time, 0);
                pCurrTime = pCurr->pTime;
                pCurrTime->pNext = NULL;
                pCurr->wait_time = *time - pCurr->arrival_time;
            }

            else
            {
                pCurrTime = getLastTimeNode(pCurr->pTime);
                prev_end = pCurrTime->end;
                pCurrTime->pNext = createTimeNode(*time, 0);
                pCurrTime = pCurrTime->pNext;
                pCurr->wait_time += *time - prev_end;
            }

            *wait_time += pCurr->wait_time;
            inside_time = 0;
            curr_id = pCurr->id;

            while (inside_time < pCurr->io_interval && pCurr != NULL && pCurr->id == curr_id)
            {
                *time += 1;
                inside_time += 1;
                pCurr->execution_time -= 1;
                pCurr->time_allotment += 1;

                if (pCurr->execution_time == 0)
                {
                    pCurrTime->end = *time;
                    pCurr->turnaround_time += pCurr->wait_time + (pCurrTime->end - pCurrTime->start);
                    display(pCurr);
                    *pId = freeNode(*pId, pCurr->id);
                    pCurr = pId;
                }

                else if (pCurr->time_allotment == time_quantum)
                {
                    //Move down queue
                    pCurrTime->end = *time;
                    return 3;
                }

                else if (inside_time == pCurr->io_interval)
                {
                    pCurrTime->end = *time;
                    int fulfill_time = pCurr->io_length + *time;

                    //Create IO node
                    pIONode = createIONode(fulfill_time, pCurr);
                    *pIOList = addIONode(*pIOList, pIONode);

                    pCurr->ready = 0;

                    *pId = reorderFrontQueue(*pId, pIONode->fulfill_time);
                    pCurr = *pId;
                }
            }

            idleTime = getIdleTime(pCurr, *time);

            if (idleTime > 0)
            {
                *time = *time - idleTime; //need to reset time para magstart again dun sa time after last process
                return 2;
            }

            if (*time > priorityBoost * interval_num)
            {
                return 1;
            }
        }

        else
        {
            // parang di naman possible na hindi sya maging ready given this setup....
            // it will only not be ready if there is idle time (already accounted for).
        }
    }

    return 0;
}

void addListNodeToQueue(Queue *currQueue, ListNode *pId)
{
    currQueue->pCurr = pId;
}

void mlfq(Queue *priorityQueue, int num_queues, ListNode *pId, int priorityBoost)
{
    int processing = 1; //condition for continuing while loop
    int curr_queue = 0;
    int robin_result = 0;
    int interval_num = 0;   //how many intervals of priority boost have passed
    int curr_time = 0;      //pass as pointer to robin function
    int wait_time = 0;      //pass as pointer to robin function
    IONode *pIOList = NULL; //pass as pointer to robin function
    ListNode *pCurrList = NULL;

    addListNodeToQueue(priorityQueue + 0, pId);

    pCurrList = (priorityQueue + 0)->pCurr;

    while (processing)
    {
        //fix robin(?) account for prioritizing arritval time over process na bumalik from I/O (oks na)

        robin_result = robin(&pCurrList, &pIOList, &curr_time, &wait_time, (priorityQueue + curr_queue)->timeQuantum, priorityBoost, interval_num);

        switch (robin_result)
        {
        case 0:
        { //completed all processes in current queue, check other queues
            int i;
            for (i = curr_queue; i < num_queues; i++)
            {
            }
        }
        break;

        case 1: //time for priority boost
            boost(priorityQueue, num_queues, curr_time);

            //if currtime > PRIORITY BOOST * interval_num
            //priority boost
            //probably make a function on this
            //note that when not moving queues, time quantum for that node is not reset
            //note that when process in IO mode, not part of PB (just check yung ready field)

            break;

        case 2: //idle time exists, check other queues
            break;

        default: //current process has to move to lower queue
            ListNode *pCurr = pCurrList;

            if (curr_queue != num_queues - 1) //means it's not on the last level
            {
                ListNode *pDest = (priorityQueue + curr_queue + 1)->pCurr;
                ListNode *pNext = moveNodeToQueue(&pDest, pCurr, curr_time);
                (priorityQueue + curr_queue)->pCurr = pNext;
            }
        }

        //check if all queues are empty, if empty, processing = 0;
        //make a function here that checks all queues from high priority to low, then get corresponding process list (if going the ready route, then check lang yung ready state ng mga nodes)
    }
}

void selectionSort(Queue priorityQueue[], int n)
{
    int i, j, min_idx;

    Queue temp;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (priorityQueue[j].priority < priorityQueue[min_idx].priority)
                min_idx = j;

        //swap(&arr[min_idx], &arr[i]);
        temp = priorityQueue[i];
        priorityQueue[i] = priorityQueue[min_idx];
        priorityQueue[min_idx] = temp;
    }
}

void insertionSort(ListNode **head)
{
    // Initialize sorted linked list
    ListNode *s = NULL;
    ListNode *a = *head;

    // Traverse the given linked list and insert every
    // node to sorted
    while (a)
    {
        // Store next for next iteration
        ListNode *next = a->pNext;

        // insert current in sorted linked list
        // sortedInsert(s, a);
        if (s == NULL || s->arrival_time > a->arrival_time)
        {
            a->pNext = s;
            s = a;
        }
        else
        {
            /* Locate the node before the point of insertion */
            ListNode *current = s;
            while (current->pNext &&
                   current->pNext->arrival_time < a->arrival_time)
            {
                current = current->pNext;
            }
            a->pNext = current->pNext;
            current->pNext = a;
        }

        // Update current
        a = next;
    }
    *head = s;
}

int main()
{
    char filename[100];
    int i, fileChecker;

    printf("Enter filename: ");
    scanf("%s", filename);
    strcat(filename, ".txt");

    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL)
    {
        printf("%s not found.", &filename);
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    int X, Y, S;
    fscanf(fptr, "%d", &X);
    fscanf(fptr, "%d", &Y);
    fscanf(fptr, "%d", &S);

    // Error checking of first line
    // *****************************************
    if (X < 2 || X > 5)
    {
        printf("Invalid X value");
        exit(1);
    }

    if (Y < 3 || Y > 100)
    {
        printf("Invalid Y value");
        exit(1);
    }

    if (S < 0)
    {
        printf("Invalid S value");
        exit(1);
    }
    // *****************************************

    //Scanning 2nd line

    Queue priorityQueues[X];

    for (i = 0; i < X; i++)
    {
        fscanf(fptr, "%d", &priorityQueues[i].priority);
        fscanf(fptr, "%d", &priorityQueues[i].queueID);
        fscanf(fptr, "%d", &priorityQueues[i].timeQuantum);
    }

    selectionSort(priorityQueues, X);

    int id[Y];
    int arrival_time[Y];
    int execution_time[Y];
    int io_length[Y];
    int io_interval[Y];
    i = 0;

    for (i = 0; i < Y; i++)
    {

        if (fscanf(fptr, "%d", &id[i]) == EOF)
        {
            printf("Invalid row amount");
            exit(1);
        }
        if (fscanf(fptr, "%d", &arrival_time[i]) == EOF)
        {
            printf("Invalid row amount");
            exit(1);
        }
        if (fscanf(fptr, "%d", &execution_time[i]) == EOF)
        {
            printf("Invalid row amount");
            exit(1);
        }
        if (fscanf(fptr, "%d", &io_length[i]) == EOF)
        {
            printf("Invalid row amount");
            exit(1);
        }
        if (fscanf(fptr, "%d", &io_interval[i]) == EOF)
        {
            printf("Invalid row amount");
            exit(1);
        }
    }

    if (fscanf(fptr, "%d", &fileChecker) != EOF)
    {
        printf("Number of rows not equal to declared Y value");
        exit(1);
    }

    // Initializing linked lists
    ListNode *pTemp = initializeListNode();
    ListNode *pId = initializeListNode();

    i = 0;

    for (i = Y - 1; i >= 0; i--)
    {
        pTemp = createListNode(id[i], arrival_time[i], execution_time[i], io_length[i], io_interval[i], 1);
        pId = addListNode(pId, pTemp);
    }

    insertionSort(&pId);

    mlfq(priorityQueues, X, pId, S);

    //printf("Average waiting time: %.2f\n\n", robin_time / (Y * 1.0));

    fclose(fptr);
}
