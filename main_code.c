#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <curl/curl.h>

#define FROM    "<abcdefghijklabcd06@gmail.com>"
#define TO      "<abcdefghijklabcd06@gmail.com>"
#define CC      "<abcdefghijklabcd06@gmail.com>"

struct upload_status {
  size_t bytes_read;
};

int file_uid = 27;
int mapping_uid = 27;

char in_url[200];
char user_id[200];
char password_login[200];

int create_gmail_label(const char* dir_name)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This is just the server URL */
    curl_easy_setopt(curl, CURLOPT_URL, in_url);
 
    /* Set the CREATE command specifying the new folder name */
    char *create_command = malloc(sizeof(char)*100);
    strcpy(create_command, "CREATE ");
    strcat(create_command, dir_name);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, create_command);
    free(create_command);
 
    /* Perform the custom request */
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform()1 failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}

int remove_directory_mail(const char *dir_name)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This is just the server URL */
    curl_easy_setopt(curl, CURLOPT_URL, in_url);
 
    /* Set the DELETE command specifying the existing folder */
    char *del_command = malloc(sizeof(char)*100);
    strcpy(del_command, "DELETE ");
    strcat(del_command, dir_name);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, del_command);
    free(del_command);
 
    /* Perform the custom request */
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}

void remove_dir(const char *dir_name){
    remove_directory_mail(dir_name);
}

/*int check_path_directory(const char* path)
{
  printf("in function\n");
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  int status = 0;
  if(curl) {
    /* Set username and password *//*
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This will list the folders within the user's mailbox. If you want to
     * list the folders within a specific folder, for example the inbox, then
     * specify the folder as a path in the URL such as /INBOX */
    /*char* path_to_check = malloc(sizeof(char)*500);
    FILE *chunk;
    chunk = fopen("out.txt", "w");
    //strcpy(chunk, '\0');
    strcpy(path_to_check, in_url);
    strcat(path_to_check, path);
    curl_easy_setopt(curl, CURLOPT_URL, path_to_check);
    //printf("%s\n", path_to_check);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
 
    /* Perform the list */
    /*res = curl_easy_perform(curl);
 
    /* Check for errors */
    /*if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform()2 failed: %s\n%s\n",
              curl_easy_strerror(res), path_to_check);
        status = 0;
    }
    fclose(chunk);
    chunk = fopen("out.txt", "r");
    char data[500];
    fgets(data, 7, chunk);
    //printf("%s\n", data);
    if (strncmp(data, "* LIST", 7) == 0) status= 1;
    else status = 0; 
 
    /* Always cleanup */
    /*curl_easy_cleanup(curl);
    fclose(chunk);
  }
  return status;
}*/

int is_dir(const char *path){
    //pthread_mutex_lock(&lock);
    path++;
    char path2[1000];
    strcpy(path2, path);
    int len = strlen(path2);
    if (path2[len-1] == '$') path2[len-1] = '\0';
    printf("in is_dir %s\n", path2);
    CURL *curl;
    CURLcode res = CURLE_OK;
 
    curl = curl_easy_init();
    int status = 0;
    if(curl) {
      /* Set username and password */
      curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
      curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
      /* This will list the folders within the user's mailbox. If you want to
       * list the folders within a specific folder, for example the inbox, then
       * specify the folder as a path in the URL such as /INBOX */
      char* path_to_check = malloc(sizeof(char)*500);
      FILE *chunk;
      chunk = fopen("out.txt", "w");
      fputs(" ", chunk);
      //strcpy(chunk, '\0');
      strcpy(path_to_check, in_url);
      strcat(path_to_check, path2);
      curl_easy_setopt(curl, CURLOPT_URL, path_to_check);
      printf("----------------- %s\n", path_to_check);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
 
      /* Perform the list */
      res = curl_easy_perform(curl);
 
      /* Check for errors */
      if(res != CURLE_OK){
        fprintf(stderr, "curl_easy_perform()2 failed: %s\n%s\n",
                curl_easy_strerror(res), path_to_check);
          status = 0;
      }
      fclose(chunk);
      chunk = fopen("out.txt", "r");
      char data[500];
      fgets(data, 100, chunk);
      printf("\t\tdata = %s\n", data);
      if (strncmp(data, " * LIST", 7) == 0) status= 1;
      else status = 0; 
 
      /* Always cleanup */
      curl_easy_cleanup(curl);
      fclose(chunk);
    }
    printf("\t\t%s-->%d\n", path2, status);
    printf("out is_dir %s\n", path2);
    //pthread_mutex_unlock(&lock);
    return status;
}

