# coding=utf-8

import os
import yaml
import sys

original = 'pal' #< Here, you need to select the version you use.
todo = ['pal', 'pal2', 'ntsc', 'ntsc2', 'jpn', 'jpn2', 'kor', 'twn']
fix_for = {}
unfix_from = {}
version_str = 'Kamek 1.0 by Treeki, RoadrunnerWMC, CLF78, John10v10 and Nin0'


def make_hex_offset(offs):
    return '0x%08X' % offs


def fix_offs_pal_v1(offs): return offs
def unfix_offs_pal_v1(offs): return offs
# jpn1

def fix_offs_jpn_v1(offs):
        if 0x80000000 <= offs <= 0x800b4600:
                return offs - 0x0
        elif 0x800b4630 <= offs <= 0x800b4630:
                return offs - 0x20
        elif 0x800b4660 <= offs <= 0x800b466b:
                return offs - 0x4C
        elif 0x800b4670 <= offs <= 0x800b46ab:
                return offs - 0x50
        elif 0x800b4734 <= offs <= 0x800b475f:
                return offs - 0xD8
        elif 0x800b4760 <= offs <= 0x800c8daf:
                return offs - 0xD0
        elif 0x800c8e50 <= offs <= 0x800e4c5f:
                return offs - 0x170
        elif 0x800e4c60 <= offs <= 0x800e4c63:
                return offs - 0x168
        elif 0x800e4c64 <= offs <= 0x800e4c6b:
                return offs - 0x174
        elif 0x800e4c6c <= offs <= 0x800e4d6f:
                return offs - 0x170
        elif 0x800e4d94 <= offs <= 0x800e4d9b:
                return offs - 0x194
        elif 0x800e4da0 <= offs <= 0x800e4ebf:
                return offs - 0x194
        elif 0x800e4ec0 <= offs <= 0x8010f1df:
                return offs - 0x190
        elif 0x8010f1ec <= offs <= 0x8010f203:
                return offs - 0x19C
        elif 0x8010f234 <= offs <= 0x8010f237:
                return offs - 0x1BC
        elif 0x8010f238 <= offs <= 0x8010f23b:
                return offs - 0x1C8
        elif 0x8010f23c <= offs <= 0x8010f23f:
                return offs - 0x1D0
        elif 0x8010f240 <= offs <= 0x8010f243:
                return offs - 0x1CC
        elif 0x8010f244 <= offs <= 0x8010f257:
                return offs - 0x1C8
        elif 0x8010f32c <= offs <= 0x8010f32f:
                return offs - 0x29C
        elif 0x8010f330 <= offs <= 0x8010f333:
                return offs - 0x298
        elif 0x8010f338 <= offs <= 0x8010f33f:
                return offs - 0x29C
        elif 0x8010f340 <= offs <= 0x8010f347:
                return offs - 0x298
        elif 0x8010f378 <= offs <= 0x8010f387:
                return offs - 0x2C8
        elif 0x8010f3ec <= offs <= 0x8010f417:
                return offs - 0x32C
        elif 0x8010f41c <= offs <= 0x802bb6bf:
                return offs - 0x330
        elif 0x802bb6d0 <= offs <= 0x802bb74f:
                return offs - 0x340
        elif 0x802bb860 <= offs <= 0x802bbbff:
                return offs - 0x450
        elif 0x802bbc90 <= offs <= 0x80317737:
                return offs - 0x4E0
        elif 0x80317750 <= offs <= 0x80322ff7:
                return offs - 0x4F8
        elif 0x803230a0 <= offs <= 0x803230c3:
                return offs - 0x5A0
        elif 0x80323118 <= offs <= 0x8035197f:
                return offs - 0x5E0
        elif 0x80351980 <= offs <= 0x80427e5f:
                return offs - 0x580
        elif 0x80427e88 <= offs <= 0x8042954b:
                return offs - 0x5A8
        elif 0x80429560 <= offs <= 0x80429563:
                return offs - 0x5BC
        elif 0x80429570 <= offs <= 0x80429d7f:
                return offs - 0x5C8
        elif 0x80429d80 <= offs <= 0x806dffff:
                return offs - 0x5C0
        elif 0x806e0000 <= offs <= 0x80779abf:
                return offs - 0x0
        elif 0x80779b78 <= offs <= 0x80779b7f:
                return offs - 0xB8
        elif 0x80779b84 <= offs <= 0x80779b93:
                return offs - 0xB8
        elif 0x80779bbc <= offs <= 0x80779bcb:
                return offs - 0xE0
        elif 0x80779c1c <= offs <= 0x8078891f:
                return offs - 0x130
        elif 0x807889d8 <= offs <= 0x807889df:
                return offs - 0x1E8
        elif 0x807889e4 <= offs <= 0x807889f3:
                return offs - 0x1E8
        elif 0x80788a1c <= offs <= 0x80788a2b:
                return offs - 0x210
        elif 0x80788a7c <= offs <= 0x80789eef:
                return offs - 0x260
        elif 0x80789f00 <= offs <= 0x808d3b87:
                return offs - 0x270
        elif 0x808d3bd4 <= offs <= 0x808d3bd7:
                return offs - 0x2B4
        elif 0x808d3bd8 <= offs <= 0x808d3bdb:
                return offs - 0x2BC
        elif 0x808d3bdc <= offs <= 0x808d3be3:
                return offs - 0x2B8
        elif 0x808d3be4 <= offs <= 0x808d3be7:
                return offs - 0x2AC
        elif 0x808d3be8 <= offs <= 0x808d3beb:
                return offs - 0x2B4
        elif 0x808d3bec <= offs <= 0x808d3bef:
                return offs - 0x2BC
        elif 0x808d3bf0 <= offs <= 0x808d3bf3:
                return offs - 0x2C4
        elif 0x808d3bf4 <= offs <= 0x808d3c17:
                return offs - 0x2B8
        elif 0x808d3c20 <= offs <= 0x80940cb3:
                return offs - 0x2C0
        elif 0x80940ea0 <= offs <= 0x80940ed3:
                return offs - 0x4AC
        elif 0x80940ed8 <= offs <= 0x80940f07:
                return offs - 0x4B0
        elif 0x80940f58 <= offs <= 0x80943187:
                return offs - 0x4E8
        elif 0x8094318c <= offs <= 0x809431ab:
                return offs - 0x4EC
        elif 0x809431b0 <= offs <= 0x809431cf:
                return offs - 0x4F0
        elif 0x809431d4 <= offs <= 0x809431f3:
                return offs - 0x4F4
        elif 0x809431f8 <= offs <= 0x809432b7:
                return offs - 0x4F8
        elif 0x809432c0 <= offs <= 0x80944e93:
                return offs - 0x500
        elif 0x80944e98 <= offs <= 0x80944eb3:
                return offs - 0x504
        elif 0x80944ec0 <= offs <= 0x80944edb:
                return offs - 0x510
        elif 0x80944ee0 <= offs <= 0x80944eeb:
                return offs - 0x514
        elif 0x809450ac <= offs <= 0x809450c3:
                return offs - 0x6D4
        elif 0x809450c8 <= offs <= 0x809450f3:
                return offs - 0x6D8
        elif 0x80945144 <= offs <= 0x80945153:
                return offs - 0x714
        elif 0x80945158 <= offs <= 0x8098a43b:
                return offs - 0x718
        elif 0x8098a43c <= offs <= 0x8098a473:
                return offs - 0x6F4
        elif 0x8098a478 <= offs <= 0x809907f7:
                return offs - 0x6F8
        elif 0x80990800 <= offs <= 0x81000000:
                return offs - 0x700
        return offs

def unfix_offs_jpn_v1(offs):
        if 0x80000000 <= offs <= 0x800b4600:
                offs -= 0x0
        elif 0x800b4610 <= offs <= 0x800b4610:
                offs += 0x20
        elif 0x800b4614 <= offs <= 0x800b461f:
                offs += 0x4C
        elif 0x800b4620 <= offs <= 0x800b465b:
                offs += 0x50
        elif 0x800b465c <= offs <= 0x800b4687:
                offs += 0xD8
        elif 0x800b4690 <= offs <= 0x800c8cdf:
                offs += 0xD0
        elif 0x800c8ce0 <= offs <= 0x800e4aef:
                offs += 0x170
        elif 0x800e4af0 <= offs <= 0x800e4af7:
                offs += 0x174
        elif 0x800e4af8 <= offs <= 0x800e4afb:
                offs += 0x168
        elif 0x800e4afc <= offs <= 0x800e4bff:
                offs += 0x170
        elif 0x800e4c00 <= offs <= 0x800e4c07:
                offs += 0x194
        elif 0x800e4c0c <= offs <= 0x800e4d2b:
                offs += 0x194
        elif 0x800e4d30 <= offs <= 0x8010f04f:
                offs += 0x190
        elif 0x8010f050 <= offs <= 0x8010f067:
                offs += 0x19C
        elif 0x8010f06c <= offs <= 0x8010f06f:
                offs += 0x1D0
        elif 0x8010f070 <= offs <= 0x8010f073:
                offs += 0x1C8
        elif 0x8010f074 <= offs <= 0x8010f077:
                offs += 0x1CC
        elif 0x8010f078 <= offs <= 0x8010f07b:
                offs += 0x1BC
        elif 0x8010f07c <= offs <= 0x8010f08f:
                offs += 0x1C8
        elif 0x8010f090 <= offs <= 0x8010f093:
                offs += 0x29C
        elif 0x8010f098 <= offs <= 0x8010f09b:
                offs += 0x298
        elif 0x8010f09c <= offs <= 0x8010f0a3:
                offs += 0x29C
        elif 0x8010f0a8 <= offs <= 0x8010f0af:
                offs += 0x298
        elif 0x8010f0b0 <= offs <= 0x8010f0bf:
                offs += 0x2C8
        elif 0x8010f0c0 <= offs <= 0x8010f0eb:
                offs += 0x32C
        elif 0x8010f0ec <= offs <= 0x802bb38f:
                offs += 0x330
        elif 0x802bb390 <= offs <= 0x802bb40f:
                offs += 0x340
        elif 0x802bb410 <= offs <= 0x802bb7af:
                offs += 0x450
        elif 0x802bb7b0 <= offs <= 0x80317257:
                offs += 0x4E0
        elif 0x80317258 <= offs <= 0x80322aff:
                offs += 0x4F8
        elif 0x80322b00 <= offs <= 0x80322b23:
                offs += 0x5A0
        elif 0x80322b38 <= offs <= 0x8035139f:
                offs += 0x5E0
        elif 0x80351400 <= offs <= 0x804278df:
                offs += 0x580
        elif 0x804278e0 <= offs <= 0x80428fa3:
                offs += 0x5A8
        elif 0x80428fa4 <= offs <= 0x80428fa7:
                offs += 0x5BC
        elif 0x80428fa8 <= offs <= 0x804297b7:
                offs += 0x5C8
        elif 0x804297c0 <= offs <= 0x806dfa3f:
                offs += 0x5C0
        elif 0x806e0000 <= offs <= 0x80779abf:
                offs -= 0x0
        elif 0x80779ac0 <= offs <= 0x80779ac7:
                offs += 0xB8
        elif 0x80779acc <= offs <= 0x80779adb:
                offs += 0xB8
        elif 0x80779adc <= offs <= 0x80779aeb:
                offs += 0xE0
        elif 0x80779aec <= offs <= 0x807887ef:
                offs += 0x130
        elif 0x807887f0 <= offs <= 0x807887f7:
                offs += 0x1E8
        elif 0x807887fc <= offs <= 0x8078880b:
                offs += 0x1E8
        elif 0x8078880c <= offs <= 0x8078881b:
                offs += 0x210
        elif 0x8078881c <= offs <= 0x80789c8f:
                offs += 0x260
        elif 0x80789c90 <= offs <= 0x808d3917:
                offs += 0x270
        elif 0x808d391c <= offs <= 0x808d391f:
                offs += 0x2BC
        elif 0x808d3920 <= offs <= 0x808d3923:
                offs += 0x2B4
        elif 0x808d3924 <= offs <= 0x808d392b:
                offs += 0x2B8
        elif 0x808d392c <= offs <= 0x808d392f:
                offs += 0x2C4
        elif 0x808d3930 <= offs <= 0x808d3933:
                offs += 0x2BC
        elif 0x808d3934 <= offs <= 0x808d3937:
                offs += 0x2B4
        elif 0x808d3938 <= offs <= 0x808d393b:
                offs += 0x2AC
        elif 0x808d393c <= offs <= 0x808d395f:
                offs += 0x2B8
        elif 0x808d3960 <= offs <= 0x809409f3:
                offs += 0x2C0
        elif 0x809409f4 <= offs <= 0x80940a27:
                offs += 0x4AC
        elif 0x80940a28 <= offs <= 0x80940a57:
                offs += 0x4B0
        elif 0x80940a70 <= offs <= 0x80942c9f:
                offs += 0x4E8
        elif 0x80942ca0 <= offs <= 0x80942cbf:
                offs += 0x4EC
        elif 0x80942cc0 <= offs <= 0x80942cdf:
                offs += 0x4F0
        elif 0x80942ce0 <= offs <= 0x80942cff:
                offs += 0x4F4
        elif 0x80942d00 <= offs <= 0x80942dbf:
                offs += 0x4F8
        elif 0x80942dc0 <= offs <= 0x80944993:
                offs += 0x500
        elif 0x80944994 <= offs <= 0x809449af:
                offs += 0x504
        elif 0x809449b0 <= offs <= 0x809449cb:
                offs += 0x510
        elif 0x809449cc <= offs <= 0x809449d7:
                offs += 0x514
        elif 0x809449d8 <= offs <= 0x809449ef:
                offs += 0x6D4
        elif 0x809449f0 <= offs <= 0x80944a1b:
                offs += 0x6D8
        elif 0x80944a30 <= offs <= 0x80944a3f:
                offs += 0x714
        elif 0x80944a40 <= offs <= 0x80989d23:
                offs += 0x718
        elif 0x80989d48 <= offs <= 0x80989d7f:
                offs += 0x6F4
        elif 0x80989d80 <= offs <= 0x809900ff:
                offs += 0x6F8
        elif 0x80990100 <= offs <= 0x80fff900:
                offs += 0x700
        return offs
# jpn2

