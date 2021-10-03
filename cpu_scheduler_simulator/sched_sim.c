#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fake_os.h"


FakeOS os;

typedef struct {
  int quantum;
} SchedRRArgs;

void schedRR(FakeOS* os, void* args_, int core){ //aggiunto core per vedere di quale parliamo *************************************************************************
  SchedRRArgs* args=(SchedRRArgs*)args_;

  // look for the first process in ready
  // if none, return
  if (! os->ready.first){
    return;
    }
  // printf(" il core entrato è %d \n",core);  //decommentare per vedere quale cpu entra nel round robin
  FakePCB* pcb=(FakePCB*) List_popFront(&os->ready);
  os->running[core]=pcb;
  
  assert(pcb->events.first);
  ProcessEvent* e = (ProcessEvent*)pcb->events.first;
  assert(e->type==CPU);

  // look at the first event
  // if duration>quantum
  // push front in the list of event a CPU event of duration quantum
  // alter the duration of the old event subtracting quantum
  if (e->duration>args->quantum) {
    ProcessEvent* qe=(ProcessEvent*)malloc(sizeof(ProcessEvent));
    qe->list.prev=qe->list.next=0;
    qe->type=CPU;
    qe->duration=args->quantum;
    e->duration-=args->quantum;
    List_pushFront(&pcb->events, (ListItem*)qe);
  }
};

int main(int argc, char** argv) {
  FakeOS_init(&os);
  SchedRRArgs srr_args;
  srr_args.quantum=5; //quanto di tempo definito
  os.schedule_args=&srr_args;
  os.schedule_fn=schedRR;
  
  for (int i=1; i<argc; ++i){
    FakeProcess new_process;
    int num_events=FakeProcess_load(&new_process, argv[i]);
    printf("loading [%s], pid: %d, events:%d \n",
           argv[i], new_process.pid, num_events);
    if (num_events) {
      FakeProcess* new_process_ptr=(FakeProcess*)malloc(sizeof(FakeProcess));
      *new_process_ptr=new_process;
      List_pushBack(&os.processes, (ListItem*)new_process_ptr);
    }
  }
  printf("num processes in queue %d\n", os.processes.size);
  while(os.running[0]
        || os.running[1] //aggiunto ogni core nel while
        || os.running[2]
        || os.running[3]
        || os.ready.first
        || os.waiting.first
        || os.processes.first){
		  //mettere un if(os.timer == x) return 0 per fare x sim_step
    FakeOS_simStep(&os);
  }
  printf("\nfinito a tempo : %d \n",os.timer);   //print messa per quando finisce e non cicla all'infinito
  printf("size della waiting finale : %d \n",os.waiting.size);
  printf("size della ready finale : %d \n",os.ready.size);
  
  }
