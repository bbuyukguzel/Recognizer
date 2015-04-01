__author__ = 'Batuhan Buyukguzel'
__version__ = '0.4 <01.04.2015>'

'''
You need install 'paramiko' library to run this application.
http://www.paramiko.org/installing.html

This code tested on Ubuntu 14.04 / Python 3.4.0
'''

import os
import paramiko
import hashlib
import time


class ssh_connection:
    def __init__(self):
        hostname = '192.168.2.233'  # BeagleBone's default IP
        self.client = paramiko.SSHClient()
        self.client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.client.connect(hostname, username='root', password='')

    # Execute command on remote connection which is Beaglebone in this case
    def run_command(self, command):
        self.command = command
        stdin, stdout, stderr = self.client.exec_command(self.command)
        return list(stdout)

    # Close ssh connection
    def close_connect(self):
        self.client.close()

    # Upload a file to remote connection
    def put_file(self, lname):
        ftp = self.client.open_sftp()
        rname = lname[22:]
        ftp.put(lname, rname)  # local -> remote
        print(rname + ' dosyasi yuklendi')
        ftp.close()

    # Download from remote connection feature is not using for now.
    # But keep here just in case
    def get_file(self, fname):
        ftp = self.client.open_sftp()
        ftp.get(fname, fname)  # remote -> local
        ftp.close()


project_name = 'Recognizer'
# path = '/home/wdnch/Desktop/Recognizer'  # len=20
path = '/home/wdnch/workspace/'  # len=22

conn = ssh_connection()  # Establish connection
conn.run_command('mkdir %s' % project_name)

# Holds directory names. Without this, code runs 'mkdir %s'
# command every loop which probably already exist on remote.
# I add this to gain a little performance.
local_directory_list = []
run_makefile = [False]  # Need pass by reference. Lists are mutable.


def walkDirectory(dir):
    basedir = dir
    subdirlist = []
    hostMD5 = ''
    notFullPath = ''
    sum = conn.run_command('md5sum ' + dir[22:] + '/*')

    # This step is necessary because executing a command on
    # remote server return with '\n' at the end of the each line.
    # And this cause a problem in '(hostMD5 in sum)' control.
    # This solution is fast and better way rather than to doing this:
    # any(hostMD5 in i for i in sum)
    sum = [i.replace('\n', '') for i in sum]

    for item in os.listdir(dir):
        # /home/wdnch/Desktop/Recognizer/cpp/GPIO.cpp
        cursor = os.path.join(basedir, item)

        # Recognizer/cpp/GPIO.cpp
        notFullPath = cursor[22:]

        if os.path.isfile(cursor):
            # 00bf2042e073f2723a07b443be8b7fbd  Recognizer/cpp/GPIO.cpp
            hostMD5 = checkMD5Sum(cursor) + '  ' + notFullPath

            # File modified (not same as remote)
            if not (hostMD5 in sum):
                # We should check hidden files because their hash does not
                # come with md5sum command. So, program either can upload
                # every time same hidden file in the loop (because value won't
                # exist in 'sum' or we never upload any hidden files
                if not ((notFullPath.split('/')[-1]).startswith('.')):
                    # Upload the file
                    conn.put_file(cursor)

                    # Uploading cpp file to remote means, cpp file modified
                    # and because of that, we need to compile again
                    if (notFullPath.endswith('.cpp')):
                        run_makefile[0] = True

        else:  # create new directory and cd this directory

            # Check hidden directories. I don't want to upload these folders
            # because they include lots of object (eg: .git, .metadata) and
            # this makes running slow
            if not (notFullPath.startswith('.')):
                subdirlist.append(cursor)

                # Check the file if exist local directory list.
                # If yes, then don't need to run mkdir command.
                # If not, then probably you create a folder while
                # code is running
                if not (notFullPath in local_directory_list):
                    local_directory_list.append(notFullPath)
                    conn.run_command('mkdir ' + notFullPath)

    for subdir in subdirlist:
        walkDirectory(subdir)


# Returns md5 hash of the given file
def checkMD5Sum(dir):
    return hashlib.md5(open(dir, 'rb').read()).hexdigest()


while (True):
    walkDirectory(path)

    if (run_makefile[0]):
        conn.run_command('make')
        print('Makefile calistirildi')
        run_makefile[0] = False

    time.sleep(5)