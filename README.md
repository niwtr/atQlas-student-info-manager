# atQlas-student-info-manager
A simple yet elegant student information management software, as my C++ Programming Language course assignment.
Instruction Document is available (only in Chinese) in the **info** folder.
See video cast:
http://v.youku.com/v_show/id_XMTYyODEwMDQ4NA==.html
## Usage: 
The software comes both in Server & Client. The server can run as a single application, while Client serves as a light-weight app and may only work when Server's running.
First put **storage.x** in your $HOME folder.
To compile Server, open **AtQlas_alpha.pro** by Qt Creator in the **Server** folder, configure, and run.
To compile Client, open **AtQlas_alpha.pro** by Qt Creator in the **Client** folder, configure and run. Make sure that Server is running first.

## First Login
We provide default administrator account for first login.
* Username: admin
* Passwd: admin
You may define your own account in the **storage.x** file, follow this code:
`[user]
username:admin,
passwd:admin,
pty:ADMIN_PTY
[\user]`

## Features

**Elegant and Neat user interface**

atQlas is your fancy student information management database.
![](https://github.com/niwtr/atQlas-student-info-manager/blob/master/screenshot/1.gif)

**Runtime Instruction Compiler (Xompiler)**

Use code to finish your boring jobs.
Even loaded with error warning!
We provide **X Macro**, a built-in information manipulation language for you to arrange your student infos. In the example below, the code 
`[do]
for student that {grade:14} do add-lesson that {grade:14}
[\do]`
adds every courses that satisfies grade=2014 to all student that is in grade 2014.
![](https://github.com/niwtr/atQlas-student-info-manager/blob/master/screenshot/2.png)

**Fuzzy Search**

Built-in wildcards for better and precise searching instructions. You may even use the same instruction for **X Macro**.
![](https://github.com/niwtr/atQlas-student-info-manager/blob/master/screenshot/6.png)

**Time Machine**

Every operations & instructions you made in atQlas are compiled automagically into runtime C++ code that is stored continuously in memory, which means you can trace back to any point of time after you boot the software. Every actions you perform in **the past** can influence your future. So take care.
We even let you use X Macro in the past time!
![](https://github.com/niwtr/atQlas-student-info-manager/blob/master/screenshot/3.png)

Hack and glory awaits!
