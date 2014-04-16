@echo off
git log --pretty=format:"%%C(yellow)%%h %%Cred%%ad %%Cblue%%an%%Cgreen%%d %%Creset%%s" --date=iso
