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
    TimeNode *pTime;
    ListNode *pNext;
};

typedef struct ganttNode GanttNode;
struct ganttNode
{
    int start;
    int end;
    int processId;
};

ListNode *initializeListNode()
{
    return NULL;
}

ListNode *createListNode(int id, int arrival_time, int execution_time, int wait_time, int turnaround_time)
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

int countTimeNodes(TimeNode *cur)
{
    int count = 0;
    while (cur != NULL)
    {
        count++;
        cur = cur->pNext;
    }
    return count;
}

void printList(ListNode *node)
{
    while (node != NULL)
    {
        printf("%d \n", node->id);
        node = node->pNext;
    }
    printf("\n");
}

void printTimeList(TimeNode *node)
{
    while (node != NULL)
    {
        printf("%d \n", node->start);
        node = node->pNext;
    }
    printf("\n");
}

ListNode *getLowestExecution(ListNode *pId, int limit)
{

    if (pId == NULL)
    {
        return NULL;
    }

    ListNode *pLowest = pId;
    ListNode *pCurr = pId->pNext;
    while (pCurr && pCurr->arrival_time <= limit)
    {

        if (pLowest->execution_time > pCurr->execution_time)
        {
            pLowest = pCurr;
        }

        pCurr = pCurr->pNext;
    }

    return pLowest;
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

int display(ListNode *pId, GanttNode ganttChart[], int ganttIndex)
{
    printf("P[%d]\n", pId->id);

    TimeNode *pCurrTime = pId->pTime;

    do
    {
        printf("Start time: %d  ", pCurrTime->start);
        printf("End time: %d\n", pCurrTime->end);
        ganttChart[ganttIndex].processId = pId->id;
        ganttChart[ganttIndex].start = pCurrTime->start;
        ganttChart[ganttIndex].end = pCurrTime->end;
        ganttIndex++;
        pCurrTime = pCurrTime->pNext;
    } while (pCurrTime);

    printf("Waiting time: %d\n", pId->wait_time);
    printf("Turnaround time: %d\n\n", pId->turnaround_time);
    printf("**********************\n\n");
    return ganttIndex;
}

int fcfs(ListNode *pId, GanttNode ganttChart[])
{
    int total_waiting_time = 0;
    int start_time = 0;
    int ganttIndex = 0;
    int idleTime = 0;
    ListNode *pCurr = pId;
    TimeNode *pCurrTime;

    while (pCurr)
    {

        if ((idleTime = getIdleTime(pCurr, start_time)) > 0)
        {
            start_time += idleTime;
        }

        pCurr->pTime = createTimeNode(start_time, 0);
        pCurrTime = pCurr->pTime;
        pCurr->wait_time = pCurrTime->start - pCurr->arrival_time;
        start_time += pCurr->execution_time;
        pCurrTime->end = start_time;
        pCurr->turnaround_time = pCurrTime->end;
        pCurrTime->pNext = NULL;

        total_waiting_time += pCurr->wait_time;

        ganttIndex = display(pCurr, ganttChart, ganttIndex);

        pCurr = pCurr->pNext;
    }

    freeList(pId);
    return total_waiting_time;
}

int nsjf(ListNode *pId, GanttNode ganttChart[])
{
    int total_waiting_time = 0;
    int upper_limit = 0;
    int start_time = 0;
    int idleTime = 0;
    int ganttIndex = 0;
    ListNode *pCurr;
    ListNode *pTemp;
    TimeNode *pCurrTime;

    while (countNodes(pId) > 0)
    {
        pCurr = getLowestExecution(pId, upper_limit);

        if ((idleTime = getIdleTime(pCurr, start_time)) > 0)
        {
            start_time += idleTime;
        }

        pCurr->pTime = createTimeNode(start_time, 0);
        pCurrTime = pCurr->pTime;
        pCurr->wait_time = pCurrTime->start - pCurr->arrival_time;
        start_time += pCurr->execution_time;
        pCurrTime->end = start_time;
        pCurr->turnaround_time = pCurrTime->end;
        pCurrTime->pNext = NULL;
        upper_limit += pCurr->execution_time;

        total_waiting_time += pCurr->wait_time;

        ganttIndex = display(pCurr, ganttChart, ganttIndex);

        pId = freeNode(pId, pCurr->id);
    }

    return total_waiting_time;
}

int psjf(ListNode *pId, GanttNode ganttChart[])
{
    int total_waiting_time = 0;
    int upper_limit = 0;
    int time = 0;
    int curr_id;
    int prev_end;
    int ganttIndex = 0;
    int idleTime = 0;
    ListNode *pCurr = getLowestExecution(pId, upper_limit);
    ListNode *pTemp = initializeListNode();
    TimeNode *pCurrTime;

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
        curr_id = pCurr->id;

        do
        {

            time += 1;
            upper_limit += 1;
            pCurr->execution_time -= 1;

            if (pCurr->execution_time == 0)
            {
                pCurrTime->end = time;
                pCurr->turnaround_time += pCurr->wait_time + (pCurrTime->end - pCurrTime->start);
                ganttIndex = display(pCurr, ganttChart, ganttIndex);
                pId = freeNode(pId, pCurr->id);
            }

            if ((pTemp = getLowestExecution(pId, upper_limit)) != NULL)
            {
                if (pTemp->id != pCurr->id)
                {
                    pCurrTime->end = time;
                    pCurr->turnaround_time += pCurr->wait_time + (pCurrTime->end - pCurrTime->start);
                    pCurr = pTemp;
                }
            }
        } while (pCurr->id == curr_id && pTemp != NULL);
    }

    return total_waiting_time;
}

