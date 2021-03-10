#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>


static int err_code;

/*
 * here are some function signatures and macros that may be helpful.
 */

void handle_error(char* fullname, char* action);
bool test_file(char* pathandname);
bool is_dir(char* pathandname);
const char* ftype_to_str(mode_t mode);
void list_file(char* pathandname, char* name, bool list_long, bool readable);
void list_dir(char* dirname, bool list_long, bool list_all, bool recursive, bool readable);

/*
 * You can use the NOT_YET_IMPLEMENTED macro to error out when you reach parts
 * of the code you have not yet finished implementing.
 */
#define NOT_YET_IMPLEMENTED(msg)                  \
    do {                                          \
        printf("Not yet implemented: " msg "\n"); \
        exit(255);                                \
    } while (0)

/*
 * PRINT_ERROR: This can be used to print the cause of an error returned by a
 * system call. It can help with debugging and reporting error causes to
 * the user. Example usage:
 *     if ( error_condition ) {
 *        PRINT_ERROR();
 *     }
 */
#define PRINT_ERROR(progname, what_happened, pathandname)               \
    do {                                                                \
        printf("%s: %s %s: %s\n", progname, what_happened, pathandname, \
               strerror(errno));                                        \
    } while (0)

/* PRINT_PERM_CHAR:
 *
 * This will be useful for -l permission printing.  It prints the given
 * 'ch' if the permission exists, or "-" otherwise.
 * Example usage:
 *     PRINT_PERM_CHAR(sb.st_mode, S_IRUSR, "r");
 */
#define PRINT_PERM_CHAR(mode, mask, ch) printf("%s", (mode & mask) ? ch : "-");
//funtion to count all files including all the subdirectories in given directory 
int countFile(char* dirname){ 
	DIR* dirp;
	struct dirent *dp;
	int count = 0;
	if((dirp = opendir(dirname))==NULL){
		return 0;
	}
	while((dp = readdir(dirp))!= NULL){
		if((strcmp(dp->d_name,".")==0) || (strcmp(dp->d_name,"..")==0)){
			continue;
		}
		
		char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      	 	snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);
		
		if(is_dir(p)){ 
			count += countFile(p); 

		}
		else{
			count ++;
		}

		free(p);

	}

	closedir(dirp);
	return count;
	

}

char* calculateSize(u_int64_t size){
	char * result = (char*) malloc(sizeof(char)*20);
	if(size < 1024){
		snprintf(result,20,"%ld",size);
	}
	else{
		snprintf(result,20,"%.1f%s",(float)size/1024,"K");
	}
	return result;
	free(result);
}
/*
 * Get username for uid. Return 1 on failure, 0 otherwise.
 */
static int uname_for_uid(uid_t uid, char* buf, size_t buflen) {
    struct passwd* p = getpwuid(uid);
    if (p == NULL) {
	//handle_error("cannot get user name","");
	err_code = err_code | 0x60;
        return 1;
    }
    strncpy(buf, p->pw_name, buflen);
    return 0;
}

/*
 * Get group name for gid. Return 1 on failure, 0 otherwise.
 */
static int group_for_gid(gid_t gid, char* buf, size_t buflen) {
    struct group* g = getgrgid(gid);
    if (g == NULL) {
	//handle_error("cannot get group name", "");
	err_code = err_code | 0x60;
        return 1;
    }
    strncpy(buf, g->gr_name, buflen);
  
    return 0;
}

/*
 * Format the supplied `struct timespec` in `ts` (e.g., from `stat.st_mtim`) as a
 * string in `char *out`. Returns the length of the formatted string (see, `man
 * 3 strftime`).
 */
static size_t date_string(struct timespec* ts, char* out, size_t len) {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    struct tm* t = localtime(&ts->tv_sec);
    if (now.tv_sec < ts->tv_sec) {
        // Future time, treat with care.
        return strftime(out, len, "%b %e %Y", t);
    } else {
        time_t difference = now.tv_sec - ts->tv_sec;
        if (difference < 31556952ull) {
            return strftime(out, len, "%b %e %H:%M", t);
        } else {
            return strftime(out, len, "%b %e %Y", t);
        }
    }
}

