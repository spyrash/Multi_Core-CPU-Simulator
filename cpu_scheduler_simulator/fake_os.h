#include "fake_process.h"
#include "linked_list.h"
#pragma once
#define  num_core 4  //numero cpu da utilizzare, quad_core in questo caso


typedef struct {
  ListItem list;
  int pid;
  ListHead events;
} FakePCB;

struct FakeOS;
typedef void (*ScheduleFn)(struct FakeOS* os, void* args , int core); //aggiunto in core per vedere di quale cpu parliamo***********************************
typedef struct FakeOS{
  FakePCB* running[num_core];  //dichiarato un array di running(cpu che lavorano in simultanea)
  ListHead ready;
  ListHead waiting;
  int timer;
  ScheduleFn schedule_fn;
  void* schedule_args;

  ListHead processes;
} FakeOS;

void FakeOS_init(FakeOS* os);
void FakeOS_simStep(FakeOS* os);
void FakeOS_destroy(FakeOS* os);
