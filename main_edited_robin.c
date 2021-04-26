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
    TimeNode *pTime;
    ListNode *pNext;
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

ListNode *createListNode(int id, int arrival_time, int execution_time, int io_length, int io_interval, int wait_time, int turnaround_time)
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
    pTemp->wait_time = wait_time;
    pTemp->turnaround_time = turnaround_time;
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

// ListNode *getMidNode(ListNode *cur)
// {
//     int curCount = 1, mid = countNodes(cur) / 2;
//     while (curCount < mid)
//     {
//         cur = cur->pNext;
//         curCount++;
//     }
//     return cur;
// }

TimeNode *initializeTimeNode()
{
    return NULL;
}

TimeNode *createTimeNode(int start, int end)
{
    TimeNode *pTemp;

    if ((pTemp = (TimeNode *)malloc(sizeof(TimeNode *))) == NULL)
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

ListNode *moveFrontToBackQueue(ListNode *pFirst, int limit)
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

int getIdleTime(ListNode *pCurr, int time)
{

    if (pCurr->arrival_time > time)
    {
        return pCurr->arrival_time - time;
    }

    else
        return 0;
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

int robin(ListNode *pId, int time_quantum)
{
    int total_waiting_time = 0;
    int prev_end;
    int curr_id;
    int inside_time;
    int time = 0;
    int idleTime = 0;
    ListNode *pCurr;
    TimeNode *pCurrTime;

    pCurr = pId;

    while (countNodes(pId) > 0)
    {

        if ((idleTime = getIdleTime(pCurr, time)) > 0)
        {
            time += idleTime;
        }

        if (pCurr->pTime == NULL)
        {
            pCurr->pTime = createTimeNode(time, 0);
            pCurrTime = pCurr->pTime;
            pCurrTime->pNext = NULL;
            pCurr->wait_time = time - pCurr->arrival_time;
        }

        else
        {
            pCurrTime = getLastTimeNode(pCurr->pTime);
            prev_end = pCurrTime->end;
            pCurrTime->pNext = createTimeNode(time, 0);
            pCurrTime = pCurrTime->pNext;
            pCurr->wait_time += time - prev_end;
        }

        total_waiting_time += pCurr->wait_time;
        inside_time = 0;
        curr_id = pCurr->id;

        while (inside_time < time_quantum && pCurr != NULL && pCurr->id == curr_id)
        {
            time += 1;
            inside_time += 1;
            pCurr->execution_time -= 1;

            if (pCurr->execution_time == 0)
            {
                pCurrTime->end = time;
                pCurr->turnaround_time += pCurr->wait_time + (pCurrTime->end - pCurrTime->start);
                display(pCurr);
                pId = freeNode(pId, pCurr->id);
                pCurr = pId;
            }

            else if (inside_time == time_quantum)
            {
                pCurrTime->end = time;

                pId = moveFrontToBackQueue(pId, time);
                pCurr = pId;
            }
        }
    }

    return total_waiting_time;
}

void robin_copy(ListNode *pId, Queue currQueue)
{
    int total_waiting_time = 0;
    int prev_end;
    int curr_id;
    int inside_time;
    int time = 0;
    int idleTime = 0;
    ListNode *pCurr;
    TimeNode *pCurrTime;

    pCurr = pId;

    while (countNodes(pId) > 0)
    {

        if ((idleTime = getIdleTime(pCurr, time)) > 0)
        {
            time += idleTime;
        }

        if (pCurr->pTime == NULL)
        {
            pCurr->pTime = createTimeNode(time, 0);
            pCurrTime = pCurr->pTime;
            pCurrTime->pNext = NULL;
            pCurr->wait_time = time - pCurr->arrival_time;
        }

        else
        {
            pCurrTime = getLastTimeNode(pCurr->pTime);
            prev_end = pCurrTime->end;
            pCurrTime->pNext = createTimeNode(time, 0);
            pCurrTime = pCurrTime->pNext;
            pCurr->wait_time += time - prev_end;
        }

        total_waiting_time += pCurr->wait_time;
        inside_time = 0;
        curr_id = pCurr->id;

        while (inside_time < currQueue.timeQuantum && pCurr != NULL && pCurr->id == curr_id)
        {
            time += 1;
            inside_time += 1;
            pCurr->execution_time -= 1;

            if (pCurr->execution_time == 0)
            {
                pCurrTime->end = time;
                pCurr->turnaround_time += pCurr->wait_time + (pCurrTime->end - pCurrTime->start);
                display(pCurr);
                pId = freeNode(pId, pCurr->id);
                pCurr = pId;
            }

            else if (inside_time == currQueue.timeQuantum)
            {
                pCurrTime->end = time;

                pId = moveFrontToBackQueue(pId, time);
                pCurr = pId;
            }
        }
    }
}

void addListNodeToQueue(Queue *currQueue, ListNode *pId)
{
    currQueue->pCurr = pId;
}

void mlfq(Queue priorityQueue[], ListNode *pId, int priorityBoost)
{
    addListNodeToQueue(&priorityQueue[0], pId);
}

void selectionSort(Queue priorityQueue[], int n)
{
    int i, j, min_idx;

    Queue temp;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (priorityQueue[j].queueID < priorityQueue[min_idx].priority)
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
        pTemp = createListNode(id[i], arrival_time[i], execution_time[i], io_length[i], io_interval[i], 0, 0);
        pId = addListNode(pId, pTemp);
    }

    insertionSort(&pId);

    mlfq(priorityQueues, pId, S);

    //printf("Average waiting time: %.2f\n\n", robin_time / (Y * 1.0));

    fclose(fptr);
}