long int findSize(char file_name[])
{
    // opening the file in read mode
    FILE* fp = fopen(file_name, "r");
  
    // checking if the file exist or not
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
  
    fseek(fp, 0L, SEEK_END);
  
    // calculating the size of the file
    long int res = ftell(fp);
  
    // closing the file
    fclose(fp);
  
    return res;
}

int create_file_send_mail(const char* path)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    long infilesize;
    FILE* upload_ctx = fopen("a.txt", "r");
 
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This will create a new message 100. Note that you should perform an
     * EXAMINE command to obtain the UID of the next message to create and a
     * SELECT to ensure you are creating the message in the OUTBOX. */
    char path2[1000];
    strcpy(path2, path);
    char* token = strtok(path2, "/");
    printf("================\nIn create mail\npath = %s\ntoken = %s\n=============\n", path, token);
    char new_url[1000];
    strcpy(new_url, in_url);
    strcat(new_url, token);
    strcat(new_url, "/");
    curl_easy_setopt(curl, CURLOPT_URL, new_url);
    //curl_easy_setopt(curl, CURLOPT_URL,"smtps://smtp.gmail.com:465/");
 
    /* In this case, we are using a callback function to specify the data. You
     * could just use the CURLOPT_READDATA option to specify a FILE pointer to
     * read from. */
    //curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    infilesize = findSize("a.txt");
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, infilesize);
 
    /* Perform the append */
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform()3 failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
    fclose(upload_ctx);
  }
 
  return (int)res;
}

void format_the_mapping_file(){
    FILE* chunk = fopen("mapping.txt", "r");
    FILE* chunk2 = fopen("mapping2.txt", "w");
    char line[2000];
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    while(fgets(line, 1000, chunk)){
        //if(strncmp(line, "*", 1) != 0) break;
        fputs(line, chunk2);
    }
    fclose(chunk);
    fclose(chunk2);
}

int retrive_mapping_mail(const char* path)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    FILE* chunk = fopen("mapping.txt", "w");
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This will fetch message 1 from the user's inbox */
    char path2[1000];
    strcpy(path2, path);
    if (strcmp(path, "\0") == 0) return 0;
    char* token = strtok(path2, "/");
    int len = strlen(token);
    if (token[len-1] == '$') token[len-1] = '\0';
    printf("================\nIn retrieve mapping mail\npath = %s\ntoken = %s\n=============\n", path, token);
    char url[500];
    char str[200];
    strcpy(url, in_url);
    strcat(url, token);
    strcat(url, "/;UID=");
    sprintf(str, "%d", mapping_uid);
    strcat(url, str);
    printf("%s\n", url);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
 
    /* Perform the fetch */
    res = curl_easy_perform(curl);

    fclose(chunk);

    //display content of mapping.txt
    printf("============= Content ==========\n");
    chunk = fopen("mapping.txt", "r");
    char getline[1000];
    for(int i=0;i<1000;i++) getline[i] = '\0';
    while(fgets(getline, 500, chunk)){
        printf("%s", getline);
    }
    printf("\n");
    fclose(chunk);
    //content over
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform()4 failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
    //fclose(chunk);

    format_the_mapping_file();
  }
 
  return (int)res;
}

