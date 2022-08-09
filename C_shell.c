#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_BUFFER_LENGTH 1024





void dir()
{

   char buf[256];
   getcwd(buf, sizeof(buf));
   printf("%s",buf);
   printf("\n");

   
}


void my_cd(char** argv)
{
chdir(argv[1]);

}


void built_in_processes(char* program_final_path, char** argv, char* path_directory)
{

    int i;
    i = sizeof(argv);
    strcpy(program_final_path, path_directory);           
    strcat(program_final_path, argv[0]);            

    for(i=0; i<strlen(program_final_path); i++){    
        if(program_final_path[i]=='\n'){      
            program_final_path[i]='\0';
        }
    }
    int pid= fork();
    
                 

    if(pid<0){
        printf("\n Forking Child Unsuccessful");
    }
     
    else if(pid==0){ 
                    
        if (execvp(program_final_path,argv) == -1 ){
        
        
        printf("Child process could not do execvp\n");
        
        }

    }else{                  
        wait(NULL);
        
    }
    

}


int exec_mult(char** argv)
{ 
   

    int pid= fork();
    
                 

    if(pid<0){
        printf("\n Forking Child Unsuccessful");
    }
     
    else if(pid==0){ 
                    
        if (execvp(argv[0],argv) == -1 ){
        
        
        printf("Child process could not do execvp\n");
        return 1;
        
        }
        else 
        {
        wait(NULL);
        return 0;
        }

    }else{                  
        wait(NULL);
        
        
    }


}


int token_parse(char** argv,int argc,char* line)
{


    char *token;                  
    token = strtok(line," ");
    int i=0;
    while(token!=NULL){
        argv[i]=token;      
        token = strtok(NULL," ");
        i++;
    }
    argv[i]=NULL;                    

    argc=i;
    
    return i;
    
    

       

}




void execute_Args_Piped(char** left_of_pipe, char** right_of_pipe)
{
 
    int pipe_fd[2]; 
    int pid1;
  
    if (pipe(pipe_fd) < 0) {
        printf("\nPipe not initialized");
        return;
    }
    pid1 = fork();
    if (pid1 < 0) {
        printf("\nforking failed");
        return;
    }
  
    if (pid1 == 0) {

        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
  
       execvp(left_of_pipe[0], left_of_pipe);
       fprintf(stderr, "Child process could not do execvp\n");
        
        }
       int pid2;
 
        pid2 = fork();
  
        if (pid2 < 0) {
            printf("\nforking failed");
            return;
        }
  

        if (pid2 == 0) {
            close(pipe_fd[1]);
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);
            execvp(right_of_pipe[0], right_of_pipe);
            fprintf(stderr, "Child process could not do execvp\n");              
            
        } 
            close(pipe_fd[0]);
            close(pipe_fd[1]);
  
            waitpid(pid1,NULL,0);
            waitpid(pid2,NULL,0);
        
    
}


void parse_Space(char* str, char** parsing_array)
{
    
    char* token;
    
    token = strtok(str," ");
    int i=0;
    while(token!=NULL){
        parsing_array[i]=token;      
        token = strtok(NULL," ");
        i++;
    }
    parsing_array[i]=NULL;                    

    
}
 

int check_pipe(char* str, char** parse_storage)
{
    int i;
    for (i = 0; i < 2; i++) {
        parse_storage[i] = strsep(&str, "|");
        
        if (parse_storage[i] == NULL)
            break;
    }
  
    if (parse_storage[1] == NULL){
    
        return 0;
        } 
    else {
    
        return 1;
    }
}




void multiple_command(char** argv,int argc)
{
   char** temp = malloc(60*sizeof(char*));
   int index_temp=0;
   int cd_flag=0;

   for(int i=0;i<argc;i++)
   {
       if (strcmp(argv[i],"cd")==0)
       {
          char* temp1;
          temp1=strdup(argv[i+1]);
          
          temp[0]=NULL;
          temp[1]=temp1;
          temp[2]=NULL;
          my_cd(temp);
          free(temp);
          cd_flag=1;
          index_temp=0;
          i=i+2;
          continue;
          
          
       
       }
   
       
       if ((strcmp(argv[i],"&&")==0) && (cd_flag==0))
       {
          
          temp[index_temp++]=NULL;
          
          int status = exec_mult(temp);
          if (status==1)
          {
          break;
          }
          free(temp);
          index_temp=0;
          continue;
          
       }
       
       char* a;
       a=strdup(argv[i]);
       temp[index_temp++]=a;
       //index_temp++;
       cd_flag=0;
   
   }
   temp[index_temp++]=NULL;
   int status = exec_mult(temp);

}



