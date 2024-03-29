#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "fake_os.h"


void FakeOS_init(FakeOS* os) {
	for(int i=0;i<num_core;i++){
       os->running[i]=0;             //inizializzo le cpu a zero
	}
  List_init(&os->ready);
  List_init(&os->waiting);
  List_init(&os->processes);
  os->timer=0;
  os->schedule_fn=0;
}
//a
void FakeOS_createProcess(FakeOS* os, FakeProcess* p) {
  // sanity check
  assert(p->arrival_time==os->timer && "time mismatch in creation");
  // we check that in the list of PCBs there is no
  // pcb having the same pid, for each core
  for(int i=0;i<num_core;i++){ 
  assert( (!os->running[i] || os->running[i]->pid!=p->pid) && "pid taken");
}


  ListItem* aux=os->ready.first;
  while(aux){
    FakePCB* pcb=(FakePCB*)aux;
    assert(pcb->pid!=p->pid && "pid taken");
    aux=aux->next;
  }

  aux=os->waiting.first;
  while(aux){
    FakePCB* pcb=(FakePCB*)aux;
    assert(pcb->pid!=p->pid && "pid taken");
    aux=aux->next;
  }
   
  //printf(" all fine, no such pcb exists \n");
  FakePCB* new_pcb=(FakePCB*) malloc(sizeof(FakePCB));
  new_pcb->list.next=new_pcb->list.prev=0;
  new_pcb->pid=p->pid;
  new_pcb->events=p->events;

  assert(new_pcb->events.first && "process without events");

  // depending on the type of the first event
  // we put the process either in ready or in waiting
  ProcessEvent* e=(ProcessEvent*)new_pcb->events.first;
  switch(e->type){
  case CPU:
    List_pushBack(&os->ready, (ListItem*) new_pcb);   
    break;
  case IO:
    List_pushBack(&os->waiting, (ListItem*) new_pcb);
    break;
  default:
    assert(0 && "illegal resource");
    ;
  }
}




void FakeOS_simStep(FakeOS* os){
  
  printf("************** TIME: %08d **************\n", os->timer);
  //printf("\n size della waiting : %d \n",os->waiting.size); //decommentare per vedere quanti processi sono in waiting
  //scan process waiting to be started
  //and create all processes starting now
  ListItem* aux=os->processes.first;
  while (aux){
    FakeProcess* proc=(FakeProcess*)aux;
    FakeProcess* new_process=0;
    if (proc->arrival_time==os->timer){
      new_process=proc;
    }
    aux=aux->next;
    if (new_process) {
      printf("\tcreate pid:%d\n", new_process->pid);
      new_process=(FakeProcess*)List_detach(&os->processes, (ListItem*)new_process);
      FakeOS_createProcess(os, new_process);
      free(new_process);
    }
  }

  // scan waiting list, and put in ready all items whose event terminates
  aux=os->waiting.first;
  while(aux) {
    FakePCB* pcb=(FakePCB*)aux;
    aux=aux->next;
    ProcessEvent* e=(ProcessEvent*) pcb->events.first;
    printf("\twaiting pid: %d\n", pcb->pid);
    assert(e->type==IO);
    e->duration--;
    printf("\t\tremaining time:%d\n",e->duration);
    if (e->duration==0){
      printf("\t\tend burst\n");
      List_popFront(&pcb->events);
      free(e);
      List_detach(&os->waiting, (ListItem*)pcb);
      if (! pcb->events.first) {
        // kill process
        printf("\t\tend process\n");
        free(pcb);
      } else {
        //handle next event
        e=(ProcessEvent*) pcb->events.first;
        switch (e->type){
        case CPU:
          printf("\t\tmove to ready\n");
          List_pushBack(&os->ready, (ListItem*) pcb);
          break;
        case IO:
          printf("\t\tmove to waiting\n");
          List_pushBack(&os->waiting, (ListItem*) pcb);
          break;
        }
      }
    }
  }

  

  // decrement the duration of running
  // if event over, destroy event
  // and reschedule process
  // if last event, destroy running
  
 //inizio gestione dei running *********
 
  FakePCB* running[num_core];
  for(int core=0;core<num_core;core++){
  running[core]=os->running[core];
  printf("running pid on core %d: %d\n",core+1, running[core]? running[core]->pid:-1); //se la cpu è in running printa il pid del processo in esecuzione altrimenti -1
  if (running[core]) {
    ProcessEvent* e=(ProcessEvent*) running[core]->events.first;
    assert(e->type==CPU);
    e->duration--;
    printf("\t\tremaining time:%d\n",e->duration);
    if (e->duration==0){
      printf("\t\tend burst\n");
      List_popFront(&running[core]->events);
      free(e);
      if (! running[core]->events.first) {
        printf("\t\tend process\n");
        free(running[core]); // kill process
      } else {
        e=(ProcessEvent*) running[core]->events.first;
        switch (e->type){
        case CPU:
          printf("\t\tmove to ready\n");
          List_pushBack(&os->ready, (ListItem*) running[core]);
          break;
        case IO:
          printf("\t\tmove to waiting\n");
          List_pushBack(&os->waiting, (ListItem*) running[core]);
          break;
        }
      }
      os->running[core] = 0;
    }
  }
} //chiusura del for (per il num_core)
  
  
   //fine gestione dei running *******
   
  // call schedule, if defined
  for (int core=0; core < num_core; core++){
   if (os->schedule_fn && (!os->running[core])){         //chiamo la schedule(round robin) su ogni core
     (*os->schedule_fn)(os, os->schedule_args,core); 
   }}
  
	  

  // if running not defined and ready queue not empty
  // put the first in ready to run,for each core
  for (int core=0; core < num_core; core++){
 if ((!os->running[core]) && os->ready.first) {  
    os->running[core]=(FakePCB*) List_popFront(&os->ready);
  } }

  ++os->timer;

}

void FakeOS_destroy(FakeOS* os) {
}
