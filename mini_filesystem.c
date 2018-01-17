//	Shujun Liu
// 	Nick Khoury, 3749509
// 	Prof. Tian He
//	csci 4061
//	25 March 2015

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <sys/time.h>
#include "mini_filesystem.h"



int Search_Directory(char* filename);
int Add_to_Directory(char * filename, int inodenum);
Inode Inode_Read(int nodenum);
int Inode_write(int nodenum, Inode ainode);
int Block_Read(int blocknum, int numbytes, char* astring);
int Block_Write(int blocknum,int length, char* astring);
int Superblock_Write(Super_block asuperblock);
Super_block Superblock_Read();



//FUNCTION HEADERS
int Initialize_Filesystem(char* log_filename);
int Create_File(char* filename);
int Open_File(char* filename);
int Read_File(int inode_number, int offset, int count, char* to_read);
int Write_File(int inode_number, int offset, char* to_write, int sz);
int Close_file(int inode_number);


int Initialize_Filesystem(char* log_filename){
	FILE *fd;
	int i;
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
		
	for(i=0;i<128;i++){
		strcpy(Directory_Structure[i].Filename,"");
		Directory_Structure[i].Inode_Number='\0';
	
		Inode_List[i].User_Id='\0';
		Inode_List[i].Group_Id='\0';
		Inode_List[i].File_Size='\0';
		Inode_List[i].Start_Block='\0';
		Inode_List[i].End_Block='\0';
		Inode_List[i].Flag='\0';
	}
	for(i=0;i<8192;i++){
		Disk_Blocks[i]=NULL;
	}
	fclose(fd);
	
	return (unsigned long)fd; 

}



int Create_File(char* filename){
	int nodenum;
	int numblock;
	//int size;
    //numblock = (int) ((size/512)+1);
    printf("In create file the num block is %d\n", numblock);
    
   if(Search_Directory(filename)!=-1){	   
		//FILENAME IS IN THE FILESYSTEM
		fprintf(stdout, "error, filename already in file system: %s\n", filename);
		return -1;   
   } 
	else{ //filename not find. then Add_to_Directory, add inode, update superblock	
		fprintf(stdout, "filename not in file system: %s\n", filename);	
		nodenum=Superblock.next_free_inode; //it's post increament, nodenum is the current open one it is 0 first tile use it 
		
		/*Inode newInode;
		newInode.Inode_Number=nodenum;
		newInode.User_Id = getuid();
		newInode.Group_Id = getgid();
		newInode.File_Size = size;
		newInode.Start_Block = Superblock.next_free_block;// at the begining the free block is 0
		newInode.End_Block = Superblock.next_free_block+numblock -1;
		
		printf("In create file the newInode.Inode_Number is %d\n", newInode.Inode_Number);
		printf("In create file the newInode User_Id is %d\n", newInode.User_Id);
		printf("In create file the newInode.Group_Id is %d\n", newInode.Group_Id );
		printf("In create file the newInode.File_Size is %d\n",newInode.File_Size);
		printf("In create file the newInode.Start_Block is %d\n", newInode.Start_Block);
		printf("In create file the newInode.End_Block is %d\n", newInode.End_Block);
		if( Inode_write(nodenum,newInode)==0){
			fprintf(stdout, "In the create file, Add a Inode, do the innode_write sucessfully: %d\n", nodenum);
		}
		else
		{
			fprintf(stdout, "Add a Inode, do the innode_write NOTTTTT sucessfully: %d\n", nodenum);
			return -1;
		}*/
		
		
		if(Add_to_Directory(filename, nodenum)==0){
			fprintf(stdout, "Add to diretory sucessfully: %s %d\n", filename,nodenum);	
			
		}
		else{
			
			fprintf(stdout, "Add to diretory NOT sucessfully: %s %d\n", filename,nodenum);
			return -1;
		}
		
		//Superblock.next_free_block= Superblock.next_free_block+numblock;
	   // Superblock.next_free_inode++;
	    printf("In create file Superblock.next_free_inode: %d\n", Superblock.next_free_inode);
		return nodenum;
		}				
}


int Open_File(char* file_n){
	 int i, found;
       for (i=0, found=0; i<128 && found == 0; i++){
               if (strcmp(Directory_Structure[i].Filename , file_n) == 0)
               {
                       found = 1; // WE ARE DONE SEARCHING
                       Inode_List[i].Flag = 1;

               } // END IF
       } // END FOR

       return 0;

}


