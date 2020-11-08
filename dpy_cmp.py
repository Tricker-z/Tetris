import sys
import chardet
import re

def parse_dpy_file(dpy_file_path: str):
    test_depend = dict()
    f = open(dpy_file_path, 'rb')
    data = f.read()
    file_encoding = chardet.detect(data).get('encoding')
    with open(dpy_file_path, 'r', encoding = file_encoding) as f:
        for line in f.readlines():
            if line.find('->') != -1:
                # start of test file
                elements = line[:-4].split('@@')
                key = f'{elements[0]}:{elements[1]}'
                test_depend[key] = dict()
            else:
                elements = line.strip('\n').split('@@@')
                # key - class, val - (checksum, format)
                elements[0] = elements[0].replace(re.search(r'\$\$[0-9a-z]{8}', elements[0]).group(), '')
                test_depend[key][elements[0]] = (elements[1], elements[2])
    return test_depend

if __name__ == '__main__':
    argv = sys.argv

    dpy_file_1 = argv[1]
    dpy_file_2 = argv[2]

    dpy_dict_1 = parse_dpy_file(dpy_file_1)
    dpy_dict_2 = parse_dpy_file(dpy_file_2)

    depend_set_1 = dpy_dict_1.keys() - dpy_dict_2.keys()
    depend_set_2 = dpy_dict_2.keys() - dpy_dict_1.keys()

    print(f'Testfiles in {dpy_file_1} not in {dpy_file_2}:', end=' ')
    print([item for item in depend_set_1])
    print()
    
    print(f'Testfiles in {dpy_file_2} not in {dpy_file_1}:', end=' ')
    print([item for item in depend_set_2])
    print()

    print('Testfiles difference:')
    for test in dpy_dict_1.keys() & dpy_dict_2.keys():
        class_dict_1 = dpy_dict_1[test]
        class_dict_2 = dpy_dict_2[test]

        class_list_1 = list()
        class_list_2 = list()
        class_list = list()

        for citem in class_dict_1.keys():
            if citem not in class_dict_2.keys():
                class_list_1.append(citem)
            elif class_dict_1[citem][0] != class_dict_2[citem][0] or class_dict_1[citem][1] != class_dict_2[citem][1]:
                class_list.append(citem)
        
        for citem in class_dict_2.keys():
            if citem not in class_dict_1.keys():
                class_list_2.append(citem)

        if len(class_list_1) != 0 or len(class_list_2) != 0 or len(class_list) != 0:
            print(test)
            if(len(class_list_1) != 0):
                print(f'New classes in {dpy_file_1}', end=' ')
                print(class_list_1)
            if(len(class_list_2) != 0):
                print(f'New classes in {dpy_file_2}', end=' ')
                print(class_list_2)
            if(len(class_list) != 0):
                for citem in class_list:
                    print(f'{citem}: {",".join(class_dict_1[citem])} {",".join(class_dict_2[citem])}')
            print()
