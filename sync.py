__author__ = 'Batuhan Buyukguzel'
__version__ = '0.3 <27.03.2015>'

'''
You need install 'paramiko' library to run this application.
http://www.paramiko.org/installing.html

This code tested on Ubuntu 14.04 / Python 3.4.0


TODO: Don't upload the hidden files. Check '.' at the beginning of the filename
'''

import os
import paramiko
import hashlib
import time


class ssh_connection:

    def __init__(self):
        hostname = '192.168.2.233' # BeagleBone's default IP
        self.client = paramiko.SSHClient()
        self.client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.client.connect(hostname, username='root', password='')


    def run_command(self, command):
        self.command = command
        stdin, stdout, stderr = self.client.exec_command(self.command)
        return list(stdout)


    def close_connect(self):
        self.client.close()


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
#path = '/home/wdnch/Desktop/Recognizer'  # len=20
path = '/home/wdnch/workspace/'  # len=22


conn = ssh_connection()
conn.run_command('mkdir %s' % project_name)


def walkDirectory(dir):
    basedir = dir
    subdirlist = []
    hostMD5 = ''
    notFullPath = ''
    sum = conn.run_command('md5sum ' + dir[22:] + '/*')

    # This step is necessary because executing a command on
    # remote server return with '\n' at the end of the each line.
    # And this cause a problem in '(hostMD5 in sum)' control.
    # This solution is fast and better way to doing this:
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

            if not (hostMD5 in sum):
                # upload the file
                conn.put_file(cursor)
        else:
            # create new directory and cd this directory
            subdirlist.append(cursor)
            conn.run_command('mkdir ' + notFullPath)

    for subdir in subdirlist:
        walkDirectory(subdir)


def checkMD5Sum(dir):
    return hashlib.md5(open(dir, 'rb').read()).hexdigest()


while (True):
    walkDirectory(path)
    time.sleep(5)