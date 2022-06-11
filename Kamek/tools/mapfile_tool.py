# coding=utf-8

import os
import yaml

original = 'pal'
todo = ['pal2', 'ntsc', 'ntsc2', 'jpn', 'jpn2', 'kor', 'twn']
fix_for = {}
version_str = 'Kamek 0.3 by Treeki and RoadRunnerWMC'


def make_hex_offset(offs):
    return '0x%08X' % offs

def fix_offs_pal_v1(offs):
    return offs


def fix_offs_pal_v2(offs):
    if 0x800CF6E8 <= offs <= 0x800CF90F:
        return offs + 8

    elif 0x807685A0 <= offs <= 0x807AAA70:
        return offs + 0x40

    elif 0x807AAA74 <= offs <= 0x809907FF:
        return offs + 0x10

    elif offs >= 0x80990800:
        return offs + 0x20

    return offs


def fix_offs_ntsc_v1(offs):
    # .text section
    if 0x800B4604 <= offs <= 0x800C8E4C:
        return offs - 0x50

    elif 0x800C8E50 <= offs <= 0x800E4D70:
        return offs - 0xF0

    elif 0x800E4EC0 <= offs <= 0x8010F200:
        return offs - 0x110

    elif 0x8010F430 <= offs <= 0x802BB6BC:
        return offs - 0x140

    elif 0x802BB6C0 <= offs <= 0x802BB74C:
        return offs - 0x150

    elif 0x802BB860 <= offs <= 0x802BBBFC:
        return offs - 0x260

    elif 0x802BBC90 <= offs <= 0x802EDCC0:
        return offs - 0x2F0

    # .ctors, .dtors, .rodata, part of .data section
    elif 0x802EDCE0 <= offs <= 0x80317734:
        return offs - 0x300

    # .data section
    elif 0x80317750 <= offs <= 0x80322FE0:
        return offs - 0x318

    elif 0x80323118 <= offs <= 0x8032E77C:
        return offs - 0x348

    elif 0x8032E780 <= offs <= 0x8035197C:
        return offs - 0x340

    # .sdata section, part of .sbss
    elif 0x80351980 <= offs <= 0x80427E87:
        return offs - 0x300

    # .sbss, .sdata2, .sbss2 sections
    elif 0x80427E88 <= offs <= 0x80429563:
        return offs - 0x310

    elif 0x80429564 <= offs <= 0x80429D7F:
        return offs - 0x2F8

    elif 0x80429D80 <= offs <= 0x807684BF:
        return offs - 0x2E0

    # part of d_basesNP, d_enemiesNP, d_en_bossNP
    elif offs >= 0x8098A43C:
        return offs + 0x20

    return offs


def fix_offs_ntsc_v2(offs):
    offs = fix_offs_ntsc_v1(offs)
    if 0x800CF5F8 <= offs <= 0x800CF81F:
        return offs + 8

    elif 0x807685A0 <= offs <= 0x807AAA70:
        return offs + 0x40

    elif 0x807AAA74 <= offs <= 0x8099081C:
        return offs + 0x10

    elif offs >= 0x80990820:
        return offs + 0x20

    return offs


