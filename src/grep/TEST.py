#!python3

from itertools import combinations_with_replacement
from random import shuffle
from os import system

falshivka = "./s21_grep"
original = "grep"

file_name = 'TEST-FILE'
file_format = ".testing"

template = '[0-9]'
test_file = './TEST-SUITE/TEST-FILE_001'
test_template = './TEST-SUITE/TEST-TEMPLATE'

suite_flag = [
    f'-e {template} -e {test_template} ./TEST-SUITE/*',
    f'-ie {template} ./TEST-SUITE/*',
    f'-ve {template} ./TEST-SUITE/*',
    f'-ce {template} ./TEST-SUITE/*',
    f'-le {template} ./TEST-SUITE/*',
    f'-ne {template} ./TEST-SUITE/*',
    f'-iv {template} ./TEST-SUITE/*',
    f'-ic {template} ./TEST-SUITE/*',
    f'-il {template} ./TEST-SUITE/*',
    f'-in {template} ./TEST-SUITE/*',
    f'-vc {template} ./TEST-SUITE/*',
    f'-vl {template} ./TEST-SUITE/*',
    f'-vn {template} ./TEST-SUITE/*',
    f'-cl {template} ./TEST-SUITE/*',
    f'-cn {template} ./TEST-SUITE/*',
    f'-cs {template} ./TEST-SUITE/*',
    f'-ln {template} ./TEST-SUITE/*',
    f'-l {template} ./TEST-SUITE/*',
    f'-v {template} ./TEST-SUITE/*',
    f'-c {template} ./TEST-SUITE/*',
    f'-n {template} ./TEST-SUITE/*',
    f'[0-9] ./TEST-SUITE/*',
    f'-i RY ./TEST-SUITE/*'
]

suite_flag1 = [
    f'-e {template} -e {test_template} {test_file}',
    f'-ie {template} {test_file}',
    f'-ve {template} {test_file}',
    f'-ce {template} {test_file}',
    f'-le {template} {test_file}',
    f'-ne {template} {test_file}',
    f'-iv {template} {test_file}',
    f'-ic {template} {test_file}',
    f'-il {template} {test_file}',
    f'-in {template} {test_file}',
    f'-vc {template} {test_file}',
    f'-vl {template} {test_file}',
    f'-vn {template} {test_file}',
    f'-cl {template} {test_file}',
    f'-cn {template} {test_file}',
    f'-cs {template} {test_file}',
    f'-ln {template} {test_file}',
    f'-l {template} {test_file}',
    f'-v {template} {test_file}',
    f'-c {template} {test_file}',
    f'-n {template} {test_file}',
    f'[0-9] {test_file}',
    f'-i RY {test_file}'
]

def file_comparison(a, b):
    with open(a) as file_1:
        with open(b) as file_2:
            a, b = file_1.read(), file_2.read()
            if a != b:

                count = 0
                for i in range(len(a)):
                    if a[i] == '\n':
                        count += 1
                    if len(b) == i:
                        break
                    if a[i] != b[i]:
                        break
                print()
                print("===============A===============")
                print(a[max(0, i-50):min(len(a), i+50)])
                print("===============B===============")
                print(b[max(0, i-50):min(len(b), i+50)])
                print("===============================")
                print("Line:", count, " char:", i)
                input("Press any key to continue:\n")

            else:
                print("\033[42m\033[1m YES \033[0m")

for index in range(len(suite_flag)):
    print(f'Current TEST [{index + 1} / {len(suite_flag)}] - {suite_flag[index]} - ', end='')
    system(f'{falshivka} {suite_flag[index]} > {file_name + "-0" + file_format}')
    system(f'{original} {suite_flag[index]} > {file_name + "-1" + file_format}')
    file_comparison(file_name+'-0'+file_format, file_name+'-1'+file_format)

for index in range(len(suite_flag1)):
    print(f'Current TEST [{index + 1} / {len(suite_flag1)}] - {suite_flag1[index]} - ', end='')
    system(f'{falshivka} {suite_flag1[index]} > {file_name + "-0" + file_format}')
    system(f'{original} {suite_flag1[index]} > {file_name + "-1" + file_format}')
    file_comparison(file_name+'-0'+file_format, file_name+'-1'+file_format)


system('rm -rf *' + file_format)
