#!/usr/bin/env python3

#
# Bin/ELF HMAC Hash Injector 
#
# 2020-2023 (c) Mikhail Paulyshka
#

import argparse
import hashlib
import hmac
import os
import sys

from elftools.elf.elffile import ELFFile

HASH_SIZE = 32


def get_filesize(fileobj):
    """
    returns size of opened file
    """

    # save position
    pos = fileobj.tell()

    # move to end and remember
    fileobj.seek(0, 2)
    result = fileobj.tell()

    # restore position
    fileobj.seek(pos)

    return result


def parse_key(keypath):
    """
    reads HMAC key from HMACKey.h file
    """

    with open(keypath, 'r') as f:
        d = f.read().replace('\n', '')
        d = d[d.find("{") + 1:d.rfind("}")]  # hash array
        d = d.replace(' ', '').replace(',', '').replace('0x', '')
        return bytearray.fromhex(d)


def calculate_hmac(filepath, key, exclude_from, exclude_to):
    """
    calculates HMAC-SHA256 from STM32 binary file omitting 32 last bytes
    """
    h = hmac.new(key, digestmod=hashlib.sha256)
    with open(filepath, 'rb') as f:
        arr = f.read()
        arr_result = b""
        if exclude_from is not None and exclude_to is not None:
            arr_result += arr[0:int(exclude_from, 16)]
            print('l1 = %s' % len(arr_result))
            arr_result += arr[int(exclude_to, 16):len(arr) - HASH_SIZE]
        else:
            arr_result += arr[:len(arr) - HASH_SIZE]

        h.update(arr_result)
        return h.digest()


def replace_padding(binpath, elfpath):
    """
    replaces padding between sections with 0x00
    """
    with open(elfpath, 'rb') as f:
        elf = ELFFile(f)

        vaddr_start = elf.get_segment(0)['p_vaddr']

        # list which contains starts and ends of the sections (in offset from the .bin file start)
        section_pairs = list()

        # get file section ranges
        for section in elf.iter_sections():
            # we are not interested in null sections
            if section.is_null():
                continue

            # we are not interested in nobits sections (which occupies zero bytes)
            if section['sh_type'] == 'SHT_NOBITS':
                continue

            # we are not interested in sections with zero size
            if section['sh_addr'] == 0:
                continue

            section_start = section['sh_addr'] - vaddr_start
            section_end = section_start + section['sh_size']

            section_pairs.append((section_start, section_end))

        section_pairs.sort(key=lambda x: x[0])

        # open file
        with open(binpath, 'r+b') as f:

            # get file size
            f.seek(0, 2)
            file_size = f.tell()

            for k in range(1, len(section_pairs)):
                # get padding start and end
                pad_start = section_pairs[k - 1][1]
                pad_end = section_pairs[k][0]

                # padding is not required if difference is zero
                if pad_end - pad_start == 0:
                    continue

                # skip very large padding
                if pad_end - pad_start >= 32:
                    continue

                # skip if padding outside the bin file
                if pad_start > file_size or pad_end > file_size:
                    continue

                f.seek(pad_start, 0)
                f.write(bytearray('\x00' * (pad_end - pad_start), encoding='ascii'))


def write_hmac_bin(filepath, fwhash):
    """
    writes HMAC hash to the latest 32 bytes of file
    """
    with open(filepath, 'r+b') as f:
        file_size = get_filesize(f)
        f.seek(file_size - HASH_SIZE)
        f.write(fwhash)


def write_hmac_elf(filepath, fwhash):
    """
    writes HMAC hash to the FLASH_HMAC_BLOCK section of ELF file
    """

    offset = 0
    with open(filepath, 'rb') as f:
        elf = ELFFile(f)
        hmac_section = elf.get_section_by_name('.FLASH_HMAC_SECTION')
        offset = hmac_section['sh_offset']

    with open(filepath, 'r+b') as f:
        f.seek(offset)
        f.write(fwhash)


#
# main
#

def main():
    parser = argparse.ArgumentParser(prog='OBC HMAC Hash Injector')
    parser.add_argument('bin_path')
    parser.add_argument('elf_path')
    parser.add_argument('hmac_path')
    parser.add_argument('-ef', '--exclude_from')
    parser.add_argument('-et', '--exclude_to')
    args = parser.parse_args()

    # check file availability
    if not os.path.exists(args.bin_path):
        print('error: bin file does not exists %s' % args.bin_path)
        sys.exit(2)
    if not os.path.exists(args.elf_path):
        print('error: elf file does not exists %s' % args.elf_path)
        sys.exit(3)
    if not os.path.exists(args.hmac_path):
        print('error: hmac file does not exists %s' % args.hmac_path)
        sys.exit(4)

    # parse key
    hmac_key = parse_key(args.hmac_path)

    # replace 0xFF padding between file sections to 0x00 padding to make .bin compatible with
    # the .ELF files flashed via STM32CubeProgrammer or SEGGER J-Flash/Ozone
    replace_padding(args.bin_path, args.elf_path)

    # calculate hash
    hmac_hash = calculate_hmac(args.bin_path, hmac_key, args.exclude_from, args.exclude_to)

    # write hash to bin file
    write_hmac_bin(args.bin_path, hmac_hash)

    # write hash to elf file
    write_hmac_elf(args.elf_path, hmac_hash)

    print(' * Done')


if __name__ == "__main__":
    main()