int Read_File(int inode_number, int offset, int count, char* to_read){ //to_read is malloc  B
	int i,remaining,size,startblocknum,endblocknum,numblock;
	
	size = Inode_List[inode_number].File_Size;
	numblock = (int) (size/512)+1;         ///////newadd
	printf("IN readfile, the size of the inode is %d\n", size);
	startblocknum = Inode_List[inode_number].Start_Block;
	printf("In read file startblocknum:  %d\n", startblocknum );
	endblocknum = Inode_List[inode_number].End_Block;
	printf("In read file endblocknum:  %d\n", endblocknum );
	remaining = size%512;
	int bytewrite=0;
	int y;
	//For the given inode number, first check if the provided offset and number of bytes
	//to be read is correct by comparing it with the size of the file
	/*if(offset<0 || offset > size)
	{
		printf("Error: offset is wrong, too small, or too big");
		return -1;
	}
	else if(offset + count>size)
	{
		printf("Error: count is too big, can nor read that much, excessed the filesize");
		return -1;
	}*/
	int x;
	for(i=startblocknum;i<endblocknum;i++){		
		x = Block_Read(i,512,to_read);
		bytewrite = bytewrite+x;
		//printf("hellooooooooooo: %d\n", bytewrite);
		//printf("HHHHHHHHHHHHHH: %d\n", x);
		to_read = to_read + 512;
	}
	   y = Block_Read(endblocknum, remaining, to_read);
	   bytewrite= bytewrite+y;
	   
	printf("In read file the real read is Readbytewrite: %d\n", bytewrite);
	printf("Readsize: %d\n",size);
	//Superblock.next_free_inode++; ////newadd 
	//Superblock.next_free_block=Superblock.next_free_block+numblock-1; ///////newadd
	if(bytewrite==size){
		return bytewrite;
	}
	else{
		fprintf(stdout, "did not read file sucessfully: %d %d\n", bytewrite,size);
		return -1;		
	}
	
}



int Write_File(int inode_number, int offset, char* to_write, int sz){
	
	//asumming the offset is 0
	int i,j,k,size,numblock,newstartblock,newendblock,nodenum,bytewrite,remaining,y;
	Inode ainode;
	size = sz;
	printf("In write file the size of the input is %d\n", size);
	
	numblock = (int) ((size/512)+1);
	printf("In write file the numblock is %d\n", numblock);
	
	newstartblock = Superblock.next_free_block;        //use to be -1 !!!!!!
	printf("In write file the newstartblock is %d\n", newstartblock);
	
	newendblock =newstartblock+numblock-1; 
	printf("In write file the newendblock is %d\n", newendblock);
	
	nodenum=Superblock.next_free_inode;	
	printf("In write file the nodenum form super block is %d\n", nodenum);
	
	bytewrite=0;
	remaining = size%512;
		
	if(offset<0 ||offset >size)
	{
		printf("Error: offset is wrong, too small, or too big");
		return -1;
    }
    //Block write write the provided string to the file blocks

    for(i=newstartblock;i<newendblock;i++){
		Block_Write(i,512,to_write);
		to_write = to_write +512;
		bytewrite=bytewrite+512;
	}
	
	y=Block_Write(newendblock,remaining,to_write);
	bytewrite = bytewrite + y;
	
	printf("!!!!!!the real bytewrite is %d\n", bytewrite);
		
	printf("each loop, the bytewrite %d\n", bytewrite);
 	
	printf("ARE you out here? %d\n", i);
	
	//bytewrite=bytewrite+Block_Write(newendblock,remaining,to_write);
    
    //create a new inode	
	ainode.Inode_Number=inode_number;
	ainode.User_Id=getuid();
	ainode.Group_Id = getuid();
	ainode.File_Size = size;
	ainode.Start_Block = newstartblock;
	ainode.End_Block = newendblock;
	ainode.Flag = 1;                
	//update the inode since this changes the file size, last block etc
	if( Inode_write(nodenum,ainode)==0){
		fprintf(stdout, "In write file, Add a Inode, do the innode_write sucessfully: %d\n", nodenum);
		fprintf(stdout,"In write file,the size of the Inode %d\n", Inode_List[nodenum].File_Size);
	}
	else
	{
		fprintf(stdout, "In write file,Add a Inode, do the innode_write NOTTTTT sucessfully: %d\n", nodenum);
		return -1;
	}
	
	   //update Superblock
   Superblock.next_free_inode++;
   
   Superblock.next_free_block+=numblock;
   printf("In write file Writebytewrite: %d\n", bytewrite);
   printf("In write file writesize: %d\n",size);
   
   printf("In write file Superblock.next_free_inode: %d\n",Superblock.next_free_inode);
   printf("In write file Superblock.next_free_block: %d\n",Superblock.next_free_block);
   //printf("haaaaaaaaaaaaaaaaaaaa, %d\n",strlen(to_write));
  // printf("%s\n",to_write);
	if(bytewrite==size){
		return bytewrite;
	}
	else{
		fprintf(stdout, "In write file, Error bytewrites and szie not equal: %d %d\n", bytewrite,size);
		return -1;		
	}
	
	}
    

