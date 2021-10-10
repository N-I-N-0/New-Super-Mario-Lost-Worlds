.text
.align 4
#include <asm_setup.S>


.extern IsGravityEnabled			#Orig -> 0x802f6900
.extern GravityFloat_maybe			#Orig -> 0x8042bb44 = -4.0f
.extern line_80054688				#Orig -> 0x80054688
.extern line_8005714C				#Orig -> 0x8005714C
.extern ZoneMaxYValue_maybe			#Orig -> 0x80001910
.extern ContinueFromFirstHook		#Orig -> 0x80070EF8
.extern ContinueFromSecondHook		#Orig -> 0x800E4540
.extern ContinueFromThirdHook		#Orig -> 0x80057108
.extern ContinueFromFourthHook		#Orig -> 0x80127720
.extern ContinueFromFifthHook		#Orig -> 0x80057154
.extern ContinueFromSixthHook_One	#Orig -> 0x8004C5EC
.extern ContinueFromSixthHook_Two	#Orig -> 0x8004C5C4
.extern ContinueFromSeventhHook		#Orig -> 0x8006CDE4
.extern ContinueFromEighthHook		#Orig -> 0x801C0D20
.extern ContinueFromNinthHook		#Orig -> 0x800B07F0
.extern ContinueFromTenthHook		#Orig -> 0x80128258
.extern ContinueFromEleventhHook	#Orig -> 0x80926774
.extern ContinueFromTwelvethHook	#Orig -> 0x8006F7B8
.extern ContinueFromThirteenthHook	#Orig -> 0x80064F54
.extern ContinueFromFourteenthHook	#Orig -> 0x80070CC4
.extern loc_8005475C
.extern loc_8005467C

.extern playersUpsideDown


.global FirstHook		#0x80070ef4 -> 0x802f929c
.global SecondHook		#0x800e453c -> 0x802f69a0
.global ThirdHook		#0x80057104 -> 0x802f6b00
.global FourthHook		#0x8012771c -> 0x802f6c00
.global FifthHook		#0x8005714c -> 0x802f6d00
.global SixthHook		#0x8004c5bc -> 0x802f6800
.global SeventhHook		#0x8006cde0 -> 0x802f6908
.global EighthHook		#0x801c0d1c -> 0x802f6740
.global NinthHook		#0x800b07ec -> 0x80001800 / 0x803482C0 (now i get why it broke in newer and refused to work on emu lmao)
.global TenthHook		#0x80128254 -> 0x80001990 / 0x80348450
.global EleventhHook	#0x80926770 -> 0x80001b20 / 0x803485E0
.global TwelvethHook	#0x8006f7b4
.global ThirteenthHook	#0x80064f50
.global FourteenthHook	#0x80070CC0
.global FifteenthHook	#0x80054678

FirstHook:	#Basically, sets the gravity floats depending of the gravity swap flag
	mflr	r0
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	stwu	r6, -0x4(r1)
	stwu	r7, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4

	cmpwi	r4, 0
	#.int 0x48000000
	bne     continueFirst

	lhz		r4, -0x1E4(r3)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerFirst
	lis     r4, 0
	b		continueFirst
	
isPlayerFirst:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, -0x1E8(r3)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)
	