def fix_offs_jpn_v1(offs):
    # .text section
    if 0x800B4604 <= offs <= 0x800B475C:
        return offs - 0x50

    elif 0x800B4760 <= offs <= 0x800C8DAC:
        return offs - 0xD0

    elif 0x800C8E50 <= offs <= 0x800E4D6C:
        return offs - 0x170

    elif 0x800E4D94 <= offs <= 0x800E4EB4:
        return offs - 0x194

    elif 0x800E4EB8 <= offs <= 0x8010F1D0:
        return offs - 0x190

    elif 0x8010F430 <= offs <= 0x802BB6BC:
        return offs - 0x330

    elif 0x802BB6D0 <= offs <= 0x802BB74C:
        return offs - 0x340

    elif 0x802BB860 <= offs <= 0x802BBBFC:
        return offs - 0x450

    # end of .text, .ctors, .dtors, .rodata, part of .data section
    elif 0x802BBC90 <= offs <= 0x80317734:
        return offs - 0x4E0

    # .data section
    elif 0x80317750 <= offs <= 0x80322FDC:
        return offs - 0x4F8

    elif 0x80323118 <= offs <= 0x8035197C:
        return offs - 0x5E0

    # .sdata, part of .sbss section
    elif 0x80351980 <= offs <= 0x80427E5F:
        return offs - 0x580

    elif 0x80427E88 <= offs <= 0x8042954B:
        return offs - 0x5A8

    elif 0x80429570 <= offs <= 0x80429D7F:
        return offs - 0x5C8

    # part of .sdata2, .sbss2 section
    elif 0x80429D80 <= offs <= 0x807684BF:  # end offset is right before d_profileNP header
        return offs - 0x5C0

    # d_profileNP and d_basesNP
    # "no change" gap ends at 8779ABC
    elif 0x80779C70 <= offs <= 0x8078891F:
        return offs - 0x130

    elif 0x80788AD0 <= offs <= 0x80789EEF:
        return offs - 0x260

    elif 0x80789F00 <= offs <= 0x808D3B87:
        return offs - 0x270

    elif 0x808D3BD4 <= offs <= 0x808D3C1F:
        return offs - 0x2B4

    elif 0x808D3C20 <= offs <= 0x80940C47:
        return offs - 0x2C0

    elif 0x80940F58 <= offs <= 0x80943167:
        return offs - 0x4E8

    elif 0x809431F8 <= offs <= 0x8094329F:
        return offs - 0x4F8

    elif 0x809432C0 <= offs <= 0x80944E77:
        return offs - 0x500

    elif 0x80945144 <= offs <= 0x80945153:
        return offs - 0x714

    elif 0x80945158 <= offs <= 0x8098A36B:
        return offs - 0x718

    elif 0x8098A478 <= offs <= 0x8098FF18:
        return offs - 0x6F8

    # d_enemiesNP
    # this offset starts at the .rel header
    elif offs >= 0x80990800:
        return offs - 0x700

    return offs


def fix_offs_jpn_v2(offs):
    offs = fix_offs_jpn_v1(offs)
    if 0x800CF578 <= offs <= 0x800CF79F:
        return offs + 8

    elif 0x807685A0 <= offs <= 0x807AA7FF:
        return offs + 0x40

    elif 0x807AA800 <= offs <= 0x809900FF:
        return offs + 0x10

    elif offs >= 0x80990100:
        return offs + 0x20

    return offs


