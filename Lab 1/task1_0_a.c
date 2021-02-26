/*
0. (a) Extend code snippet 1 to check for read and write access permissions of a given file:

 #include <stdio.h>
 #include <unistd.h>
 #include <errno.h>
 int main (int argc, char* argv[])
 {
   char* filepath = argv[1];
   int returnval;
   // Check file existence
   returnval = access (filepath, F_OK);
   if (returnval == 0)
     printf ("\n %s exists\n", filepath);
   else
   {
     if (errno == ENOENT)
       printf ("%s does not exist\n", filepath);
     else if (errno == EACCES)
       printf ("%s is not accessible\n", filepath);
return 0; }
   // Check read access ...
   // Check write access ...
   return 0;
}
*/
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char* argv[]) {

    char* filepath = argv[1];
    int returnval;

    // Check file existence
    returnval = access (filepath, F_OK);
    if (returnval == 0)
        printf ("\n %s exists\n", filepath);
    else {
        if (errno == ENOENT)
            printf ("%s does not exist\n", filepath);
        else if (errno == EACCES)
            printf ("%s is not accessible\n", filepath);
        return 0; 
    }

    // Check read access 
    returnval = access(filepath, R_OK);
    if (returnval == 0)
        printf ("\n %s has read access\n", filepath);
    else {
        printf ("\n %s doesn't have read access\n", filepath);
    }

    // Check write access 
    returnval = access(filepath, W_OK);
    if (returnval == 0)
        printf ("\n %s has write access\n", filepath);
    else {
        printf("\n %s does not have write access", filepath);
    }
    
    return 0;
}