continueFirst:
	lis		r6, GravityFloat_maybe@h		#Store Gravity float in r6
	ori		r6, r6, GravityFloat_maybe@l
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	beq		firstHook_gravityNormal			#Set back the gravity floats to normal
	lis		r4, 0x4080						#r4 = 0x40800000 = 4.0f
	stw		r4, 0(r6)						#Store r4's content (A.K.A. 4.0f) at 0x8042bb44 (GravityFloat_maybe) (which was -4.0f before)
	stw		r4, 0xF8(r30)					#Storing it at 0x00F8(r30) too, for some reason
	#stw		r4, -0x5e40(r5)
	lis		r4, 0x40C0						#r4 = 0x40C00000 = 6.0f
	stw		r4, 0x70(r6)					#Store r4's content (A.K.A. 6.0f) at 0x8042BBB4 (which was -6.0f before)
	lis		r4, 0							#r4 = 0
	stw		r4, 0x74(r6)					#Store r4's content (A.K.A. 0.0f) at 0x8042BBB8 (which was -2.0f before)
	lfs		f1, 0xB0(r30)					#Store r30->pos.y in f1
	lfs		f0, 0(r6)						#Store 0(r6) in f0, which is now 4.0f
	fadds	f1, f1, f0						#f1 = f1 + f0
	fadds	f1, f1, f0						#f1 = f1 + f0
	stfs	f1, 0x98(r30)					#Store f1 at 0x98(r30)
	lis		r4, 0x8000						#r4 = 0x80000000 = -0.0f ???
	stw		r4, 0x104(r30)					#Store r4's content (A.K.A. -0.0f) in r30->rot.y
	lbz		r4, 0x274(r30)					#Store 0x274(r30) in r4 (seems to be collmgr->isOnTopOfGround or smth)
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	beq		firstHook_return				#Return from first hook and continue the orig function
	li		r4, 0							#r4 = 0
	stw		r4, 0x274(r30)					#Store r4's content (A.K.A. 0) at 0x274(r30)
	lis		r4, 0							#What's the fucking point ? r4 ALREADY IS 0 !!!
	stw		r4, 0xEC(r30)					#Store r4's content (A.K.A. 0) at 0xEC(r30)
	li		r4, 0x3099						#r4 = 0x3099
	stw		r4, 0x10D4(r30)					#Store r4's content (A.K.A. 0x3099) at 0x10D4(r30)
	lis		r4, 0x4000						#r4 = 0x40000000 = 2.0f
	stw		r4, 0x74(r6)					#Store r4's content (A.K.A. 2.0f) at 0x8042BBB8 (which was 0.0f before)
	lis		r4, 0x6000						#r4 = 0x60000000 = "nop" instruction
	lis		r7, line_80054688@h
	ori		r7, r7, line_80054688@l
	stw		r4, 0(r7)						#This is the worse way i've ever seen to nop a line. If it works then fine, i don't have the quarter of that guy's talent afterall.
	#lis		r4, 0x4180
	#ori		r4, r4, 0x0008					#Wait why ? It seems to correspond to a "blt- this+0xC" instruction but weird anyway
	#stw		r4, 0x2ac4(r7)
	#lis		r7, line_8005714C@h
	#ori		r7, r7, line_8005714C@l
	#stw		r4, 0(r7)

	lwzu	r7, 0(r1)						#I have no idea what this line and the 5 following lines do, gotta ask grop on that next time i have occasion to
	lwzu	r6, 0x4(r1)
	lwzu	r5, 0x4(r1)
	lwzu	r4, 0x4(r1)
	addi 	r1, r1, 4
	b		ContinueFromFirstHook			#Continue at first hook's orig function
firstHook_gravityNormal:
	lis		r4, 0xC080						#r4 = 0xC0800000 = -4.0f
	stw		r4, 0(r6)						#Store r4's content (A.K.A. -4.0f) at 0x8042bb44 (GravityFloat_maybe) (which was whatever (probably 4.0f) before)
	lis		r4, 0xC0C0						#r4 = 0xC0C00000 = -6.0f
	stw		r4, 0x70(r6)					#Store r4's content (A.K.A. -6.0f) at 0x8042BBB4 (which was whatever (probably 6.0f) before)
	stw		r4, 0xF8(r30)					#Store r4's content (A.K.A. -6.0f) in r30->max_speed.y
	lis		r4, 0xC000						#r4 = 0xC0000000 = -2.0f
	stw		r4, 0x74(r6)					#Store r4's content (A.K.A. -2.0f) at 0x8042BBB8 (which was whatever (probably 2.0f) before)
	lis		r4, 0							#r4 = 0
	stw		r4, 0x104(r30)					#Store r4's content (A.K.A. 0) in r30->rot.y
firstHook_return:
	lwzu	r7, 0(r1)						#I have no idea what this line and the 4 following lines do, gotta ask grop on that next time i have occasion to
	lwzu	r6, 0x4(r1)
	lwzu	r5, 0x4(r1)
	lwzu	r4, 0x4(r1)
	addi 	r1, r1, 4
	b		ContinueFromFirstHook			#Continue at first hook's orig function