def fix_offs_kor_v1(offs):
    if 0x800122C0 <= offs <= 0x800122F4:
        return offs + 0x14
    elif 0x80012450 <= offs <= 0x80050E5F:
        return offs + 0x70
    elif 0x80050E70 <= offs <= 0x80052F24:
        return offs + 0xD0
    elif 0x80052F28 <= offs <= 0x8005C450:
        return offs + 0xE0
    elif 0x8005C458 <= offs <= 0x8005C4A8:
        return offs + 0xF8
    elif 0x8005C4AC <= offs <= 0x8005C6C0:
        return offs + 0x108
    elif 0x8005C6D0 <= offs <= 0x8005F84C:
        return offs + 0x100
    elif 0x8005F850 <= offs <= 0x8005F874:
        return offs + 0xC8
    elif 0x8005F880 <= offs <= 0x8009623F:
        return offs + 0x140
    elif 0x80096240 <= offs <= 0x800B471C:
        return offs + 0x180
    elif 0x800B4720 <= offs <= 0x800B475F:
        return offs + 0xB8
    elif 0x800B4760 <= offs <= 0x800C8DAC:
        return offs + 0xC0
    elif 0x800C8E50 <= offs <= 0x800CF6F0:
        return offs + 0x20
    elif 0x800CF6F4 <= offs <= 0x800CF904:
        return offs + 0x28
    elif 0x800CF904 <= offs <= 0x800E4D90:
        return offs + 0x20
    elif 0x800E4D94 <= offs <= 0x800E4EBF:
        return offs - 0x4
    elif 0x800F7950 <= offs <= 0x800F79BF:
        return offs + 0xC
    elif 0x800F79C0 <= offs <= 0x800FD5E8:
        return offs + 0x10
    elif 0x800FD5EC <= offs <= 0x800FD60B:
        return offs + 0x44
    elif 0x800FD610 <= offs <= 0x801011BC:
        return offs + 0x40
    elif 0x801011C0 <= offs <= 0x8010127C:
        return offs + 0xB0
    elif 0x80101280 <= offs <= 0x80106F34:
        return offs + 0x100
    elif 0x80106F38 <= offs <= 0x80106F6F:
        return offs + 0x108
    elif 0x80106F70 <= offs <= 0x80106FB8:
        return offs + 0x110
    elif 0x80106FBC <= offs <= 0x80106FE7:
        return offs + 0x128
    elif 0x80106FF0 <= offs <= 0x80107044:
        return offs + 0x120
    elif 0x801070D0 <= offs <= 0x80107108:
        return offs + 0x1F0
    elif 0x8010710C <= offs <= 0x801071F0:
        return offs + 0x210
    elif 0x801071F4 <= offs <= 0x8010727F:
        return offs + 0x21C
    elif 0x80107280 <= offs <= 0x80107FB0:
        return offs + 0x350
    elif 0x80107FB4 <= offs <= 0x8010F418:
        return offs + 0x550
    elif 0x8010F41C <= offs <= 0x80150E74:
        return offs + 0x3D0
    elif 0x80150E90 <= offs <= 0x80150EB8:
        return offs + 0x3C0
    elif 0x80150ED4 <= offs <= 0x80151410:
        return offs + 0x3B0
    elif 0x80151414 <= offs <= 0x8015145C:
        return offs + 0x3C4
    elif 0x80151460 <= offs <= 0x80152410:
        return offs + 0x3D0
    elif 0x80152414 <= offs <= 0x80152418:
        return offs + 0x3DC
    elif 0x80152450 <= offs <= 0x801590C0:
        return offs + 0x3E0
    elif 0x801590C4 <= offs <= 0x801590E8:
        return offs + 0x3EC
    elif 0x801590EC <= offs <= 0x8015912C:
        return offs + 0x3F0
    elif 0x80159130 <= offs <= 0x8015918C:
        return offs + 0x3F4
    elif 0x80159190 <= offs <= 0x802BB6BC:
        return offs + 0x400
    elif 0x802BB6D0 <= offs <= 0x802BB74F:
        return offs + 0x3F0
    elif 0x802BB860 <= offs <= 0x802BBBFF:
        return offs + 0x2E0
    elif 0x802BBC90 <= offs <= 0x802E467C:
        return offs + 0x250
    elif 0x802E4680 <= offs <= 0x802EDCC0:
        return offs + 0x348
    elif 0x802EDCE0 <= offs <= 0x802F4B23:
        return offs + 0x340
    elif 0x802F4B24 <= offs <= 0x802F4B83:
        return offs + 0x348
    elif 0x802F4B84 <= offs <= 0x802F4BE3:
        return offs + 0x350
    elif 0x802F4BE4 <= offs <= 0x802F5D42:
        return offs + 0x358
    elif 0x802F5D43 <= offs <= 0x802FD29F:
        return offs + 0xC538
    elif 0x802FD2A0 <= offs <= 0x80317734:
        return offs + 0xC540
    elif 0x80317750 <= offs <= 0x8032080F:
        return offs + 0xC528
    elif 0x80320810 <= offs <= 0x8032164C:
        return offs + 0xC570
    elif 0x80321650 <= offs <= 0x80321F14:
        return offs + 0xC578
    elif 0x80321F18 <= offs <= 0x80321F33:
        return offs + 0xC588
    elif 0x80321F38 <= offs <= 0x80321F6C:
        return offs + 0xC598
    elif 0x80321F70 <= offs <= 0x80321FE7:
        return offs + 0xC59C
    elif 0x803224F0 <= offs <= 0x80322FDC:
        return offs + 0xCAC8
    elif 0x80323118 <= offs <= 0x803279B4:
        return offs + 0xC9E0
    elif 0x803279E4 <= offs <= 0x8032E777:
        return offs + 0xC9E8
    elif 0x8032E780 <= offs <= 0x80351973:
        return offs + 0xC9E0
    elif 0x80351974 <= offs <= 0x80427E5F:
        return offs + 0xCA00
    elif 0x80427E88 <= offs <= 0x8042823F:
        return offs + 0xC9D8
    elif 0x80428244 <= offs <= 0x80428253:
        return offs + 0xC9D4
    elif 0x80428258 <= offs <= 0x80429427:
        return offs + 0xC9D0
    elif 0x80429428 <= offs <= 0x80429497:
        return offs + 0xC9D8
    elif 0x80429498 <= offs <= 0x80429547:
        return offs + 0xC9E0
    elif 0x80429570 <= offs <= 0x8042D537:
        return offs + 0xC9C0
    elif 0x8042D538 <= offs <= 0x8076859F:
        return offs + 0xC9C8
    elif 0x8076B300 <= offs <= 0x8076D45F:
        return offs + 0x8
    elif 0x8076DD40 <= offs <= 0x8076DEDF:
        return offs + 0x1C
    elif 0x8076DEE0 <= offs <= 0x80779ABC:
        return offs + 0x20
    elif 0x80779C1C <= offs <= 0x8078891C:
        return offs - 0x110
    elif 0x80788A7C <= offs <= 0x8079D44C:
        return offs - 0x240
    elif 0x8079D450 <= offs <= 0x8079D8AC:
        return offs + 0x40D0
    elif 0x8079D8B0 <= offs <= 0x8079D978:
        return offs + 0x40D4
    elif 0x8079D97C <= offs <= 0x8079D9A8:
        return offs + 0x40F0
    elif 0x8079D9AC <= offs <= 0x807AAA2F:
        return offs + 0x4100
    elif 0x807AAA6C <= offs <= 0x808173EC:
        return offs + 0x40D0
    elif 0x808173F0 <= offs <= 0x808174C8:
        return offs + 0x40F0
    elif 0x808174CC <= offs <= 0x8081751C:
        return offs + 0x40FC
    elif 0x80817520 <= offs <= 0x80826DBC:
        return offs + 0x4100
    elif 0x80826E80 <= offs <= 0x80826FF0:
        return offs + 0x4220
    elif 0x80827038 <= offs <= 0x80827214:
        return offs + 0x4230
    elif 0x80827218 <= offs <= 0x8082728F:
        return offs + 0x4238
    elif 0x80827290 <= offs <= 0x8086E97C:
        return offs + 0x4240
    elif 0x8086E980 <= offs <= 0x8086E9CB:
        return offs + 0x4254
    elif 0x8086E9D0 <= offs <= 0x8086EB68:
        return offs + 0x4250
    elif 0x8086EB80 <= offs <= 0x8086EBAC:
        return offs + 0x4274
    elif 0x8086EBB0 <= offs <= 0x808D3B84:
        return offs + 0x4280
    elif 0x808D3C20 <= offs <= 0x808EF2F7:
        return offs + 0x4230
    elif 0x808EF300 <= offs <= 0x808FB93C:
        return offs + 0x4200
    elif 0x808FB940 <= offs <= 0x808FBCDC:
        return offs + 0x4208
    elif 0x808FBCE0 <= offs <= 0x808FBCE4:
        return offs + 0x4210
    elif 0x808FBCE8 <= offs <= 0x808FBCFC:
        return offs + 0x421C
    elif 0x808FBD00 <= offs <= 0x808FBFCF:
        return offs + 0x4220
    elif 0x808FBFD0 <= offs <= 0x80902A78:
        return offs + 0x4260
    elif 0x80902A90 <= offs <= 0x80902ABF:
        return offs + 0x424C
    elif 0x80902AC0 <= offs <= 0x8090946F:
        return offs + 0x4250
    elif 0x80909500 <= offs <= 0x8091F148:
        return offs + 0x41C0
    elif 0x8091F170 <= offs <= 0x8091F244:
        return offs + 0x41D0
    elif 0x8081F248 <= offs <= 0x8091F26B:
        return offs + 0x41DC
    elif 0x8091F270 <= offs <= 0x80933C68:
        return offs + 0x41E0
    elif 0x80933C6C <= offs <= 0x809351FF:
        return offs + 0x41E8
    elif 0x80935200 <= offs <= 0x809382CC:
        return offs + 0x4300
    elif 0x809382D0 <= offs <= 0x80940CBF:
        return offs + 0x4310
    elif 0x80940F58 <= offs <= 0x80944E77:
        return offs + 0x40E8
    elif 0x80945144 <= offs <= 0x80949458:
        return offs + 0x3EE0
    elif 0x80949498 <= offs <= 0x8094977C:
        return offs + 0x3EEC
    elif 0x80949780 <= offs <= 0x8094A37F:
        return offs + 0x3EF0
    elif 0x8094A490 <= offs <= 0x8094A5E3:
        return offs + 0x50A8
    elif 0x8094A5E8 <= offs <= 0x8094A604:
        return offs + 0x50B4
    elif 0x8094A608 <= offs <= 0x8094C057:
        return offs + 0x50B8
    elif 0x8094C198 <= offs <= 0x8094CA7E:
        return offs + 0x50C0
    elif 0x8094CACC <= offs <= 0x8094D584:
        return offs + 0x50C8
    elif 0x8094D5E0 <= offs <= 0x8098A36B:
        return offs + 0x50D0
    elif 0x8098A4A0 <= offs <= 0x809942F4:
        return offs + 0x4FE0
    elif 0x809942F8 <= offs <= 0x80A0A55C:
        return offs + 0x5500
    elif 0x80A0A560 <= offs <= 0x80A0A62C:
        return offs + 0x5504
    elif 0x80A0A62C <= offs <= 0x80A0B1A0:
        return offs + 0x5520
    elif 0x80A0B1AC <= offs <= 0x80A0B4EC:
        return offs + 0x5594
    elif 0x80A0B4F0 <= offs <= 0x80A38EBC:
        return offs + 0x55A0
    elif 0x80A38ED0 <= offs <= 0x80B1141F:
        return offs + 0x55D0
    elif 0x80B11420 <= offs <= 0x80B304D8:
        return offs + 0x55E0
    elif offs >= 0x80B30550:
        return offs + 0x55F0

    return offs