def fix_offs_jpn_v2(offs):
        offs = fix_offs_jpn_v1(offs)
        if 0x80000000 <= offs <= 0x800cf12f:
                return offs - 0x0
        elif 0x800cf130 <= offs <= 0x800cf133:
                return offs + 0x4
        elif 0x800cf134 <= offs <= 0x800cf193:
                return offs + 0x8
        elif 0x800cf194 <= offs <= 0x800cf197:
                return offs + 0xC
        elif 0x800cf198 <= offs <= 0x800cf19f:
                return offs + 0x18
        elif 0x800cf1b0 <= offs <= 0x800cf1b7:
                return offs - 0x8
        elif 0x800cf1b8 <= offs <= 0x800cf577:
                return offs - 0x0
        elif 0x800cf578 <= offs <= 0x800cf57b:
                return offs + 0x4
        elif 0x800cf57c <= offs <= 0x800cf5cb:
                return offs + 0x8
        elif 0x800cf5cc <= offs <= 0x800cf5cf:
                return offs + 0xC
        elif 0x800cf5d0 <= offs <= 0x800cf677:
                return offs + 0x10
        elif 0x800cf678 <= offs <= 0x800cf67b:
                return offs + 0x14
        elif 0x800cf67c <= offs <= 0x800cf683:
                return offs + 0x20
        elif 0x800cf694 <= offs <= 0x800cf69b:
                return offs - 0x0
        elif 0x800cf69c <= offs <= 0x800cf797:
                return offs + 0x8
        elif 0x800cf7a0 <= offs <= 0x800e0bbb:
                return offs - 0x0
        elif 0x800e0bc0 <= offs <= 0x800e0bcf:
                return offs - 0x4
        elif 0x800e0bd4 <= offs <= 0x800e0bdb:
                return offs - 0x8
        elif 0x800e0bdc <= offs <= 0x800e0c6b:
                return offs + 0x10
        elif 0x800e0c74 <= offs <= 0x800e0c87:
                return offs + 0x8
        elif 0x800e0c8c <= offs <= 0x800e0c9b:
                return offs + 0x4
        elif 0x800e0ca0 <= offs <= 0x807683f6:
                return offs - 0x0
        elif 0x807683f7 <= offs <= 0x8076842b:
                return offs + 0xF
        elif 0x8076842c <= offs <= 0x80768462:
                return offs + 0x1E
        elif 0x80768463 <= offs <= 0x8076849b:
                return offs + 0x2D
        elif 0x8076849c <= offs <= 0x8076849f:
                return offs + 0x3C
        elif 0x807684a0 <= offs <= 0x807aa7c0:
                return offs + 0x40
        elif 0x807aa7fc <= offs <= 0x809900ff:
                return offs + 0x10
        elif 0x80990100 <= offs <= 0x81000000:
                return offs + 0x20
        return offs

def unfix_offs_jpn_v2(offs):
        if 0x80000000 <= offs <= 0x800cf12f:
                offs -= 0x0
        elif 0x800cf134 <= offs <= 0x800cf137:
                offs -= 0x4
        elif 0x800cf13c <= offs <= 0x800cf19b:
                offs -= 0x8
        elif 0x800cf1a0 <= offs <= 0x800cf1a3:
                offs -= 0xC
        elif 0x800cf1a8 <= offs <= 0x800cf1af:
                offs += 0x8
        elif 0x800cf1b0 <= offs <= 0x800cf1b7:
                offs -= 0x18
        elif 0x800cf1b8 <= offs <= 0x800cf577:
                offs -= 0x0
        elif 0x800cf57c <= offs <= 0x800cf57f:
                offs -= 0x4
        elif 0x800cf584 <= offs <= 0x800cf5d3:
                offs -= 0x8
        elif 0x800cf5d8 <= offs <= 0x800cf5db:
                offs -= 0xC
        elif 0x800cf5e0 <= offs <= 0x800cf687:
                offs -= 0x10
        elif 0x800cf68c <= offs <= 0x800cf68f:
                offs -= 0x14
        elif 0x800cf694 <= offs <= 0x800cf69b:
                offs -= 0x0
        elif 0x800cf69c <= offs <= 0x800cf6a3:
                offs -= 0x20
        elif 0x800cf6a4 <= offs <= 0x800cf79f:
                offs -= 0x8
        elif 0x800cf7a0 <= offs <= 0x800e0bbb:
                offs -= 0x0
        elif 0x800e0bbc <= offs <= 0x800e0bcb:
                offs += 0x4
        elif 0x800e0bcc <= offs <= 0x800e0bd3:
                offs += 0x8
        elif 0x800e0bec <= offs <= 0x800e0c7b:
                offs -= 0x10
        elif 0x800e0c7c <= offs <= 0x800e0c8f:
                offs -= 0x8
        elif 0x800e0c90 <= offs <= 0x800e0c9f:
                offs -= 0x4
        elif 0x800e0ca0 <= offs <= 0x807683f6:
                offs -= 0x0
        elif 0x80768406 <= offs <= 0x8076843a:
                offs -= 0xF
        elif 0x8076844a <= offs <= 0x80768480:
                offs -= 0x1E
        elif 0x80768490 <= offs <= 0x807684c8:
                offs -= 0x2D
        elif 0x807684d8 <= offs <= 0x807684db:
                offs -= 0x3C
        elif 0x807684e0 <= offs <= 0x807aa800:
                offs -= 0x40
        elif 0x807aa80c <= offs <= 0x8099010f:
                offs -= 0x10
        elif 0x80990120 <= offs <= 0x81000020:
                offs -= 0x20
        return unfix_offs_jpn_v1(offs)
# kor

