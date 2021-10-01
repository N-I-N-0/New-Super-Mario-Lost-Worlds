#include <game.h>
/* Code Debugging Functions */

/* CDPrintCurrentAddress()
   This function prints the address the code execution is at when calling this functions (link register).
*/
extern "C" void CDPrintCurrentAddress();
int actuallyPrintLine(int addr){
	OSReport("Code execution at: 0x%x\n", addr);
	return addr;
}

/* CDWait()
   This function adds an infinite jump to itself, effectively halting the code execution. Pause the game in
   dolphin once the window shows 0 fps and replace the current instruction with a nop instruction. You now
   are at the current address the game is executing and can start debugging. 
*/
inline void CDWait() {
	while(true) {}
}

/* CDFloat2Hex(float f)
   This function prints a float in it's hex representation, if you don't have an internet connection and
   can't use https://www.h-schmidt.net/FloatConverter/IEEE754.html for example.
*/
void CDFloat2Hex(float f) {
	OSReport("Float: %f = Hex: 0x%x\n", f, f);
}