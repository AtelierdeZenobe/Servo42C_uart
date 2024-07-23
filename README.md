# Servo42C_uart

## Get the code

- Create a new mbed6 program (Shared instance: https://forums.mbed.com/t/how-to-create-an-instance-of-mbed-to-be-used-as-shared-instance-later/13261), give it a name of your choice (E.g. MotorBase)
- Open a terminal (Terminal -> new terminal)  
- In the terminal, init git and add the remote origin
```
cd MotorBase
git init
git remote add origin git@github.com:AtelierdeZenobe/Servo42C_uart.git
```
- In the termininal, get the actual files from remote, and checkout the main branch
```
git pull origin main
```
Now, all files must be present under MotorBase, in their respective folders.
- Remove the automatically created MotorBase/main.cpp. Mbed should now automatically track the correct files. 

## Setup the hardware

TO UPDATE for generic use

![Schematic](./images/Servo42C_uart.png)

## Modify the code

- Create a new branch REMOTELY (easier than doing it locally and then pushing it)
https://github.com/AtelierdeZenobe/Servo42C_uart/branches => New branch.
Give it a name, for example "newBranch". Use your currently checked out branch (main) as the source branch.

- In the terminal, pull the new branch and check it out
```
git pull origin newBranch
git checkout newBranch
```

- Modify or add files. When ready to add the files, add them to be commited
```
git add <filepath>
```

- Commit the changes (multiple files can be git add before commiting)
```
git commit -m "Description of the change"
``` 

- Push the changes to remote
```
git push
```

- Congratulation, you pushed your changes to the new branch remotely. Don't worry, you didn't change any thing in the main branch.

- Never use git merge to merge to changes to the main. Instead, create a pull request from GitHub. And ask for help !