def fix_offs_kor_v1(offs):
        offs = fix_offs_pal_v2(offs)
        if 0x80000000 <= offs <= 0x800122c0:
                return offs - 0x0
        elif 0x800122c1 <= offs <= 0x800122eb:
                return offs + 0x14
        elif 0x800122f0 <= offs <= 0x800122f7:
                return offs + 0x14
        elif 0x80012300 <= offs <= 0x8001230b:
                return offs + 0x28
        elif 0x8001230c <= offs <= 0x8001230f:
                return offs + 0x2C
        elif 0x80012314 <= offs <= 0x8001231f:
                return offs + 0x3C
        elif 0x80012320 <= offs <= 0x80012337:
                return offs + 0x40
        elif 0x80012338 <= offs <= 0x8001233f:
                return offs + 0x78
        elif 0x80012364 <= offs <= 0x80012397:
                return offs + 0x54
        elif 0x80012398 <= offs <= 0x8001239f:
                return offs + 0x58
        elif 0x800123a4 <= offs <= 0x800123bb:
                return offs + 0x54
        elif 0x800123c0 <= offs <= 0x800123eb:
                return offs + 0x50
        elif 0x800123ec <= offs <= 0x8001244f:
                return offs + 0x64
        elif 0x80012450 <= offs <= 0x8004dea7:
                return offs + 0x70
        elif 0x8004deac <= offs <= 0x8004deb7:
                return offs + 0x6C
        elif 0x8004dec0 <= offs <= 0x8004dedf:
                return offs + 0x68
        elif 0x8004dee0 <= offs <= 0x8004df37:
                return offs + 0x70
        elif 0x8004df3c <= offs <= 0x8004df47:
                return offs + 0x6C
        elif 0x8004df50 <= offs <= 0x8004df6f:
                return offs + 0x68
        elif 0x8004df70 <= offs <= 0x80050e5f:
                return offs + 0x70
        elif 0x80050e60 <= offs <= 0x80052f03:
                return offs + 0xD0
        elif 0x80052f24 <= offs <= 0x8005c453:
                return offs + 0xE0
        elif 0x8005c454 <= offs <= 0x8005c457:
                return offs + 0xE8
        elif 0x8005c458 <= offs <= 0x8005c48b:
                return offs + 0xF8
        elif 0x8005c48c <= offs <= 0x8005c497:
                return offs + 0xFC
        elif 0x8005c498 <= offs <= 0x8005c6c7:
                return offs + 0x108
        elif 0x8005c6d0 <= offs <= 0x8005f813:
                return offs + 0x100
        elif 0x8005f850 <= offs <= 0x8005f877:
                return offs + 0xC8
        elif 0x8005f880 <= offs <= 0x8009624f:
                return offs + 0x140
        elif 0x80096250 <= offs <= 0x80096253:
                return offs + 0x150
        elif 0x80096254 <= offs <= 0x8009625b:
                return offs + 0x13C
        elif 0x8009625c <= offs <= 0x8009626f:
                return offs + 0x148
        elif 0x80096278 <= offs <= 0x8009628f:
                return offs + 0x17C
        elif 0x80096290 <= offs <= 0x800b45ff:
                return offs + 0x180
        elif 0x800b4600 <= offs <= 0x800b4600:
                return offs + 0x190
        elif 0x800b4630 <= offs <= 0x800b4630:
                return offs + 0x170
        elif 0x800b4670 <= offs <= 0x800b46ab:
                return offs + 0x140
        elif 0x800b4734 <= offs <= 0x800b475f:
                return offs + 0xB8
        elif 0x800b4760 <= offs <= 0x800c8daf:
                return offs + 0xC0
        elif 0x800c8e50 <= offs <= 0x800e4c5f:
                return offs + 0x20
        elif 0x800e4c60 <= offs <= 0x800e4c63:
                return offs + 0x28
        elif 0x800e4c64 <= offs <= 0x800e4c6b:
                return offs + 0x1C
        elif 0x800e4c6c <= offs <= 0x800e4d6f:
                return offs + 0x20
        elif 0x800e4d94 <= offs <= 0x800e4d9b:
                return offs - 0x4
        elif 0x800e4da0 <= offs <= 0x800e4ebf:
                return offs - 0x4
        elif 0x800e4ec0 <= offs <= 0x800f794f:
                return offs - 0x0
        elif 0x800f7950 <= offs <= 0x800f79bf:
                return offs + 0xC
        elif 0x800f79c0 <= offs <= 0x800fd5eb:
                return offs + 0x10
        elif 0x800fd5ec <= offs <= 0x800fd60b:
                return offs + 0x44
        elif 0x800fd610 <= offs <= 0x801011bf:
                return offs + 0x40
        elif 0x801011c0 <= offs <= 0x8010127f:
                return offs + 0xB0
        elif 0x80101280 <= offs <= 0x80106f37:
                return offs + 0x100
        elif 0x80106f38 <= offs <= 0x80106f6f:
                return offs + 0x108
        elif 0x80106f70 <= offs <= 0x80106fbb:
                return offs + 0x110
        elif 0x80106fbc <= offs <= 0x80106fbf:
                return offs + 0x114
        elif 0x80106fc0 <= offs <= 0x80106fc3:
                return offs + 0x12C
        elif 0x80106fc4 <= offs <= 0x80106fc7:
                return offs + 0x11C
        elif 0x80106fc8 <= offs <= 0x80106fe7:
                return offs + 0x128
        elif 0x80106ff0 <= offs <= 0x80107047:
                return offs + 0x120
        elif 0x80107048 <= offs <= 0x80107053:
                return offs + 0x124
        elif 0x80107074 <= offs <= 0x80107077:
                return offs + 0x114
        elif 0x80107078 <= offs <= 0x8010708f:
                return offs + 0x118
        elif 0x80107090 <= offs <= 0x80107090:
                return offs + 0x120
        elif 0x80107091 <= offs <= 0x8010709f:
                return offs + 0x144
        elif 0x801070a4 <= offs <= 0x801070ab:
                return offs + 0x174
        elif 0x801070b0 <= offs <= 0x801070b0:
                return offs + 0x170
        elif 0x801070b1 <= offs <= 0x801070b3:
                return offs + 0x188
        elif 0x801070b4 <= offs <= 0x801070b7:
                return offs + 0x194
        elif 0x801070b8 <= offs <= 0x801070bb:
                return offs + 0x188
        elif 0x801070bc <= offs <= 0x801070bf:
                return offs + 0x194
        elif 0x801070c0 <= offs <= 0x801070c3:
                return offs + 0x18C
        elif 0x801070c8 <= offs <= 0x801070cf:
                return offs + 0x1C0
        elif 0x801070d0 <= offs <= 0x8010710b:
                return offs + 0x1F0
        elif 0x8010710c <= offs <= 0x80107113:
                return offs + 0x1FC
        elif 0x80107114 <= offs <= 0x801071f3:
                return offs + 0x210
        elif 0x801071f4 <= offs <= 0x8010727f:
                return offs + 0x21C
        elif 0x80107280 <= offs <= 0x80107317:
                return offs + 0x350
        elif 0x80107318 <= offs <= 0x8010733b:
                return offs + 0x35C
        elif 0x8010733c <= offs <= 0x801073bf:
                return offs + 0x368
        elif 0x801073c0 <= offs <= 0x801074ef:
                return offs + 0x4E0
        elif 0x801074f0 <= offs <= 0x801074f7:
                return offs + 0x4E4
        elif 0x801074f8 <= offs <= 0x801074fb:
                return offs + 0x4D8
        elif 0x801074fc <= offs <= 0x80107503:
                return offs + 0x4E0
        elif 0x80107504 <= offs <= 0x8010751b:
                return offs + 0x4E4
        elif 0x80107520 <= offs <= 0x801076c7:
                return offs + 0x4E0
        elif 0x801076c8 <= offs <= 0x80107707:
                return offs + 0x4E4
        elif 0x80107708 <= offs <= 0x80107787:
                return offs + 0x4F0
        elif 0x80107788 <= offs <= 0x801077b3:
                return offs + 0x4FC
        elif 0x801077c0 <= offs <= 0x801079f3:
                return offs + 0x4F0
        elif 0x801079f4 <= offs <= 0x80107b5b:
                return offs + 0x520
        elif 0x80107b60 <= offs <= 0x80107c27:
                return offs + 0x520
        elif 0x80107c28 <= offs <= 0x80107c5f:
                return offs + 0x524
        elif 0x80107c60 <= offs <= 0x80107c63:
                return offs + 0x530
        elif 0x80107c64 <= offs <= 0x80107c67:
                return offs + 0x60C
        elif 0x80107c68 <= offs <= 0x80107c6b:
                return offs + 0x62C
        elif 0x80107c6c <= offs <= 0x80107c6f:
                return offs + 0x620
        elif 0x80107c70 <= offs <= 0x80107cef:
                return offs + 0x528
        elif 0x80107cf0 <= offs <= 0x80107d27:
                return offs + 0x530
        elif 0x80107d28 <= offs <= 0x80107d3b:
                return offs + 0x534
        elif 0x80107d3c <= offs <= 0x80107d53:
                return offs + 0x538
        elif 0x80107d54 <= offs <= 0x80107d57:
                return offs + 0x430
        elif 0x80107d58 <= offs <= 0x80107d5b:
                return offs + 0x43C
        elif 0x80107d5c <= offs <= 0x80107d5f:
                return offs + 0x534
        elif 0x80107d60 <= offs <= 0x80107d77:
                return offs + 0x538
        elif 0x80107d80 <= offs <= 0x80107e83:
                return offs + 0x530
        elif 0x80107e84 <= offs <= 0x80107fb3:
                return offs + 0x538
        elif 0x80107fb4 <= offs <= 0x80107fcf:
                return offs + 0x544
        elif 0x80107fd0 <= offs <= 0x8010f1b3:
                return offs + 0x550
        elif 0x8010f1b4 <= offs <= 0x8010f1b7:
                return offs + 0x570
        elif 0x8010f1b8 <= offs <= 0x8010f1bb:
                return offs + 0x550
        elif 0x8010f1bc <= offs <= 0x8010f1cf:
                return offs + 0x56C
        elif 0x8010f1d0 <= offs <= 0x8010f1df:
                return offs + 0x570
        elif 0x8010f1ec <= offs <= 0x8010f203:
                return offs + 0x564
        elif 0x8010f234 <= offs <= 0x8010f237:
                return offs + 0x544
        elif 0x8010f238 <= offs <= 0x8010f23b:
                return offs + 0x538
        elif 0x8010f23c <= offs <= 0x8010f23f:
                return offs + 0x530
        elif 0x8010f240 <= offs <= 0x8010f243:
                return offs + 0x534
        elif 0x8010f244 <= offs <= 0x8010f257:
                return offs + 0x538
        elif 0x8010f32c <= offs <= 0x8010f32f:
                return offs + 0x464
        elif 0x8010f330 <= offs <= 0x8010f333:
                return offs + 0x468
        elif 0x8010f338 <= offs <= 0x8010f33f:
                return offs + 0x464
        elif 0x8010f340 <= offs <= 0x8010f347:
                return offs + 0x468
        elif 0x8010f378 <= offs <= 0x8010f387:
                return offs + 0x438
        elif 0x8010f3ec <= offs <= 0x8010f417:
                return offs + 0x3D4
        elif 0x8010f41c <= offs <= 0x80150e77:
                return offs + 0x3D0
        elif 0x80150e80 <= offs <= 0x80150e83:
                return offs + 0x3C8
        elif 0x80150e90 <= offs <= 0x80150ebb:
                return offs + 0x3C0
        elif 0x80150ec4 <= offs <= 0x80150ec7:
                return offs + 0x3B8
        elif 0x80150ed4 <= offs <= 0x8015140f:
                return offs + 0x3B0
        elif 0x80151410 <= offs <= 0x8015145f:
                return offs + 0x3C4
        elif 0x80151460 <= offs <= 0x80152413:
                return offs + 0x3D0
        elif 0x80152414 <= offs <= 0x8015241f:
                return offs + 0x3DC
        elif 0x80152424 <= offs <= 0x80158837:
                return offs + 0x3E0
        elif 0x8015883c <= offs <= 0x801589cf:
                return offs + 0x3DC
        elif 0x801589d0 <= offs <= 0x801590c3:
                return offs + 0x3E0
        elif 0x801590c4 <= offs <= 0x801590eb:
                return offs + 0x3EC
        elif 0x801590ec <= offs <= 0x8015912f:
                return offs + 0x3F0
        elif 0x80159130 <= offs <= 0x8015918f:
                return offs + 0x3F4
        elif 0x80159190 <= offs <= 0x8015acdf:
                return offs + 0x400
        elif 0x8015ace0 <= offs <= 0x8015ace3:
                return offs + 0x410
        elif 0x8015ace4 <= offs <= 0x8015ace7:
                return offs + 0x3FC
        elif 0x8015ace8 <= offs <= 0x8015acef:
                return offs + 0x400
        elif 0x8015acf0 <= offs <= 0x8015acf3:
                return offs + 0x40C
        elif 0x8015acf4 <= offs <= 0x8015acfb:
                return offs + 0x400
        elif 0x8015acfc <= offs <= 0x8015ad2b:
                return offs + 0x404
        elif 0x8015ad2c <= offs <= 0x8015ad2f:
                return offs + 0x3B8
        elif 0x8015ad30 <= offs <= 0x802bb6bf:
                return offs + 0x400
        elif 0x802bb6d0 <= offs <= 0x802bb74f:
                return offs + 0x3F0
        elif 0x802bb860 <= offs <= 0x802bbbff:
                return offs + 0x2E0
        elif 0x802bbc90 <= offs <= 0x802e467c:
                return offs + 0x250
        elif 0x802e4680 <= offs <= 0x802edcd7:
                return offs + 0x348
        elif 0x802edce0 <= offs <= 0x802f4b1f:
                return offs + 0x340
        elif 0x802f4b20 <= offs <= 0x802f4b7f:
                return offs + 0x348
        elif 0x802f4b80 <= offs <= 0x802f4bdf:
                return offs + 0x350
        elif 0x802f4be0 <= offs <= 0x802f5d43:
                return offs + 0x358
        elif 0x802f5d44 <= offs <= 0x802fd29f:
                return offs + 0xC538
        elif 0x802fd2a0 <= offs <= 0x80317737:
                return offs + 0xC540
        elif 0x80317750 <= offs <= 0x8032080f:
                return offs + 0xC528
        elif 0x80320810 <= offs <= 0x8032164f:
                return offs + 0xC570
        elif 0x80321650 <= offs <= 0x80321f07:
                return offs + 0xC578
        elif 0x80321f08 <= offs <= 0x80321f17:
                return offs + 0xC57C
        elif 0x80321f18 <= offs <= 0x80321f33:
                return offs + 0xC588
        elif 0x80321f38 <= offs <= 0x80321f6f:
                return offs + 0xC584
        elif 0x80321f70 <= offs <= 0x80321f97:
                return offs + 0xC588
        elif 0x80321f98 <= offs <= 0x80321fe7:
                return offs + 0xC58C
        elif 0x80321fe8 <= offs <= 0x80322003:
                return offs + 0xC594
        elif 0x80322004 <= offs <= 0x8032204f:
                return offs + 0xC5B4
        elif 0x80322050 <= offs <= 0x8032206b:
                return offs + 0xC600
        elif 0x8032206c <= offs <= 0x803220bf:
                return offs + 0xC61C
        elif 0x803220c0 <= offs <= 0x803220df:
                return offs + 0xC670
        elif 0x803220e0 <= offs <= 0x80322147:
                return offs + 0xC690
        elif 0x80322148 <= offs <= 0x803221a7:
                return offs + 0xC6F4
        elif 0x803221a8 <= offs <= 0x803221c3:
                return offs + 0xC754
        elif 0x803221c4 <= offs <= 0x80322233:
                return offs + 0xC76C
        elif 0x80322234 <= offs <= 0x803222a3:
                return offs + 0xC7D8
        elif 0x803222a4 <= offs <= 0x803222c3:
                return offs + 0xC848
        elif 0x803222c4 <= offs <= 0x80322337:
                return offs + 0xC86C
        elif 0x80322338 <= offs <= 0x80322353:
                return offs + 0xC8E0
        elif 0x80322354 <= offs <= 0x803223c7:
                return offs + 0xC8FC
        elif 0x803223c8 <= offs <= 0x803223e7:
                return offs + 0xC968
        elif 0x803223e8 <= offs <= 0x80322447:
                return offs + 0xC988
        elif 0x80322448 <= offs <= 0x803224d7:
                return offs + 0xCAB8
        elif 0x803224d8 <= offs <= 0x803224e3:
                return offs + 0xCAD0
        elif 0x803224f0 <= offs <= 0x80322ff7:
                return offs + 0xCAC8
        elif 0x803230a0 <= offs <= 0x803230c3:
                return offs + 0xCA20
        elif 0x80323118 <= offs <= 0x803279cf:
                return offs + 0xC9E0
        elif 0x803279d0 <= offs <= 0x8032e777:
                return offs + 0xC9E8
        elif 0x8032e780 <= offs <= 0x8035197f:
                return offs + 0xC9E0
        elif 0x80351980 <= offs <= 0x80427e5f:
                return offs + 0xCA00
        elif 0x80427e88 <= offs <= 0x8042823f:
                return offs + 0xC9D8
        elif 0x80428244 <= offs <= 0x80428253:
                return offs + 0xC9D4
        elif 0x80428258 <= offs <= 0x80429427:
                return offs + 0xC9D0
        elif 0x80429428 <= offs <= 0x80429497:
                return offs + 0xC9D8
        elif 0x80429498 <= offs <= 0x804294df:
                return offs + 0xC9E0
        elif 0x804294e0 <= offs <= 0x804294e3:
                return offs + 0xC9E4
        elif 0x804294e8 <= offs <= 0x8042954b:
                return offs + 0xC9E0
        elif 0x80429560 <= offs <= 0x80429563:
                return offs + 0xC9CC
        elif 0x80429570 <= offs <= 0x8042d537:
                return offs + 0xC9C0
        elif 0x8042d538 <= offs <= 0x8042d547:
                return offs + 0xC9C8
        elif 0x8042d548 <= offs <= 0x8042d54b:
                return offs + 0xC9CC
        elif 0x8042d550 <= offs <= 0x8042f427:
                return offs + 0xC9C8
        elif 0x8042f430 <= offs <= 0x806d363f:
                return offs + 0xC9C0
        elif 0x806e0000 <= offs <= 0x807683f2:
                return offs - 0x0
        elif 0x80768406 <= offs <= 0x80768436:
                return offs - 0x13
        elif 0x8076844a <= offs <= 0x8076847c:
                return offs - 0x26
        elif 0x80768490 <= offs <= 0x807684c4:
                return offs - 0x39
        elif 0x807684d8 <= offs <= 0x807684df:
                return offs - 0x4C
        elif 0x807684e0 <= offs <= 0x8076b27f:
                return offs - 0x40
        elif 0x8076b280 <= offs <= 0x8076ca93:
                return offs - 0x38
        elif 0x8076ca94 <= offs <= 0x8076d45f:
                return offs - 0x28
        elif 0x8076d4a0 <= offs <= 0x8076dd7c:
                return offs - 0x40
        elif 0x8076dd80 <= offs <= 0x8076dd83:
                return offs - 0x28
        elif 0x8076dd88 <= offs <= 0x8076dd8b:
                return offs - 0x2C
        elif 0x8076dd8c <= offs <= 0x8076dd8f:
                return offs - 0x28
        elif 0x8076dd90 <= offs <= 0x8076dd93:
                return offs - 0x30
        elif 0x8076dd94 <= offs <= 0x8076dd97:
                return offs - 0x28
        elif 0x8076dd98 <= offs <= 0x8076dd9b:
                return offs - 0x30
        elif 0x8076dd9c <= offs <= 0x8076ddcf:
                return offs - 0x2C
        elif 0x8076ddd0 <= offs <= 0x8076ddd3:
                return offs - 0x28
        elif 0x8076ddd4 <= offs <= 0x8076df1f:
                return offs - 0x24
        elif 0x8076df20 <= offs <= 0x80779afc:
                return offs - 0x20
        elif 0x80779bb8 <= offs <= 0x80779bd3:
                return offs - 0xD8
        elif 0x80779bfc <= offs <= 0x80779c0b:
                return offs - 0x100
        elif 0x80779c5c <= offs <= 0x8078895f:
                return offs - 0x150
        elif 0x80788a18 <= offs <= 0x80788a33:
                return offs - 0x208
        elif 0x80788a5c <= offs <= 0x80788a6b:
                return offs - 0x230
        elif 0x80788abc <= offs <= 0x8079d57f:
                return offs - 0x280
        elif 0x8079d580 <= offs <= 0x8079d8f3:
                return offs + 0x4090
        elif 0x8079d8f4 <= offs <= 0x8079d9bb:
                return offs + 0x4094
        elif 0x8079d9bc <= offs <= 0x8079d9eb:
                return offs + 0x40B0
        elif 0x8079d9ec <= offs <= 0x8079e1db:
                return offs + 0x40C0
        elif 0x8079e1dc <= offs <= 0x8079e2f3:
                return offs + 0x40D0
        elif 0x8079e2f8 <= offs <= 0x8079e2fb:
                return offs + 0x40CC
        elif 0x8079e318 <= offs <= 0x8079e387:
                return offs + 0x40DC
        elif 0x8079e3a8 <= offs <= 0x8079e3cb:
                return offs + 0x40BC
        elif 0x8079e3d0 <= offs <= 0x8079e3d3:
                return offs + 0x40B8
        elif 0x8079e3f0 <= offs <= 0x8079e45f:
                return offs + 0x40C8
        elif 0x8079e480 <= offs <= 0x8079e497:
                return offs + 0x40A8
        elif 0x8079e4a0 <= offs <= 0x8079eb23:
                return offs + 0x40A0
        elif 0x8079eb24 <= offs <= 0x8079eb63:
                return offs + 0x40B0
        elif 0x8079eb64 <= offs <= 0x8079ebdb:
                return offs + 0x40C4
        elif 0x8079ebe0 <= offs <= 0x807a124f:
                return offs + 0x40C0
        elif 0x807a1250 <= offs <= 0x807a1253:
                return offs + 0x40DC
        elif 0x807a1254 <= offs <= 0x807a1257:
                return offs + 0x40C0
        elif 0x807a1258 <= offs <= 0x807a125b:
                return offs + 0x40D8
        elif 0x807a125c <= offs <= 0x807a125f:
                return offs + 0x40B4
        elif 0x807a1260 <= offs <= 0x807a1273:
                return offs + 0x40B8
        elif 0x807a1274 <= offs <= 0x807a43af:
                return offs + 0x40C0
        elif 0x807a43b0 <= offs <= 0x807a43b3:
                return offs + 0x40C4
        elif 0x807a43b8 <= offs <= 0x807a43bb:
                return offs + 0x40B8
        elif 0x807a43bc <= offs <= 0x807a44bf:
                return offs + 0x40BC
        elif 0x807a44c0 <= offs <= 0x807a7b63:
                return offs + 0x40C0
        elif 0x807a7b64 <= offs <= 0x807a7b67:
                return offs + 0x40C4
        elif 0x807a7b6c <= offs <= 0x807a7b6f:
                return offs + 0x40B8
        elif 0x807a7b70 <= offs <= 0x807a7c1f:
                return offs + 0x40BC
        elif 0x807a7c20 <= offs <= 0x80817390:
                return offs + 0x40C0
        elif 0x80817391 <= offs <= 0x80817393:
                return offs + 0x40C8
        elif 0x80817394 <= offs <= 0x80817397:
                return offs + 0x40E0
        elif 0x80817398 <= offs <= 0x8081739b:
                return offs + 0x40CC
        elif 0x8081739c <= offs <= 0x8081739f:
                return offs + 0x40D4
        elif 0x808173a0 <= offs <= 0x808173a7:
                return offs + 0x40C8
        elif 0x808173a8 <= offs <= 0x808173b3:
                return offs + 0x40D0
        elif 0x808173b4 <= offs <= 0x808173b7:
                return offs + 0x40A8
        elif 0x808173b8 <= offs <= 0x808173eb:
                return offs + 0x40CC
        elif 0x808173ec <= offs <= 0x808173ef:
                return offs + 0x40E0
        elif 0x808173f0 <= offs <= 0x808173f7:
                return offs + 0x40E8
        elif 0x80817400 <= offs <= 0x808174db:
                return offs + 0x40E0
        elif 0x808174dc <= offs <= 0x8081752f:
                return offs + 0x40EC
        elif 0x80817530 <= offs <= 0x80826ddf:
                return offs + 0x40F0
        elif 0x80826e90 <= offs <= 0x80827003:
                return offs + 0x4210
        elif 0x80827004 <= offs <= 0x80827007:
                return offs + 0x4214
        elif 0x80827008 <= offs <= 0x8082700f:
                return offs + 0x4218
        elif 0x80827014 <= offs <= 0x80827023:
                return offs + 0x4228
        elif 0x80827028 <= offs <= 0x80827043:
                return offs + 0x4224
        elif 0x80827048 <= offs <= 0x80827227:
                return offs + 0x4220
        elif 0x80827228 <= offs <= 0x8082729f:
                return offs + 0x4228
        elif 0x808272a0 <= offs <= 0x8086e98f:
                return offs + 0x4230
        elif 0x8086e990 <= offs <= 0x8086e9db:
                return offs + 0x4244
        elif 0x8086e9e0 <= offs <= 0x8086eabb:
                return offs + 0x4240
        elif 0x8086eabc <= offs <= 0x8086eabf:
                return offs + 0x4244
        elif 0x8086eac0 <= offs <= 0x8086eac3:
                return offs + 0x423C
        elif 0x8086eac4 <= offs <= 0x8086eb7b:
                return offs + 0x4240
        elif 0x8086eb7c <= offs <= 0x8086eb7f:
                return offs + 0x4244
        elif 0x8086eb80 <= offs <= 0x8086eb83:
                return offs + 0x4260
        elif 0x8086eb84 <= offs <= 0x8086eb87:
                return offs + 0x4268
        elif 0x8086eb88 <= offs <= 0x8086eb8b:
                return offs + 0x4260
        elif 0x8086eb8c <= offs <= 0x8086ebbf:
                return offs + 0x4264
        elif 0x8086ebc0 <= offs <= 0x808d3b97:
                return offs + 0x4270
        elif 0x808d3be4 <= offs <= 0x808d3be7:
                return offs + 0x422C
        elif 0x808d3be8 <= offs <= 0x808d3beb:
                return offs + 0x4224
        elif 0x808d3bec <= offs <= 0x808d3bef:
                return offs + 0x4228
        elif 0x808d3bf8 <= offs <= 0x808d3bfb:
                return offs + 0x422C
        elif 0x808d3bfc <= offs <= 0x808d3bff:
                return offs + 0x4224
        elif 0x808d3c00 <= offs <= 0x808d3c03:
                return offs + 0x421C
        elif 0x808d3c04 <= offs <= 0x808d3c27:
                return offs + 0x4228
        elif 0x808d3c30 <= offs <= 0x808ef2df:
                return offs + 0x4220
        elif 0x808ef310 <= offs <= 0x808fb94f:
                return offs + 0x41F0
        elif 0x808fb950 <= offs <= 0x808fb95b:
                return offs + 0x41FC
        elif 0x808fb994 <= offs <= 0x808fbcef:
                return offs + 0x41F8
        elif 0x808fbcf8 <= offs <= 0x808fbd0f:
                return offs + 0x420C
        elif 0x808fbd10 <= offs <= 0x808fbfdf:
                return offs + 0x4210
        elif 0x808fbfe0 <= offs <= 0x80902a87:
                return offs + 0x4250
        elif 0x80902aa0 <= offs <= 0x80902acf:
                return offs + 0x423C
        elif 0x80902ad0 <= offs <= 0x8090947f:
                return offs + 0x4240
        elif 0x80909510 <= offs <= 0x8091f15f:
                return offs + 0x41B0
        elif 0x8091f160 <= offs <= 0x8091f167:
                return offs + 0x41B4
        elif 0x8091f168 <= offs <= 0x8091f17f:
                return offs + 0x41B8
        elif 0x8091f180 <= offs <= 0x8091f257:
                return offs + 0x41C0
        elif 0x8091f258 <= offs <= 0x8091f27b:
                return offs + 0x41D4
        elif 0x8091f280 <= offs <= 0x809337eb:
                return offs + 0x41D0
        elif 0x809337ec <= offs <= 0x8093520f:
                return offs + 0x41D8
        elif 0x80935210 <= offs <= 0x809382cf:
                return offs + 0x42F0
        elif 0x809382e0 <= offs <= 0x80940c77:
                return offs + 0x4300
        elif 0x80940c7c <= offs <= 0x80940c9b:
                return offs + 0x42FC
        elif 0x80940ca0 <= offs <= 0x80940cbf:
                return offs + 0x42F8
        elif 0x80940cc4 <= offs <= 0x80940ccf:
                return offs + 0x42F4
        elif 0x80940eb0 <= offs <= 0x80940ee3:
                return offs + 0x4114
        elif 0x80940ee8 <= offs <= 0x80940f17:
                return offs + 0x4110
        elif 0x80940f68 <= offs <= 0x80944ea3:
                return offs + 0x40D8
        elif 0x80944ea8 <= offs <= 0x80944ec7:
                return offs + 0x40D4
        elif 0x80944ed0 <= offs <= 0x80944efb:
                return offs + 0x40CC
        elif 0x809450bc <= offs <= 0x809450d3:
                return offs + 0x3F0C
        elif 0x809450d8 <= offs <= 0x80945103:
                return offs + 0x3F08
        elif 0x80945154 <= offs <= 0x809494a7:
                return offs + 0x3ED0
        elif 0x809494a8 <= offs <= 0x8094978f:
                return offs + 0x3EDC
        elif 0x80949790 <= offs <= 0x8094a387:
                return offs + 0x3EE0
        elif 0x8094a388 <= offs <= 0x8094a3bb:
                return offs + 0x5090
        elif 0x8094a3bc <= offs <= 0x8094a443:
                return offs + 0x5094
        elif 0x8094a444 <= offs <= 0x8094a46f:
                return offs + 0x5098
        elif 0x8094a470 <= offs <= 0x8094a49b:
                return offs + 0x509C
        elif 0x8094a4a0 <= offs <= 0x8094a5f3:
                return offs + 0x5098
        elif 0x8094a5f8 <= offs <= 0x8094a617:
                return offs + 0x50A4
        elif 0x8094a618 <= offs <= 0x8094c0d7:
                return offs + 0x50A8
        elif 0x8094c0d8 <= offs <= 0x8094c123:
                return offs + 0x50B0
        elif 0x8094c124 <= offs <= 0x8094c14b:
                return offs + 0x50B4
        elif 0x8094c150 <= offs <= 0x8094c177:
                return offs + 0x50B0
        elif 0x8094c178 <= offs <= 0x8094c1a3:
                return offs + 0x50B4
        elif 0x8094c1a8 <= offs <= 0x8094cabb:
                return offs + 0x50B0
        elif 0x8094cabc <= offs <= 0x8094d5cb:
                return offs + 0x50B8
        elif 0x8094d5cc <= offs <= 0x8098a37b:
                return offs + 0x50C0
        elif 0x8098a398 <= offs <= 0x8098a3b3:
                return offs + 0x50A4
        elif 0x8098a488 <= offs <= 0x8099080f:
                return offs + 0x4FD0
        elif 0x80990820 <= offs <= 0x80994317:
                return offs + 0x4FC0
        elif 0x80994318 <= offs <= 0x80a0a57f:
                return offs + 0x54E0
        elif 0x80a0a580 <= offs <= 0x80a0a5a7:
                return offs + 0x54E4
        elif 0x80a0a5b4 <= offs <= 0x80a0a5ef:
                return offs + 0x54E8
        elif 0x80a0a5f0 <= offs <= 0x80a0a5ff:
                return offs + 0x54EC
        elif 0x80a0a600 <= offs <= 0x80a0a60f:
                return offs + 0x54F0
        elif 0x80a0a610 <= offs <= 0x80a0a633:
                return offs + 0x54F8
        elif 0x80a0a63c <= offs <= 0x80a0a63f:
                return offs + 0x54FC
        elif 0x80a0a640 <= offs <= 0x80a0a643:
                return offs + 0x5504
        elif 0x80a0a644 <= offs <= 0x80a0a647:
                return offs + 0x54FC
        elif 0x80a0a648 <= offs <= 0x80a0a64b:
                return offs + 0x54F4
        elif 0x80a0a64c <= offs <= 0x80a0a67f:
                return offs + 0x54FC
        elif 0x80a0a680 <= offs <= 0x80a0b1c3:
                return offs + 0x5500
        elif 0x80a0b1c4 <= offs <= 0x80a0b1c7:
                return offs + 0x5578
        elif 0x80a0b1c8 <= offs <= 0x80a0b1cb:
                return offs + 0x5500
        elif 0x80a0b1cc <= offs <= 0x80a0b50f:
                return offs + 0x5574
        elif 0x80a0b510 <= offs <= 0x80a38ee0:
                return offs + 0x5580
        elif 0x80a38ee1 <= offs <= 0x80a38ee3:
                return offs + 0x55A4
        elif 0x80a38ee4 <= offs <= 0x80a38eef:
                return offs + 0x55AC
        elif 0x80a38ef0 <= offs <= 0x80b1143f:
                return offs + 0x55B0
        elif 0x80b11440 <= offs <= 0x80b304fb:
                return offs + 0x55C0
        elif 0x80b304fc <= offs <= 0x80b30513:
                return offs + 0x55CC
        elif 0x80b3051c <= offs <= 0x80b3051f:
                return offs + 0x55C8
        elif 0x80b30520 <= offs <= 0x80b30523:
                return offs + 0x55DC
        elif 0x80b30524 <= offs <= 0x80b30527:
                return offs + 0x55D0
        elif 0x80b30528 <= offs <= 0x80b3052b:
                return offs + 0x55D8
        elif 0x80b3052c <= offs <= 0x80b3052f:
                return offs + 0x55CC
        elif 0x80b30530 <= offs <= 0x80b30533:
                return offs + 0x55BC
        elif 0x80b30534 <= offs <= 0x80b30537:
                return offs + 0x55D8
        elif 0x80b30538 <= offs <= 0x80b3053f:
                return offs + 0x55CC
        elif 0x80b30540 <= offs <= 0x80b30543:
                return offs + 0x55D4
        elif 0x80b30544 <= offs <= 0x80b30547:
                return offs + 0x55CC
        elif 0x80b30548 <= offs <= 0x80b3054b:
                return offs + 0x55D0
        elif 0x80b3054c <= offs <= 0x80b3054f:
                return offs + 0x55E8
        elif 0x80b30550 <= offs <= 0x80b3055f:
                return offs + 0x55CC
        elif 0x80b30560 <= offs <= 0x80b30563:
                return offs + 0x55DC
        elif 0x80b30564 <= offs <= 0x80b3056b:
                return offs + 0x55C8
        elif 0x80b3056c <= offs <= 0x80b3056f:
                return offs + 0x55CC
        elif 0x80b30570 <= offs <= 0x80b89aaf:
                return offs + 0x55D0
        elif 0x80b89ac0 <= offs <= 0x81000000:
                return offs + 0x55C0
        return offs

