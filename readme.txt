README

Keshav Krishna
2019csb1224
cs303
==========================================================

1. What does this program do
This progarms implements a fuse based file system, in which the "storage disk" is an email account.
The files in the file system are mapped to mails in the email account, such that file names are treated as subject and content of the files are treated as bidy of the mail.
The file system is mounted and then various commands like mkdir, rmdir, less, unlink, ls, nano are run on the system.

2. A description of how this program works (i.e. its logic)
The program implements functions of fuse so that the file system can work. 
From the configuration file received, the main function extracts the url, port number, user id, password and it uses these values to connect to an email server.
Files of the file system are stored in a mail with subject mapping_mail and it is assumed that mapping_mail is the latest mail for all the directories.
When the ls command is enetered, do_readdir is called and it retrieves the names of directories by using imap-list.
It obtains the names of the files using a function retrieve_mapping_mail, which uses the mapping mail of the directory to get the names of the files.
These names are added to a buffer using filler function of fuse.
Then is_dir and is_file functions are called to determine if the names in buffer are actual files or directories or not.
mkdir and rmdir command calls create folder and delet folder command using curl to make folder and delete folders respectively.
Mails are sent when new files are created or older files are changed. These mails are also accompanied by mapping mails which contains their names and their uids.
less -r -f can be used to read the content of the files, which internally calls do_read function to get the content of the files by accessing the corresponding mapping mail.

3. How to compile and run this program
gcc main_code.c -o lsysfs `pkg-config fuse --cflags --libs` -lcurl
./lsysfs -f -s /home/keshav/Documents/cs303/assign5/mount_point_2 configuration.txt
Then open another terminal:
cd /home/keshav/Documents/cs303/assign5/mount_point_2
To unmount in case of segmentation fault:
fusermount -u /home/keshav/Documents/cs303/assign5/mount_point_2/

4. Provide a snapshot of a sample run
provided in design_doc.pdf

configuration.txt:
imaps://imap.gmail.com:993/
abcdefghijkl@gmail.com
password


Terminal:
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5$ cd mount_point_2
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2$ cd INBOX
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2/INBOX$ ls
ls: cannot access 'INBOX': No such file or directory
0file.txt  file14.txt  file20.txt  file21.txt  file22.txt  INBOX  new_mail.txt
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2/INBOX$ unlink file22.txt
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2/INBOX$ ls
ls: cannot access 'INBOX': No such file or directory
0file.txt  file14.txt  file20.txt  file21.txt  INBOX  new_mail.txt
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2/INBOX$ echo hi how are you\nI am fine >>file23.txt
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2/INBOX$ less -r -f file23.txt
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2/INBOX$ cd ..
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2$ ls
dir0  INBOX  new_folder  root
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2$ mkdir dir123
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2$ ls
dir0  dir123  INBOX  new_folder  root
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2$ rmdir dir0
keshav@keshav-Lenovo-ideapad:~/Documents/cs303/assign5/mount_point_2$ ls
dir123  INBOX  new_folder  root