SecondHook:	#Basically, set some gravity float if the gravity flag is 0x10
	addi	r11, r1, 32						#Stack shit
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	stwu	r6, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag's address in r5
	ori		r5, r5, IsGravityEnabled@l
	stw		r0, 0(r5)						#Store r0 in Gravity Flag from r5
	cmpwi	r0, 0x10						#Compare r0 with 0x10, if not equal, then lower line
	bne		secondHook_return				#Return from second hook and continue the orig function
	lis		r6, GravityFloat_maybe@h		#Store Gravity float's address in r6
	ori		r6, r6, GravityFloat_maybe@l
	lis		r4, 0x4080						#r4 = 0x40800000 = 4.0f
	stw		r4, 0(r6)						#Store r4's content (A.K.A. 4.0f) at 0x8042BBB4 (which was probably -4.0f before)
secondHook_return:
	lwzu	r6, 0(r1)						#More stack shit iirc
	lwzu	r5, 4(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b		ContinueFromSecondHook			#Continue at second hook's orig function


ThirdHook:	#Basically, do addition or subtraction shit with floats depending of the gravity flag
	stwu	r4, -0x4(r1)					#Stack shit again cuz ppc big brain
	stwu	r5, -0x4(r1)
	stwu	r6, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	
	cmpwi	r4, 0
	#.int 0x48000000
	bne     continueThird

	lhz		r4, 0x8(r30)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerThird
	lis     r4, 0
	b		continueThird
	
isPlayerThird:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r30)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueThird:
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	beq		thirdHook_AddFloats				#So, lemme explain the 7 following lines - if r4 = 0, then f0 = f1 + f0, otherwise then f0 = f1 - f0
	b		thirdHook_SubtractFloats
thirdHook_AddFloats:
	fadds	f0, f1, f0
	b		thirdHook_return
thirdHook_SubtractFloats:
	fsubs	f0, f1, f0
thirdHook_return:
	lwzu	r6, 0(r1)						#Guess what ? Stack Shit
	lwzu	r5, 4(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b		ContinueFromThirdHook


FourthHook:	#Basically, change the current player's y speed and set some gravity float depending of the gravity flag, probably applies to enemies too
	stwu	r4, -0x4(r1)					#STACK SHIIIIT (seriously what does it do ?)
	stwu	r5, -0x4(r1)
	stwu	r6, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	bne		continueForth
	
	lhz		r4, 0x8(r31)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerForth
	lis     r4, 0
	b		fourthHook_Return
	
isPlayerForth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r31)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	beq		fourthHook_Return
	
	
continueForth:
	lis		r6, GravityFloat_maybe@h		#Store Gravity float in r6
	ori		r6, r6, GravityFloat_maybe@l
	lis		r4, 0xC000						#r4 = 0xC0000000 = -2.0f
	stw		r4, 0x74(r6)					#Store r4's content (A.K.A. -2.0f) at 0x8042BBB8 (which was probably 2.0f before)
	fneg	f1, f1							#f1 = -f1
fourthHook_Return:
	stfs	f1, 0xEC(r31)					#Store f1 in r31->speed.y (r31 being this, A.K.A. the current player)
	lwzu	r6, 0(r1)						#Stack Shit OwO !
	lwzu	r5, 4(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b		ContinueFromFourthHook


FifthHook:	#Basically, do shit with floats and store it in the current player's y speed or not, probably applies to enemies too
 	stwu	r4, -0x4(r1)					#UwU
 	stwu	r5, -0x4(r1)
 	stwu	r6, -0x4(r1)
 	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
 	ori		r5, r5, IsGravityEnabled@l
 	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
 	cmpwi	r4, 0							#Compare r4 with 0, if it's not equal, then lower line
 	
	bne		fifthHook_doBLEComparaison
	
	
	lhz		r4, 0x8(r30)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerFifth
	lis     r4, 0
	b		continueFifth
	
isPlayerFifth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r30)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueFifth:	
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	bne		fifthHook_doBLEComparaison		#So - If no gravity swap, then if f0 =< f29, then return, otherwise, set r31->speed.y to f29 - If gravity, same but if f0 => f29
	fcmpo	cr0, f0, f29
 	bge		fifthHook_return
 	b		fifthHook_storeF29IntoYSpeed