/*
 * Print help message and exit.
 */
static void help() {
    printf("ls: List files\n");
    printf("ls -h: readable format\n");
    printf("ls -a: List all files\n");
    printf("ls -l: List files in long listing format\n");
    printf("ls -R: List files recursively in subdirectories\n");
    printf("\t--help: Print this help\n");
    exit(0);
}

/*
 * call this when there's been an error.
 * The function should:
 * - print a suitable error message (this is already implemented)
 * - set appropriate bits in err_code
 */
void handle_error(char* what_happened, char* fullname) {
    PRINT_ERROR("ls", what_happened, fullname);

    if(errno == ENOENT){  	//file not exist
	err_code = err_code | 0x08;
    }
    else if(errno == EACCES){   //cannot access file
	err_code = err_code | 0x10;
    }
    else{		 	//other error
	err_code = err_code | 0x20;
    }
	err_code = err_code | 0x40;//default error value (if have error)
   
    return;
}

/*
 * test_file():
 * test whether stat() returns successfully and if not, handle error.
 * Use this to test for whether a file or dir exists
 */
bool test_file(char* pathandname) {
    struct stat sb;
    if (stat(pathandname, &sb)) {
       
	if(errno == ENOENT){ //file not exit
	    handle_error("No such file", pathandname);
	}
	else if(errno == EACCES){ //canno access file
	    handle_error("cannot access", pathandname);
	}
	else{ //other error
	    handle_error("Unknown error",pathandname);  
	}
        return false;
    }
    return true;
}

/*
 * is_dir(): tests whether the argument refers to a directory.
 * precondition: test_file() returns true. that is, call this function
 * only if test_file(pathandname) returned true.
 */
bool is_dir(char* pathandname) {
    struct stat sb;
    stat(pathandname, &sb);
    if (S_ISDIR(sb.st_mode)){
	return true;
    }
    
    return false;
}

/* convert the mode field in a struct stat to a file type, for -l printing */
const char* ftype_to_str(mode_t mode) {
	if(S_ISDIR(mode)){ //print the first 10 char in -l option
		printf("d");
	}
	else if(S_ISLNK(mode)){
		printf("y");
	}
	else{
		printf("-");
	}

	PRINT_PERM_CHAR(mode, S_IRUSR, "r");
	PRINT_PERM_CHAR(mode, S_IWUSR, "w");
	PRINT_PERM_CHAR(mode, S_IXUSR, "x");
 	PRINT_PERM_CHAR(mode, S_IRGRP, "r");
 	PRINT_PERM_CHAR(mode, S_IWGRP, "w");
 	PRINT_PERM_CHAR(mode, S_IXGRP, "x");
 	PRINT_PERM_CHAR(mode, S_IROTH, "r");
	PRINT_PERM_CHAR(mode, S_IWOTH, "w");
	PRINT_PERM_CHAR(mode, S_IXOTH, "x");

    return "?";
}

/* list_file():
 * implement the logic for listing a single file.
 * This function takes:
 *   - pathandname: the directory name plus the file name.
 *   - name: just the name "component".
 *   - list_long: a flag indicated whether the printout should be in
 *   long mode.
 *
 *   The reason for this signature is convenience: some of the file-outputting
 *   logic requires the full pathandname (specifically, testing for a directory
 *   so you can print a '/' and outputting in long mode), and some of it
 *   requires only the 'name' part. So we pass in both. An alternative
 *   implementation would pass in pathandname and parse out 'name'.
 */
