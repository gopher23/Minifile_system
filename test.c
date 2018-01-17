/*
//	Shujun Liu, 4943950
// 	Nick Khoury, 3749509
// 	Prof. Tian He
//	csci 4061
//	25 March 2015
*/


/*
	USAGE: 	$ make
		$ ./test [input_dir] [output_dir] [log_filename]

in our terminal we use this to run after cd'ind to the directory you put the A3 in:
	make
	./test input_directory out log.txt

*/

//INCLUDE
#include "mini_filesystem.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <dirent.h>
#include <sys/stat.h>

#include <unistd.h>
#include <errno.h>


#define MAX_DIR_PATH 2048
 
void findfile(char* pattern);

char *a[128];
FILE * fp6;
FILE * fp7;

int main (int argc, char* argv[]) 
{
  char *dir_path;		// Path to the directory
  struct stat dir_stat;		// Used by stat()
 
  int filesystem;
  int sz,m,l; 
  int ch;
  DIR *inputdir, *outtdir;
    FILE *fd;	
	FILE *fd1;
	FILE *fd2;
	FILE *fd4;
	FILE *fp5;
	
	int  i;
	int j;
	int k;
	mode_t perms = 0740; // Default permissions for newly created directory
						
	char pcwd[MAX_DIR_PATH + 1];	// Current working directory
	char base[MAX_DIR_PATH + 1];    //The image name
		
    inputdir = opendir ( argv[1] ); //open input directory 
    outtdir = opendir ( argv[2] );  //open output drectory

    dir_path = argv[1];   // First, save the path of current working directory 
   
   
   
   if (!getcwd (pcwd, MAX_DIR_PATH + 1))
    {
      perror ("getcwd:");
      return -1;
    }
    
   printf("pcwd, The current working direcotry is %s\n", pcwd);	 // /home/shujun/Downloads/NEWA3					   
   
  if (stat (dir_path, &dir_stat) == -1)
    {
      perror ("stat:");
      exit (1);
    }
  
  if (!S_ISDIR (dir_stat.st_mode))
    {
      fprintf (stderr, "'%s' is not a directory\n", dir_path);
      exit (1);
    }

  // Change into the given directory
  
  if (chdir (dir_path) == -1)
    {
      fprintf (stderr, "Cannot change to directory '%s': ", dir_path);
      perror ("");
      exit (1);
    }
 
  // Recursively scan the directory for the given file name pattern
     char ipath[128];
     memcpy(base, pcwd, strlen(pcwd)+1);
     memcpy(ipath, pcwd, strlen(pcwd)+1); //pcwd is /home/shujun/Downloads/NEWA3
     strcat(base, "/base.txt"); //create a file to have images base names
     strcat(ipath, "/out.txt"); // crete a file to have the images path home/shujun/Downloads/NEWA3/out.txt 
    
     printf ("the ipath is %s \n", ipath);
     printf ("the base is %s \n", base);
     fp6 = fopen(ipath, "w");
     fp7 = fopen(base, "w");
     if (fp6==NULL) {
		perror("failed to open pcwd file.\n ");
		return -1;
	}
	if (fp7==NULL) {
		perror("failed to open base file.\n ");
		return -1;
	}
      	
	 findfile(".jpg");
	 fclose(fp6);
	 fclose (fp7);
	
	
	fprintf(stdout, "\n  ___________________\n\tBEGIN TEST\n  ____________________\n");
	// Read command line arguments
    /* check for valid number of command-line arguments */ 
    
	if (argc != 4){
	fprintf(stderr, "Usage: %s processes_count output_directory input_directory\n", argv[0]);
	return -1; 
	 }
	/*Intput_directory – Check if it exists, if it doesn’t, you should exit with appropriate error message. */
    
    if ( inputdir ==  0 ) {
        fprintf (stderr, "Error in opening input directory:  %s ", argv[1] );
        perror( "argv[1]");
    }
    
    /*check output_dir if it exists, if it does, use that, if it doesn’t, create one*/
    
    if ( outtdir ==  0 ) {
     if ((mkdir (argv[2], perms)) == 0) // mkdir() returns 0 when succesful. What does it return otherwise?
        {
            printf ("New output directory created successfully.\n");
    
         }
    }
    
    if(access (argv[3], F_OK) !=-1){ //check log file
		if(remove(argv[3])==0)
		{
			printf("File deleted successfully\n"); //remove exist one	
		}	
		else
		{
			printf("Error: unable to delete the file\n");
		}	
	}
	
	 //FILE DESCRIPTOR	create new file  open log file 
	fd = fopen (argv[3], "w");
	if (!(fd = fopen (argv[3], "w"))) {
		perror("failed to open argv[3] file.\n ");
		return -1;
	}
	
	
	//ABOVE WAS CHECKING ARGV AND PUT PATH TO THE A.TXT FILE AND
	//WE CAN START READING IT	
	
	filesystem = Initialize_Filesystem(argv[3]); //"LOG_OUT.txt"
	if(filesystem !=-1)
	{
		printf("Initialize_Filesystem scuessfully\n");
	}
	else{
		fprintf(stderr, "Initialize_Filesystem: %s\n", argv[3]);
		return -1;
	}

	fp6 = fopen(ipath, "r");
	  if (fp6==NULL) {
		perror("failed to open pcwd file to read .\n ");
		return -1;
	}
	fp7 = fopen(base, "r");
	if (fp7==NULL) {
		perror("failed to open base file to read .\n ");
		return -1;
	}
	
	char line[256];
	char line_b[256];
	char out_path[256];
	
	char dup[256]; //
	char sub[256];//
	char sub_path[256];//

	char ex[128];//
	char html[256];////
	char b[128];
	char c[128];
	
	memcpy(html, pcwd, strlen(pcwd)+1);////
    strcat(html, "/");////
    strcat(html, argv[1]);////
    strcat(html, "/");////
    strcat(html, argv[2]);////
    strcat(html, "/");////
    strcat(html, "filesystem.html");////
    
    printf("!the html is: %s\n",html);////
    
	
	
	 FILE * fp8;
		
	 fp8 = fopen(html ,"w");
	 fprintf(fp8,"%s", "<html>\n<head>\n<title>Images</title>\n</head>\n<body>\n");
	 

	
	  int nodenum = 0;
      while(fgets(line, sizeof(line), fp6) && fgets(line_b, sizeof(line_b), fp7) ){
		strtok(line, "\n");
		strtok(line_b, "\n");
		printf("hahahahahahhhhhhhhhh%s\n",line);
		printf("hahahahahah%s\n",line_b);
		
		Create_File(line_b);
		Open_File(line_b);
		
		printf("lilne %s", line);
		fd4 = fopen(line,"r");
			if (fd4 == NULL) {
            puts("cannot open the line from fd4 the out txt file");
            exit(1);
          
            }
			fseek(fd4, 0L, SEEK_END);
			sz = ftell(fd4);
			fseek(fd4, 0L, SEEK_SET);
			char* A = (char*) malloc(sizeof(char)*sz);			
						
			m=0;
			while( (ch = fgetc(fd4)) != EOF )
			{    A[m] = ch;m++;
			}
	
    printf("the size of A %d\n", sz);
    
    char* B = (char*) malloc(sizeof(char)*sz);
    memcpy(out_path, pcwd, strlen(pcwd)+1);
    strcat(out_path, "/");
    strcat(out_path, argv[1]);
    strcat(out_path, "/");
    strcat(out_path, argv[2]);
    strcat(out_path, "/");
    strcat(out_path, line_b);
   // strcat(out_path, "/");
    //strcat(out_path, line_b);
  //  printf("the outpath is ~~~~~~~~~~~~` %s======\n", out_path);
    fp5 = fopen(out_path, "w");
  //  /home/shujun/Downloads/NEWA3/out/3leaves.jpg
    if (fp5 == NULL) {
      puts("Not able to open this file!!!!!!!!!");

      exit(1);
   }  
    Write_File(nodenum, 0, A, sz);
	Read_File(nodenum, 0 ,sz,B);
    k = 0;
     while(k<sz)
     {
         fputc(B[k], fp5);
         k++;
     }	   
     //free(A);            //NEW ADDED
     
     //free(B); 
        strcpy(b,"<a href=");
		strcat(b,"\"");	
	
		strcat(b,line_b);
		strcat(b,"\"");
		strcat(b,">\n");
		fprintf(fp8,"%s", b);
     
		strcpy(c,"<img src=");
		strcat(c,"\"");
		//strcat(c,"/thumbs/");
		strcat(c,dup);
		strcat(c,"_thumb.jpg");
		strcat(c,"\"");
		strcat(c,"/></a>");
		
		fprintf(fp8,"%s",c);
		
		
     	memcpy(dup,line_b, strlen(line_b)+1); //
		strtok(dup,".jpg");//
		printf("In dup is ::%s==\n",dup);//
		
		memcpy(sub, dup, strlen(dup)+1);//
		strcat(sub, "_thumb.jpg");//
		printf("the sub is : %s\n", sub);//
		
		memcpy(sub_path, pcwd, strlen(pcwd)+1);//
		strcat(sub_path, "/");//
		strcat(sub_path, argv[1]);//
		strcat(sub_path, "/");//
		strcat(sub_path, argv[2]);//
		strcat(sub_path, "/");//
		strcat(sub_path, sub);//
	
		printf("subpath is : %s\n", sub_path);//
		
		memcpy(ex, "convert ", strlen("convert ")+1);//
		strcat(ex, line);//
		strcat(ex, " -thumbnail 200x200 ");//
		strcat(ex, sub_path);	//
		system(ex);//
     Close_file(0); 
     nodenum++; 
}



	


fprintf(fp8,"%s", "</body>\n</html>\n");













}




