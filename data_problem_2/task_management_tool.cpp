/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

/* @Author
Student Name: Ahmet Alper Yılmaz
Student ID : 150180037
Date: <9.11.2019> */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head = NULL; // setting our head pointer to null
}

void WorkPlan::close()
{
	Task * iter = head->previous; // we will start deleting from the end  
	Task * trv= iter;
	Task * trvs= iter;
	while(iter != head){ // when we finish deleting one day, going to previous day until we come to the head
		while(iter->counterpart != NULL){ // deleting days task times by using counterparts
			trv = iter;
			trvs = iter;
			while(trv->counterpart !=NULL){
				trv = trv->counterpart;
			}
			while(trvs->counterpart != trv){
				trvs = trvs->counterpart;
			}
			trvs->counterpart = NULL;
			delete(trv);
		}
	if(iter->counterpart == NULL){ // if we come to the last task of the day
		trv = iter;
		trvs = iter;
	}
	iter = iter->previous; // when we finish deleting one day, going to previous day
	delete(trv);
	}
	while(head->counterpart != NULL){ // deleting head day's tasks
		trv = iter;
		trvs = iter; 
			while(trv->counterpart !=NULL){
				trv = trv->counterpart;
			}
			while(trvs->counterpart != trv){
				trvs = trvs->counterpart;
			}
			trvs->counterpart = NULL;
			delete(trv);
	}
	if(head->counterpart==NULL){ // finally the last task is head, deleting head
		delete(head);
	}
}

