// 
// 
// 

#include <FC_ObjectTasker.h>


FC_ObjectTasker::FC_ObjectTasker(uint8_t maxTaskAmt)
	: MaxAmtOfTasks(maxTaskAmt)
{
	tasksArray = new FC_Task* [MaxAmtOfTasks];
	amtOfTasks = 0;
}


FC_ObjectTasker::~FC_ObjectTasker()
{
	delete[] tasksArray;
}


bool FC_ObjectTasker::addTask(FC_Task* task, uint32_t interval, uint16_t maxDuration)
{
	if (amtOfTasks >= MaxAmtOfTasks) // array is full
		return false;

	// set task interval and maximum duration
	task->setProperties(interval, maxDuration);

	// Add to the task pointers array
	tasksArray[amtOfTasks] = task;
	amtOfTasks++;

	return true;
}


uint32_t FC_ObjectTasker::getCurrentTime()
{
	return curTime;
}


void FC_ObjectTasker::run()
{
	curTime = micros();
	for (uint8_t i = 0; i < amtOfTasks; i++)
	{
		/*curTime = micros();*/ // measuring time there cause worse efficiency than before for loop

		//if time has elapsed -> execute the task
		if (curTime >= tasksArray[i]->nextExecutionTime)
		{
			tasksArray[i]->nextExecutionTime = curTime + tasksArray[i]->interval;
			tasksArray[i]->execute();
		}
	}
}


void FC_ObjectTasker::execute()
{
	run();
}