fifthHook_doBLEComparaison:
 	fcmpo	cr0, f0, f29
 	ble		fifthHook_return
fifthHook_storeF29IntoYSpeed:
 	stfs	f29, 0xEC(r30)					#Store f29 in r31->speed.y (r31 being this, A.K.A. the current player)
fifthHook_return:
 	lwzu	r6, 0(r1)						#ÈwÉ
 	lwzu	r5, 4(r1)
 	lwzu	r4, 4(r1)
 	addi	r1, r1, 4
 	b		ContinueFromFifthHook

# FifthHook:								#Don't know why this was added - apparently disables tripple jumping while the other version does not
# 	stwu	r4, -0x4(r1)					#UwU
# 	stwu	r5, -0x4(r1)
# 	stwu	r6, -0x4(r1)
# 	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
# 	ori		r5, r5, IsGravityEnabled@l
# 	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
# 	fabs	f28, f0
#	fabs	f31, f29
#	fcmpo	cr0, f28, f31
#	ble-	fifthHook_return
#	stfs	f29, 0xEC(r30)
# fifthHook_return:
#	lfs		f31, 0x4(r5)
# 	lwzu	r6, 0(r1)						#ÈwÉ
# 	lwzu	r5, 4(r1)
# 	lwzu	r4, 4(r1)
# 	addi	r1, r1, 4
# 	b		ContinueFromFifthHook


SixthHook:	#I don't want to comment anymore at this point
	stwu	r4, -0x4(r1)					#Meh.
	stwu	r5, -0x4(r1)
	stwu	r6, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it's not equal, then lower line
	bne		sixthHook_DoFNEGShit			#I said i don't want to comment anymore at this point !!!


	lhz		r4, 0x8(r29)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerSixth
	lis     r4, 0
	b		continueSixth
	
isPlayerSixth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r29)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueSixth:
	cmpwi	r4, 0							#Compare r4 with 0, if it's not equal, then lower line
	bne		sixthHook_DoFNEGShit			#I said i don't want to comment anymore at this point !!!




	fcmpo	cr0, f0, f1
	ble-	sixthHook_firstReturn
	b		sixthHook_secondReturn
sixthHook_DoFNEGShit:
	fneg	f1, f1
	fcmpo	cr0, f0, f1
	fneg	f1, f1
	bge-	sixthHook_firstReturn
	b		sixthHook_secondReturn
sixthHook_firstReturn:
	lwzu	r6, 0(r1)						#Oh, there's two returns now ?!
	lwzu	r5, 4(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b		ContinueFromSixthHook_One
sixthHook_secondReturn:
	lwzu	r6, 0(r1)						#Rah shit !
	lwzu	r5, 4(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b		ContinueFromSixthHook_Two


SeventhHook:#Basically, do shit with actor properties
	lfs		f2, 0xEC(r3)					#Store r3->speed.y in f2
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	bne     continueSeventh2
	
	lhz		r4, 0x8(r3)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerSeventh
	lis     r4, 0
	b		continueSeventh
	
isPlayerSeventh:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r3)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueSeventh:
	cmpwi	r4, 0		
	beq		seventhHook_gravityNormal		#80dfcd9c
	
continueSeventh2:
	lis		r4, 0x4080						#r4 = 0x40800000 = 4.0f
	stw		r4, 0xF8(r3)					#Store r4's content (A.K.A. 4.0f) in r3->max_speed.y
	lis		r4, 0x8000						#r4 = 0x80000000 = -0.0f ???
	stw		r4, 0x104(r3)					#Store r4's content (A.K.A. 4.0f) in r3->rot.y
	b		seventhHook_return
	#I have no idea how you can get there, but it's in the code so i'm rewriting it anyway
	lis		r5, GravityFloat_maybe@h		#Store Gravity float in r6
	ori		r5, r5, GravityFloat_maybe@l
	lfs		f1, 0xB0(r3)					#Store r3->pos.y in f1
	lfs		f0, 0(r5)						#Store Gravity float in f0
	fadds	f1, f1, f0						#f1 = f1 + f0
	fadds	f1, f1, f0						#f1 = f1 + f0
	stfs	f1, 0x98(r3)					#Store f1 in r3->matrix[7]
	b		seventhHook_return
seventhHook_gravityNormal:
	lis		r4, 0xC080						#r4 = 0xC0800000 = -4.0f
	stw		r4, 0xF8(r3)					#Store r4's content (A.K.A. -4.0f) in r3->max_speed.y
	lis		r4, 0							#r4 = 0
	stw		r4, 0x104(r3)					#Store r4's content (A.K.A. 0) in r3->rot.y
seventhHook_return:
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b		ContinueFromSeventhHook


EighthHook:	#Meh.
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	

	bne     continueEigth2
	
	lhz		r4, 0x8(r31)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerEigth
	lis     r4, 0
	b		continueEigth
	
isPlayerEigth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r31)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueEigth:
	cmpwi	r4, 0		
	beq		eighthHook_return
	