def fix_offs_twn_v1(offs):
    offs = fix_offs_kor_v1(offs)

    if 0x80302278 <= offs <= 0x803097DF:
        return offs - 0x1C30
    elif 0x803097E0 <= offs <= 0x803361EC:
        return offs - 0x1C40
    elif 0x803361F0 <= offs <= 0x8033682C:
        return offs - 0x1C48
    elif 0x80336830 <= offs <= 0x8035E37C:
        return offs - 0x1C40
    elif 0x8035E380 <= offs <= 0x807684C0:
        return offs - 0x1C00
    elif 0x807A1A6C <= offs <= 0x807A1A98:
        return offs - 0x1C
    elif 0x807A1AA8 <= offs <= 0x807A1ADC:
        return offs - 0x2C
    elif 0x807A1AE0 <= offs <= 0x807A2298:
        return offs - 0x20
    elif 0x807A22AC <= offs <= 0x807A2BC0:
        return offs - 0x30
    elif 0x807A2BD4 <= offs <= 0x807A2C10:
        return offs - 0x40
    elif 0x807A2C28 <= offs <= 0x807A2C94:
        return offs - 0x54
    elif 0x807A2CA0 <= offs <= 0x8094D337:
        return offs - 0x50
    elif 0x8094D33B <= offs <= 0x8094D668:
        return offs - 0x5C
    elif 0x8094D670 <= offs <= 0x8094F420:
        return offs - 0x60
    elif 0x8094F438 <= offs <= 0x8094F68B:
        return offs - 0x68
    elif 0x8094F69C <= offs <= 0x8095110F:
        return offs - 0x78
    elif 0x80951258 <= offs <= 0x80951B3C:
        return offs - 0x80
    elif 0x80951B94 <= offs <= 0x8095264C:
        return offs - 0x88
    elif 0x809526B8 <= offs <= 0x809957DC:
        return offs - 0x90
    elif offs >= 0x809957E0:
        return offs - 0x80
    return offs


