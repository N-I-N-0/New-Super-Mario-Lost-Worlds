import os

class DolOverlord():
        def __init__(self, dolPath):
            '''DolOffsets Constructor'''
            self.dolOffsets = []
            self.realPointers = []
            self.sectionSizes = []

            try:
                with open(dolPath, "rb") as f:
                    self.dol = f.read()
            except:
                print("Couldn't open file \"{}\"".format(dolPath))
                return

            try:
                for i in range(0, 0x48, 4):
                    self.dolOffsets += [int.from_bytes(self.dol[i:i+4], byteorder='big', signed=False)]
                    self.realPointers += [int.from_bytes(self.dol[i+0x48:i+0x4C], byteorder='big', signed=False)]
                    self.sectionSizes += [int.from_bytes(self.dol[i+0x90:i+0x94], byteorder='big', signed=False)]
            except:
                print("Something wrong with dol!")

            for dolOffset in self.dolOffsets:
                if (dolOffset > 0x10000000):
                    print("Dol offset out of range (>0x10000000)!")
                    return

            for realPointer in self.realPointers:
                if (realPointer != 0 and (realPointer < 0x80004000 or realPointer > 0x81200000)):
                    print("Dol got a not allowed pointer!")
                    return

            for sectionSize in self.sectionSizes:
                if (sectionSize > 0x10000000):
                    print("Section size too large (>0x10000000)!")
                    return

        def getOffsetFromPointer(self, pointer):
            for i in range(0, len(self.dolOffsets)):
                if (pointer >= self.realPointers[i] and pointer < (self.realPointers[i] + self.sectionSizes[i])):
                    return self.dolOffsets[i] + (pointer - self.realPointers[i]);
            return 0;


        def getOffset(self, address):
            # Get pointer value, check if it's valid
            pointerValue = int(address.replace("0x", ""), 16)
            if (pointerValue < 0x80004000 or pointerValue > 0x81200000):
                print("Invalid address (address must be < 0x80004000 or > 0x81200000)")
                return -1

            # Get offset from address, check if it's in the DOL Range
            offsetValue = self.getOffsetFromPointer(pointerValue)
            if offsetValue > 0:
                if isVerbose:
                    print("Address: {} -> Offset: {}".format("0x%08X" % pointerValue, "0x%08X" % offsetValue))
                return offsetValue
            else:
                print("Address is outside of DOL range.")
                return -1


        def getAddressFromOffset(self, offset):
            for i in range(0, len(self.dolOffsets)):
                if (offset >= self.dolOffsets[i] and offset <= (self.dolOffsets[i] + self.sectionSizes[i])):
                    return self.realPointers[i] + (offset - self.dolOffsets[i])
            return 0;

        def getAddress(self, offset):
            offsetValue = int(offset.replace("0x", ""), 16)

            # Get address from offset, check if it's in the DOL Range
            addressValue = self.getAddressFromOffset(offsetValue)
            if (addressValue > 0):
                if isVerbose:
                    print("Offset: {} -> Address: {}".format("0x%08X" % offsetValue, "0x%08X" % addressValue))
                return addressValue
            else:
                print("Offset is outside of DOL range.")
                return -1


def getOffset(dolPath, address):
    dolOff = DolOverlord(dolPath)
    return dolOff.getOffset(address)


def getAddress(dolPath, offset):
    dolOff = DolOverlord(dolPath)
    return dolOff.getAddress(offset)



isVerbose = False

def main(argv):
    if len(argv) > 1:
        if argv[1] in ("-offset", "-o"):
            if len(argv) > 3:
                getOffset(argv[2], argv[3])
            else:
                print("Example: -offset \"main.dol\" \"0x800B64EC\"")
        elif argv[1] in ("-address", "-a"):
            if len(argv) > 3:
                getAddress(argv[2], argv[3])
            else:
                print("Example: -address \"main.dol\" \"0x000B252C\"")

    else:
        print("Wrong usage!")

if __name__ == '__main__':
    import sys

    isVerbose = True

    main(sys.argv)


#todo:
#- compare dols
#- create Riivolution patch from differences
#- patch into dol
#  - from Riivolution patch
#- rel support?