int robin(ListNode *pId, int time_quantum, GanttNode ganttChart[])
{
    int total_waiting_time = 0;
    int prev_end;
    int curr_id;
    int inside_time;
    int time = 0;
    int idleTime = 0;
    int ganttIndex = 0;
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
                ganttIndex = display(pCurr, ganttChart, ganttIndex);
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

void selectionSort(GanttNode arr[], int n)
{
    int i, j, min_idx;

    GanttNode temp;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j].start < arr[min_idx].start)
                min_idx = j;

        //swap(&arr[min_idx], &arr[i]);
        temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
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

void printGantt(GanttNode ganttChart[])
{

    int i = 0, j = 0, idleCheck = 0;

    while (ganttChart[i].processId != -1)
        i++;

    selectionSort(ganttChart, i);
    i = 0;

    printf("\n\n          GANTT CHART\n");
    printf("        ***************\n\n");
    printf("+");
    if (ganttChart[0].start != 0)
    {
        printf("--+");
    }
    while (ganttChart[i].processId != -1)
    {
        if ((ganttChart[i].start == ganttChart[i - 1].end || idleCheck == 1) && i != 0)
        {
            if (ganttChart[i].processId > 9 && ganttChart[i].processId < 100)
                printf("---+");
            else if (ganttChart[i].processId > 99)
                printf("----+");
            else
                printf("--+");
            i++;
            idleCheck = 0;
        }
        else if (i == 0)
        {
            if (ganttChart[i].processId > 9 && ganttChart[i].processId < 100)
                printf("---+");
            else if (ganttChart[i].processId > 99)
                printf("----+");
            else
                printf("--+");
            i++;
        }
        else
        {
            printf("--+");
            idleCheck = 1;
        }
    }

    printf("\n|");
    i = 0;
    idleCheck = 0;

    while (ganttChart[i].processId != -1)
    {
        if (i == 0 && ganttChart[i].start != 0 && idleCheck == 0)
        {
            printf("  |");
            idleCheck = 1;
        }
        else if (i == 0)
        {
            printf("P%d", ganttChart[i].processId);
            printf("|");
            i++;
            idleCheck = 0;
        }
        else if ((ganttChart[i].start == ganttChart[i - 1].end || idleCheck == 1) && i != 0)
        {
            printf("P%d", ganttChart[i].processId);
            printf("|");
            i++;
            idleCheck = 0;
        }
        else
        {
            printf("  |");
            idleCheck = 1;
        }
    }

    printf("\n");
    printf("+");
    i = 0;
    if (ganttChart[0].start != 0)
    {
        printf("--+");
    }
    idleCheck = 0;

    while (ganttChart[i].processId != -1)
    {
        if ((ganttChart[i].start == ganttChart[i - 1].end || idleCheck == 1) && i != 0)
        {
            if (ganttChart[i].processId > 9 && ganttChart[i].processId < 100)
                printf("---+");
            else if (ganttChart[i].processId > 99)
                printf("----+");
            else
                printf("--+");
            i++;
            idleCheck = 0;
        }
        else if (i == 0)
        {
            if (ganttChart[i].processId > 9 && ganttChart[i].processId < 100)
                printf("---+");
            else if (ganttChart[i].processId > 99)
                printf("----+");
            else
                printf("--+");
            i++;
        }
        else
        {
            printf("--+");
            idleCheck = 1;
        }
    }

    i = 0;
    idleCheck = 0;
    printf("\n");

    printf("0");
    if (ganttChart[0].start != 0)
    {
        if (ganttChart[0].processId > 9 && ganttChart[0].processId < 100)
            printf("   ");
        else if (ganttChart[0].processId > 99)
            printf("    ");
        else
            printf("  ");
        printf("%d", ganttChart[0].start);
    }

    while (ganttChart[i].processId != -1)
    {
        if (i == 0)
        {
            if (ganttChart[i].processId > 9 && ganttChart[0].processId < 100)
                printf("   ");
            else if (ganttChart[i].processId > 99)
                printf("    ");
            else
                printf("  ");
            if (ganttChart[i].end > 9)
                printf("\b");
            else if (ganttChart[i].end > 99)
                printf("\b\b");
            printf("%d", ganttChart[i].end);
            i++;
        }
        if ((ganttChart[i].start == ganttChart[i - 1].end || idleCheck == 1) && i != 0)
        {
            if (ganttChart[i].processId > 9 && ganttChart[i].processId < 100)
                printf("   ");
            else if (ganttChart[i].processId > 99)
                printf("    ");
            else
                printf("  ");

            if (ganttChart[i].end > 9 && ganttChart[i].end < 100)
                printf("\b");
            else if (ganttChart[i].end > 99)
                printf("\b\b");
            printf("%d", ganttChart[i].end);
            i++;
            idleCheck = 0;
        }
        else
        {
            printf("  ");
            if (ganttChart[i].start > 9 && ganttChart[i].start < 100)
                printf("\b");
            else if (ganttChart[i].start > 99)
                printf("\b\b");
            printf("%d", ganttChart[i].start);
            idleCheck = 1;
        }
    }
    printf("\n\n");
}

