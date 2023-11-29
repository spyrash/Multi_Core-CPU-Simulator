
# Progetto Fake OS Simulation
## Descrizione
Il progetto implementa una simulazione di un sistema operativo fittizio (Fake OS). Il sistema include la gestione di processi, CPU burst, e IO burst. Il scheduler implementato è un round-robin su più core.

## Requisiti
- Compilatore GCC (GNU Compiler Collection)
- Sistema operativo Linux o ambiente UNIX-like
## Compilazione
Per compilare il programma, eseguire il seguente comando nella directory del progetto:

```
 gcc -o sched_sim linked_list.c fake_os.c fake_process.c sched_sim.c -std=gnu99 -Wall 
```
## Esecuzione
Il programma accetta file di input che specificano la sequenza di eventi per ciascun processo. Esempio di file di input:

```
PROCESS 1 0
CPU_BURST 1
IO_BURST 30
CPU_BURST 1
IO_BURST 30
CPU_BURST 1
IO_BURST 30
CPU_BURST 1
```

Per eseguire il programma, utilizzare il seguente comando:

```
./sched_sim <file_processo1> <file_processo2> ... <file_processoN>
```
Dove `<file_processo1>`, `<file_processo2>`, ...,` <file_processoN>` sono i file di input per i singoli processi.

## Test
Nella cartella test sono presenti file di input per i test. Per eseguire i test, utilizzare il seguente comando:


```
./sched_sim test/*
```
## Specifiche Tecniche
Il simulatore di sistema operativo è implementato in linguaggio C.
La gestione della lista concatenata è implementata nella libreria linked_list.c.
Il main programma sched_sim.c inizializza il sistema operativo fittizio, carica i processi, e avvia la simulazione.
Gli scheduler sono implementati nella funzione schedRR in sched_sim.c utilizzando un algoritmo round-robin.
Il tempo di quantum per ciascun core può essere regolato nella variabile srr_args.quantum in sched_sim.c.
I file di input dei processi seguono uno specifico formato, come mostrato negli esempi di test.
Per ulteriori dettagli sul funzionamento del codice, consultare il codice sorgente nei file linked_list.c, fake_os.c, fake_process.c, e sched_sim.c.
