import ctypes

def u32(value):
    return ctypes.c_uint32(value).value

def ppcmask(mb, me):
    maskmb = u32(0xFFFFFFFF >> mb)
    maskme = u32(0xFFFFFFFF << (31 - me))
    return maskmb & maskme if mb <= me else maskmb | maskme

def rlwinm(rA, rS, SH, MB, ME):
    print("r%d = (r%d << %d) & 0x%X" % (rA, rS, SH, ppcmask(MB, ME)))

def extlwi(rA, rS, n, b):
    rlwinm(rA, rS, b, 0, n - 1)

def extrwi(rA, rS, n, b):
    rlwinm(rA, rS, b + n, 32 - n, 31)

def rotlwi(rA, rS, n):
    rlwinm(rA, rS, n, 0, 31)

def rotrwi(rA, rS, n):
    rlwinm(rA, rS, 32 - n, 0, 31)

def slwi(rA, rS, n):
    rlwinm(rA, rS, n, 0, 31 - n)

def srwi(rA, rS, n):
    rlwinm(rA, rS, 32 - n, n, 31)

def clrlwi(rA, rS, n):
    rlwinm(rA, rS, 0, n, 31)

def clrrwi(rA, rS, n):
    rlwinm(rA, rS, 0, 0, 31 - n)

def clrlslwi(rA, rS, b, n):
    rlwinm(rA, rS, n, b - n, 31 - n)

def rlwimi(rA, rS, SH, MB, ME):
    print("r%d = (r%d << %d) | (r%d & ~0x%X)" % (rA, rS, SH, rA, ppcmask(MB, ME)))

def inslwi(rA, rS, n, b):
    rlwimi(rA, rS, 32 - b, b, b + n - 1)

def insrwi(rA, rS, n, b):
    rlwimi(rA, rS, 32 - (b + n), b, (b + n) - 1)

if __name__ == '__main__':
    import sys
    r0 = 0
    r1 = 1
    r2 = 2
    r3 = 3
    r4 = 4
    r5 = 5
    r6 = 6
    r7 = 7
    r8 = 8
    r9 = 9
    r10 = 10
    r11 = 11
    r12 = 12
    r13 = 13
    r14 = 14
    r15 = 15
    r16 = 16
    r17 = 17
    r18 = 18
    r19 = 19
    r20 = 20
    r21 = 21
    r22 = 22
    r23 = 23
    r24 = 24
    r25 = 25
    r26 = 26
    r27 = 27
    r28 = 28
    r29 = 29
    r30 = 30
    r31 = 31
    eval(sys.argv[1])