void
findfile (char *pattern)
{
  DIR *dir;			// Pointer to the scanned directory
  struct dirent *entry;		//  Pointer to one directory entry
  char cwd[MAX_DIR_PATH + 1];	// Current working directory
  struct stat dir_stat;		// Used by stat()

  // First, save the path of current working directory 
   
  if (!getcwd (cwd, MAX_DIR_PATH + 1))
    {
      perror ("getcwd:");
      return;
    }
  
  // Open the directory to read
  
  dir = opendir (".");
  if (!dir)
    {
      fprintf (stderr, "Cannot read directory '%s': ", cwd);
      perror ("");
      return;
    }

  /*=======================================================
   * 
   * Scan the directory, traversing each sub-directory, and 
   * matching the pattern for each file / directory name. 
   *
   ========================================================*/

  while ((entry = readdir (dir)))
    {
      // Check if the pattern matches
      
      if (entry->d_name && strstr (entry->d_name, pattern))
	{
		
	   fprintf (fp6, "%s/%s\n", cwd, entry->d_name);
	   fprintf(fp7, "%s\n",entry->d_name);

	}
   
      // Check if the given entry is a directory
      
      if (stat (entry->d_name, &dir_stat) == -1)
	{
	  perror ("stat:");
	  continue;
	}
      
      // Skip the "." and ".." entries, to avoid loops
      
      if (strcmp (entry->d_name, ".") == 0)
	continue;
      
      if (strcmp (entry->d_name, "..") == 0)
	continue;
      
      /* Is this a directory? */
      if (S_ISDIR (dir_stat.st_mode))
	{
	  /* Change into the new directory */
	  if (chdir (entry->d_name) == -1)
	    {
	      fprintf (stderr, "Cannot chdir into '%s': ", entry->d_name);
	      perror ("");
	      continue;
	    }
	  
	  /* check this directory */
	  findfile (pattern);

	  /* Finally, restore the original working directory. */
	  
	  if (chdir ("..") == -1)
	    {
	      fprintf (stderr, "Cannot chdir back to '%s': ", cwd);
	      perror ("");
	      exit (1);
	    }
	}
	
    }
    
} // End findfile()






