def do_mapfile(src, dest, fix_offset):
    new = []
    with open(src) as f:
        mapfile = [x.rstrip() for x in f]

    for line in mapfile:
        pos = line.find('= 0x80')
        if pos != -1:
            oldoffs = line[pos+2:pos + 12]
            newoffs = fix_offset(int(oldoffs, 16))
            line = line.replace(oldoffs, "0x%X" % newoffs)

        new.append(line + '\n')

    with open(dest, 'w') as f:
        f.writelines(new)


def work_on_hook(hook, name, func):
    error = 'Missing hook type'
    try:
        t = hook['type']

        if t == 'patch':
            error = 'Missing address'
            hook['addr_%s' % name] = func(hook['addr_%s' % original])

        elif t == 'branch_insn' or t == 'add_func_pointer':
            error = 'Missing source address'
            hook['src_addr_%s' % name] = func(hook['src_addr_%s' % original])

            if 'target_func_%s' % original in hook:
                error = 'Missing target function'
                hook['target_func_%s' % name] = func(hook['target_func_%s' % original])

        elif t == 'nop_insn':
            error = 'Missing area'
            area = hook['area_%s' % original]

            if isinstance(area, list):
                start = func(area[0])
                new_area = [start, start + (area[1] - area[0])]
            else:
                new_area = func(area)

            hook['area_%s' % name] = new_area

    except KeyError:
        print('Key Error %s in %s' % (error, name))


def do_module(src, dest):
    with open(src) as f:
        m = yaml.safe_load(f.read())

    for name, func in fix_for.items():
        if 'hooks' in m:
            for hook in m['hooks']:
                work_on_hook(hook, name, func)

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
        funcname = globals().get('fix_offs_{}_v{}'.format(entry.replace('2', ''), 2 if '2' in entry else 1))
        do_mapfile('kamek_%s.x' % original, 'kamek_%s.x' % entry, funcname)
        fix_for[entry] = funcname

    already_done = set()
    do_project('NewerProjectKP.yaml', already_done)


if __name__ == '__main__':
    main()
