import sys
import os

executePath = os.getcwd()

def finish(code):
    global executePath
    os.chdir(executePath)
    sys.exit(code)

def error(text):
    print('[ERROR] ' + text)
    finish(1)

def nativeToolsError():
    error('Make sure to run from Native Tools Command Prompt.')