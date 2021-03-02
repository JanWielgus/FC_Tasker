/**
 * @file SimpleTasker.h
 * @author Jan Wielgus
 * @brief Simple tasker class
 * @date 2020-03-06
 * 
 */

#ifndef SIMPLETASKER_H
#define SIMPLETASKER_H

#include <ITasker.h>
#include <Task.h>

// TODO: maxDuration is probably useless, remove it from code   


class SimpleTasker : public ITasker, public Task
{
protected:
    const uint8_t MaxAmtOfTasks;
    Task** tasksArray; // array of MaxAmtOfTasks size
    uint8_t amtOfTasks = 0; // current amount of tasks (at most MaxAmtOfTasks)
    uint32_t lastLoopTime = 0; // current time (used in run() method)

    const float LoadFilterBeta = 0.999f;
    float taskerLoad = 0; // 0 - 0%, 1 - 100%, theoretically could be higher


public:
    SimpleTasker(uint8_t maxTaskAmt);
    virtual ~SimpleTasker();

    /**
     * @brief Add task and set its frequency and maxDuration.
     * 
     * @param task Pointer to concrete class that extend Task abstract class.
     * @param frequency Task running frequency.
     * @param maxDuration Measured task maxDuration (if not checked, set 0).
     * @return false if tasks array is full and this task was not added,
     * true otherwise.
     */
    bool addTask(Task* task, float frequency, uint16_t maxDuration) override;

    /**
     * @brief Add task and set it's frequency.
     * 
     * @param task Pointer to concrete class that extend Task abstract class.
     * @param frequency Task running frequency.
     * @return false if tasks array is full and this task was not added,
     * true otherwise.
     */
    bool addTask(Task* task, float frequency) override;

    /**
     * @brief Return (almost) current time. Faster than micros().
     */
    uint32_t getCurrentTime_micros() override;

    /**
     * @brief This should be the only method in loop(). Execute one Task execution time checking.
     * 
     */
    void runLoop() override;

    /**
     * @return Tasker load in percents.
     */
    float getTaskerLoad(); // TODO: if this feature is workign, add it to the interface


protected:
    /**
     * @brief Enable to add mainTasker to another mainTasker as a task.
     * 
     */
    void execute() override;
};


#endif