def unfix_offs_kor_v1(offs):
        if 0x80000000 <= offs <= 0x800122c0:
                offs -= 0x0
        elif 0x800122d5 <= offs <= 0x800122ff:
                offs -= 0x14
        elif 0x80012304 <= offs <= 0x8001230b:
                offs -= 0x14
        elif 0x80012328 <= offs <= 0x80012333:
                offs -= 0x28
        elif 0x80012338 <= offs <= 0x8001233b:
                offs -= 0x2C
        elif 0x80012350 <= offs <= 0x8001235b:
                offs -= 0x3C
        elif 0x80012360 <= offs <= 0x80012377:
                offs -= 0x40
        elif 0x800123b0 <= offs <= 0x800123b7:
                offs -= 0x78
        elif 0x800123b8 <= offs <= 0x800123eb:
                offs -= 0x54
        elif 0x800123f0 <= offs <= 0x800123f7:
                offs -= 0x58
        elif 0x800123f8 <= offs <= 0x8001240f:
                offs -= 0x54
        elif 0x80012410 <= offs <= 0x8001243b:
                offs -= 0x50
        elif 0x80012450 <= offs <= 0x800124b3:
                offs -= 0x64
        elif 0x800124c0 <= offs <= 0x8004df17:
                offs -= 0x70
        elif 0x8004df18 <= offs <= 0x8004df23:
                offs -= 0x6C
        elif 0x8004df28 <= offs <= 0x8004df47:
                offs -= 0x68
        elif 0x8004df50 <= offs <= 0x8004dfa7:
                offs -= 0x70
        elif 0x8004dfa8 <= offs <= 0x8004dfb3:
                offs -= 0x6C
        elif 0x8004dfb8 <= offs <= 0x8004dfd7:
                offs -= 0x68
        elif 0x8004dfe0 <= offs <= 0x80050ecf:
                offs -= 0x70
        elif 0x80050f30 <= offs <= 0x80052fd3:
                offs -= 0xD0
        elif 0x80053004 <= offs <= 0x8005c533:
                offs -= 0xE0
        elif 0x8005c53c <= offs <= 0x8005c53f:
                offs -= 0xE8
        elif 0x8005c550 <= offs <= 0x8005c583:
                offs -= 0xF8
        elif 0x8005c588 <= offs <= 0x8005c593:
                offs -= 0xFC
        elif 0x8005c5a0 <= offs <= 0x8005c7cf:
                offs -= 0x108
        elif 0x8005c7d0 <= offs <= 0x8005f913:
                offs -= 0x100
        elif 0x8005f918 <= offs <= 0x8005f93f:
                offs -= 0xC8
        elif 0x8005f9c0 <= offs <= 0x8009638f:
                offs -= 0x140
        elif 0x80096390 <= offs <= 0x80096397:
                offs -= 0x13C
        elif 0x800963a0 <= offs <= 0x800963a3:
                offs -= 0x150
        elif 0x800963a4 <= offs <= 0x800963b7:
                offs -= 0x148
        elif 0x800963f4 <= offs <= 0x8009640b:
                offs -= 0x17C
        elif 0x80096410 <= offs <= 0x800b477f:
                offs -= 0x180
        elif 0x800b4790 <= offs <= 0x800b4790:
                offs -= 0x190
        elif 0x800b47a0 <= offs <= 0x800b47a0:
                offs -= 0x170
        elif 0x800b47b0 <= offs <= 0x800b47eb:
                offs -= 0x140
        elif 0x800b47ec <= offs <= 0x800b4817:
                offs -= 0xB8
        elif 0x800b4820 <= offs <= 0x800c8e6f:
                offs -= 0xC0
        elif 0x800c8e70 <= offs <= 0x800e4c7f:
                offs -= 0x20
        elif 0x800e4c80 <= offs <= 0x800e4c87:
                offs -= 0x1C
        elif 0x800e4c88 <= offs <= 0x800e4c8b:
                offs -= 0x28
        elif 0x800e4c8c <= offs <= 0x800e4d8f:
                offs -= 0x20
        elif 0x800e4d90 <= offs <= 0x800e4d97:
                offs += 0x4
        elif 0x800e4d9c <= offs <= 0x800e4ebb:
                offs += 0x4
        elif 0x800e4ec0 <= offs <= 0x800f794f:
                offs -= 0x0
        elif 0x800f795c <= offs <= 0x800f79cb:
                offs -= 0xC
        elif 0x800f79d0 <= offs <= 0x800fd5fb:
                offs -= 0x10
        elif 0x800fd630 <= offs <= 0x800fd64f:
                offs -= 0x44
        elif 0x800fd650 <= offs <= 0x801011ff:
                offs -= 0x40
        elif 0x80101270 <= offs <= 0x8010132f:
                offs -= 0xB0
        elif 0x80101380 <= offs <= 0x80107037:
                offs -= 0x100
        elif 0x80107040 <= offs <= 0x80107077:
                offs -= 0x108
        elif 0x80107080 <= offs <= 0x801070cb:
                offs -= 0x110
        elif 0x801070d0 <= offs <= 0x801070d3:
                offs -= 0x114
        elif 0x801070e0 <= offs <= 0x801070e3:
                offs -= 0x11C
        elif 0x801070ec <= offs <= 0x801070ef:
                offs -= 0x12C
        elif 0x801070f0 <= offs <= 0x8010710f:
                offs -= 0x128
        elif 0x80107110 <= offs <= 0x80107167:
                offs -= 0x120
        elif 0x8010716c <= offs <= 0x80107177:
                offs -= 0x124
        elif 0x80107188 <= offs <= 0x8010718b:
                offs -= 0x114
        elif 0x80107190 <= offs <= 0x801071a7:
                offs -= 0x118
        elif 0x801071b0 <= offs <= 0x801071b0:
                offs -= 0x120
        elif 0x801071d5 <= offs <= 0x801071e3:
                offs -= 0x144
        elif 0x80107218 <= offs <= 0x8010721f:
                offs -= 0x174
        elif 0x80107220 <= offs <= 0x80107220:
                offs -= 0x170
        elif 0x80107239 <= offs <= 0x8010723b:
                offs -= 0x188
        elif 0x80107240 <= offs <= 0x80107243:
                offs -= 0x188
        elif 0x80107248 <= offs <= 0x8010724b:
                offs -= 0x194
        elif 0x8010724c <= offs <= 0x8010724f:
                offs -= 0x18C
        elif 0x80107250 <= offs <= 0x80107253:
                offs -= 0x194
        elif 0x80107288 <= offs <= 0x8010728f:
                offs -= 0x1C0
        elif 0x801072c0 <= offs <= 0x801072fb:
                offs -= 0x1F0
        elif 0x80107308 <= offs <= 0x8010730f:
                offs -= 0x1FC
        elif 0x80107324 <= offs <= 0x80107403:
                offs -= 0x210
        elif 0x80107410 <= offs <= 0x8010749b:
                offs -= 0x21C
        elif 0x801075d0 <= offs <= 0x80107667:
                offs -= 0x350
        elif 0x80107674 <= offs <= 0x80107697:
                offs -= 0x35C
        elif 0x801076a4 <= offs <= 0x80107727:
                offs -= 0x368
        elif 0x801078a0 <= offs <= 0x801079cf:
                offs -= 0x4E0
        elif 0x801079d0 <= offs <= 0x801079d3:
                offs -= 0x4D8
        elif 0x801079d4 <= offs <= 0x801079db:
                offs -= 0x4E4
        elif 0x801079dc <= offs <= 0x801079e3:
                offs -= 0x4E0
        elif 0x801079e8 <= offs <= 0x801079ff:
                offs -= 0x4E4
        elif 0x80107a00 <= offs <= 0x80107ba7:
                offs -= 0x4E0
        elif 0x80107bac <= offs <= 0x80107beb:
                offs -= 0x4E4
        elif 0x80107bf8 <= offs <= 0x80107c77:
                offs -= 0x4F0
        elif 0x80107c84 <= offs <= 0x80107caf:
                offs -= 0x4FC
        elif 0x80107cb0 <= offs <= 0x80107ee3:
                offs -= 0x4F0
        elif 0x80107f14 <= offs <= 0x8010807b:
                offs -= 0x520
        elif 0x80108080 <= offs <= 0x80108147:
                offs -= 0x520
        elif 0x8010814c <= offs <= 0x80108183:
                offs -= 0x524
        elif 0x80108184 <= offs <= 0x80108187:
                offs -= 0x430
        elif 0x80108190 <= offs <= 0x80108193:
                offs -= 0x530
        elif 0x80108194 <= offs <= 0x80108197:
                offs -= 0x43C
        elif 0x80108198 <= offs <= 0x80108217:
                offs -= 0x528
        elif 0x80108220 <= offs <= 0x80108257:
                offs -= 0x530
        elif 0x8010825c <= offs <= 0x8010826f:
                offs -= 0x534
        elif 0x80108270 <= offs <= 0x80108273:
                offs -= 0x60C
        elif 0x80108274 <= offs <= 0x8010828b:
                offs -= 0x538
        elif 0x8010828c <= offs <= 0x8010828f:
                offs -= 0x620
        elif 0x80108290 <= offs <= 0x80108293:
                offs -= 0x534
        elif 0x80108294 <= offs <= 0x80108297:
                offs -= 0x62C
        elif 0x80108298 <= offs <= 0x801082af:
                offs -= 0x538
        elif 0x801082b0 <= offs <= 0x801083b3:
                offs -= 0x530
        elif 0x801083bc <= offs <= 0x801084eb:
                offs -= 0x538
        elif 0x801084f8 <= offs <= 0x80108513:
                offs -= 0x544
        elif 0x80108520 <= offs <= 0x8010f703:
                offs -= 0x550
        elif 0x8010f708 <= offs <= 0x8010f70b:
                offs -= 0x550
        elif 0x8010f724 <= offs <= 0x8010f727:
                offs -= 0x570
        elif 0x8010f728 <= offs <= 0x8010f73b:
                offs -= 0x56C
        elif 0x8010f740 <= offs <= 0x8010f74f:
                offs -= 0x570
        elif 0x8010f750 <= offs <= 0x8010f767:
                offs -= 0x564
        elif 0x8010f76c <= offs <= 0x8010f76f:
                offs -= 0x530
        elif 0x8010f770 <= offs <= 0x8010f773:
                offs -= 0x538
        elif 0x8010f774 <= offs <= 0x8010f777:
                offs -= 0x534
        elif 0x8010f778 <= offs <= 0x8010f77b:
                offs -= 0x544
        elif 0x8010f77c <= offs <= 0x8010f78f:
                offs -= 0x538
        elif 0x8010f790 <= offs <= 0x8010f793:
                offs -= 0x464
        elif 0x8010f798 <= offs <= 0x8010f79b:
                offs -= 0x468
        elif 0x8010f79c <= offs <= 0x8010f7a3:
                offs -= 0x464
        elif 0x8010f7a8 <= offs <= 0x8010f7af:
                offs -= 0x468
        elif 0x8010f7b0 <= offs <= 0x8010f7bf:
                offs -= 0x438
        elif 0x8010f7c0 <= offs <= 0x8010f7eb:
                offs -= 0x3D4
        elif 0x8010f7ec <= offs <= 0x80151247:
                offs -= 0x3D0
        elif 0x80151248 <= offs <= 0x8015124b:
                offs -= 0x3C8
        elif 0x80151250 <= offs <= 0x8015127b:
                offs -= 0x3C0
        elif 0x8015127c <= offs <= 0x8015127f:
                offs -= 0x3B8
        elif 0x80151284 <= offs <= 0x801517bf:
                offs -= 0x3B0
        elif 0x801517d4 <= offs <= 0x80151823:
                offs -= 0x3C4
        elif 0x80151830 <= offs <= 0x801527e3:
                offs -= 0x3D0
        elif 0x801527f0 <= offs <= 0x801527fb:
                offs -= 0x3DC
        elif 0x80152804 <= offs <= 0x80158c17:
                offs -= 0x3E0
        elif 0x80158c18 <= offs <= 0x80158dab:
                offs -= 0x3DC
        elif 0x80158db0 <= offs <= 0x801594a3:
                offs -= 0x3E0
        elif 0x801594b0 <= offs <= 0x801594d7:
                offs -= 0x3EC
        elif 0x801594dc <= offs <= 0x8015951f:
                offs -= 0x3F0
        elif 0x80159524 <= offs <= 0x80159583:
                offs -= 0x3F4
        elif 0x80159590 <= offs <= 0x8015b0df:
                offs -= 0x400
        elif 0x8015b0e0 <= offs <= 0x8015b0e3:
                offs -= 0x3FC
        elif 0x8015b0e4 <= offs <= 0x8015b0e7:
                offs -= 0x3B8
        elif 0x8015b0e8 <= offs <= 0x8015b0ef:
                offs -= 0x400
        elif 0x8015b0f0 <= offs <= 0x8015b0f3:
                offs -= 0x410
        elif 0x8015b0f4 <= offs <= 0x8015b0fb:
                offs -= 0x400
        elif 0x8015b0fc <= offs <= 0x8015b0ff:
                offs -= 0x40C
        elif 0x8015b100 <= offs <= 0x8015b12f:
                offs -= 0x404
        elif 0x8015b130 <= offs <= 0x802bbabf:
                offs -= 0x400
        elif 0x802bbac0 <= offs <= 0x802bbb3f:
                offs -= 0x3F0
        elif 0x802bbb40 <= offs <= 0x802bbedf:
                offs -= 0x2E0
        elif 0x802bbee0 <= offs <= 0x802e48cc:
                offs -= 0x250
        elif 0x802e49c8 <= offs <= 0x802ee01f:
                offs -= 0x348
        elif 0x802ee020 <= offs <= 0x802f4e5f:
                offs -= 0x340
        elif 0x802f4e68 <= offs <= 0x802f4ec7:
                offs -= 0x348
        elif 0x802f4ed0 <= offs <= 0x802f4f2f:
                offs -= 0x350
        elif 0x802f4f38 <= offs <= 0x802f609b:
                offs -= 0x358
        elif 0x8030227c <= offs <= 0x803097d7:
                offs -= 0xC538
        elif 0x803097e0 <= offs <= 0x80323c77:
                offs -= 0xC540
        elif 0x80323c78 <= offs <= 0x8032cd37:
                offs -= 0xC528
        elif 0x8032cd80 <= offs <= 0x8032dbbf:
                offs -= 0xC570
        elif 0x8032dbc8 <= offs <= 0x8032e47f:
                offs -= 0xC578
        elif 0x8032e484 <= offs <= 0x8032e493:
                offs -= 0xC57C
        elif 0x8032e4a0 <= offs <= 0x8032e4bb:
                offs -= 0xC588
        elif 0x8032e4bc <= offs <= 0x8032e4f3:
                offs -= 0xC584
        elif 0x8032e4f8 <= offs <= 0x8032e51f:
                offs -= 0xC588
        elif 0x8032e524 <= offs <= 0x8032e573:
                offs -= 0xC58C
        elif 0x8032e57c <= offs <= 0x8032e597:
                offs -= 0xC594
        elif 0x8032e5b8 <= offs <= 0x8032e603:
                offs -= 0xC5B4
        elif 0x8032e650 <= offs <= 0x8032e66b:
                offs -= 0xC600
        elif 0x8032e688 <= offs <= 0x8032e6db:
                offs -= 0xC61C
        elif 0x8032e730 <= offs <= 0x8032e74f:
                offs -= 0xC670
        elif 0x8032e770 <= offs <= 0x8032e7d7:
                offs -= 0xC690
        elif 0x8032e83c <= offs <= 0x8032e89b:
                offs -= 0xC6F4
        elif 0x8032e8fc <= offs <= 0x8032e917:
                offs -= 0xC754
        elif 0x8032e930 <= offs <= 0x8032e99f:
                offs -= 0xC76C
        elif 0x8032ea0c <= offs <= 0x8032ea7b:
                offs -= 0xC7D8
        elif 0x8032eaec <= offs <= 0x8032eb0b:
                offs -= 0xC848
        elif 0x8032eb30 <= offs <= 0x8032eba3:
                offs -= 0xC86C
        elif 0x8032ec18 <= offs <= 0x8032ec33:
                offs -= 0xC8E0
        elif 0x8032ec50 <= offs <= 0x8032ecc3:
                offs -= 0xC8FC
        elif 0x8032ed30 <= offs <= 0x8032ed4f:
                offs -= 0xC968
        elif 0x8032ed70 <= offs <= 0x8032edcf:
                offs -= 0xC988
        elif 0x8032ef00 <= offs <= 0x8032ef8f:
                offs -= 0xCAB8
        elif 0x8032efa8 <= offs <= 0x8032efb3:
                offs -= 0xCAD0
        elif 0x8032efb8 <= offs <= 0x8032fabf:
                offs -= 0xCAC8
        elif 0x8032fac0 <= offs <= 0x8032fae3:
                offs -= 0xCA20
        elif 0x8032faf8 <= offs <= 0x803343af:
                offs -= 0xC9E0
        elif 0x803343b8 <= offs <= 0x8033b15f:
                offs -= 0xC9E8
        elif 0x8033b160 <= offs <= 0x8035e35f:
                offs -= 0xC9E0
        elif 0x8035e380 <= offs <= 0x8043485f:
                offs -= 0xCA00
        elif 0x80434860 <= offs <= 0x80434c17:
                offs -= 0xC9D8
        elif 0x80434c18 <= offs <= 0x80434c27:
                offs -= 0xC9D4
        elif 0x80434c28 <= offs <= 0x80435df7:
                offs -= 0xC9D0
        elif 0x80435e00 <= offs <= 0x80435e6f:
                offs -= 0xC9D8
        elif 0x80435e78 <= offs <= 0x80435ebf:
                offs -= 0xC9E0
        elif 0x80435ec4 <= offs <= 0x80435ec7:
                offs -= 0xC9E4
        elif 0x80435ec8 <= offs <= 0x80435f2b:
                offs -= 0xC9E0
        elif 0x80435f2c <= offs <= 0x80435f2f:
                offs -= 0xC9CC
        elif 0x80435f30 <= offs <= 0x80439ef7:
                offs -= 0xC9C0
        elif 0x80439f00 <= offs <= 0x80439f0f:
                offs -= 0xC9C8
        elif 0x80439f14 <= offs <= 0x80439f17:
                offs -= 0xC9CC
        elif 0x80439f18 <= offs <= 0x8043bdef:
                offs -= 0xC9C8
        elif 0x8043bdf0 <= offs <= 0x806dffff:
                offs -= 0xC9C0
        elif 0x806e0000 <= offs <= 0x807683f2:
                offs -= 0x0
        elif 0x807683f3 <= offs <= 0x80768423:
                offs += 0x13
        elif 0x80768424 <= offs <= 0x80768456:
                offs += 0x26
        elif 0x80768457 <= offs <= 0x8076848b:
                offs += 0x39
        elif 0x8076848c <= offs <= 0x80768493:
                offs += 0x4C
        elif 0x807684a0 <= offs <= 0x8076b23f:
                offs += 0x40
        elif 0x8076b248 <= offs <= 0x8076ca5b:
                offs += 0x38
        elif 0x8076ca6c <= offs <= 0x8076d437:
                offs += 0x28
        elif 0x8076d460 <= offs <= 0x8076dd3c:
                offs += 0x40
        elif 0x8076dd58 <= offs <= 0x8076dd5b:
                offs += 0x28
        elif 0x8076dd5c <= offs <= 0x8076dd5f:
                offs += 0x2C
        elif 0x8076dd60 <= offs <= 0x8076dd63:
                offs += 0x30
        elif 0x8076dd64 <= offs <= 0x8076dd67:
                offs += 0x28
        elif 0x8076dd68 <= offs <= 0x8076dd6b:
                offs += 0x30
        elif 0x8076dd6c <= offs <= 0x8076dd6f:
                offs += 0x28
        elif 0x8076dd70 <= offs <= 0x8076dda3:
                offs += 0x2C
        elif 0x8076dda8 <= offs <= 0x8076ddab:
                offs += 0x28
        elif 0x8076ddb0 <= offs <= 0x8076defb:
                offs += 0x24
        elif 0x8076df00 <= offs <= 0x80779adc:
                offs += 0x20
        elif 0x80779ae0 <= offs <= 0x80779afb:
                offs += 0xD8
        elif 0x80779afc <= offs <= 0x80779b0b:
                offs += 0x100
        elif 0x80779b0c <= offs <= 0x8078880f:
                offs += 0x150
        elif 0x80788810 <= offs <= 0x8078882b:
                offs += 0x208
        elif 0x8078882c <= offs <= 0x8078883b:
                offs += 0x230
        elif 0x8078883c <= offs <= 0x8079d2ff:
                offs += 0x280
        elif 0x807a1610 <= offs <= 0x807a1983:
                offs -= 0x4090
        elif 0x807a1988 <= offs <= 0x807a1a4f:
                offs -= 0x4094
        elif 0x807a1a6c <= offs <= 0x807a1a9b:
                offs -= 0x40B0
        elif 0x807a1aac <= offs <= 0x807a229b:
                offs -= 0x40C0
        elif 0x807a22ac <= offs <= 0x807a23c3:
                offs -= 0x40D0
        elif 0x807a23c4 <= offs <= 0x807a23c7:
                offs -= 0x40CC
        elif 0x807a23f4 <= offs <= 0x807a2463:
                offs -= 0x40DC
        elif 0x807a2464 <= offs <= 0x807a2487:
                offs -= 0x40BC
        elif 0x807a2488 <= offs <= 0x807a248b:
                offs -= 0x40B8
        elif 0x807a24b8 <= offs <= 0x807a2527:
                offs -= 0x40C8
        elif 0x807a2528 <= offs <= 0x807a253f:
                offs -= 0x40A8
        elif 0x807a2540 <= offs <= 0x807a2bc3:
                offs -= 0x40A0
        elif 0x807a2bd4 <= offs <= 0x807a2c13:
                offs -= 0x40B0
        elif 0x807a2c28 <= offs <= 0x807a2c9f:
                offs -= 0x40C4
        elif 0x807a2ca0 <= offs <= 0x807a530f:
                offs -= 0x40C0
        elif 0x807a5310 <= offs <= 0x807a5313:
                offs -= 0x40B4
        elif 0x807a5314 <= offs <= 0x807a5317:
                offs -= 0x40C0
        elif 0x807a5318 <= offs <= 0x807a532b:
                offs -= 0x40B8
        elif 0x807a532c <= offs <= 0x807a532f:
                offs -= 0x40DC
        elif 0x807a5330 <= offs <= 0x807a5333:
                offs -= 0x40D8
        elif 0x807a5334 <= offs <= 0x807a846f:
                offs -= 0x40C0
        elif 0x807a8470 <= offs <= 0x807a8473:
                offs -= 0x40B8
        elif 0x807a8474 <= offs <= 0x807a8477:
                offs -= 0x40C4
        elif 0x807a8478 <= offs <= 0x807a857b:
                offs -= 0x40BC
        elif 0x807a8580 <= offs <= 0x807abc23:
                offs -= 0x40C0
        elif 0x807abc24 <= offs <= 0x807abc27:
                offs -= 0x40B8
        elif 0x807abc28 <= offs <= 0x807abc2b:
                offs -= 0x40C4
        elif 0x807abc2c <= offs <= 0x807abcdb:
                offs -= 0x40BC
        elif 0x807abce0 <= offs <= 0x8081b450:
                offs -= 0x40C0
        elif 0x8081b459 <= offs <= 0x8081b45b:
                offs -= 0x40C8
        elif 0x8081b45c <= offs <= 0x8081b45f:
                offs -= 0x40A8
        elif 0x8081b464 <= offs <= 0x8081b467:
                offs -= 0x40CC
        elif 0x8081b468 <= offs <= 0x8081b46f:
                offs -= 0x40C8
        elif 0x8081b470 <= offs <= 0x8081b473:
                offs -= 0x40D4
        elif 0x8081b474 <= offs <= 0x8081b477:
                offs -= 0x40E0
        elif 0x8081b478 <= offs <= 0x8081b483:
                offs -= 0x40D0
        elif 0x8081b484 <= offs <= 0x8081b4b7:
                offs -= 0x40CC
        elif 0x8081b4cc <= offs <= 0x8081b4cf:
                offs -= 0x40E0
        elif 0x8081b4d8 <= offs <= 0x8081b4df:
                offs -= 0x40E8
        elif 0x8081b4e0 <= offs <= 0x8081b5bb:
                offs -= 0x40E0
        elif 0x8081b5c8 <= offs <= 0x8081b61b:
                offs -= 0x40EC
        elif 0x8081b620 <= offs <= 0x8082aecf:
                offs -= 0x40F0
        elif 0x8082b0a0 <= offs <= 0x8082b213:
                offs -= 0x4210
        elif 0x8082b218 <= offs <= 0x8082b21b:
                offs -= 0x4214
        elif 0x8082b220 <= offs <= 0x8082b227:
                offs -= 0x4218
        elif 0x8082b23c <= offs <= 0x8082b24b:
                offs -= 0x4228
        elif 0x8082b24c <= offs <= 0x8082b267:
                offs -= 0x4224
        elif 0x8082b268 <= offs <= 0x8082b447:
                offs -= 0x4220
        elif 0x8082b450 <= offs <= 0x8082b4c7:
                offs -= 0x4228
        elif 0x8082b4d0 <= offs <= 0x80872bbf:
                offs -= 0x4230
        elif 0x80872bd4 <= offs <= 0x80872c1f:
                offs -= 0x4244
        elif 0x80872c20 <= offs <= 0x80872cfb:
                offs -= 0x4240
        elif 0x80872cfc <= offs <= 0x80872cff:
                offs -= 0x423C
        elif 0x80872d00 <= offs <= 0x80872d03:
                offs -= 0x4244
        elif 0x80872d04 <= offs <= 0x80872dbb:
                offs -= 0x4240
        elif 0x80872dc0 <= offs <= 0x80872dc3:
                offs -= 0x4244
        elif 0x80872de0 <= offs <= 0x80872de3:
                offs -= 0x4260
        elif 0x80872de8 <= offs <= 0x80872deb:
                offs -= 0x4260
        elif 0x80872dec <= offs <= 0x80872def:
                offs -= 0x4268
        elif 0x80872df0 <= offs <= 0x80872e23:
                offs -= 0x4264
        elif 0x80872e30 <= offs <= 0x808d7e07:
                offs -= 0x4270
        elif 0x808d7e0c <= offs <= 0x808d7e0f:
                offs -= 0x4224
        elif 0x808d7e10 <= offs <= 0x808d7e13:
                offs -= 0x422C
        elif 0x808d7e14 <= offs <= 0x808d7e17:
                offs -= 0x4228
        elif 0x808d7e1c <= offs <= 0x808d7e1f:
                offs -= 0x421C
        elif 0x808d7e20 <= offs <= 0x808d7e23:
                offs -= 0x4224
        elif 0x808d7e24 <= offs <= 0x808d7e27:
                offs -= 0x422C
        elif 0x808d7e2c <= offs <= 0x808d7e4f:
                offs -= 0x4228
        elif 0x808d7e50 <= offs <= 0x808f34ff:
                offs -= 0x4220
        elif 0x808f3500 <= offs <= 0x808ffb3f:
                offs -= 0x41F0
        elif 0x808ffb4c <= offs <= 0x808ffb57:
                offs -= 0x41FC
        elif 0x808ffb8c <= offs <= 0x808ffee7:
                offs -= 0x41F8
        elif 0x808fff04 <= offs <= 0x808fff1b:
                offs -= 0x420C
        elif 0x808fff20 <= offs <= 0x809001ef:
                offs -= 0x4210
        elif 0x80900230 <= offs <= 0x80906cd7:
                offs -= 0x4250
        elif 0x80906cdc <= offs <= 0x80906d0b:
                offs -= 0x423C
        elif 0x80906d10 <= offs <= 0x8090d6bf:
                offs -= 0x4240
        elif 0x8090d6c0 <= offs <= 0x8092330f:
                offs -= 0x41B0
        elif 0x80923314 <= offs <= 0x8092331b:
                offs -= 0x41B4
        elif 0x80923320 <= offs <= 0x80923337:
                offs -= 0x41B8
        elif 0x80923340 <= offs <= 0x80923417:
                offs -= 0x41C0
        elif 0x8092342c <= offs <= 0x8092344f:
                offs -= 0x41D4
        elif 0x80923450 <= offs <= 0x809379bb:
                offs -= 0x41D0
        elif 0x809379c4 <= offs <= 0x809393e7:
                offs -= 0x41D8
        elif 0x80939500 <= offs <= 0x8093c5bf:
                offs -= 0x42F0
        elif 0x8093c5e0 <= offs <= 0x80944f77:
                offs -= 0x4300
        elif 0x80944f78 <= offs <= 0x80944f97:
                offs -= 0x42FC
        elif 0x80944f98 <= offs <= 0x80944fb7:
                offs -= 0x42F8
        elif 0x80944fb8 <= offs <= 0x80944fc3:
                offs -= 0x42F4
        elif 0x80944fc4 <= offs <= 0x80944ff7:
                offs -= 0x4114
        elif 0x80944ff8 <= offs <= 0x80945027:
                offs -= 0x4110
        elif 0x80945040 <= offs <= 0x80948f7b:
                offs -= 0x40D8
        elif 0x80948f7c <= offs <= 0x80948f9b:
                offs -= 0x40D4
        elif 0x80948f9c <= offs <= 0x80948fc7:
                offs -= 0x40CC
        elif 0x80948fc8 <= offs <= 0x80948fdf:
                offs -= 0x3F0C
        elif 0x80948fe0 <= offs <= 0x8094900b:
                offs -= 0x3F08
        elif 0x80949024 <= offs <= 0x8094d377:
                offs -= 0x3ED0
        elif 0x8094d384 <= offs <= 0x8094d66b:
                offs -= 0x3EDC
        elif 0x8094d670 <= offs <= 0x8094e267:
                offs -= 0x3EE0
        elif 0x8094f418 <= offs <= 0x8094f44b:
                offs -= 0x5090
        elif 0x8094f450 <= offs <= 0x8094f4d7:
                offs -= 0x5094
        elif 0x8094f4dc <= offs <= 0x8094f507:
                offs -= 0x5098
        elif 0x8094f50c <= offs <= 0x8094f537:
                offs -= 0x509C
        elif 0x8094f538 <= offs <= 0x8094f68b:
                offs -= 0x5098
        elif 0x8094f69c <= offs <= 0x8094f6bb:
                offs -= 0x50A4
        elif 0x8094f6c0 <= offs <= 0x8095117f:
                offs -= 0x50A8
        elif 0x80951188 <= offs <= 0x809511d3:
                offs -= 0x50B0
        elif 0x809511d8 <= offs <= 0x809511ff:
                offs -= 0x50B4
        elif 0x80951200 <= offs <= 0x80951227:
                offs -= 0x50B0
        elif 0x8095122c <= offs <= 0x80951257:
                offs -= 0x50B4
        elif 0x80951258 <= offs <= 0x80951b6b:
                offs -= 0x50B0
        elif 0x80951b74 <= offs <= 0x80952683:
                offs -= 0x50B8
        elif 0x8095268c <= offs <= 0x8098f43b:
                offs -= 0x50C0
        elif 0x8098f43c <= offs <= 0x8098f457:
                offs -= 0x50A4
        elif 0x8098f458 <= offs <= 0x809957df:
                offs -= 0x4FD0
        elif 0x809957e0 <= offs <= 0x809992d7:
                offs -= 0x4FC0
        elif 0x809997f8 <= offs <= 0x80a0fa5f:
                offs -= 0x54E0
        elif 0x80a0fa64 <= offs <= 0x80a0fa8b:
                offs -= 0x54E4
        elif 0x80a0fa9c <= offs <= 0x80a0fad7:
                offs -= 0x54E8
        elif 0x80a0fadc <= offs <= 0x80a0faeb:
                offs -= 0x54EC
        elif 0x80a0faf0 <= offs <= 0x80a0faff:
                offs -= 0x54F0
        elif 0x80a0fb08 <= offs <= 0x80a0fb2b:
                offs -= 0x54F8
        elif 0x80a0fb38 <= offs <= 0x80a0fb3b:
                offs -= 0x54FC
        elif 0x80a0fb3c <= offs <= 0x80a0fb3f:
                offs -= 0x54F4
        elif 0x80a0fb40 <= offs <= 0x80a0fb43:
                offs -= 0x54FC
        elif 0x80a0fb44 <= offs <= 0x80a0fb47:
                offs -= 0x5504
        elif 0x80a0fb48 <= offs <= 0x80a0fb7b:
                offs -= 0x54FC
        elif 0x80a0fb80 <= offs <= 0x80a106c3:
                offs -= 0x5500
        elif 0x80a106c8 <= offs <= 0x80a106cb:
                offs -= 0x5500
        elif 0x80a1073c <= offs <= 0x80a1073f:
                offs -= 0x5578
        elif 0x80a10740 <= offs <= 0x80a10a83:
                offs -= 0x5574
        elif 0x80a10a90 <= offs <= 0x80a3e460:
                offs -= 0x5580
        elif 0x80a3e485 <= offs <= 0x80a3e487:
                offs -= 0x55A4
        elif 0x80a3e490 <= offs <= 0x80a3e49b:
                offs -= 0x55AC
        elif 0x80a3e4a0 <= offs <= 0x80b169ef:
                offs -= 0x55B0
        elif 0x80b16a00 <= offs <= 0x80b35abb:
                offs -= 0x55C0
        elif 0x80b35ac8 <= offs <= 0x80b35adf:
                offs -= 0x55CC
        elif 0x80b35ae4 <= offs <= 0x80b35ae7:
                offs -= 0x55C8
        elif 0x80b35aec <= offs <= 0x80b35aef:
                offs -= 0x55BC
        elif 0x80b35af4 <= offs <= 0x80b35af7:
                offs -= 0x55D0
        elif 0x80b35af8 <= offs <= 0x80b35afb:
                offs -= 0x55CC
        elif 0x80b35afc <= offs <= 0x80b35aff:
                offs -= 0x55DC
        elif 0x80b35b00 <= offs <= 0x80b35b03:
                offs -= 0x55D8
        elif 0x80b35b04 <= offs <= 0x80b35b0b:
                offs -= 0x55CC
        elif 0x80b35b0c <= offs <= 0x80b35b0f:
                offs -= 0x55D8
        elif 0x80b35b10 <= offs <= 0x80b35b13:
                offs -= 0x55CC
        elif 0x80b35b14 <= offs <= 0x80b35b17:
                offs -= 0x55D4
        elif 0x80b35b18 <= offs <= 0x80b35b1b:
                offs -= 0x55D0
        elif 0x80b35b1c <= offs <= 0x80b35b2b:
                offs -= 0x55CC
        elif 0x80b35b2c <= offs <= 0x80b35b33:
                offs -= 0x55C8
        elif 0x80b35b34 <= offs <= 0x80b35b37:
                offs -= 0x55E8
        elif 0x80b35b38 <= offs <= 0x80b35b3b:
                offs -= 0x55CC
        elif 0x80b35b3c <= offs <= 0x80b35b3f:
                offs -= 0x55DC
        elif 0x80b35b40 <= offs <= 0x80b8f07f:
                offs -= 0x55D0
        elif 0x80b8f080 <= offs <= 0x810055c0:
                offs -= 0x55C0
        return unfix_offs_pal_v2(offs)