void list_file(char* pathandname, char* name, bool list_long, bool readable) {


	if(list_long){//-l case 
 	    struct stat sb;
	    stat(pathandname,&sb);
	    char gname[20];
	    char uname[20];
	    char time[20];
	    ftype_to_str(sb.st_mode);
	    int g = group_for_gid(sb.st_gid,gname,20);
	    int u = uname_for_uid(sb.st_uid,uname,20);
	    date_string(&sb.st_mtim,time,sizeof(time));
            char *p = malloc(strlen(gname)+strlen(uname)+strlen(time)+20);
	   
	    if(readable){ //-h case 
		char *size = calculateSize(sb.st_size);
		if(u==1 && g ==1){ //check for uid and gid 
	                 printf(" %ld %d %d %ld %s %s \n",sb.st_nlink, sb.st_uid, sb.st_gid, sb.st_size,time, name);  //special print for u =1 and g=1 to keep format 
			 free(size);
		 	 free(p);
			 return;
	 	   }
	   	 else if(g==1){ 
		         snprintf(p,strlen(gname)+strlen(uname)+strlen(time)+20,"%2ld %-7s %-7d %5s %-12s ",sb.st_nlink,uname,sb.st_gid,size,time );	
	    	}
	   	 else if(u==1){
		         snprintf(p,strlen(gname)+strlen(uname)+strlen(time)+20,"%2ld %-7d %-7s %5s %-12s  ",sb.st_nlink,sb.st_uid,gname,size,time);
	    
	 	   }
	  	  else{
	  	 	 snprintf(p,strlen(gname)+strlen(uname)+strlen(time)+20,"%2ld %-7s %-7s %5s %-15s  ",sb.st_nlink,uname,gname,size,time);
	   	   }
		  free(size);
	    }
	    else{ 
	          if(u==1 && g ==1){
			 printf(" %ld %d %d %ld %s %s \n",sb.st_nlink, sb.st_uid, sb.st_gid, sb.st_size,time, name); 
		 	free(p);
		 	return;
	            }
	   	   else if(g==1){
			snprintf(p,strlen(gname)+strlen(uname)+strlen(time)+20,"%2ld %-7s %-7d %5ld %-12s ",sb.st_nlink,uname,sb.st_gid,sb.st_size,time);	
	           }
	           else if(u==1){
			snprintf(p,strlen(gname)+strlen(uname)+strlen(time)+20,"%2ld %-7d %-7s %5ld %-12s ",sb.st_nlink,sb.st_uid,gname,sb.st_size,time);
	    	   }
	   	    else{
	snprintf(p,strlen(gname)+strlen(uname)+strlen(time)+20,"%2ld %-7s %-7s %5ld %-12s ",sb.st_nlink,uname,gname,sb.st_size,time );
	   	   }

	    
	     }
	    printf("%-2s",p);   
	    	    free(p);
	}

	if(is_dir(pathandname)){
	    if((strcmp(name,".")==0)||(strcmp(name,"..")==0)){
                printf("%s",name);
	    }
	    else{
                printf("%s/", name);
	    }
	}
	else{
   		printf("%s", name);
	}


	//symlink implement (extra credit) 
   	char buf[1024]; 
	ssize_t len;
	if((len=readlink(pathandname,buf,sizeof(buf)-1))!=-1){
	 	buf[len] = '\0';
		printf(" -> %s",buf);
	    } 

	printf("\n");
	return;
}

/* list_dir():
 * implement the logic for listing a directory.
 * This function takes:
 *    - dirname: the name of the directory
 *    - list_long: should the directory be listed in long mode?
 *    - list_all: are we in "-a" mode?
 *    - recursive: are we supposed to list sub-directories?
 */