void WorkPlan::add(Task *task)
{
	if (head == NULL){ // if head is null, creating a task and filling it.
		Task * temp = new Task;
		head = temp;
		head->next = head;
		head->previous = head;
		head->counterpart = NULL;
		head->day = task->day;
		head->time = task->time;
		head->name = new char[strlen(task->name)];
		strcpy(head->name , task->name);
		head->priority = task->priority;
		return;
	}
	if (head->day > task->day){ // if adding task's day is less than head's day,we are changing the head
		Task *temp = new Task;
		Task *iter = head;
		temp->previous = head->previous;
		head->previous = temp;
		temp->next = head;
		while (iter->next != head){
			iter = iter->next;
		}
		iter->next = temp;
		temp->name = new char[strlen(task->name)];
		strcpy(temp->name , task->name);
		temp->day = task->day;
		temp->time = task->time;
		temp->priority = task->priority;
		temp->counterpart = NULL;
		head = temp;
		return;	
	}
	if (head->day == task->day){
		if (head->time > task->time){ // adding task's time is less than head's time, we are changing head again
			Task *temp = new Task;
			Task *iter = head;
			temp->counterpart = head;
			if (head->next == head){ // if the head is the only day in the linked list
				head->next = NULL;
				head->previous = NULL;
				temp->previous = temp;
				temp->next = temp;
				head = temp;
				return;
			}
			head->next->previous = temp;
			temp->next = head->next;
			temp->previous = head->previous;
			while (iter->next != head){ // finding the head's previous and setting its next to temp
				iter = iter->next;
			}
			iter->next = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			head->next = NULL;
			head->previous = NULL;
			head = temp;
			return;
		}
		if (head->time == task->time){ // if times are equal looking for priority
			if(head->priority < task->priority){ // adding task's priority is bigger than head's task, we are delaying the head's task and making adding task, head 
				Task * temp = new Task;
				temp->name = new char[strlen(head->name)];
				strcpy(temp->name , head->name);
				temp->day = head->day;
				temp->time = head->time;
				temp->priority = head->priority;
				checkAvailableNextTimesFor(head);
				temp->day = getUsableDay();
				temp->time = getUsableTime();
				strcpy(head->name , task->name);
				head->priority = task->priority;
				add(temp);
				delete(temp);
				return;
			}
			if(head->priority > task->priority){ // delaying adding's task
				checkAvailableNextTimesFor(task);
				task->day = getUsableDay();
				task->time = getUsableTime();
				add(task);
				return;
			}
		}
		if (head->counterpart == NULL){ // adding task's time is bigger than head's time and head is the only task in the day
			Task *temp = new Task;
			head->counterpart = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			temp->counterpart = NULL;
			return;
		}
		Task *iter = head;
		while (iter->counterpart != NULL && iter->counterpart->time < task->time){ // finding the correct task position for adding task
			iter = iter->counterpart;
		}
		if(iter->counterpart == NULL){ // if adding task's time is bigger than that day's tasks
			Task *temp = new Task;
			iter->counterpart = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			temp->counterpart = NULL;
			return;
		}
		if (iter->counterpart->time == task->time){ // if adding task's time is equal to another task
			if(iter->counterpart->priority > task->priority){ // looking for priorities and less priority task will be delayed.
				checkAvailableNextTimesFor(task);
				task->day = getUsableDay();
				task->time = getUsableTime();
				add(task);
				return;
			}
			if(task->priority > iter->counterpart->priority){ // looking for priorities and less priority task will be delayed.
				Task * temp = new Task;
				temp->name = new char[strlen(iter->counterpart->name)];
				strcpy(temp->name , iter->counterpart->name);
				temp->day =iter->counterpart->day;
				temp->time =iter->counterpart->time;
				temp->priority =iter->counterpart->priority;
				checkAvailableNextTimesFor(iter->counterpart);
				temp->day = getUsableDay();
				temp->time = getUsableTime();
				strcpy(iter->counterpart->name , task->name);
				iter->counterpart->priority = task->priority;
				add(temp);
				delete(temp);
				return;
			}
		}
		if (iter->counterpart->time > task->time){	// adding task's time is somewhere between the first task of the day and last task of the day
			Task *temp = new Task;
			temp->counterpart = iter->counterpart;
			iter->counterpart = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			return;
		}
	}
	Task *iter = head; // finding the adding task's day in the linked list
	while(iter->next != head && iter->next->day < task->day){
		iter = iter->next;
	}
	if (iter->next == head){ // if adding task's day is bigger than last day
		Task *temp = new Task;
		temp->next = head;
		temp->previous = iter;
		head->previous = temp;
		iter->next = temp;
		temp->name = new char[strlen(task->name)];
		strcpy(temp->name , task->name);
		temp->day = task->day;
		temp->time = task->time;
		temp->priority = task->priority;
		temp->counterpart = NULL;
		return;
	}
	if (iter->next->day > task->day){ // adding task's day is somewhere between first day and last day and doesn't exist in the linked list
		Task * temp = new Task;
		temp->next = iter->next;
		temp->previous = iter;
		iter->next->previous = temp;
		iter->next = temp;
		temp->name = new char[strlen(task->name)];
		strcpy(temp->name , task->name);
		temp->day = task->day;
		temp->time = task->time;
		temp->priority = task->priority;
		temp->counterpart = NULL;
		return;
	}
	if (iter->next->day == task->day){ // finding the adding's task day in the linked list
		Task *iterator = iter->next;
		if (iterator->time > task->time){ // adding task will be the first task of the day
			Task *temp = new Task;
			temp->counterpart = iterator;
			iterator->next->previous = temp;
			temp->next = iterator->next;
			iterator->next = NULL;
			temp->previous = iterator->previous;
			iterator->previous = NULL;
			iter->next = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			iterator = temp;
			return;
		}
		if (iterator->time == task->time){ // adding task's day and time are already in the linked list so we are looking to the priority
			if(iterator->priority > task->priority){ // looking for priorities and less priority task will be delayed.
				checkAvailableNextTimesFor(task);
				task->day = getUsableDay();
				task->time = getUsableTime();
				add(task);
				return;
			}
			if(task->priority > iterator->priority){ // looking for priorities and less priority task will be delayed.
				Task * temp = new Task;
				temp->name = new char[strlen(iterator->name)];
				strcpy(temp->name , iterator->name);
				temp->day =iterator->day;
				temp->time =iterator->time;
				temp->priority =iterator->priority;
				checkAvailableNextTimesFor(iterator);
				temp->day = getUsableDay();
				temp->time = getUsableTime();
				strcpy(iterator->name , task->name);
				iterator->priority = task->priority;
				add(temp);
				delete(temp);
				return;
			}
		}
		if (iterator->counterpart == NULL){ // if the adding task's time is bigger than the first task of the day and there is only one task in that day
			Task *temp = new Task;
			iterator->counterpart = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			temp->counterpart = NULL;
			return;
		}
		Task *trs = iterator; // finding the correct task position for adding task
		while (trs->counterpart != NULL && trs->counterpart->time < task->time){
			trs = trs->counterpart;
		}
		if(trs->counterpart == NULL){ // adding task's time is bigger than that day's tasks
			Task *temp = new Task;
			trs->counterpart = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			temp->counterpart = NULL;
			return;
		}
		if (trs->counterpart->time == task->time){ // adding task's time is already in that day so we are looking to priorities.
			if(trs->counterpart->priority > task->priority){ // looking for priorities and less priority task will be delayed.
				checkAvailableNextTimesFor(task);
				task->day = getUsableDay();
				task->time = getUsableTime();
				add(task);
				return;
			}
			if(task->priority > trs->counterpart->priority){ // looking for priorities and less priority task will be delayed.
				Task * temp = new Task;
				temp->name = new char[strlen(trs->counterpart->name)];
				strcpy(temp->name , trs->counterpart->name);
				temp->day =trs->counterpart->day;
				temp->time =trs->counterpart->time;
				temp->priority =trs->counterpart->priority;
				checkAvailableNextTimesFor(trs->counterpart);
				temp->day = getUsableDay();
				temp->time = getUsableTime();
				strcpy(trs->counterpart->name , task->name);
				trs->counterpart->priority = task->priority;
				add(temp);
				delete(temp);
				return;
			}
		}
		if (trs->counterpart->time > task->time){ // adding task's time is between the first task of the day and last task of the day 
			Task *temp = new Task;
			temp->counterpart = trs->counterpart;
			trs->counterpart = temp;
			temp->name = new char[strlen(task->name)];
			strcpy(temp->name , task->name);
			temp->day = task->day;
			temp->time = task->time;
			temp->priority = task->priority;
			return;
		}
	}
			 
} 