# ntsc1

def fix_offs_ntsc_v1(offs):
        if 0x80000000 <= offs <= 0x800b4600:
                return offs - 0x0
        elif 0x800b4630 <= offs <= 0x800b463f:
                return offs - 0x20
        elif 0x800b4650 <= offs <= 0x800b466b:
                return offs - 0x1C
        elif 0x800b4670 <= offs <= 0x800b46bb:
                return offs - 0x20
        elif 0x800b46cc <= offs <= 0x800b46fb:
                return offs - 0x30
        elif 0x800b4724 <= offs <= 0x800b475f:
                return offs - 0x58
        elif 0x800b4760 <= offs <= 0x800c8daf:
                return offs - 0x50
        elif 0x800c8e50 <= offs <= 0x800e4c5f:
                return offs - 0xF0
        elif 0x800e4c60 <= offs <= 0x800e4c63:
                return offs - 0xE8
        elif 0x800e4c64 <= offs <= 0x800e4c6b:
                return offs - 0xF4
        elif 0x800e4c6c <= offs <= 0x800e4d6f:
                return offs - 0xF0
        elif 0x800e4d94 <= offs <= 0x800e4d9b:
                return offs - 0x114
        elif 0x800e4da0 <= offs <= 0x800e4ebf:
                return offs - 0x114
        elif 0x800e4ec0 <= offs <= 0x8010f203:
                return offs - 0x110
        elif 0x8010f234 <= offs <= 0x8010f237:
                return offs - 0x134
        elif 0x8010f238 <= offs <= 0x8010f243:
                return offs - 0x144
        elif 0x8010f244 <= offs <= 0x802bb6bf:
                return offs - 0x140
        elif 0x802bb6d0 <= offs <= 0x802bb74f:
                return offs - 0x150
        elif 0x802bb860 <= offs <= 0x802bbbff:
                return offs - 0x260
        elif 0x802bbc90 <= offs <= 0x802edccf:
                return offs - 0x2F0
        elif 0x802edce0 <= offs <= 0x80317737:
                return offs - 0x300
        elif 0x80317750 <= offs <= 0x80322ffb:
                return offs - 0x318
        elif 0x80323000 <= offs <= 0x8032301b:
                return offs - 0x31C
        elif 0x80323020 <= offs <= 0x8032307b:
                return offs - 0x320
        elif 0x80323080 <= offs <= 0x803230cf:
                return offs - 0x324
        elif 0x803230e8 <= offs <= 0x803230ff:
                return offs - 0x33C
        elif 0x8032310c <= offs <= 0x8032e77f:
                return offs - 0x348
        elif 0x8032e780 <= offs <= 0x8035197f:
                return offs - 0x340
        elif 0x80351980 <= offs <= 0x80429563:
                return offs - 0x300
        elif 0x80429564 <= offs <= 0x80429d7f:
                return offs - 0x2F8
        elif 0x80429d80 <= offs <= 0x806dffff:
                return offs - 0x2E0
        elif 0x806e0000 <= offs <= 0x8098a43b:
                return offs - 0x0
        elif 0x8098a43c <= offs <= 0x8098a473:
                return offs + 0x24
        elif 0x8098a478 <= offs <= 0x81000000:
                return offs + 0x20
        return offs