void list_dir(char* dirname, bool list_long, bool list_all, bool recursive,bool readable) {

    DIR *dirp;
    struct dirent *dp;
    if((dirp = opendir(dirname))==NULL){ //check if dir exits
	 handle_error("cannot access", dirname); 
	 return;
    }
	
    if(list_all && recursive){ //-aR case
	while((dp = readdir(dirp))!=NULL){ //loop until no more file
		char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      	  	snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);
		list_file(p,dp->d_name,list_long,readable);
		free(p);
	}
	 printf("\n");
	 rewinddir(dirp);
	 while((dp = readdir(dirp))!= NULL){
		 if((strcmp(dp->d_name,".")==0)||(strcmp(dp->d_name,"..")==0)){
			continue;
		 }
		 char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      		 snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);

		 if(is_dir(p)){
			 printf("%s:\n",p);
			 list_dir(p,list_long,list_all,recursive,readable);
		 }
		 free(p);
	 }
	closedir(dirp);

	return;
	
    }
    else if(recursive){  //-R case
	while((dp = readdir(dirp))!=NULL){
		if(dp->d_name[0] == '.'){
			continue;
		}
		char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
		snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);
		list_file(p,dp->d_name,list_long,readable);
		free(p);
	}
	 printf("\n");
	 rewinddir(dirp);
	 while((dp = readdir(dirp))!= NULL){
		 if((strcmp(dp->d_name,".")==0)||(strcmp(dp->d_name,"..")==0)){
			continue;
		 }
		 if(dp->d_name[0] =='.'){
			 continue;
		 }
		 char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      		 snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);

		 if(is_dir(p)){
			 printf("%s:\n",p);
			 list_dir(p,list_long,list_all,recursive,readable);
		 }
		 free(p);
	 }
	closedir(dirp);

	return;
    }

  // int file_count;
  // if(list_long){ // this function count the number of files and subdirectories files
  //	file_count = countFile(dirname);
  // 	printf("Total %d\n", file_count);
  // }

    while((dp = readdir(dirp))!=NULL){ //-a and default case 

	if((strcmp(dp->d_name,".")==0)||(strcmp(dp->d_name,"..")==0)){
	 	if(list_all){	
			char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      	  		snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);
		 	list_file(p,dp->d_name,list_long,readable);
			free(p);
		}	
		continue;
	}
	if(dp->d_name[0] == '.'){
		if(list_all){	
			char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      	  		snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);
		 	list_file(p,dp->d_name,list_long,readable);
			free(p);
		}	
		continue;

	}
	 char *p = malloc(strlen(dirname)+strlen(dp->d_name)+2);		
      	 snprintf(p,strlen(dirname)+strlen(dp->d_name)+2,"%s/%s",dirname,dp->d_name);
	 list_file(p,dp->d_name,list_long,readable);
	 free(p);
    }	
    closedir(dirp);
   
}



int main(int argc, char* argv[]) {
    
    int opt;
    err_code = 0x00;
    bool list_long = false, list_all = false,list_recursive = false, readable = false;// bool to check different conditions 
   
    struct option opts[] = {
        {.name = "help", .has_arg = 0, .flag = NULL, .val = '\a'}};
	//handle arguments
     while ((opt = getopt_long(argc, argv, "1alRh", opts, NULL)) != -1) {
        switch (opt) {
            case '\a':
                // Handle the case that the user passed in `--help`. (In the
                // long argument array above, we used '\a' to indicate this
                // case.)
                help();
                break;
            case '1':

                break;
            case 'a':
                list_all = true;
                break;
      
	    case 'l':
		list_long = true;
		break;

	    case 'R':
		list_recursive = true;
		break;	
	    case 'h':
		readable = true;
		break;

            default:
                printf("Unimplemented flag %d\n", opt);
                break;
        }
    }
    char *dirname = ".";
    if(argc == optind){ //without any argument
	  list_dir(dirname,list_long,list_all,list_recursive,readable);
    }
  

    for (int i = optind; i < argc; i++) { //go through arguments 
	char *name = argv[i];	
	char * p = malloc(strlen(name)+strlen(dirname)+2);

	snprintf(p,strlen(argv[i])+strlen(dirname)+2,"%s/%s",dirname,argv[i]);
	if(test_file(p)){

		if(is_dir(p)){
			if(list_recursive){
				printf("%s:\n",name);
			}
		     list_dir(name,list_long,list_all,list_recursive,readable);
		    }	
		else{
		    list_file(p,name,list_long,readable);
		}
	}
       free(p);
    }
   // printf("echo $?\n");
   // printf("%d\n",err_code); //To print out the err number
    exit(err_code);
  }
