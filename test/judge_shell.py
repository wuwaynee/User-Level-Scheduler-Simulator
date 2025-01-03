from subprocess import Popen, PIPE, run
from os.path import exists 
import sys
import os
import time

executable = './scheduler_simulator'
algo = 'FCFS'
demo_txt = './test/test_shell.txt'
prompt = ""

class Part1:
    def __init__(self):
        self.accept = [False] * 10
        self.demo = []
        with open(demo_txt, 'r') as f:
            self.demo = list(f.readlines())

    def test1(self, err):
        input = 'cat ' + demo_txt + '\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        output = result.stdout.split('\n')[:-1]
        if err:
            print(result.stdout)

        self.accept[0] = True
        for i in range(len(self.demo)):
            if not self.demo[i].strip('\n') == output[i]:
                self.accept[0] = False
                break

    def test2(self, err):
        p = Popen(executable)
        pid = p.pid + 2
        p.kill()

        input = 'cat ' + demo_txt + ' &\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)

        output = result.stdout.split('\n')
        if output[0].find(str(pid)) < 0:
            return
        
        self.accept[1] = True
        for i in range(len(self.demo)):
            if i == 0:
                if not output[1:][i].find(self.demo[i]):
                    self.accept[1] = False
                    break
                continue
            if not self.demo[i].strip('\n') == output[1:][i]:
                self.accept[1] = False
                break
        
    def test3(self, err):
        input = 'cat < ' + demo_txt + '\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = result.stdout.split('\n')[:-1]
        self.accept[2] = True
        for i in range(len(self.demo)):
            if not self.demo[i].strip('\n') == output[i]:
                self.accept[2] = False
                break

    def test4(self, err):
        input = 'cat ' + demo_txt + ' > out1\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        with open('out1', 'r') as f:
            lines = f.readlines()
            if lines == self.demo and result.stdout == prompt * 2:
                self.accept[3] = True

    def test5(self, err):
        input = 'cat < ' + demo_txt + ' > out2\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        with open('out2', 'r') as f:
            lines = f.readlines()
            if lines == self.demo and result.stdout == prompt * 2:
                self.accept[4] = True

    def test6(self, err):
        input = 'cat ' + demo_txt + ' | tail -2\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = result.stdout.split('\n')[:-1]
        self.accept[5] = True
        for i in range(len(self.demo[-2:])):
            if not self.demo[-2:][i].strip('\n') == output[i]:
                self.accept[5] = False
                break

    def test7(self, err):
        input = 'cat ' + demo_txt + ' | grep os\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = result.stdout.split('\n')[:-1]
        index = [0, 2, 3, 5]
        self.accept[6] = True
        for i in range(len(index)):
            if not self.demo[index[i]].strip('\n') == output[i]:
                self.accept[6] = False
                break

    def test8(self, err):
        input = 'cat ' + demo_txt + ' | tail -5 | head -3 | grep os\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = result.stdout.split('\n')[:-1]
        index = [2, 3]
        self.accept[7] = True
        for i in range(len(index)):
            if not self.demo[index[i]].strip('\n') == output[i]:
                self.accept[7] = False
                break
    
    def test9(self, err):
        input = 'cat < ' + demo_txt + ' | tail -5 | head -3 | grep os > out3\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        with open('out3', 'r') as f:
            lines = f.readlines()
            if lines == self.demo[2:4] and result.stdout == prompt * 2:
                self.accept[8] = True

    def test10(self, err):
        p = Popen(executable)
        pid = p.pid + 5
        p.kill()

        input = 'cat < ' + demo_txt + ' | tail -5 | head -3 | grep os > out4 &\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        time.sleep(0.1)
        with open('out4', 'r') as f:
            lines = f.readlines()
            if lines == self.demo[2:4] and result.stdout.find(str(pid)) > 0 and result.stdout.find(str(pid)) < 15:
                self.accept[9] = True