int create_mapping_mail(const char *path)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    long infilesize;
    FILE* upload_ctx = fopen("mapping2.txt", "r");
 
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This will create a new message 100. Note that you should perform an
     * EXAMINE command to obtain the UID of the next message to create and a
     * SELECT to ensure you are creating the message in the OUTBOX. */
    char path2[1000];
    strcpy(path2, path);
    char* token = strtok(path2, "/");
    char new_url[1000];
    strcpy(new_url, in_url);
    strcat(new_url, token);
    strcat(new_url, "/");
    curl_easy_setopt(curl, CURLOPT_URL, new_url);
    //curl_easy_setopt(curl, CURLOPT_URL,"smtps://smtp.gmail.com:465/");
 
    /* In this case, we are using a callback function to specify the data. You
     * could just use the CURLOPT_READDATA option to specify a FILE pointer to
     * read from. */
    //curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    //printf("reached here\n");
    infilesize = findSize("mapping2.txt");
    //printf("Not here\n");
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, infilesize);
 
    /* Perform the append */
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform()5 failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
    fclose(upload_ctx);
    printf("==============Mapping mail sent================\n");
  }
 
  return (int)res;
}

void add_dir(const char *dir_name){
    //create new folder in gmail directory
    int res = create_gmail_label(dir_name);
    FILE* chunk = fopen("mapping2.txt", "w");
    fputs("TO: " TO "\r\n", chunk);
    fputs("FROM: " FROM "\r\n", chunk);
    fputs("Subject: Mapping_mail\r\n", chunk);
    //fputs("* sample.txt 1", chunk);
    fclose(chunk);
    create_mapping_mail(dir_name);
}

void add_file(const char *filename){
    FILE* chunk = fopen("a.txt", "w");
    fputs("TO: " TO "\r\n", chunk);
    fputs("FROM: " FROM "\r\n", chunk);
    fputs("Subject: ", chunk);
    char path2[1000];
    strcpy(path2, filename);
    char* token = strtok(path2, "/");
    token = strtok(NULL, "/");
    printf("====== token = %s\n", token);
    fputs(token, chunk);
    fputs("\r\n", chunk);
    fputs("\r\n", chunk);
    fclose(chunk);
    create_file_send_mail(filename);
    file_uid++;
    printf("Mail is sent\n");
    retrive_mapping_mail(filename);
    //content of mapping 2
    chunk = fopen("mapping2.txt", "r");
    char line[2000];
    while(fgets(line, 1000, chunk)){
      printf("%s", line);
    }
    fclose(chunk);
    //over sub code
    chunk = fopen("mapping2.txt", "a");
    fputs("* ", chunk);
    char temp[1000];
    strcpy(temp, filename);
    char* token23 = strtok(temp, "/");
    token23 = strtok(NULL, "/");
    if (token23 == NULL) {
        fputs(filename, chunk);
    }
    else{
        fputs(token23, chunk);
    }
    fputs(" ", chunk);
    char str[100];
    sprintf(str, "%d", file_uid);
    fputs(str, chunk);
    fputs("\n", chunk);
    fclose(chunk);
    create_mapping_mail(filename);
    file_uid++;
    printf("hehe123456-------------\n");
    mapping_uid = file_uid;
}

int is_file(const char *path){
    //retrive mail can be used to get the file
    //but uid is needed
    //create a new mail mapping file names to uids
    printf("============== is_file =============\n");
    path++;
    char path2[1000];
    strcpy(path2, path);
    printf("\t\tpath = %s\n", path);
    retrive_mapping_mail(path);
    FILE* chunk = fopen("mapping.txt", "r");
    char line[2000];
    fgets(line, 1000, chunk);
    fgets(line, 1000, chunk);
    fgets(line, 1000, chunk);
    printf("%s\n", path2);
    char* token2 = strtok(path2, "/");
    if(token2 != NULL) {printf("%s\n", token2);token2 = strtok(NULL, "/");}
    while(fgets(line, 1000, chunk)){
        printf("\t\t%s", line);
      char* token = strtok(line, " ");
      if (token != NULL) token = strtok(NULL, " ");
      
        printf("\t\ttoken = %s\npath = %s\n", token, token2);
        if (strcmp(token, token2) == 0) return 1;
        //token = strtok(NULL, " ");
    }
    fclose(chunk);
    return 0;
}