def unfix_offs_ntsc_v1(offs):
        if 0x80000000 <= offs <= 0x800b4600:
                offs -= 0x0
        elif 0x800b4610 <= offs <= 0x800b461f:
                offs += 0x20
        elif 0x800b4634 <= offs <= 0x800b464f:
                offs += 0x1C
        elif 0x800b4650 <= offs <= 0x800b469b:
                offs += 0x20
        elif 0x800b469c <= offs <= 0x800b46cb:
                offs += 0x30
        elif 0x800b46cc <= offs <= 0x800b4707:
                offs += 0x58
        elif 0x800b4710 <= offs <= 0x800c8d5f:
                offs += 0x50
        elif 0x800c8d60 <= offs <= 0x800e4b6f:
                offs += 0xF0
        elif 0x800e4b70 <= offs <= 0x800e4b77:
                offs += 0xF4
        elif 0x800e4b78 <= offs <= 0x800e4b7b:
                offs += 0xE8
        elif 0x800e4b7c <= offs <= 0x800e4c7f:
                offs += 0xF0
        elif 0x800e4c80 <= offs <= 0x800e4c87:
                offs += 0x114
        elif 0x800e4c8c <= offs <= 0x800e4dab:
                offs += 0x114
        elif 0x800e4db0 <= offs <= 0x8010f0f3:
                offs += 0x110
        elif 0x8010f0f4 <= offs <= 0x8010f0ff:
                offs += 0x144
        elif 0x8010f100 <= offs <= 0x8010f103:
                offs += 0x134
        elif 0x8010f104 <= offs <= 0x802bb57f:
                offs += 0x140
        elif 0x802bb580 <= offs <= 0x802bb5ff:
                offs += 0x150
        elif 0x802bb600 <= offs <= 0x802bb99f:
                offs += 0x260
        elif 0x802bb9a0 <= offs <= 0x802ed9df:
                offs += 0x2F0
        elif 0x802ed9e0 <= offs <= 0x80317437:
                offs += 0x300
        elif 0x80317438 <= offs <= 0x80322ce3:
                offs += 0x318
        elif 0x80322ce4 <= offs <= 0x80322cff:
                offs += 0x31C
        elif 0x80322d00 <= offs <= 0x80322d5b:
                offs += 0x320
        elif 0x80322d5c <= offs <= 0x80322dab:
                offs += 0x324
        elif 0x80322dac <= offs <= 0x80322dc3:
                offs += 0x33C
        elif 0x80322dc4 <= offs <= 0x8032e437:
                offs += 0x348
        elif 0x8032e440 <= offs <= 0x8035163f:
                offs += 0x340
        elif 0x80351680 <= offs <= 0x80429263:
                offs += 0x300
        elif 0x8042926c <= offs <= 0x80429a87:
                offs += 0x2F8
        elif 0x80429aa0 <= offs <= 0x806dfd1f:
                offs += 0x2E0
        elif 0x806e0000 <= offs <= 0x8098a43b:
                offs -= 0x0
        elif 0x8098a460 <= offs <= 0x8098a497:
                offs -= 0x24
        elif 0x8098a498 <= offs <= 0x81000020:
                offs -= 0x20
        return offs
