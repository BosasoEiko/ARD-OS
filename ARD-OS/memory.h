#define MAX_PID 256       //Max number of processes
#define CLUSTER_SIZE 256  //Size of a cluster in the MAT (in bytes)

File mat = fclear("mat");  //Memory Allocation Table:
File mem = fclear("mem");  //Virtual memory (actual memory for the programs)
uint32_t pat[MAX_PID];     //Program Allocation Table: stores the addresses of the programs in the MAT (could be done in a file, but this is faster)

void pwrites(const uint32_t pid, const uint32_t val, const uint8_t size, const uint32_t pos) {
  uint32_t end;  //End of the last cluster available for the program
  mem.seekSet(pos);
  for (int8_t i = size - 1; i >= 0; i--) mem.write(val >> i * 8);
}

void palloc(const uint32_t pid, const uint8_t num = 1) {  //Allocates one or more clusters to a program's memory
  mat.seekSet(pat[pid]);
  uint32_t next;
  do {                       //While next is not 0 (so there is another cluster)
    mat.seekSet(mat.curPosition() + 8); //Skips the start of the cluster in memory and the used space in the cluster in memory
    next = freads(mat, 4);   //Address of the next start of the cluster in the MAT
    mat.seekSet(next);
  } while (next);
  mat.seekSet(mat.curPosition() - 4);
}