class Part2:
    def __init__(self):
        self.accept = [False] * 5
        self.pwd = os.getcwd()
        self.help_content = []

    def help(self):
        input = 'help\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        temp = result.stdout.split('\n')[:-1]
        temp[0] = temp[0][len(prompt):]
        self.help_content = temp

    def cd(self, err):
        input = 'pwd\ncd ..\npwd\ncd ' + self.pwd + '\npwd\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        
        output = result.stdout.split('\n')[:-1]
        self.accept[0] = True
        if not self.pwd == output[0] and self.pwd == output[2]:
            self.accept[0] = False
            return
        if not output[0][:output[0].rfind('/')] == output[1]:
            self.accept[0] = False
            return

    def echo(self, err):
        input = 'echo qwertyuiop\necho asdfghjkl\necho -n 123\necho -n -n -n\necho 321\necho --version\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = prompt + 'qwertyuiop\n' + \
                 prompt + 'asdfghjkl\n' + \
                 prompt + '123'+ \
                 prompt + '-n -n' + \
                 prompt + '321\n' + \
                 prompt + '--version\n' + \
                 prompt
        if result.stdout == output:
            self.accept[1] = True

    def history(self, err):
        input = 'history\nexit\n'
        result = run([executable, algo], stdout=PIPE, stderr=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)

        if ''.join(result.stdout.split(prompt)) == '':
            self.accept[2] = True

    def record(self, err):
        self.accept[3] = True

        input = 'echo qwertyuiop\necho asdfghjkl\nrecord\nreplay 2\nrecord\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = result.stdout.split('\n')
        if not output[0] == prompt + 'qwertyuiop' and output[1] == prompt + 'asdfghjkl':
            self.accept[3] = False
            return
        if not output[2].find('1') > 0 and output[2].find('echo qwertyuiop') > 0:
            self.accept[3] = False
            return
        if not output[3].find('2') > 0 and output[3].find('echo asdfghjkl') > 0:
            self.accept[3] = False
            return
        if not output[4].find('3') > 0 and output[4].find('record') > 0:
            self.accept[3] = False
            return
        if not output[5] == prompt + 'asdfghjkl':
            self.accept[3] = False
            return
        if not output[6].find('1') > 0 and output[6].find('echo qwertyuiop') > 0:
            self.accept[3] = False
            return
        if not output[7].find('2') > 0 and output[7].find('echo asdfghjkl') > 0:
            self.accept[3] = False
            return
        if not output[8].find('3') > 0 and output[8].find('record') > 0:
            self.accept[3] = False
            return
        if not output[9].find('4') > 0 and output[9].find('echo asdfghjkl') > 0:
            self.accept[3] = False
            return
        if not output[10].find('5') > 0 and output[10].find('record') > 0:
            self.accept[3] = False
            return
        
        input = 'record\nreplay 1 | head -1\nreplay 1 | head -1 > out5\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        output = result.stdout.split('\n')
        if not output[0].find('1') > 0 and output[0].find('record') > 0:
            self.accept[3] = False
            return
        if not output[1].find('1') > 0 and output[1].find('record') > 0:
            self.accept[3] = False
            return
        with open('out5', 'r') as f:
            lines = f.readlines()
            if not lines[0].find('1') > 0 and lines[0].find('record') > 0:
                self.accept[3] = False
                return

    def mypid(self, err):
        self.accept[4] = True

        p = Popen(executable)
        pid = p.pid + 1
        p.kill()

        input = 'mypid -i\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        if not result.stdout.find(str(pid)) > 0:
            self.accept[4] = False
            return

        p = Popen(executable)
        pid = p.pid + 1
        p.kill()
        ppid = os.getpid()
        input = 'mypid -i\nmypid -p ' + str(pid) + '\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        if not result.stdout.find(str(ppid)) > 0:
            self.accept[4] = False
            return

        p = Popen(executable)
        pid = p.pid + 1
        p.kill()
        ppid = os.getpid()
        input = 'mypid -c ' + str(ppid) + '\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        if not result.stdout.find(str(pid)) > 0:
            self.accept[4] = False
            return

        p = Popen(executable)
        pid = p.pid + 1
        p.kill()
        ppid = os.getpid()
        gpid = os.getppid()
        input = 'mypid -c ' + str(gpid) + '\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(result.stdout)
        if not result.stdout.find(str(ppid)) > 0:
            self.accept[4] = False
            return

        input = 'mypid -c 1\nexit\n'
        result = run([executable, algo], stdout=PIPE, 
                      input=input, encoding='ascii')
        if err:
            print(len(result.stdout))
        if not len(result.stdout) > 50:
            self.accept[4] = False
            return

def judge_all_tests(args):
    part1 = Part1()
    part1.test8(args[7])
    part1.test9(args[8])
    part1.test10(args[9])
    if part1.accept[7] and part1.accept[8] and part1.accept[9]:
        part1.accept = [True] * 10
    else:
        part1.test1(args[0])
        part1.test2(args[1])
        part1.test3(args[2])
        part1.test4(args[3])
        part1.test5(args[4])
        part1.test6(args[5])
        part1.test7(args[6])

    part2 = Part2()
    part2.help()
    part2.cd(args[10])
    part2.echo(args[11])
    part2.history(args[12])
    part2.record(args[13])
    part2.mypid(args[14])

    return part2.help_content, part1.accept, part2.accept

from termcolor import colored
accepted = colored("Accepted", 'green')
failed = colored("Failed", 'red')

def print_result(result):
    for line in result[0]:
        print(line)
    print()

    part2_command = ["cd", "echo", "history", "record and replay", "mypid"]
    for j in range(1, 3):
        print("Part {}:".format(j))
        for i, res in enumerate(result[j]):
            print("{}.{}".format(j, i+1), end=" ")
            if res:
                print(accepted, end=" ")
            else:
                print(failed, end=" ")
            if j == 2:
                print("({})".format(part2_command[i]), end=" ")
            print()
        print()

if __name__ == '__main__':
    if not exists(executable):
        print("The executable file is not existed. Please compile the source code first.")
        sys.exit(0)
    if not exists(demo_txt):
        print("The demo.txt file is not existed. Please copy that file to this folder.")
        sys.exit(0)
    
    prompt = run([executable, algo], stdout=PIPE, 
                  input='exit\n', encoding='ascii').stdout

    args = [False] * 15
    if len(sys.argv) > 2 and sys.argv[1] == "all":
        args = [True] * 15
    else:
        for i in sys.argv:
            if i == str(1.1):
                args[0] = True
            if i == str(1.2):
                args[1] = True
            if i == str(1.3):
                args[2] = True
            if i == str(1.4):
                args[3] = True
            if i == str(1.5):
                args[4] = True
            if i == str(1.6):
                args[5] = True
            if i == str(1.7):
                args[6] = True
            if i == str(1.8):
                args[7] = True
            if i == str(1.9):
                args[8] = True
            if i == str(1.10):
                args[9] = True
            if i == str(2.1):
                args[10] = True
            if i == str(2.2):
                args[11] = True
            if i == str(2.3):
                args[12] = True
            if i == str(2.4):
                args[13] = True
            if i == str(2.5):
                args[14] = True

    result = judge_all_tests(args)
    print_result(result)
    
    for i in range(5):
        if exists('out{}'.format(i+1)):
            os.remove('out{}'.format(i+1))