/*
	
	
	FILE *fp1,*fp2;
 int ch;
	fp1 = fopen("/home/shujun/Downloads/NEWA3/input_directory/woodpile.jpg", "r");
   if (fp1 == NULL) {
      puts("cannot open this file");
      exit(1);
   }
	fseek(fp1, 0L, SEEK_END);
   int sz = ftell(fp1);
   fseek(fp1, 0L, SEEK_SET);
   
   char* A = (char*) malloc(sizeof(char)*sz);

   fp2 = fopen("/home/shujun/Downloads/hyland1.jpg", "w");
   if (fp2 == NULL) {
      puts("Not able to open this file");
      fclose(fp1);
      exit(1);
   }
 
     i = 0;
     while( (ch = fgetc(fp1)) != EOF )
     {    A[i] = ch;i++;
     }
     i = 0;
     while(i<sz)
     {
         fputc(A[i], fp2);
         i++;
     }
   fcloseall();	
	*/










/*
	Log_Filename = log_filename;
	Count = 0;
	Superblock.next_free_block=0;
	Superblock.next_free_inode=0;
	if(access (log_filename, F_OK) !=-1){
		if(remove(log_filename)==0)
		{
			printf("File deleted successfully"); //remove exist one	
		}	
		else
		{
			printf("Error: unable to delete the file");
		}	 						
	}	 
	//FILE DESCRIPTOR	create new file 
	fd = fopen (log_filename, "w");
	if (!(fd = fopen (log_filename, "w"))) {
	perror("failed to open file. ");
	return -1;
	}
    
    
    
  
	
	
	
	
	/*char* dest[100];
			for(i=0;i<100;i++){
				dest[i]="\0";
			}
			memcpy(dest,"\"",1);
			strcat(dest,line);
			strcat(dest,"\"");
			//fprintf("the path is %s/n",dest);
			for(i=0;i<100;i++){
				printf("%s",dest[i]);
			}
			printf("\n");*/





	/*Create_File("/home/shujun/DownloRead_File(0, 0, sz, B);ads/NEWA3/input_directory/3leaves.jpg");
	Open_File("/home/shujun/Downloads/NEWA3/input_directory/3leaves.jpg");
	fd4 = fopen("/home/shujun/Downloads/NEWA3/input_directory/3leaves.jpg", "r");
			if (fd4 == NULL) {
            puts("cannot open this file");
            exit(1);
            }
			fseek(fd4, 0L, SEEK_END);
			sz = ftell(fd4);
			fseek(fd4, 0L, SEEK_SET);
			char* A = (char*) malloc(sizeof(char)*sz);			
						
			m=0;
			while( (ch = fgetc(fd4)) != EOF )
			{    A[m] = ch;m++;
			}
	Write_File(0, 0, A);
	char* B = (char*) malloc(sizeof(char)*sz);	
	
	Close_file(0);
	
	*/
	
	

	/*
	//open a.txt file to get the path
	fd2= fopen("/home/shujun/Downloads/NEWA3/input_directory/a.txt", "r");
	if(fd2!=NULL)
	{
		char line[128];
		FILE *fp4;
	    int k =0;
		int newlen,ch,sz,m,len,n, nodenum=0;
		while(fgets(line,sizeof line,fd2)!=NULL)
		{
			/*printf("\n");
			len = strlen(line);
			newlen=len+3;
			char * dest[newlen];
			strcpy(dest,"\"");
			strcat(dest,line);
			dest[strlen(dest)]='\"';
			//strcat(dest,'\"');
		
			printf("%s",dest);
			printf("%d",strlen(dest));
			
			fputs(line, stdout);
			len = strlen(line);
			//printf("%d",len);
			char* dest = (char*) malloc(sizeof(char)*(len+3));
			strcpy(dest,"\"");
			
			strcat(dest,line);
			dest[strlen(line)]='\"';
			//strcat(dest,"\"");
			if(dest==NULL)
			{
				puts("some kind of error");
				return -1;
			}
			puts(dest);
		
			
		    /*for (n=57;n>0;n--){
		    
			printf("%s\n",dest+n);
			
		 }*/
		    //printf ("%d ",pData[n]);
			/*
			dest[0]="\"";
			
			for(n=0;n<len;n++)
			{    
				dest[n+1] =line[n];
			}
			dest[len+1]="\"";
			dest[len+2]="\0";
			
			 //printf("%s",dest);
			 //dest = dest+len+1;
			 
			 //strcpy(path,dest);
			
			//printf("The key is:%s",dest);
			
			fp4 = fopen((char*)dest, "r");
			if (fp4 == NULL) {
            puts("cannot open this file");
            exit(1);
            }
			fseek(fp4, 0L, SEEK_END);
			sz = ftell(fp4);
			fseek(fp4, 0L, SEEK_SET);
			char* A = (char*) malloc(sizeof(char)*sz);			
						
			m=0;
			while( (ch = fgetc(fp4)) != EOF )
			{    A[m] = ch;m++;
			}
						
			
	else
	{
		perror("/home/shujun/Downloads/NEWA3/input_directory/a.txt");
		return -1;
	}
    	
}*/









		//strcpy(line,'\"');
	  // printf("!!!!!!!!!!!!!!%s\n", line);
		//printf("!!!!!!!!!!!!!!%s\n", line);
      //  char z[10] = "\"";
       // memcpy(newpath,z,strlen(z)+1);
	    //memcpy(newpath,line,strlen(line)+1);
	  //  strcat(newpath,z);
		//printf("the new path is @@@@@@@@@2 %s\n", fgets(line, sizeof(line), fp6));
 
     // snprintf(somestr, sizeof line, "\"%s", line);
      //snprintf(somestr, sizeof somestr, "%s\"", somestr);
      // snprintf(line, strlen(line)+1, "\"%s\"", line);
