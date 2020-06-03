// myls.c ... my very own "ls" implementation

#include <stdlib.h>
#include <stdio.h>
#include <bsd/string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXDIRNAME 100
#define MAXFNAME   200
#define MAXNAME    20

char *rwxmode(mode_t, char *);
char *username(uid_t, char *);
char *groupname(gid_t, char *);

int main(int argc, char *argv[])
{
   // string buffers for various names
   char dirname[MAXDIRNAME];
   // char uname[MAXNAME+1]; // UNCOMMENT this line
   // char gname[MAXNAME+1]; // UNCOMMENT this line
   // char mode[MAXNAME+1]; // UNCOMMENT this line

   // collect the directory name, with "." as default
   if (argc < 2)
      strlcpy(dirname, ".", MAXDIRNAME);
   else
      strlcpy(dirname, argv[1], MAXDIRNAME);

   // check that the name really is a directory
   struct stat info;
   if (stat(dirname, &info) < 0)
      { perror(argv[0]); exit(EXIT_FAILURE); }
   if ((info.st_mode & S_IFMT) != S_IFDIR)
      { fprintf(stderr, "%s: Not a directory\n",argv[0]); exit(EXIT_FAILURE); }

   // open the directory to start reading
   // DIR *df; // UNCOMMENT this line
   // ... TODO ...

   // read directory entries
   // struct dirent *entry; // UNCOMMENT this line
   // ... TODO ...

   // finish up
   // closedir(df); // UNCOMMENT this line
   return EXIT_SUCCESS;
}

// convert octal mode to -rwxrwxrwx string
char *rwxmode(mode_t mode, char *str)
{
    switch (mode & S_IFMT) {
        case S_IFDIR:  str[0]='d';                          break;
        case S_IFLNK:  str[0]='l';                          break;
        case S_IFREG:  str[0]='-';                          break;
        default:       str[0]='?';                          break;
    }
    
    str[1]='-';str[2]='-';str[3]='-';//permission for owner
    str[4]='-';str[5]='-';str[6]='-'; //permission for group
    str[7]='-';str[8]='-';str[9]='-'; //permission for others
    
    if(mode & S_IRUSR) str[1]='r';
    if(mode & S_IWUSR) str[2]='w';
    if(mode & S_IXUSR) str[3]='x';
    if(mode & S_IRGRP) str[4]='r';
    if(mode & S_IWGRP) str[5]='w';
    if(mode & S_IXGRP) str[6]='x';
    if(mode & S_IROTH) str[7]='r';
    if(mode & S_IWOTH) str[8]='w';
    if(mode & S_IXOTH) str[9]='x';
    
    str[10]='\0';
    return str;
}

// convert user id to user name
char *username(uid_t uid, char *name)
{
    struct passwd *uinfo = getpwuid(uid);
    if (uinfo == NULL){
        snprintf(name, MAXNAME, "%d?", (int)uid);
    }
    else{
        snprintf(name, MAXNAME, "%s", uinfo->pw_name);
    }
    return name;
}

// convert group id to group name
char *groupname(gid_t gid, char *name)
{
    struct group *ginfo = getgrgid(gid);
    if (ginfo == NULL){
        snprintf(name, MAXNAME, "%d?", (int)gid);
    }
    else{
        snprintf(name, MAXNAME, "%s", ginfo->gr_name);
    }
    return name;
}