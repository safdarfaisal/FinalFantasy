# FinalFantasy
My first C++ school project

```
To compile
  g++ FF8.cpp
  
Useful git commands
  git clone <url>
  After making changes locally,
    git add <changed/added files>
    git commit -m "<short commit message>"
    git push origin master
  To update to latest commit on github:
    git pull origin master
    
Enscript command to generate printable postscript:

enscript -1 --fancy-header=safdar --word-wrap --line-numbers -p out.ps --highlight=cpp --mark-wrapped-lines=arrow --margins=:::80 FF8.cpp

safdar.hdr file that has to be copied into enscript lib folder is checked in.
```