# ntsc2

def fix_offs_ntsc_v2(offs):
        offs = fix_offs_ntsc_v1(offs)
        if 0x80000000 <= offs <= 0x800cf1af:
                return offs - 0x0
        elif 0x800cf1b0 <= offs <= 0x800cf1b3:
                return offs + 0x4
        elif 0x800cf1b4 <= offs <= 0x800cf213:
                return offs + 0x8
        elif 0x800cf214 <= offs <= 0x800cf217:
                return offs + 0xC
        elif 0x800cf218 <= offs <= 0x800cf21f:
                return offs + 0x18
        elif 0x800cf230 <= offs <= 0x800cf237:
                return offs - 0x8
        elif 0x800cf238 <= offs <= 0x800cf5f7:
                return offs - 0x0
        elif 0x800cf5f8 <= offs <= 0x800cf5fb:
                return offs + 0x4
        elif 0x800cf5fc <= offs <= 0x800cf64b:
                return offs + 0x8
        elif 0x800cf64c <= offs <= 0x800cf64f:
                return offs + 0xC
        elif 0x800cf650 <= offs <= 0x800cf6f7:
                return offs + 0x10
        elif 0x800cf6f8 <= offs <= 0x800cf6fb:
                return offs + 0x14
        elif 0x800cf6fc <= offs <= 0x800cf703:
                return offs + 0x20
        elif 0x800cf714 <= offs <= 0x800cf71b:
                return offs - 0x0
        elif 0x800cf71c <= offs <= 0x800cf817:
                return offs + 0x8
        elif 0x800cf820 <= offs <= 0x800e0c3b:
                return offs - 0x0
        elif 0x800e0c40 <= offs <= 0x800e0c4f:
                return offs - 0x4
        elif 0x800e0c54 <= offs <= 0x800e0c5b:
                return offs - 0x8
        elif 0x800e0c5c <= offs <= 0x800e0ceb:
                return offs + 0x10
        elif 0x800e0cf4 <= offs <= 0x800e0d07:
                return offs + 0x8
        elif 0x800e0d0c <= offs <= 0x800e0d1b:
                return offs + 0x4
        elif 0x800e0d20 <= offs <= 0x807683f6:
                return offs - 0x0
        elif 0x807683f7 <= offs <= 0x8076842b:
                return offs + 0xF
        elif 0x8076842c <= offs <= 0x80768462:
                return offs + 0x1E
        elif 0x80768463 <= offs <= 0x8076849b:
                return offs + 0x2D
        elif 0x8076849c <= offs <= 0x8076849f:
                return offs + 0x3C
        elif 0x807684a0 <= offs <= 0x807aaa30:
                return offs + 0x40
        elif 0x807aaa6c <= offs <= 0x8099081f:
                return offs + 0x10
        elif 0x80990820 <= offs <= 0x81000000:
                return offs + 0x20
        return offs

def unfix_offs_ntsc_v2(offs):
        if 0x80000000 <= offs <= 0x800cf1af:
                offs -= 0x0
        elif 0x800cf1b4 <= offs <= 0x800cf1b7:
                offs -= 0x4
        elif 0x800cf1bc <= offs <= 0x800cf21b:
                offs -= 0x8
        elif 0x800cf220 <= offs <= 0x800cf223:
                offs -= 0xC
        elif 0x800cf228 <= offs <= 0x800cf22f:
                offs += 0x8
        elif 0x800cf230 <= offs <= 0x800cf237:
                offs -= 0x18
        elif 0x800cf238 <= offs <= 0x800cf5f7:
                offs -= 0x0
        elif 0x800cf5fc <= offs <= 0x800cf5ff:
                offs -= 0x4
        elif 0x800cf604 <= offs <= 0x800cf653:
                offs -= 0x8
        elif 0x800cf658 <= offs <= 0x800cf65b:
                offs -= 0xC
        elif 0x800cf660 <= offs <= 0x800cf707:
                offs -= 0x10
        elif 0x800cf70c <= offs <= 0x800cf70f:
                offs -= 0x14
        elif 0x800cf714 <= offs <= 0x800cf71b:
                offs -= 0x0
        elif 0x800cf71c <= offs <= 0x800cf723:
                offs -= 0x20
        elif 0x800cf724 <= offs <= 0x800cf81f:
                offs -= 0x8
        elif 0x800cf820 <= offs <= 0x800e0c3b:
                offs -= 0x0
        elif 0x800e0c3c <= offs <= 0x800e0c4b:
                offs += 0x4
        elif 0x800e0c4c <= offs <= 0x800e0c53:
                offs += 0x8
        elif 0x800e0c6c <= offs <= 0x800e0cfb:
                offs -= 0x10
        elif 0x800e0cfc <= offs <= 0x800e0d0f:
                offs -= 0x8
        elif 0x800e0d10 <= offs <= 0x800e0d1f:
                offs -= 0x4
        elif 0x800e0d20 <= offs <= 0x807683f6:
                offs -= 0x0
        elif 0x80768406 <= offs <= 0x8076843a:
                offs -= 0xF
        elif 0x8076844a <= offs <= 0x80768480:
                offs -= 0x1E
        elif 0x80768490 <= offs <= 0x807684c8:
                offs -= 0x2D
        elif 0x807684d8 <= offs <= 0x807684db:
                offs -= 0x3C
        elif 0x807684e0 <= offs <= 0x807aaa70:
                offs -= 0x40
        elif 0x807aaa7c <= offs <= 0x8099082f:
                offs -= 0x10
        elif 0x80990840 <= offs <= 0x81000020:
                offs -= 0x20
        return unfix_offs_ntsc_v1(offs)
# pal2

def fix_offs_pal_v2(offs):
        if 0x80000000 <= offs <= 0x800cf29f:
                return offs - 0x0
        elif 0x800cf2a0 <= offs <= 0x800cf2a3:
                return offs + 0x4
        elif 0x800cf2a4 <= offs <= 0x800cf303:
                return offs + 0x8
        elif 0x800cf304 <= offs <= 0x800cf307:
                return offs + 0xC
        elif 0x800cf308 <= offs <= 0x800cf30f:
                return offs + 0x18
        elif 0x800cf320 <= offs <= 0x800cf327:
                return offs - 0x8
        elif 0x800cf328 <= offs <= 0x800cf6e7:
                return offs - 0x0
        elif 0x800cf6e8 <= offs <= 0x800cf6eb:
                return offs + 0x4
        elif 0x800cf6ec <= offs <= 0x800cf73b:
                return offs + 0x8
        elif 0x800cf73c <= offs <= 0x800cf73f:
                return offs + 0xC
        elif 0x800cf740 <= offs <= 0x800cf7e7:
                return offs + 0x10
        elif 0x800cf7e8 <= offs <= 0x800cf7eb:
                return offs + 0x14
        elif 0x800cf7ec <= offs <= 0x800cf7f3:
                return offs + 0x20
        elif 0x800cf804 <= offs <= 0x800cf80b:
                return offs - 0x0
        elif 0x800cf80c <= offs <= 0x800cf907:
                return offs + 0x8
        elif 0x800cf910 <= offs <= 0x800e0d2b:
                return offs - 0x0
        elif 0x800e0d30 <= offs <= 0x800e0d3f:
                return offs - 0x4
        elif 0x800e0d44 <= offs <= 0x800e0d4b:
                return offs - 0x8
        elif 0x800e0d4c <= offs <= 0x800e0ddb:
                return offs + 0x10
        elif 0x800e0de4 <= offs <= 0x800e0df7:
                return offs + 0x8
        elif 0x800e0dfc <= offs <= 0x800e0e0b:
                return offs + 0x4
        elif 0x800e0e10 <= offs <= 0x807683f6:
                return offs - 0x0
        elif 0x807683f7 <= offs <= 0x8076842b:
                return offs + 0xF
        elif 0x8076842c <= offs <= 0x80768462:
                return offs + 0x1E
        elif 0x80768463 <= offs <= 0x8076849b:
                return offs + 0x2D
        elif 0x8076849c <= offs <= 0x8076849f:
                return offs + 0x3C
        elif 0x807684a0 <= offs <= 0x807aaa30:
                return offs + 0x40
        elif 0x807aaa6c <= offs <= 0x809907ff:
                return offs + 0x10
        elif 0x80990800 <= offs <= 0x81000000:
                return offs + 0x20
        return offs

def unfix_offs_pal_v2(offs):
        if 0x80000000 <= offs <= 0x800cf29f:
                offs -= 0x0
        elif 0x800cf2a4 <= offs <= 0x800cf2a7:
                offs -= 0x4
        elif 0x800cf2ac <= offs <= 0x800cf30b:
                offs -= 0x8
        elif 0x800cf310 <= offs <= 0x800cf313:
                offs -= 0xC
        elif 0x800cf318 <= offs <= 0x800cf31f:
                offs += 0x8
        elif 0x800cf320 <= offs <= 0x800cf327:
                offs -= 0x18
        elif 0x800cf328 <= offs <= 0x800cf6e7:
                offs -= 0x0
        elif 0x800cf6ec <= offs <= 0x800cf6ef:
                offs -= 0x4
        elif 0x800cf6f4 <= offs <= 0x800cf743:
                offs -= 0x8
        elif 0x800cf748 <= offs <= 0x800cf74b:
                offs -= 0xC
        elif 0x800cf750 <= offs <= 0x800cf7f7:
                offs -= 0x10
        elif 0x800cf7fc <= offs <= 0x800cf7ff:
                offs -= 0x14
        elif 0x800cf804 <= offs <= 0x800cf80b:
                offs -= 0x0
        elif 0x800cf80c <= offs <= 0x800cf813:
                offs -= 0x20
        elif 0x800cf814 <= offs <= 0x800cf90f:
                offs -= 0x8
        elif 0x800cf910 <= offs <= 0x800e0d2b:
                offs -= 0x0
        elif 0x800e0d2c <= offs <= 0x800e0d3b:
                offs += 0x4
        elif 0x800e0d3c <= offs <= 0x800e0d43:
                offs += 0x8
        elif 0x800e0d5c <= offs <= 0x800e0deb:
                offs -= 0x10
        elif 0x800e0dec <= offs <= 0x800e0dff:
                offs -= 0x8
        elif 0x800e0e00 <= offs <= 0x800e0e0f:
                offs -= 0x4
        elif 0x800e0e10 <= offs <= 0x807683f6:
                offs -= 0x0
        elif 0x80768406 <= offs <= 0x8076843a:
                offs -= 0xF
        elif 0x8076844a <= offs <= 0x80768480:
                offs -= 0x1E
        elif 0x80768490 <= offs <= 0x807684c8:
                offs -= 0x2D
        elif 0x807684d8 <= offs <= 0x807684db:
                offs -= 0x3C
        elif 0x807684e0 <= offs <= 0x807aaa70:
                offs -= 0x40
        elif 0x807aaa7c <= offs <= 0x8099080f:
                offs -= 0x10
        elif 0x80990820 <= offs <= 0x81000020:
                offs -= 0x20
        return offs