Task * WorkPlan::getTask(int day, int time)
{
	Task * iter = head;
	while (iter->day != day){
		iter = iter->next;
	} // finding the target day in the linked list
	Task * trs = iter;
	while (trs->time != time){
	trs = trs->counterpart;
	} // finding the target time in the target day
	return trs; // returning its address
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{
	int day=delayed->day; // day and time variables holding informations. 
	int time=delayed->time; 
	Task * iter = head;
	while (iter->day != day){ // finding the delayed day with iter in the linked list
		iter = iter->next;
	}
	Task * trs = iter;
	Task * trvs = delayed;
	while(iter->next != head){
		while(trs->counterpart != NULL){ // moving in the day's times with trs and counterparts
			trs = trs->counterpart;
		}
		do{ 
			if(time != trvs->time){ // if the day and time are not already taken by another task, that day and time are usable. 
				usable_day = day;
				usable_time = time;
				return;
			}
			if(time<16){ // time increase until 16
				time++;
			}
			if(trvs->counterpart != NULL){ // moving in the day's times with trs and counterparts
				trvs = trvs->counterpart;
			}	
		}while(trvs->time <= trs->time && time !=16 && time != trs->time); //if we come to the last task of the day or time is 16 or 16 is already taken by another task; moving to the next day
		time = 8;															
		iter = iter->next;
		trs = iter;
		trvs = iter;
		day++;
	}
	if (iter->next == head){ // if we are at the last day of the linked list, same thing will be done
		while(trs->counterpart != NULL){
			trs = trs->counterpart;
		}
		do{ 
			if(time != trvs->time){
				usable_day = day;
				usable_time = time;
				return;
			}
			if(time<16){
				time++;
			}
			if(trvs->counterpart != NULL){
				trvs = trvs->counterpart;
			}   	   	
		}while(trvs->time <= trs->time && time !=16 && time != trs->time);
		time = 8;
		iter = iter->next;
		trs = iter;
		trvs = iter;
		day++;
		usable_day = day;
		usable_time = time;
	}

}

void WorkPlan::delayAllTasksOfDay(int day)
{
	Task * iters = head; // finding the day with iters
	while(iters->day != day){
		iters = iters->next;
	}
	while(iters->counterpart != NULL){ //creating a temp for holding info and creating trvs for removing old task 
		Task * trvs = new Task; 
		Task * temp = new Task;
		temp->day = iters->day;
		temp->time = iters->time;
		temp->priority = iters->priority;
		temp->name = new char[strlen(iters->name)];
		strcpy(temp->name , iters->name);
		while(iters->day == temp->day){ // finding the usable day and time, in the next other days
		checkAvailableNextTimesFor(iters);
		temp->day= getUsableDay();
		temp->time = getUsableTime();
		iters->time = temp->time;
		}
		trvs = iters;
		iters = iters->counterpart;
		remove(trvs); // removing old task
		add(temp); // adding new task and delaying will be done.
	} // if iters->counterpart is NULL, we are at the last task of the day
	Task * trvse = new Task; //creating a tempo for holding info and creating trvse for removing old task
	Task * tempo = new Task;
	tempo->day = iters->day;
	tempo->time = iters->time;
	tempo->priority = iters->priority;
	tempo->name = new char[strlen(iters->name)];
	strcpy(tempo->name , iters->name);
	while(iters->day == tempo->day){ // finding the usable day and time, in the next other days
	checkAvailableNextTimesFor(iters);
	tempo->day= getUsableDay();
	tempo->time = getUsableTime();
	iters->time = tempo->time;
	}
	trvse = iters;
	remove(trvse);
	add(tempo);
	
}

void WorkPlan::remove(Task *target)
{
	Task * iter = head; // finding the target day
	while (iter->day != target->day){
		iter = iter->next;
	}
	Task * trv = iter; // finding the target time
	while (trv->time != target->time){
		trv = trv->counterpart; 
	}
	if (trv==head){ // if we are removing the head day and time
		if (head->counterpart == NULL){ // if the head is the only task in the day, we are making next day as a new head
			Task * temp = head;
			temp->next->previous = temp->previous;
			temp->previous->next = temp->next;
			head = head->next;
			delete(temp);
			return;
		} // if the head is not the only task in the day, we are making its counterpart new head of the linked list
		trv->counterpart->previous = trv->previous;
		trv->counterpart->next = trv->next;
		trv->next->previous = trv->counterpart;
		trv->previous->next = trv->counterpart;
		trv->next = NULL;
		trv->previous = NULL;
		head = trv->counterpart;
		iter = head;
		trv->counterpart = NULL;
		delete(trv);
		return;
	}
	if (trv->counterpart == NULL){ // if the removing tasks counterpart is null that means there is no task later than removing task in that
		if(trv->next != NULL){ // if the removing task is the only task in the day
			trv->previous->next = trv->next;
			trv->next->previous = trv->previous;
			delete(trv);
			return;
		} 
		if (trv->next == NULL){ // if the removing task is the last task of that day
			while(iter->counterpart != trv){
				iter = iter->counterpart;
			}
			iter->counterpart = NULL;
			delete(trv);
			return;
		}
	}
	if(trv->next != NULL && trv->counterpart != NULL){ // if the removing task is the first task in that day and there are other tasks in that day
		trv->counterpart->previous = trv->previous;
		trv->counterpart->next = trv->next;
		trv->next->previous = trv->counterpart;
		trv->previous->next = trv->counterpart;
		trv->next = NULL;
		trv->previous = NULL;
		trv->counterpart = NULL;
		delete(trv);
		return;
	}
	if(trv->counterpart != NULL && trv->next == NULL){ // if the removing task is in somewhere between the first task of the day and last task of the day
		while(iter->counterpart != trv){
			iter = iter->counterpart;
		}
		iter->counterpart = trv->counterpart;
		delete(trv);
		return;
	}

}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
