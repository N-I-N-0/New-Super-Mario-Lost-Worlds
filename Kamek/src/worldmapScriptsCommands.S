.text
.align 4

#include <asm_setup.S>

.extern returnForCalledFromExecute
.extern continueInCalledFromExecute
.extern FUN_808fbd00
.extern makeWorldSelectionShowUp__Fv

.global checkIfNewCommand
checkIfNewCommand:
	beq     return
	cmpwi   r4, 0xA9					#new command 169
	beq     prepareThisShop
	cmpwi	r4, 0xAA					#new command 170
	beq		prepareThisWorldSelection
	b       continueInCalledFromExecute

return:
	b       returnForCalledFromExecute

prepareThisShop:
	bl      FUN_808fbd00
	b       continueInCalledFromExecute

prepareThisWorldSelection:
	bl      FUN_808fbd00
	bl		makeWorldSelectionShowUp__Fv
	b       continueInCalledFromExecute