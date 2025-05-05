# README

---

## 1. What does this program do

This program implements a FUSE‑based file system in which the “storage disk” is an email account.  
- File names map to email subjects.  
- File contents map to email bodies.  
- Once mounted, you can run standard commands (`mkdir`, `rmdir`, `less`, `unlink`, `ls`, `nano`, etc.) on the file system.

---

## 2. How it works (logic)

1. **Initialization**  
   - Read configuration file to extract:
     - IMAP URL
     - Port number
     - User ID
     - Password  
   - Connect to the email server using these credentials.

2. **Directory & file mapping**  
   - All files in a directory are tracked via a special “mapping mail” (an email whose subject is `mapping_mail`).  
   - The mapping mail contains the list of file names and their UIDs.

3. **Handling `ls`**  
   - `do_readdir` is called.  
   - Use IMAP LIST to get subdirectory names.  
   - Retrieve the mapping mail for the current directory to get file names.  
   - Add each name to FUSE’s buffer via the `filler()` function.  
   - Use `is_dir` / `is_file` helpers to distinguish files vs. directories.

4. **Creating & removing directories**  
   - `mkdir` → issues IMAP “create folder” via `curl`.  
   - `rmdir` → issues IMAP “delete folder” via `curl`.

5. **File creation & modification**  
   - On write/close, send an email with the file’s new content.  
   - Update the directory’s mapping mail with the file’s name and UID.

6. **Reading file contents**  
   - `less -r -f` or any read → calls `do_read`.  
   - Fetch the file’s mapping mail and extract the body.

---

## 3. How to compile & run

```bash
gcc main_code.c -o lsysfs `pkg-config fuse --cflags --libs` -lcurl

# Mount the filesystem (foreground, single‐threaded)
./lsysfs -f -s /home/keshav/Documents/cs303/assign5/mount_point_2 configuration.txt

# In another terminal, navigate into the mount point:
cd /home/keshav/Documents/cs303/assign5/mount_point_2

# If you need to unmount (e.g. after a segfault):
fusermount -u /home/keshav/Documents/cs303/assign5/mount_point_2/
```

## 4. Provide a snapshot of a sample run
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
