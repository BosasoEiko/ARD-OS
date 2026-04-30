#define PTR_SIZE 4  //Size in bytes of the pointers

/* MEMORY STRUCTURE
[PP] [MBP] [PROGRAM] [MP] [R0] [MEMORY]

PP (Program Pointer): Points to the instruction that needs to be executed
MBP (Memory Base Pointer): Points to the start of the usable memory segment (R0)
PROGRAM: Copy of the program executable in memory
MP (Memory Pointer): Points to the next free byte of memory (accessed by the program with 0x-4)
R0 (Register 0): Zero register, read-only (accessed by the program with 0x00)
MEMORY: Usable memory segment (accessed by the program with 0x04...)
*/

void load(File &exe, const uint32_t pos) {  //Loads executable in a defined position in memory (TODO: automatic memory sectors)
  mem.seekSet(pos);

  fwrites(mem, pos + 2 * PTR_SIZE, PTR_SIZE);               //PP
  fwrites(mem, pos + 3 * PTR_SIZE + exe.size(), PTR_SIZE);  //MBP
  fcopy(exe, mem, 0, -1);                                   //PROGRAM
  fwrites(mem, pos + 4 * PTR_SIZE + exe.size(), PTR_SIZE);  //MP
}

void execute(const uint32_t start) {  //Executes an instruction of a program loaded in memory, with its memory space starting at start
  mem.seekSet(start);

  uint32_t pp[2] = {
    freads(mem, PTR_SIZE),  //Program pointer value
    start                   //Program pointer position
  };

  uint32_t mbp[2] = {
    freads(mem, PTR_SIZE),  //Program pointer value
    start + PTR_SIZE        //Program pointer position
  };

  uint32_t mp[2] = {
    freads(mem, PTR_SIZE, mbp[0] - PTR_SIZE),  //Program pointer value
    mbp[0] - PTR_SIZE                          //Program pointer position
  };

  mem.seekSet(pp[0]);
  uint32_t inst[7] = {
    freads(mem, SET_SIZE),  //op
    freads(mem, SET_SIZE),  //type 1
    freads(mem, PTR_SIZE),  //data 1
    freads(mem, SET_SIZE),  //type 2
    freads(mem, PTR_SIZE),  //data 2
    freads(mem, SET_SIZE),  //type 3
    freads(mem, PTR_SIZE),  //data 3
  };

  switch (inst[0]) {
    default:
      break;
  }
}