int main()
{
    char filename[100];
    GanttNode ganttChart[500];
    int i, fileChecker;
    for (i = 0; i < 500; i++)
    {
        ganttChart[i].processId = -1;
        ganttChart[i].start = -1;
        ganttChart[i].end = -1;
    }

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

    int X, Y, Z;
    fscanf(fptr, "%d", &X);
    fscanf(fptr, "%d", &Y);
    fscanf(fptr, "%d", &Z);

    // Error checking of first line
    // *****************************************
    if (X != 0 && X != 1 && X != 2 && X != 3)
    {
        printf("Invalid X value");
        exit(1);
    }

    if (Y < 3 || Y > 100)
    {
        printf("Invalid Y value");
        exit(1);
    }

    if (X != 3 && Z != 1)
    {
        printf("Invalid Z value");
        exit(1);
    }
    // *****************************************

    int id[Y];
    int arrival_time[Y];
    int execution_time[Y];
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
    }

    if (fscanf(fptr, "%d", &fileChecker) != EOF)
    {
        printf("Number of rows not equal to declared Y value");
        exit(1);
    }
    i = 0;

    // Initializing linked lists
    ListNode *pTemp = initializeListNode();
    ListNode *pId = initializeListNode();

    for (i = Y - 1; i >= 0; i--)
    {
        pTemp = createListNode(id[i], arrival_time[i], execution_time[i], 0, 0);
        pId = addListNode(pId, pTemp);
    }

    insertionSort(&pId);

    if (X == 0)
    {
        int fcfs_time = fcfs(pId, ganttChart);

        printGantt(ganttChart);
        printf("Average waiting time: %.2f\n\n", fcfs_time / (Y * 1.0));
    }
    else if (X == 1)
    {
        int nsjf_time = nsjf(pId, ganttChart);

        printGantt(ganttChart);
        printf("Average waiting time: %.2f\n\n", nsjf_time / (Y * 1.0));
    }
    else if (X == 2)
    {
        int psjf_time = psjf(pId, ganttChart);

        printGantt(ganttChart);
        printf("Average waiting time: %.2f\n\n", psjf_time / (Y * 1.0));
    }

    else if (X == 3)
    {
        int robin_time = robin(pId, Z, ganttChart);

        printGantt(ganttChart);
        printf("Average waiting time: %.2f\n\n", robin_time / (Y * 1.0));
    }

    fclose(fptr);
}
