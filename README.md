# coauthors
## Prerequisites
- Linux with curl installed
- Windows 10
- Old Windows with curl installed
## Installation
Download the prepare-commit-msg file from the root folder, to your local .git/hooks folder of the repository you want to use the program in.
The file needs to be marked as a executable file.

Here is a direct link for that file: https://raw.githubusercontent.com/jvbsl/coauthors/master/prepare-commit-msg
You can also execute one of the below scripts, where you replace [YOUR LOCAL GIT REPOSITORY] with the location of your local repository:
### Windows
- `curl https://raw.githubusercontent.com/jvbsl/coauthors/master/prepare-commit-msg -o [YOUR LOCAL GIT REPOSITORY]/.git/hooks/prepare-commit-msg`
### Linux
- `echo "[YOUR LOCAL GIT REPOSITORY]/.git/hooks/prepare-commit-msg" | xargs -I_folder sh -c 'curl https://raw.githubusercontent.com/jvbsl/coauthors/master/prepare-commit-msg -o _folder && chmod +x _folder'`
## Usage
Create a '.coauthors' file in your local git repository root folder.
The file should look something like this:
```
+username1 <email1@gmail.com>
-username2 <email2@gmail.com>
```
Where the `+` at the beginning denotes that the user should be selected as a coauthor by default.
And `-` denotes that the user can be chosen but is not selected by default.
