/**
 * @file SimpleTasker.cpp
 * @author Jan Wielgus
 * @brief 
 * @date 2020-07-31
 * 
 */

#include <SimpleTasker.h>


SimpleTasker::SimpleTasker(uint8_t maxTaskAmt)
    : MaxAmtOfTasks(maxTaskAmt)
{
    if (MaxAmtOfTasks > 0)
        tasksArray = new Task*[MaxAmtOfTasks];
    
    amtOfTasks = 0;
}


SimpleTasker::~SimpleTasker()
{
    if (MaxAmtOfTasks > 0)
        delete[] tasksArray;
}


bool SimpleTasker::addTask(Task* task, float frequency, uint16_t maxDuration)
{
    if (amtOfTasks >= MaxAmtOfTasks)
        return false;

    task->setProperties(frequency, maxDuration);

    // TODO: handle maxDuration param

    // TODO: prevent duplication in task list

    tasksArray[amtOfTasks] = task;
    amtOfTasks++;

    return true;
}


bool SimpleTasker::addTask(Task* task, float frequency)
{
    return addTask(task, frequency, 0);
}


uint32_t SimpleTasker::getCurrentTime_micros()
{
    return lastLoopTime;
}


void SimpleTasker::runLoop()
{
    lastLoopTime = micros();
    uint32_t executedTasksTotalInterval = 0; // summed intervals of executed tasks in this loop

    for (uint8_t i=0; i < amtOfTasks; i++)
    {
        if (lastLoopTime >= tasksArray[i]->nextExecutionTime_us)
        {
            tasksArray[i]->nextExecutionTime_us = lastLoopTime + tasksArray[i]->interval_us;
            tasksArray[i]->execute();

            executedTasksTotalInterval += tasksArray[i]->interval_us;
        }
    }

    // update tasker load
    uint32_t executionsTotalTime = micros() - lastLoopTime;
    if (executedTasksTotalInterval > 0)
        taskerLoad = LoadFilterBeta*taskerLoad + (1-LoadFilterBeta)*((float)executionsTotalTime / executedTasksTotalInterval);
    lastLoopTime += executionsTotalTime; // make time in this variable more recent
}


float SimpleTasker::getTaskerLoad()
{
    return taskerLoad * 100.f;
}


void SimpleTasker::execute()
{
    runLoop();
}