void write_to_file(const char *path, const char *new_content){
    int ch = is_file(path);
    if (ch == 0) return;
    FILE* chunk = fopen("a.txt", "w");
    fputs("TO: " TO "\r\n", chunk);
    fputs("FROM: " FROM "\r\n", chunk);
    fputs("Subject: ", chunk);
    path++;
    char path123[500];
    strcpy(path123, path);
    char *tok = strtok(path123, "/");
    tok = strtok(NULL, "/");
    printf("write to file3 ============== %s\n", tok);
    fputs(path, chunk);
    fputs("\r\n", chunk);
    fputs(new_content, chunk);
    fclose(chunk);
    create_file_send_mail(path);
    printf("hehe123-------------\n");
    file_uid++;
    retrive_mapping_mail(path);
    chunk = fopen("mapping.txt", "r");
    FILE* chunk2;
    chunk2 = fopen("mapping2.txt", "w");
    char line[2000];
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    while(fgets(line, 1000, chunk)){
      char line2[2000];
      strcpy(line2, line);
      char* token = strtok(line, " ");
      int check = 0;
      while(token != NULL) {
        if (strcmp(token, tok) == 0){
          check = 1;
          strcpy(line, "* ");
          strcat(line, tok);
          strcat(line, " ");
          char str[100];
          sprintf(str, "%d", file_uid);
          strcat(line, str);
          strcat(line, "\n");
        }
        token = strtok(NULL, " ");
      }
      if (check == 0)fputs(line2, chunk2);
      else fputs(line, chunk2);
    }
    fclose(chunk);
    fclose(chunk2);
    create_mapping_mail(path);
    file_uid++;
    printf("hehe1234-------------\n");
    mapping_uid = file_uid;
}

void remove_file(const char *path){
    retrive_mapping_mail(path);
    char path123[500];
    strcpy(path123, path);
    char *tok = strtok(path123, "/");
    tok = strtok(NULL, "/");
    FILE *chunk = fopen("mapping.txt", "r");
    FILE *chunk2 = fopen("mapping2.txt", "w");
    char line[2000];
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    fgets(line, 1000, chunk);
    fputs(line, chunk2);
    while(fgets(line, 1000, chunk)){
        char line2[2000];
        strcpy(line2, line);
        char *token = strtok(line, " ");
        int check = 0;
        while(token != NULL){
            if (strcmp(token, tok) == 0){
                check = 1;
            }
            token = strtok(NULL, " ");
        }
        if (check == 0)fputs(line2, chunk2);
    }
    fclose(chunk);
    fclose(chunk2);
    create_mapping_mail(path);
    file_uid++;
    mapping_uid = file_uid;
}

static int do_getattr( const char *path, struct stat *st )
{
	st->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
	st->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
	st->st_atime = time( NULL ); // The last "a"ccess of the file/directory is right now
	st->st_mtime = time( NULL ); // The last "m"odification of the file/directory is right now
	
	if ( strcmp( path, "/" ) == 0 || is_dir( path ) == 1 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
	}
	else if ( is_file( path ) == 1 )
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	else
	{
		return -ENOENT;
	}
	
	return 0;
}

