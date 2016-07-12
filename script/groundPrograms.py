import os
import threading
import subprocess
import traceback
from os import listdir
from os.path import isfile, join
import time

import signal

# maxTime in Seconds to run the test program
maxTime = 300
# path to the folder with the test data
dirs = ["/mnt/hgfs/Bachelor/instances/CombinedConfiguration",
        "/mnt/hgfs/Bachelor/instances/GraphColouring",
        "/mnt/hgfs/Bachelor/instances/KnightTourWithHoles",
        "/mnt/hgfs/Bachelor/instances/Labyrinth",
        "/mnt/hgfs/Bachelor/instances/StableMarriage",
        "/mnt/hgfs/Bachelor/instances/Visit-all"]

class Command(object):
    """
    Enables to run subprocess commands in a different thread with TIMEOUT option.
    Based on jcollado's solution:
    http://stackoverflow.com/questions/1191374/subprocess-with-timeout/4825933#4825933
    """
    command = None
    process = None
    status = None
    output, error = '', ''

    def __init__(self, command):
        self.command = command

    def run(self, timeout=None, **kwargs):
        """ Run a command then return: (status, output, error). """

        def target(**kwargs):
            try:
                self.process = subprocess.Popen(self.command, shell=True, preexec_fn=os.setsid, **kwargs)
                self.output, self.error = self.process.communicate()
                self.status = self.process.returncode
            except:
                self.error = traceback.format_exc()
                self.status = -1

        # default stdout and stderr
        if 'stdout' not in kwargs:
            kwargs['stdout'] = subprocess.PIPE
        if 'stderr' not in kwargs:
            kwargs['stderr'] = subprocess.PIPE
        # thread

        start = time.time()
        thread = threading.Thread(target=target, kwargs=kwargs)
        thread.start()
        thread.join(timeout)
        end = time.time()
        stop = 0
        if thread.is_alive():
            os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)
            thread.join()
            stop = 1
        return self.status, self.output, self.error, end - start, stop

for d in dirs:
    for a in listdir(d):
        f = join(d, a)
        if isfile(f) & (not isfile(d + "/ground/" + a)):
            print ("\ninstance: " + a)
            com = "gringo " + os.path.dirname(f) + "/" + os.path.basename(f) + " " + os.path.dirname(f) + "/" + "encoding.asp"
            c = Command(com)
            ret = c.run(timeout=maxTime)
            if(not ret[4]):
                print (" - ground")
                fi = open(d + "/ground/" + a, 'w')
                fi.write(str(ret[1]))
                fi.close()
            else:
                print (" - fail")