#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <string>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

void CleanInfo(TasksInfo& info){
  for (int i = 0; i < 4; ++i){
    if (info[static_cast<TaskStatus>(i)] == 0) {
      info.erase(static_cast<TaskStatus>(i));
    }
  }
}

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const{
    return team.at(person);
  }
  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person){
    team[person][TaskStatus::NEW]++;
  }
  
  // Обновить статусы по данному количеству задач конкретного разработчика
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count){
    TasksInfo updated_tasks, untouched_tasks;
    if (!team.count(person)) return make_tuple(updated_tasks, untouched_tasks);

    updated_tasks[TaskStatus::IN_PROGRESS] = min(team[person][TaskStatus::NEW], task_count);
    untouched_tasks[TaskStatus::NEW] = team[person][TaskStatus::NEW] - updated_tasks[TaskStatus::IN_PROGRESS];
    task_count -= updated_tasks[TaskStatus::IN_PROGRESS];

    updated_tasks[TaskStatus::TESTING] = min(team[person][TaskStatus::IN_PROGRESS], task_count);
    untouched_tasks[TaskStatus::IN_PROGRESS] = team[person][TaskStatus::IN_PROGRESS] - updated_tasks[TaskStatus::TESTING];
    task_count -= updated_tasks[TaskStatus::TESTING];

    updated_tasks[TaskStatus::DONE] = min(team[person][TaskStatus::TESTING], task_count);
    untouched_tasks[TaskStatus::TESTING] = team[person][TaskStatus::TESTING] - updated_tasks[TaskStatus::DONE];
    task_count -= updated_tasks[TaskStatus::DONE];


    team[person][TaskStatus::NEW] -= updated_tasks[TaskStatus::IN_PROGRESS];
    team[person][TaskStatus::IN_PROGRESS] += updated_tasks[TaskStatus::IN_PROGRESS];

    team[person][TaskStatus::IN_PROGRESS] -= updated_tasks[TaskStatus::TESTING];
    team[person][TaskStatus::TESTING] += updated_tasks[TaskStatus::TESTING];

    team[person][TaskStatus::TESTING] -= updated_tasks[TaskStatus::DONE];
    team[person][TaskStatus::DONE] += updated_tasks[TaskStatus::DONE];

    CleanInfo(updated_tasks);
    CleanInfo(untouched_tasks);
    CleanInfo(team[person]);

    return make_tuple(updated_tasks, untouched_tasks);
  }

private:
  map<string, TasksInfo> team;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
  cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
      ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
      ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
      ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
  TasksInfo updated_tasks, untouched_tasks;
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  return 0;
}