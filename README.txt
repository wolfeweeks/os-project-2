To compile and run this project, enter the following commands:
1. make
2. ./master [-t maxSeconds] numOfProcs
  where:
    -t: the maximum number of seconds to allow the program to run
    numOfProcs: the number of child processes to spawn
  
Problems:
  Couldn't figure out how to kill slave processes on signal interrupts