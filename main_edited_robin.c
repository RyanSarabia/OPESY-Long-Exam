/*************************************************************** 
Names:  Loquinte, Kenneth
        Sarabia, Ryan
Group: 64
Section: S15

//Priority
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
    int queue; //-1 means it is an IO Node
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
    int queue;
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
    pTemp->queue = 0;
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

// ListNode *reorderFrontArrival(ListNode *pFirst, int limit)
// {
//     ListNode *pTemp;
//     ListNode *pCurr;
//     int moveCount = 0;

//     if (pFirst->pNext != NULL)
//     {
//         pTemp = pFirst;
//         pCurr = pFirst;
//         pFirst = pFirst->pNext;

//         while (pCurr->pNext != NULL && pCurr->pNext->arrival_time <= limit)
//         {
//             pCurr = pCurr->pNext;
//             moveCount++;
//         }

//         if (pCurr->pNext == NULL)
//         {
//             pTemp->pNext = NULL;
//             pCurr->pNext = pTemp;
//         }

//         else if (moveCount == 0)
//         {
//             pFirst = pTemp;
//         }

//         else
//         {
//             pTemp->pNext = pCurr->pNext;
//             pCurr->pNext = pTemp;
//         }
//     }

//     else
//     {
//         pFirst = pFirst;
//     }
//     return pFirst;
// }

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

ListNode *moveProcessToBack(ListNode *pFirst)
{

    ListNode *pCurr;
    ListNode *pTemp;
    pTemp = pFirst;
    pCurr = pFirst;

    if (pFirst->pNext == NULL)
    {
        return pFirst;
    }
    else
    {
        pFirst = pCurr->pNext;
        while (pCurr->pNext != NULL)
        {
            pCurr = pCurr->pNext;
        }
        pCurr->pNext = pTemp;
        pTemp->pNext = NULL;
        return pFirst;
    }
}

ListNode *moveNodeToQueue(ListNode **pDest, ListNode *pID, int curr_time, int case_value)
{
    ListNode *pNext;

    pNext = pID->pNext;
    pID->pNext = NULL;

    pID->time_allotment = 0; //not sure if nassave to... 90% sure

    pID->queue = (*pDest)->queue;
    *pDest = addListNode(*pDest, pID);

    switch (case_value)
    {
    case 1:
    { //For process boost
        // *pDest = reorderFrontArrival(*pDest, curr_time);
        *pDest = moveProcessToBack(*pDest);
    }
    break;

    case 2:
    { //For adding to back of the queue
        *pDest = moveProcessToBack(*pDest);
    }
    break;
    }

    return pNext;
}

TimeNode *initializeTimeNode()
{
    return NULL;
}

TimeNode *createTimeNode(int start, int end, int queue)
{
    TimeNode *pTemp;

    if ((pTemp = (TimeNode *)malloc(sizeof(TimeNode))) == NULL)
    {
        printf("Not enough memory.");
        return NULL;
    }
    pTemp->start = start;
    pTemp->end = end;
    pTemp->queue = queue;
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
    pTemp->pNext = pFirst;
    pFirst = pTemp;
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
        if (pCurr->fulfill_time <= curr_time)
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

//Sorts top queue based on arrival time
void boost(Queue *priorityQueue, int num_queues, int curr_time)
{
    ListNode *pCurr;
    ListNode *pFirst;
    ListNode *pTemp;
    ListNode *pTop;
    int isNodeFront = 1;

    pTop = priorityQueue->pCurr;

    int i;
    for (i = 1; i < num_queues; i++)
    {

        pCurr = (priorityQueue + i)->pCurr;

        while (pCurr != NULL)
        {

            if (pCurr->ready)
            {
                if (pTop == NULL)
                {
                    pTemp = pCurr->pNext;
                    pTop = pCurr;
                    pTop->queue = 0;
                    pTop->time_allotment = 0;
                    pTop->pNext = NULL;
                    pCurr = pTemp;
                    continue;
                }

                else
                {

                    pTemp = moveNodeToQueue(&pTop, pCurr, curr_time, 1); // case 1 for boost

                    if (isNodeFront)
                    {
                        (priorityQueue + i)->pCurr = pTemp;
                    }
                }
            }

            else
            {
                isNodeFront = 0;
                pTemp = pCurr->pNext;
            }

            pCurr = pTemp;
        }
    }

    priorityQueue->pCurr = pTop;

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
        if (pCurrTime->queue >= 0)
        {
            printf("Q[%d] Start time: %d  ", pCurrTime->queue, pCurrTime->start);
            printf("End time: %d\n", pCurrTime->end);
        }
        else
        {
            printf("[IO] Start time: %d  ", pCurrTime->start);
            printf("End time: %d\n", pCurrTime->end);
        }
        pCurrTime = pCurrTime->pNext;
    } while (pCurrTime);

    printf("Waiting time: %d\n", pId->wait_time);
    printf("Turnaround time: %d\n\n", pId->turnaround_time);
    printf("**********************\n\n");
}

int robin(ListNode **pId, IONode **pIOList, int *curr_time, int *total_waiting_time, int time_quantum, int priorityBoost, int interval_num, int next_priority_time, float *average_time)
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
    // 3 -- move to a more prioritized process
    // 4 -- priority boost just after process finished executing (has implications on the ordering of processes)
    // 5 -- process expended its time allotment
    while (countNodes(*pId) > 0)
    {
        if (*pIOList != NULL)
        {
            *pIOList = processIO(*pIOList, *curr_time);
        }

        if (pCurr->ready == 1)
        {
            if (pCurr->pTime == NULL)
            {
                pCurr->pTime = createTimeNode(*time, 0, pCurr->queue);
                pCurrTime = pCurr->pTime;
                pCurrTime->pNext = NULL;
                pCurr->wait_time = *time - pCurr->arrival_time;
            }

            else
            {
                pCurrTime = getLastTimeNode(pCurr->pTime);
                prev_end = pCurrTime->end;
                pCurrTime->pNext = createTimeNode(*time, 0, pCurr->queue);
                pCurrTime = pCurrTime->pNext;
                pCurr->wait_time += *time - prev_end;
            }

            *wait_time += pCurr->wait_time;
            inside_time = 0;
            curr_id = pCurr->id;

            while (pCurr != NULL && pCurr->id == curr_id && (pCurr->io_interval == 0 || inside_time < pCurr->io_interval))
            {

                *time += 1;
                inside_time += 1;
                pCurr->execution_time -= 1;
                pCurr->time_allotment += 1;

                if (pCurr->execution_time == 0)
                {

                    pCurrTime->end = *time;
                    pCurr->turnaround_time = *curr_time - pCurr->arrival_time;
                    *average_time += pCurr->wait_time;
                    display(pCurr);
                    *pId = freeNode(*pId, pCurr->id);
                    pCurr = *pId;

                    if (*time > priorityBoost * interval_num && pCurr != NULL)
                    {
                        return 4;
                    }
                }

                else if (pCurr->time_allotment == time_quantum)
                {
                    //Move down queue
                    pCurrTime->end = *time;

                    if (inside_time == pCurr->io_interval)
                    {

                        pCurrTime->end = *time;
                        int fulfill_time = pCurr->io_length + *time;

                        //Create IO node
                        pIONode = createIONode(fulfill_time, pCurr);
                        *pIOList = addIONode(*pIOList, pIONode);

                        prev_end = pCurrTime->end;
                        pCurrTime->pNext = createTimeNode(*time, 0, -1);
                        pCurrTime = pCurrTime->pNext;
                        pCurrTime->end = fulfill_time;
                        pCurr->wait_time += *time - prev_end;

                        pCurr->ready = 0;
                    }

                    if (*time >= (priorityBoost * interval_num))
                    {
                        break;
                    }

                    else
                    {
                        return 5;
                    }
                }

                else if (inside_time == pCurr->io_interval)
                {

                    pCurrTime->end = *time;
                    int fulfill_time = pCurr->io_length + *time;

                    //Create IO node
                    pIONode = createIONode(fulfill_time, pCurr);
                    *pIOList = addIONode(*pIOList, pIONode);

                    prev_end = pCurrTime->end;
                    pCurrTime->pNext = createTimeNode(*time, 0, -1);
                    pCurrTime = pCurrTime->pNext;
                    pCurrTime->end = fulfill_time;
                    pCurr->wait_time += *time - prev_end;

                    pCurr->ready = 0;

                    *pId = reorderFrontQueue(*pId, pIONode->fulfill_time);

                    pCurr = *pId;
                }

                else if (*time == next_priority_time && next_priority_time != -1)
                {

                    pCurrTime->end = *time;

                    return 3;
                }
            }

            if (pCurr == NULL)
            {
                return 0;
            }

            idleTime = getIdleTime(pCurr, *time);

            if (pCurr->ready == 0 || idleTime > 0)
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
        }
    }

    return 0;
}

void addListNodeToQueue(Queue *currQueue, ListNode *pId)
{
    currQueue->pCurr = pId;
}

int findProcessQueue(Queue *priorityQueue, ListNode *pId, int num_queues)
{
    int i;
    ListNode *pTemp;

    for (i = 0; i < num_queues; i++)
    {
        pTemp = (priorityQueue + i)->pCurr;

        while (pTemp != NULL)
        {

            if (pTemp->id == pId->id)
            {
                return i;
            }
            pTemp = pTemp->pNext;
        }
    }

    return 0;
}

IONode *getNearestFulfill(IONode *pFirst, Queue *priorityQueue, int num_queues)
{
    int current_shortest_queue = -1, queue_checked;
    ListNode *pCurr = NULL;
    int shortest_fulfill = -1;

    while (pFirst != NULL) //Find IO with shortest fulfill_time
    {
        if (shortest_fulfill == -1)
        {
            shortest_fulfill = pFirst->fulfill_time;
            pCurr = pFirst->pProcess;
            current_shortest_queue = findProcessQueue(priorityQueue, pCurr, num_queues);
        }
        else
        {
            if (pFirst->fulfill_time < shortest_fulfill)
            {
                shortest_fulfill = pFirst->fulfill_time;
                pCurr = pFirst->pProcess;
                current_shortest_queue = findProcessQueue(priorityQueue, pCurr, num_queues);
            }
            else if (pFirst->fulfill_time == shortest_fulfill)
            {
                //check for process in higher priority queue

                current_shortest_queue = findProcessQueue(priorityQueue, pCurr, num_queues);
                queue_checked = findProcessQueue(priorityQueue, pFirst->pProcess, num_queues);

                if (queue_checked < current_shortest_queue)
                {
                    current_shortest_queue = queue_checked;
                    shortest_fulfill = pFirst->fulfill_time;
                    pCurr = pFirst->pProcess;
                }
            }
        }
        pFirst = pFirst->pNext;
    }

    return pFirst;
}

int checkQueues(Queue *priorityQueue, ListNode **pCurrList, int *curr_queue, IONode **pIOList, int num_queues, int *curr_time)
{

    int i, idle_time, shortest_fulfill = -1, curr_queue_temp;
    int current_shortest_queue = -1, queue_checked;
    ListNode *pCurr = NULL, *pTemp;
    IONode *pIOCurr = *pIOList;

    for (i = 0; i < num_queues; i++)
    {
        if ((priorityQueue + i)->pCurr == NULL)
        {
            continue;
        }

        if (((priorityQueue) + i)->pCurr->ready == 1)
        { //check if ready
            if (((priorityQueue) + i)->pCurr->arrival_time <= *curr_time)
            { //check if process has arrived

                *pCurrList = ((priorityQueue) + i)->pCurr; //set pCurrList and curr_queue to highest priority queue with arrived process
                *curr_queue = i;

                return 0; //Found ready process that has already arrived
            }
        }
    }

    pIOCurr = getNearestFulfill(pIOCurr, priorityQueue, num_queues);

    //Check if the next IO is shorter than arrival time of next process
    if ((priorityQueue + 0)->pCurr != NULL)
    {

        if (pCurr != NULL && current_shortest_queue != -1)
        { //Check if there are processes in IO

            if (current_shortest_queue == 0)
            { //if shortest IO is at highest priority queue
                if (shortest_fulfill <= (priorityQueue + 0)->pCurr->arrival_time)
                { //Shortest fulfill_time is nearer than shortest arrival_time
                    *pCurrList = ((priorityQueue) + 0)->pCurr;
                    *curr_queue = 0;
                    *curr_time = shortest_fulfill; //skip to shortest fulfill_time
                }
                else
                {
                    *pCurrList = ((priorityQueue) + 0)->pCurr;
                    *curr_queue = 0;
                    *curr_time = (priorityQueue + 0)->pCurr->arrival_time; //skip to shortest arrival_time
                }
            }
            else
            {
                if (shortest_fulfill < (priorityQueue + 0)->pCurr->arrival_time) //IO in lower queues is nearer than shortest process arrival in top queue
                {
                    *pCurrList = ((priorityQueue) + current_shortest_queue)->pCurr;
                    *curr_queue = current_shortest_queue;
                    *curr_time = shortest_fulfill; //skip to shortest fulfill_time
                }
                else
                {
                    *pCurrList = ((priorityQueue) + 0)->pCurr;
                    *curr_queue = 0;
                    *curr_time = (priorityQueue + 0)->pCurr->arrival_time; //skip to shortest arrival_time
                }
            }
        }
        else
        {

            *pCurrList = ((priorityQueue) + 0)->pCurr;
            *curr_queue = 0;
            *curr_time = (priorityQueue + 0)->pCurr->arrival_time; //skip to shortest arrival_time
        }
    }
    // all queues are empty

    return 1;
}

//Gets closest arrival/fulfill_time from queue higher than curr_queue
int getNextPriorityTime(Queue *priorityQueue, IONode *pFirst, int curr_queue, int curr_time)
{

    int min_time = -1;
    IONode *pTemp = pFirst;
    int i;
    for (i = 0; i < curr_queue; i++)
    {

        if ((priorityQueue + i)->pCurr == NULL)
        {
            continue;
        }

        if (min_time == -1)
        {
            if ((priorityQueue + i)->pCurr->arrival_time > curr_time)
            {
                min_time = (priorityQueue + i)->pCurr->arrival_time;
            }
        }

        else if ((priorityQueue + i)->pCurr->arrival_time < min_time && (priorityQueue + i)->pCurr->arrival_time > curr_time)
        {
            min_time = (priorityQueue + i)->pCurr->arrival_time;
        }
    }

    while (pTemp != NULL)
    {

        if (min_time == -1)
        {
            if (pTemp->fulfill_time > curr_time && pTemp->pProcess->queue < curr_queue)
            {
                min_time = pTemp->fulfill_time;
            }
        }

        else if (pTemp->fulfill_time < min_time && pTemp->fulfill_time > curr_time && pTemp->pProcess->queue < curr_queue)
        {
            min_time = pTemp->fulfill_time;
        }

        pTemp = pTemp->pNext;
    }

    if (min_time < curr_time)
    {
        min_time = -1;
    }

    return min_time;
}

void mlfq(Queue *priorityQueue, int num_queues, ListNode *pId, int priorityBoost, float *average_time)
{
    int processing = 1; //condition for continuing while loop
    int curr_queue = 0;
    int robin_result = 0;
    int interval_num = 1;        //how many intervals of priority boost have passed
    int curr_time = 0;           //pass as pointer to robin function
    int wait_time = 0;           //pass as pointer to robin function
    int next_priority_time = -1; //pass to robin function
    int check_queue_return;
    IONode *pIOList = NULL; //pass as pointer to robin function
    ListNode *pCurrList = NULL;

    addListNodeToQueue(priorityQueue + 0, pId);

    pCurrList = (priorityQueue + 0)->pCurr;
    curr_time = pCurrList->arrival_time;

    while (processing)
    {

        robin_result = robin(&pCurrList, &pIOList, &curr_time, &wait_time, (priorityQueue + curr_queue)->timeQuantum, priorityBoost, interval_num, next_priority_time, average_time);

        switch (robin_result)
        {
        case 0:
        {
            //completed all processes in current queue, check other queues
            (priorityQueue + curr_queue)->pCurr = NULL;
            int i = 0;
            while ((priorityQueue + i)->pCurr != NULL && i < num_queues)
            {
                i++;
            }
            if (i < num_queues)
            {
                curr_queue = i;
            }
        }
        break;

        case 1: //time for priority boost
            pCurrList = moveProcessToBack(pCurrList);
            (priorityQueue + curr_queue)->pCurr = pCurrList;

            //PROBLEM: nammove to back kapag sinundan yung isang process na kakatapos lang
            if (pIOList != NULL)
            {
                pIOList = processIO(pIOList, curr_time);
            }

            boost(priorityQueue, num_queues, curr_time);

            interval_num++;

            //if currtime > PRIORITY BOOST * interval_num
            //priority boost
            //probably make a function on this
            //note that when not moving queues, time quantum for that node is not reset
            //note that when process in IO mode, not part of PB (just check yung ready field)

            break;

        case 2:
        { //idle time exists, check other queues
        }
        break;

        case 3:
        {

            pCurrList = moveProcessToBack(pCurrList);

            (priorityQueue + curr_queue)->pCurr = pCurrList;

            //higher priority process is ready
            //do nothing
            break;
        }

        case 4:
        {
            //boost but without moving curr to back
            (priorityQueue + curr_queue)->pCurr = pCurrList;

            if (pIOList != NULL)
            {
                pIOList = processIO(pIOList, curr_time);
            }
            boost(priorityQueue, num_queues, curr_time);

            interval_num++;
            break;
        }

        default:
        { //current process has to move to lower queue
            ListNode *pCurr = pCurrList;

            if (curr_queue != num_queues - 1) //means it's not on the last level
            {
                pCurr->time_allotment = 0;
                if ((priorityQueue + curr_queue + 1)->pCurr == NULL)
                {

                    ListNode *pNext = pCurrList->pNext;
                    pCurr->pNext = NULL;
                    pCurr->queue = curr_queue + 1;
                    (priorityQueue + curr_queue + 1)->pCurr = pCurr;
                    (priorityQueue + curr_queue)->pCurr = pNext;
                }

                else
                {
                    ListNode *pDest = (priorityQueue + curr_queue + 1)->pCurr;
                    ListNode *pNext = moveNodeToQueue(&pDest, pCurr, curr_time, 2);
                    (priorityQueue + curr_queue)->pCurr = pNext;
                }
            }
        }
        }

        if (pIOList != NULL)
        {
            pIOList = processIO(pIOList, curr_time);
        }

        //make a function here that checks all queues from high priority to low, then get corresponding process list (if going the ready route, then check lang yung ready state ng mga nodes)
        check_queue_return = checkQueues(priorityQueue, &pCurrList, &curr_queue, &pIOList, num_queues, &curr_time);

        switch (check_queue_return)
        {
        case 0:
            next_priority_time = getNextPriorityTime(priorityQueue, pIOList, curr_queue, curr_time);
            break;
        default:
            processing = 0;
        }
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
        if (s == NULL || s->arrival_time > a->arrival_time)
        {
            a->pNext = s;
            s = a;
        }

        else if (s->arrival_time == a->arrival_time)
        {
            if (s->id > a->id)
            {
                a->pNext = s;
                s = a;
            }

            else //Locate node before point of insertion (ID BASED)
            {
                ListNode *current = s;

                while (current->pNext && current->pNext->id < a->id && current->pNext->arrival_time <= a->arrival_time)
                {
                    current = current->pNext;
                }
                a->pNext = current->pNext;
                current->pNext = a;
            }
        }

        else
        {
            /* Locate the node before the point of insertion */
            ListNode *current = s;

            while (current->pNext && current->pNext->arrival_time < a->arrival_time)
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
        priorityQueues[i].pCurr = NULL;
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

    float average_time = 0;

    mlfq(priorityQueues, X, pId, S, &average_time);

    printf("\nAverage waiting time: %.2f\n\n", average_time / (Y * 1.0));

    fclose(fptr);
}