void set_uids(char* path){
    CURL *curl;
    CURLcode res = CURLE_OK;
    
    curl = curl_easy_init();
    if(curl) {
        FILE* chunk = fopen("uids_list.txt", "w");
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
    
        /* This is just the server URL */
        int len = strlen(path);
        if (path[len-1] == '$') path[len-1] = '\0';
        char url[500];
        strcpy(url, in_url);
        strcat(url, path);

        curl_easy_setopt(curl, CURLOPT_URL, url);
    
        /* Set the CREATE command specifying the new folder name */
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "UID SEARCH ALL");

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
    
        /* Perform the custom request */
        res = curl_easy_perform(curl);
    
        /* Check for errors */
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    
        /* Always cleanup */
        curl_easy_cleanup(curl);
        fclose(chunk);
        chunk = fopen("uids_list.txt", "r");
        char line[10000];
        fgets(line, 9000, chunk);
        char *token = strtok(line, " ");
        char result[100];
        while(token != NULL){
            strcpy(result, token);
            token = strtok(NULL, " ");
        }
        file_uid = atoi(result);
        mapping_uid = atoi(result);
        printf("mapping_uid and file_uid changed\n");
        printf("%d\n",mapping_uid);
        printf("mapping_uid and file_uid changed\n");
        printf("mapping_uid and file_uid changed\n");
        printf("mapping_uid and file_uid changed\n");
        printf("mapping_uid and file_uid changed\n");
        fclose(chunk);
    }
}

static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
  filler( buffer, ".", NULL, 0 ); // Current Directory
  filler( buffer, "..", NULL, 0 ); // Parent Directory
  char* path2 = malloc(sizeof(char)*5000);
  strcpy(path2, path);
  is_dir(path2);
  path2++;
  printf("************ do_readdir is called: path = %s\n", path2);
  if (strcmp(path, "/") != 0) set_uids(path2);
  retrive_mapping_mail(path2);
  //if(strcmp(path, "/") == 0){
    FILE *chunk = fopen("out.txt", "r");
    char line[1000];
    while(fgets(line, 1000, chunk)){
      printf("line = %s", line);
      char *token = strtok(line, " ");
      token = strtok(NULL, " ");
      token = strtok(NULL, " ");
      token = strtok(NULL, " ");
      token = strtok(NULL, " ");
      if (strncmp(token, "\"/\"", 3) != 0){
        token++;
        char* token2 = malloc(sizeof(char)*100);
        for(int i=0;i<100;i++) token2[i] = '\0';
        int len = strlen(token);
        //printf("len = %d\n", len-3);
        strncpy(token2, token, len-3);
        //token2[len-2] = '\0';
        printf("%s\n", token2);
        char token3[1000];
        strcpy(token3, token2);
        strcat(token3, "$");
        printf("%s %s\n", token3, path2);
        if (strcmp(token3, path2) != 0){
            printf("(((((((((((( put in buffer\n");
            printf("%s\n", token2);
            filler(buffer, token2, NULL, 0);
        }
        //printf("buffer = %s\n", (char*)buffer);
      }
    }
    fclose(chunk);
    //FILE* chunk;
    //char line[2000];
    chunk = fopen("mapping.txt", "r");
    fgets(line, 1000, chunk);
    fgets(line, 1000, chunk);
    fgets(line, 1000, chunk);
    while(fgets(line, 1000, chunk)){
        printf("%s", line);
      char *token = strtok(line, " ");
      token = strtok(NULL, " ");
      filler(buffer, token, NULL, 0);
    }
    fclose(chunk);
  //}
  return 0;
}

int get_content_of_file(int file_uid_retrieve, char *path)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    FILE* chunk = fopen("content.txt", "w");
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, user_id);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password_login);
 
    /* This will fetch message 1 from the user's inbox */

    char url[500];
    char str[200];
    char* token = strtok(path, "/");
    strcpy(url, in_url);
    strcat(url, token);
    strcat(url, "/;UID=");
    sprintf(str, "%d", file_uid_retrieve);
    strcat(url, str);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    printf("read = %s\n", url);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
 
    /* Perform the fetch */
    res = curl_easy_perform(curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform()6 failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
    fclose(chunk);
  }
 
  return (int)res;
}