continueEigth2:


	lis		r5, GravityFloat_maybe@h		#Store Gravity float in r6
	ori		r5, r5, GravityFloat_maybe@l
	cmpwi	r31, 0							#Get if the r31 pointer is null, if yes, return
	beq		eighthHook_return
	cmpwi	r3, 0							#Get if the r3 pointer is null, if yes, return
	beq		eighthHook_return
	lis		r4, 0x8200
	cmpw	r31, r4							#Get if r31 is in the good range, if it's not, return
	bge		eighthHook_return
	lis		r4, 0x8150
	cmpw	r31, r4							#Get if r31 is in the good range, if it's not, return
	ble		eighthHook_return
	lbz		r4, 0x9(r31)					#Store r31->name in r4 ???
	cmpwi	r4, 51							#If r31 is a goomba
	beq		eighthHook_ItsAnEnemy
	cmpwi	r4, 133							#If r31 is a bob-omb
	beq		eighthHook_ItsAnEnemy
	cmpwi	r4, 134							#If r31 is a para bob-omb
	beq		eighthHook_ItsAnEnemy
	cmpwi	r4, 54							#If r31 is a koopa troopa
	beq		eightHook_ItsAKoopa
	b		eighthHook_return				#If it's anything else, return
eightHook_ItsAKoopa:
	lfs		f1, 0x70(r5)					#Store 0x70(r5) in f1
	fadds	f2, f2, f1						#f2 = f2 + f1
	fadds	f2, f2, f1						#f2 = f2 + f1
	b		eighthHook_return				#If it's anything else, return
eighthHook_ItsAnEnemy:
	lfs		f1, 0x70(r5)					#Store 0x70(r5) in f1
	fadds	f2, f2, f1						#f2 = f2 + f1
	lfs		f1, 0x10(r5)					#Store 0x10(r5) in f1
	fsubs	f2, f2, f1						#f2 = f2 + f1
eighthHook_return:
	stfs	f2, 0x1C(r3)					#Store f2 in r3->link_connect.next ???
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b ContinueFromEighthHook


NinthHook:
	lfs		f1, 0x70(r3)					#Store r3->ActivePhysics.secondFloatArray in f1
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line

	bne     continueNinth2
	
	lhz		r4, 0x8(r3)						#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerNinth
	lis     r4, 0
	b		continueNinth
	
isPlayerNinth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r3)						#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueNinth:
	cmpwi	r4, 0		
	beq		ninthHook_return
	
continueNinth2:
	fcmpo	cr0, f1, f0						#if f1 =< f0, then lower line
	ble		ninthHook_return
	fneg	f1, f1							#f1 = -f1
ninthHook_return:
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b ContinueFromNinthHook


TenthHook:
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	bne     continueTenth2
	
	lhz		r4, 0x8(r29)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerTenth
	lis     r4, 0
	b		continueTenth
	
isPlayerTenth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r29)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueTenth:
	cmpwi	r4, 0		
	beq		tenthHook_return
	
continueTenth2:
	fneg	f1, f1							#f1 = -f1
tenthHook_return:
	stfs	f1, 0xEC(r29)					#Store f1 in r29->speed.y
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b ContinueFromTenthHook