# twn

def fix_offs_twn_v1(offs):
        offs = fix_offs_kor_v1(offs)
        if 0x80000000 <= offs <= 0x800b47a0:
                return offs - 0x0
        elif 0x800b47a4 <= offs <= 0x800b47ab:
                return offs + 0x4
        elif 0x800b47b0 <= offs <= 0x800e712f:
                return offs - 0x0
        elif 0x800e7130 <= offs <= 0x800e7133:
                return offs + 0xC
        elif 0x800e7134 <= offs <= 0x800e7137:
                return offs - 0x4
        elif 0x800e7138 <= offs <= 0x800e713b:
                return offs - 0x0
        elif 0x800e713c <= offs <= 0x800e714b:
                return offs + 0x4
        elif 0x800e7150 <= offs <= 0x80158c17:
                return offs - 0x0
        elif 0x80158c18 <= offs <= 0x80158dab:
                return offs + 0x4
        elif 0x80158db0 <= offs <= 0x80300647:
                return offs - 0x0
        elif 0x80302278 <= offs <= 0x803097cf:
                return offs - 0x1C30
        elif 0x803097e0 <= offs <= 0x803343af:
                return offs - 0x1C40
        elif 0x803343b8 <= offs <= 0x8033b15f:
                return offs - 0x1C48
        elif 0x8033b160 <= offs <= 0x8035e37f:
                return offs - 0x1C40
        elif 0x8035e380 <= offs <= 0x806dffff:
                return offs - 0x1C00
        elif 0x806e0000 <= offs <= 0x807a1a33:
                return offs - 0x0
        elif 0x807a1a50 <= offs <= 0x807a1a9b:
                return offs - 0x1C
        elif 0x807a1aac <= offs <= 0x807a1adf:
                return offs - 0x2C
        elif 0x807a1ae0 <= offs <= 0x807a229b:
                return offs - 0x20
        elif 0x807a22ac <= offs <= 0x807a2bc3:
                return offs - 0x30
        elif 0x807a2bd4 <= offs <= 0x807a2c13:
                return offs - 0x40
        elif 0x807a2c28 <= offs <= 0x807a2c9f:
                return offs - 0x54
        elif 0x807a2ca0 <= offs <= 0x807a846f:
                return offs - 0x50
        elif 0x807a8470 <= offs <= 0x807a8473:
                return offs - 0x48
        elif 0x807a8474 <= offs <= 0x807a8477:
                return offs - 0x54
        elif 0x807a8478 <= offs <= 0x807a857b:
                return offs - 0x4C
        elif 0x807a8580 <= offs <= 0x807abc23:
                return offs - 0x50
        elif 0x807abc24 <= offs <= 0x807abc27:
                return offs - 0x48
        elif 0x807abc28 <= offs <= 0x807abc2b:
                return offs - 0x54
        elif 0x807abc2c <= offs <= 0x807abcdb:
                return offs - 0x4C
        elif 0x807abce0 <= offs <= 0x8094d377:
                return offs - 0x50
        elif 0x8094d384 <= offs <= 0x8094d66b:
                return offs - 0x5C
        elif 0x8094d670 <= offs <= 0x8094f44b:
                return offs - 0x60
        elif 0x8094f450 <= offs <= 0x8094f4d7:
                return offs - 0x64
        elif 0x8094f4dc <= offs <= 0x8094f507:
                return offs - 0x68
        elif 0x8094f50c <= offs <= 0x8094f54f:
                return offs - 0x6C
        elif 0x8094f550 <= offs <= 0x8094f68b:
                return offs - 0x68
        elif 0x8094f69c <= offs <= 0x8095117f:
                return offs - 0x78
        elif 0x80951188 <= offs <= 0x809511d3:
                return offs - 0x80
        elif 0x809511d8 <= offs <= 0x809511ff:
                return offs - 0x84
        elif 0x80951200 <= offs <= 0x80951227:
                return offs - 0x80
        elif 0x8095122c <= offs <= 0x80951277:
                return offs - 0x84
        elif 0x80951278 <= offs <= 0x80951b6b:
                return offs - 0x80
        elif 0x80951b74 <= offs <= 0x80952683:
                return offs - 0x88
        elif 0x8095268c <= offs <= 0x809957df:
                return offs - 0x90
        elif 0x809957e0 <= offs <= 0x809957df:
                return offs - 0x80
        return offs

def unfix_offs_twn_v1(offs):
        if 0x80000000 <= offs <= 0x800b47a0:
                offs -= 0x0
        elif 0x800b47a8 <= offs <= 0x800b47af:
                offs -= 0x4
        elif 0x800b47b0 <= offs <= 0x800e712f:
                offs -= 0x0
        elif 0x800e7130 <= offs <= 0x800e7133:
                offs += 0x4
        elif 0x800e7138 <= offs <= 0x800e713b:
                offs -= 0x0
        elif 0x800e713c <= offs <= 0x800e713f:
                offs -= 0xC
        elif 0x800e7140 <= offs <= 0x800e714f:
                offs -= 0x4
        elif 0x800e7150 <= offs <= 0x80158c17:
                offs -= 0x0
        elif 0x80158c1c <= offs <= 0x80158daf:
                offs -= 0x4
        elif 0x80158db0 <= offs <= 0x80300647:
                offs -= 0x0
        elif 0x80300648 <= offs <= 0x80307b9f:
                offs += 0x1C30
        elif 0x80307ba0 <= offs <= 0x8033276f:
                offs += 0x1C40
        elif 0x80332770 <= offs <= 0x80339517:
                offs += 0x1C48
        elif 0x80339520 <= offs <= 0x8035c73f:
                offs += 0x1C40
        elif 0x8035c780 <= offs <= 0x806de3ff:
                offs += 0x1C00
        elif 0x806e0000 <= offs <= 0x807a1a33:
                offs -= 0x0
        elif 0x807a1a34 <= offs <= 0x807a1a7f:
                offs += 0x1C
        elif 0x807a1a80 <= offs <= 0x807a1ab3:
                offs += 0x2C
        elif 0x807a1ac0 <= offs <= 0x807a227b:
                offs += 0x20
        elif 0x807a227c <= offs <= 0x807a2b93:
                offs += 0x30
        elif 0x807a2b94 <= offs <= 0x807a2bd3:
                offs += 0x40
        elif 0x807a2bd4 <= offs <= 0x807a2c4b:
                offs += 0x54
        elif 0x807a2c50 <= offs <= 0x807a841f:
                offs += 0x50
        elif 0x807a8420 <= offs <= 0x807a8423:
                offs += 0x54
        elif 0x807a8428 <= offs <= 0x807a842b:
                offs += 0x48
        elif 0x807a842c <= offs <= 0x807a852f:
                offs += 0x4C
        elif 0x807a8530 <= offs <= 0x807abbd3:
                offs += 0x50
        elif 0x807abbd4 <= offs <= 0x807abbd7:
                offs += 0x54
        elif 0x807abbdc <= offs <= 0x807abbdf:
                offs += 0x48
        elif 0x807abbe0 <= offs <= 0x807abc8f:
                offs += 0x4C
        elif 0x807abc90 <= offs <= 0x8094d327:
                offs += 0x50
        elif 0x8094d328 <= offs <= 0x8094d60f:
                offs += 0x5C
        elif 0x8094d610 <= offs <= 0x8094f3eb:
                offs += 0x60
        elif 0x8094f3ec <= offs <= 0x8094f473:
                offs += 0x64
        elif 0x8094f474 <= offs <= 0x8094f49f:
                offs += 0x68
        elif 0x8094f4a0 <= offs <= 0x8094f4e3:
                offs += 0x6C
        elif 0x8094f4e8 <= offs <= 0x8094f623:
                offs += 0x68
        elif 0x8094f624 <= offs <= 0x80951107:
                offs += 0x78
        elif 0x80951108 <= offs <= 0x80951153:
                offs += 0x80
        elif 0x80951154 <= offs <= 0x8095117b:
                offs += 0x84
        elif 0x80951180 <= offs <= 0x809511a7:
                offs += 0x80
        elif 0x809511a8 <= offs <= 0x809511f3:
                offs += 0x84
        elif 0x809511f8 <= offs <= 0x80951aeb:
                offs += 0x80
        elif 0x80951aec <= offs <= 0x809525fb:
                offs += 0x88
        elif 0x809525fc <= offs <= 0x8099574f:
                offs += 0x90
        elif 0x80995760 <= offs <= 0x8099575f:
                offs += 0x80
        return unfix_offs_kor_v1(offs)


def do_mapfiles(src):
    #Yeah, no. This process takes way too long. I learned some shit from this and that's that.
    """
    assignments = {}
    for entry in todo: 
        assignments[entry] = []
        if not os.path.exists(src%entry): continue
        count = lambda y: len([x for x in assignments[entry] if x[0] == y])
        with open(src % entry) as f:
            mapfile = [x.rstrip() for x in f]
        for line in mapfile:
            if not "=0x8" in line.replace(' ', ''):
                a = line
                assignments[entry].append((a, count(a)))
                continue
                
            name = line.split('=')[0].strip()
            oldoffs = line.split('=')[1].strip()[2:10]
            unfixedoffs = unfix_from[entry](int(oldoffs, 16))
            a = (name, unfixedoffs)
            assignments[entry].append((a, count(a)))
    final_assns = []
    for entry in todo:
        for i in range(len(assignments[entry])):
            a = assignments[entry][i]
            if (type(a[0]) == str):
                if a in final_assns: continue
            else:
                if (a[0][0], a[1]) in [(x[0][0], x[1]) for x in final_assns if type(x[0]) != str]:continue
            p_index = i
            for j in reversed(range(i)):
                _prv = assignments[entry][j]
                if _prv in final_assns:
                    p_index = len(final_assns) - 1 - list(reversed(final_assns)).index(_prv)
                    break
            final_assns.insert(p_index+1, a)
        None
            
    for entry in todo:
        new = []
        for a, b in final_assns:
            if type(a) is str:
                new.append(a+'\n')
                continue
            new.append("\t%s = 0x%X;\n" % (a[0], fix_for[entry](a[1])))
        
        with open(src % entry, 'w') as f:
            f.writelines(new)
    """

    for entry in todo: 
        if entry == original: continue
        new = []
        with open(src%original) as f:
            mapfile = [x.rstrip() for x in f]

        for line in mapfile:
            pos = line.find('= 0x80')
            if pos != -1:
                oldoffs = line[pos+2:pos + 12]
                newoffs = fix_for[entry](unfix_from[original](int(oldoffs, 16)))
                line = line.replace(oldoffs, "0x%X" % newoffs)

            new.append(line + '\n')

        with open(src%entry, 'w') as f:
            f.writelines(new)



def work_on_hook(hook, name):
    error = 'Missing hook type'
    try:
        t = hook['type']

        if t == 'patch':
            error = 'Missing address'
            hook['addr_%s' % name] = fix_for[name](unfix_from[original](hook['addr_%s' % original]))

        elif t == 'branch_insn' or t == 'add_func_pointer':
            error = 'Missing source address'
            hook['src_addr_%s' % name] = fix_for[name](unfix_from[original](hook['src_addr_%s' % original]))

            if 'target_func_%s' % original in hook:
                error = 'Missing target function'
                hook['target_func_%s' % name] = fix_for[name](unfix_from[original](hook['target_func_%s' % original]))

        elif t == 'nop_insn':
            error = 'Missing area'
            area = hook['area_%s' % original]

            if isinstance(area, list):
                start = fix_for[name](unfix_from[original](area[0]))
                new_area = [start, start + (area[1] - area[0])]
            else:
                new_area = fix_for[name](unfix_from[original](area))

            hook['area_%s' % name] = new_area
        
        elif t == 'gecko_patch':
            error = 'Invalid code'
            data = hook['data_%s'%original].strip().split(" ")
            i = 0
            newData = ""
            while i < len(data):
                row = [int(x, 16) for x in data[i:i+2]]
                def writeRow():
                    nonlocal i
                    nonlocal newData
                    newData += "%08X %08X " % tuple(row)
                    i+=2
                if row[0] & 0xC2000000 == 0xC2000000:
                    row[0] = fix_for[name](unfix_from[original](row[0]^0x42000000))^0x42000000
                    for j in range(row[1]+1):
                        writeRow()
                        row = [int(x, 16) for x in data[i:i+2]]
                elif row[0] & 0x04000000 == 0x04000000:
                    row[0] = fix_for[name](unfix_from[original](row[0]^0x84000000))^0x84000000
                    writeRow()
                else:
                    writeRow()
            hook['data_%s'%name] = newData
            

    except KeyError:
        print('Key Error %s for hook: %s in %s' % (error, hook, name))


def do_module(src, dest):
    with open(src) as f:
        m = yaml.safe_load(f.read())

    for entry in todo:
        if 'hooks' in m:
            for hook in m['hooks']:
                work_on_hook(hook, entry)

    with open(dest, 'w') as f:
        f.write(yaml.dump(m))


def do_project(f, already_done):
    with open(f) as f:
        proj = yaml.safe_load(f.read())

    if 'modules' in proj:
        for m in proj['modules']:
            if m not in already_done:
                already_done.add(m)
                do_module(m.replace('processed/', ''), m)


def main():
    print(version_str)

    if not os.path.isdir('processed'):
        os.mkdir('processed')
    
    for entry in todo:
        unfix_from[entry] = globals().get('unfix_offs_{}_v{}'.format(entry.replace('2', ''), 2 if '2' in entry else 1))
        fix_for[entry] = globals().get('fix_offs_{}_v{}'.format(entry.replace('2', ''), 2 if '2' in entry else 1))

    do_mapfiles('kamek_%s.x')

    already_done = set()
    do_project('NewerProjectKP.yaml', already_done)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        main()
    
    else:
        def print_error():
            print("Usage: [address region] [address] [destination region]")
            print("Regions: pal, pal2, ntsc, ntsc2, jpn, jpn2, kor, twn")
        
        if len(sys.argv) == 4:
            if sys.argv[1] in todo and sys.argv[3] in todo:
                try:
                    addr = int(sys.argv[2], 16)
                    unfix_func = globals().get('unfix_offs_{}_v{}'.format(sys.argv[1].replace('2', ''), 2 if '2' in sys.argv[1] else 1))
                    fix_func = globals().get('fix_offs_{}_v{}'.format(sys.argv[3].replace('2', ''), 2 if '2' in sys.argv[3] else 1))
                    pal_addr = unfix_func(addr)
                    print(hex(fix_func(pal_addr)))
                    
                except: print_error()
            
            else:
                print_error()
        
        else:
            print_error()
            