//TODO: check do_read
static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
  printf("==============do_read==================\n");
  int ch = is_file(path);
  if (ch == 0) return -1;
  char* path2 = malloc(sizeof(char)*500);
  for(int i=0;i<500;i++) path2[i] = '\0';
  strcpy(path2, path);
  char* path3 = malloc(sizeof(char)*500);
  for(int i=0;i<500;i++) path3[i] = '\0';
  strcpy(path3, path);
  is_dir(path2);
  path2++;
  path3++;
  retrive_mapping_mail(path2);
  FILE *chunk = fopen("mapping.txt", "r");
  char line[2000];
  fgets(line, 1000, chunk);
  fgets(line, 1000, chunk);
  fgets(line, 1000, chunk);
  int file_uid_retrieve;
  path++;
  char* token2 = strtok(path2, "/");
  token2 = strtok(NULL, "/");
  while(fgets(line, 1000, chunk)){
    char *token = strtok(line, " ");
    token = strtok(NULL, " ");
    if (strcmp(token, token2) == 0){
      token = strtok(NULL, " ");
      file_uid_retrieve = atoi(token);
    }
  }
  fclose(chunk);
  get_content_of_file(file_uid_retrieve, path3);
  chunk = fopen("content.txt", "r");
  printf("=============Content================\n");
  char content[10000];
  for(int i=0;i<1000;i++) content[i] = '\0';
  fgets(line, 1000, chunk);
  fgets(line, 1000, chunk);
  fgets(line, 1000, chunk);
  while(fgets(line, 1000, chunk)){
    strcat(content, line);
  }
  printf("%s", content);
  memcpy(buffer, content + offset, size);
  return strlen(content) - offset;
}

static int do_mkdir( const char *path, mode_t mode )
{
	path++;
	add_dir( path );
	
	return 0;
}

static int do_mknod( const char *path, mode_t mode, dev_t rdev )
{
	path++;
	add_file( path );
	
	return 0;
}

static int do_write( const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info )
{
	write_to_file( path, buffer );
	
	return size;
}

static int do_rmdir(const char* path){
    path++;
    remove_dir(path);

    return 0;
}

static int do_unlink(const char *path){
    path++;
    char path2[1000];
    strcpy(path2, path);
    char *token = strtok(path2, "/");
    token = strtok(NULL, "/");
    if (token[0]-'0' >= 0 && token[0]-'0' <= 9) {
      printf("File cannot be removed\n");
      printf("File cannot be removed\n");
      printf("File cannot be removed\n");
      printf("File cannot be removed\n");
      printf("File cannot be removed\n");
      return 0;
    }
    remove_file(path);

    return 0;
}

static struct fuse_operations operations = {
    .getattr	= do_getattr,
    .readdir	= do_readdir,
    .read		= do_read,
    .mkdir		= do_mkdir,
    .mknod		= do_mknod,
    .write		= do_write,
    .rmdir      = do_rmdir,
    .unlink     = do_unlink,
};

int main(int argc, char *argv[]){
    //add_dir("new_folder");
    //int ch = is_dir("/new_mail_2.txt");
    //if (ch == 0) printf("NO\n");
    //else printf("YES\n");
    //add_file("new_mail_6.txt");
    //int ch = is_file("/new_mail_7.txt");
    //if (ch == 0) printf("NO\n");
    //else printf("YES\n");
    //retrive_mapping_mail();
    //write_to_file("/new_mail_2.txt", "hello, world!");
    /*if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }*/
    printf("%s\n", argv[argc-1]);
    FILE* chunk = fopen(argv[argc-1], "r");
    char line[2000];

    for(int i=0;i<2000;i++) line[i] = '\0';
    fgets(line, 1000, chunk);
    int len = strlen(line);
    strncpy(in_url, line, len-1);
    printf("%s %ld\n", in_url, strlen(line));

    for(int i=0;i<2000;i++) line[i] = '\0';
    fgets(line, 1000, chunk);
    len = strlen(line);
    strncpy(user_id, line, len-1);
    printf("%s\n", user_id);

    for(int i=0;i<2000;i++) line[i] = '\0';
    fgets(line, 1000, chunk);
    len = strlen(line);
    strncpy(password_login, line, len-1);
    printf("%s\n", password_login);

    fclose(chunk);
    argc--;
    return fuse_main( argc, argv, &operations, NULL );
}