int Close_file(int inode_number){
	if(Inode_List[inode_number].Flag==1)
    {	
	Inode_List[inode_number].Flag=0;
    }

	return 0;
}

int Search_Directory(char* filename){
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Directory_Structure(Search_Directory)  -type:READ\n", time_in_mill);

	fclose(fd_log);
	
	/////////////////////////// DONE LOGGING     ///////////
	Count++;
	int i,ret;
	for (i = 0; i < 128 ; i++){
		ret=strcmp(Directory_Structure[i].Filename, filename);
		if(ret==0) //IF EVALUATES TO 0, THEN EQUAL STRINGS	
		{
			return Directory_Structure[i].Inode_Number;
		}
	}
	return -1; //-1 means not find
}

int Add_to_Directory(char * filename, int inodenum){
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Directory_Structure(Add_to_Directory)  -type:WRITE\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////
	Count++;
	int i;
	//for (i = 0; i < 128 &&Directory_Structure[i].Filename!='\0'; i++);	
		
	strcpy(Directory_Structure[inodenum].Filename,filename);
	Directory_Structure[inodenum].Inode_Number = inodenum;
			
	return 0;
}


Inode Inode_Read(int nodenum){
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Inode_List(Inode_Read)  -type:READ\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////
	int i,found;
	Count++;
	found = 0; //initial found , 0 not found
	for (i = 0; i < 128 &&found ==0; i++){
		if(Inode_List[i].Inode_Number == nodenum)
		{
			found=1;
		}				
	}	
	if (found == 0){
		Inode temp;
		temp.Inode_Number = -1;
    	temp.User_Id = -1;
    	temp.Group_Id = -1;
        temp.File_Size = -1;
    	temp.Start_Block = -1;
    	temp.End_Block = -1;
    	temp.Flag = -1;
		return temp;
	}
	return Inode_List[i];
}

int Inode_write(int nodenum, Inode ainode){
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Inode_List(Inode_write)  -type:WRITE\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////
	Count++;
	Inode_List[nodenum].Inode_Number = ainode.Inode_Number;
	Inode_List[nodenum].User_Id = ainode.User_Id;
	Inode_List[nodenum].Group_Id = ainode.Group_Id;
	Inode_List[nodenum].File_Size = ainode.File_Size;
	Inode_List[nodenum].Start_Block = ainode.Start_Block;
	Inode_List[nodenum].End_Block = ainode.End_Block;
	Inode_List[nodenum].Flag = ainode.Flag;
	return 0;	
	}	
	


int Block_Read(int blocknum, int numbytes, char* astring){ //astring is malloc B
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Disk_Blocks(Block_Read)  -type:READ\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////
	Count++;
	int i;
	//int length = strlen(astring);
	if(numbytes>512 || numbytes<0){
		return -1;
	}
	for(i=0;i<numbytes;i++)
	{
         //fputc(Disk_Blocks[blocknum][i], astring);
        // printf("%s",Disk_Blocks[blocknum]);
         memcpy(astring+i,Disk_Blocks[blocknum], 1);//????
         Disk_Blocks[blocknum]=Disk_Blocks[blocknum]+1;
     }	
		//printf("\n")
	return  i;//changed!!!!!!!!!
	
}

int Block_Write(int blocknum,int length, char* astring){ //a string is malloc A
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Disk_Blocks(Block_Write)  -type:WRITE\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////
	int i;
	Count++;
	Disk_Blocks[blocknum] =(char*) malloc( 512 * sizeof (char));
	                                    //return an array 
	if(length >512 || length <0)
	{
		return -1;
	}
	for(i=0;i<length;i++){		
    memcpy(Disk_Blocks[blocknum]+i, astring+i, 1);
    }
	return i;
}

Super_block Superblock_Read()
{
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Superblock(Superblock_Read)  -type:READ\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////
	Count++;
	return Superblock;
}

int Superblock_Write(Super_block asuperblock)
{
	/////////////////////////    WRITE TO LOG FILE   //////////MUST #include <sys/time.h>  !!!!!!!
	FILE* fd_log = fopen(Log_Filename, "a"); // open for append only
	if (fd_log == NULL) printf("ERROR opening log file in Search_Directory.\n");

	struct timeval  tv;
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond

	fprintf(fd_log, "-time:%f  -access:Superblock(Superblock_Write)  -type:WRITE\n", time_in_mill);

	fclose(fd_log);
	/////////////////////////// DONE LOGGING     ///////////

	Count++;
	Superblock.next_free_block=asuperblock.next_free_block;
	Superblock.next_free_inode=asuperblock.next_free_inode;
	return 0;
}












