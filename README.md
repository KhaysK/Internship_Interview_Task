# Internship selection | Utility

<hr>
<h3>About utility:</h3>
<ol>
<li>Using getpwnam(), getgrnam() and stat() I get data about user, group and file.</li>
<br>
<li>I compare the uid of the file and the uid of the user (whether the user is the owner) and using the S_IWUSR flag I find out if the owner has write permission.</li>
<br>
<li>I compare the gid of the file and the gid of the group (whether the group is a group of the file) and using S_IWGRP I check if it has write access.</li>
<br>
<li>I check if other users have write access using the S_IWOTH flag.</li>
<br>
<li>If access to the file is available, I check, using the POSIX macro S_ISDIR, whether the file is a directory.</li>
<ol> <li>If yes, I call the checkPermission method</li>
     <li>If not, I display the path to the file.</li> </ol>
<br>
<li>In the checkPermission method, using opendir() and readdir(), I open and read files inside a folder,
and repeat the verification procedure for each file in the folder.</li>
<br>
</ol>


<hr>

<h3>Building:</h3>

To build the project, use the following command in bash terminal:

```bash
cd <source path of Project>
gcc mycheckperm.c -o mycheckperm
```