int check_multiple(char** argv,int argc)
{

   for(int i=0;i<argc;i++)
     {
     
     if (strcmp(argv[i],"&&")==0)
     {
     //printf("%d",i);
     return 1;
     }     
     }
     
  return 0;


}

void execRedirect(char** parsed,char* str)
{

    int pid= fork();
    
                 

    if(pid<0){
        printf("\n Forking Child Unsuccessful");
    }
     
    else if(pid==0){     
    
        int file = open(str, O_WRONLY | O_CREAT, 0777);
        if (file == -1)
        {
        printf("could not open");
        }
        
        int file2 = dup2(file, STDOUT_FILENO);
        close(file);
        
        
        execvp(parsed[0],parsed);
        fprintf(stderr, "Child process could not do execvp\n");

    }else{                  
        wait(NULL);
        
    }

}



int parseRedirect(char* str,char** parse_storage)
{
    int i;
    for (i = 0; i < 2; i++) {
        parse_storage[i] = strsep(&str, ">");
        
        if (parse_storage[i] == NULL)
            break;
    }
  
    if (parse_storage[1] == NULL){
       
        return 0;
        } 
    else {
       
        return 1;
    }


}


int main(){
    char line[MAX_BUFFER_LENGTH]; 
    char* argv[100];        
    char* path_directory= "/bin/";    
    char program_final_path[50];      
    int argc;    
    char* left_of_pipe[100];
    char* right_of_pipe[100];
    char* parsed[100];
    char* parse_storage[2];
    int piped_status = 0;  
    int multi_status = 0; 
    int Redirect_status = 0;
    
                  
    printf("\033[H\033[J");  
    
    while(1>0){

	     

	    printf("12041770:Yash_Teckchandani>>> ");    
	    fgets(line, MAX_BUFFER_LENGTH, stdin);
	    if (strcmp(line,"\n")==0){
	    continue;
	    }   
	    size_t length_of_input = strlen(line);
	    if (line[length_of_input - 1] == '\n'){
	    line[length_of_input - 1] = '\0';}
	    
	    if(strcmp(line, "exit")==0)
	    {            
		break;
	    }
	    
	    
	    
	    Redirect_status = parseRedirect(line,parse_storage);
	    
	    
	    
	    if(Redirect_status)
	    {
	    
	    parse_Space(parse_storage[0],parsed);
	    execRedirect(parsed,parse_storage[1]);
	    continue;
	    
	    
	    }
	    	    
	    piped_status = check_pipe(line,parse_storage);             

	    if (piped_status) {
	    
	        parse_Space(parse_storage[0],left_of_pipe);
	        parse_Space(parse_storage[1],right_of_pipe);
	        execute_Args_Piped(left_of_pipe,right_of_pipe);
	        continue;
	        
	        
	        
	        
	    }		                           
	   
	   
	    int i;
	    i = token_parse(argv,argc,line);
	    
	    
	    
	    multi_status = check_multiple(argv,i);
	    
	    if (multi_status)
	    {
	       
	       
	       multiple_command(argv,i);
	       continue;
	       
	    
	    }
	    
	    

	    
	    
	    
	    
	    if (strcmp(argv[0],"cd") == 0 ){
	     
		   my_cd(argv);
		   continue;
		                    
	    }  
	    else if (strcmp(argv[0],"pwd") == 0){
	    
		  dir();
		  continue;
	    
	    }
	    
	    else{
	    
	       built_in_processes(program_final_path,argv,path_directory);
	    
	    }
	    
	    }
	    
	    
	    
	   

	  

	
} 