EleventhHook:								#resets values
	stwu	r1, -0x10(r1)
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	li		r4, 0							#r4 = 0
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	stw		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	stw		r4, 0(r5)
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b ContinueFromEleventhHook


TwelvethHook:
	stwu	r4, -0x4(r1)
	stwu	r5, -0x4(r1)
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r5
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	bne     continueTwelveth2
	
	lhz		r4, -0x1E4(r3)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerTwelveth
	lis     r4, 0
	b		continueTwelveth
	
isPlayerTwelveth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, -0x1E8(r3)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueTwelveth:
	cmpwi	r4, 0		
	beq		twelvethHook_return
	
continueTwelveth2:
	lbz		r4, 0x88(r3)					#"should be 4!"
	cmpwi	r4, 0							#Compare r4 with 0, if it equals, then lower line
	beq		twelvethHook_return
	lis		r5, GravityFloat_maybe@h		#Store Gravity float in r6
	ori		r5, r5, GravityFloat_maybe@l
	lfs		f3, 0(r5)
	fadds	f1, f1, f3
	fadds	f1, f1, f3
	fadds	f1, f1, f3
	lis		r4, 0
	stb		r4, 0x88(r3)
twelvethHook_return:
	fcmpo	cr0, f1, f0
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b ContinueFromTwelvethHook


ThirteenthHook:
	stwu	sp, -0x40(sp)
	stwu	r4, -0x4(sp)
	stwu	r5, -0x4(sp)
	cmpwi	r9, 0							#Quoting ErikHK - "sometimes r9 is zero for some reason.."
	beq		thirteenthHook_return
	lfs		f12, -0xd88(r9)
	lis		r5, ZoneMaxYValue_maybe@h
	ori		r5, r5, ZoneMaxYValue_maybe@l
	stfs	f12, 0(r5)
thirteenthHook_return:
	lwzu	r5, 0(r1)
	lwzu	r4, 4(r1)
	addi	r1, r1, 4
	b ContinueFromThirteenthHook





FourteenthHook:
	mr		r20, r3
	lwz		r0, 0x88(r3)
	rlwinm	r3, r0, 0x0, 0xb, 0x12
	neg		r0, r3
	or		r0, r0, r3
	srwi	r3, r0, 31						#orig
	
	mr		r21, r5
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r0
	ori		r5, r5, IsGravityEnabled@l
	lwz		r5, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r5, 0
	bne     fourteenthHook_gravityEnabled
	
	lhz		r5, -0x1E4(r20)					#name
	cmpwi   r5, 13							#is player?
	beq		isPlayerFourteenth
	lis     r5, 0
	b		continueFourteenth
	
isPlayerFourteenth:
	lwz		r5, -0x1E8(r20)					#settings
	mr		r20, r4
	lis		r4, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r4, r4, playersUpsideDown@l
	add		r5, r5, r4
	lbz		r5, 0(r5)

continueFourteenth:
	cmpwi	r5, 0	
	mr		r5, r21
	mr		r4, r20
	bne		fourteenthHook_gravityEnabled
	blr
fourteenthHook_gravityEnabled:
	mr		r5, r21
	li		r3, 1
	blr

FifteenthHook:
	beq		fifteenthHook_return
	mr		r21, r5
	mr		r20, r4
	lis		r5, IsGravityEnabled@h			#Store Gravity flag in r0
	ori		r5, r5, IsGravityEnabled@l
	lwz		r4, 0(r5)						#Get if the gravity swap is enabled in r4
	cmpwi	r4, 0
	bne     continueFifteenth2
	
	lhz		r4, 0x8(r29)					#name
	cmpwi   r4, 13							#is player?
	beq		isPlayerFifteenth
	lis     r4, 0
	b		continueFifteenth
	
isPlayerFifteenth:
	lis		r5, playersUpsideDown@h			#Store Gravity flag in r5
	ori		r5, r5, playersUpsideDown@l
	
	lwz		r4, 0x4(r29)					#settings
	add		r5, r5, r4
	lbz		r4, 0(r5)

continueFifteenth:
	cmpwi	r4, 0
	
continueFifteenth2:
	mr		r5, r21
	mr		r4, r20
	bne		fifteenthHook_return
	b		loc_8005467C
fifteenthHook_return:
	b loc_8005475C