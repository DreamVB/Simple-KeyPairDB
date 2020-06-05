#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define INVALID -1
#define MAX_KEY_SIZE 30
#define MAX_DATA 4048
#define MAX_STRING 2048
#define MAX_FILE_NAME 256

struct TKeyPair{
    char m_key[MAX_KEY_SIZE];
    char m_value[MAX_STRING];
}KeyPair[MAX_DATA];

int KeyPairCount = 0;

char lzFilename[MAX_FILE_NAME];

//Update the file
int _Update(){
    FILE *fout = NULL;
    char temp[MAX_DATA];

    int i = 0;

    //Try and open the file for updateing.
    fout = fopen(lzFilename,"w");

    if(!fout){
        return FALSE;
    }

    while(i < KeyPairCount){
        strcpy(temp,KeyPair[i].m_key);
        strcat(temp,"=");
        strcat(temp,KeyPair[i].m_value);
        strcat(temp,"\n");
        //Print out the line
        fputs(temp,fout);
        //INC counter
        i++;
    }
    //Close the file.
    fclose(fout);
    //Tidy up
    memset(temp,0,sizeof temp);
    return TRUE;
}

//Return an index of the key name
int _getKeyIndex(char *Key){
    int i = 0;
    int idx = INVALID;
    //Locate a key in the KeyPair array
    while(i < KeyPairCount){
        //Compare strings
        if(strcmp(KeyPair[i].m_key,Key) == 0){
            //Set found index
            idx = i;
            //Stop here
            break;
        }
        //INC Counter
        i++;
    }
    return idx;
}

int _addNew(char *Key, char *Value){

    //First check if we already have the key
    if(_getKeyIndex(Key) != INVALID){
        return FALSE;
    }
    //Add the new key and value to the KeyPair Database
    strcpy(KeyPair[KeyPairCount].m_key,Key);
    strcpy(KeyPair[KeyPairCount].m_value,Value);
    //INC KeyPairCount
    KeyPairCount++;

    return TRUE;
}

int _setKeyValue(char *Key, char*Value){
    int idx = INVALID;

    //Get key index
    idx = _getKeyIndex(Key);

    if(idx != INVALID){
        //Set keys new value
        strcpy(KeyPair[idx].m_value,Value);
        //Return
        return TRUE;
    }
    //Return
    return FALSE;
}

//Return a keys value
char *_getKeyValue(char *Key){
    int idx = INVALID;
    //Get key index
    idx = _getKeyIndex(Key);

    if(idx != INVALID){
        //Return key value.
        return KeyPair[idx].m_value;
    }
    return "\0";
}

//Locate the position of a char in a string
int chrPos(char *s, char c){
    int x = 0;
    int idx = INVALID;

    while(x < strlen(s)){
        if(s[x] == c){
            idx = x;
            break;
        }
        x++;
    }
    return idx;
}

int _loadDb(char *Filename){
    FILE *fp = NULL;
    char temp[MAX_STRING];
    char s0[MAX_KEY_SIZE];
    char s1[MAX_STRING];

    int sLen = 0;
    char s_pos = 0;

    KeyPairCount = 0;

    //Try and open the file.
    fp = fopen(Filename,"r");
    //Check if file was opened.
    if(!fp){
        return FALSE;
    }
    //While not end of file keep reading.
    while(!feof(fp)){
        //Get length of string
        sLen = strlen(temp);
        //Check length
        if(sLen > 0){
           if(temp[sLen-1] == '\n'){
                //Zap end of string
                temp[sLen-1]='\0';
                sLen--;
           }
           s_pos = chrPos(temp,'=');
           //Extract Key
           if(s_pos != INVALID){
                //Extract key
                memcpy(s0,&temp[0],s_pos);
                //zap end of string
                s0[s_pos] = '\0';
                //Extract Value
                memcpy(s1,&temp[s_pos + 1],sLen);
                //zap end of string
                s1[sLen] = '\0';
                //Add key and value to the KeyPair array
                strcpy(KeyPair[KeyPairCount].m_key,s0);
                strcpy(KeyPair[KeyPairCount].m_value,s1);
                //INC Counter
                KeyPairCount++;
            }
        }
        //Read in the next line
        fgets(temp,MAX_STRING,fp);
    }
    fclose(fp);
    //Tidy up
    memset(s0,0,sizeof s0);
    memset(s1,0,sizeof s1);
    memset(temp,0,sizeof temp);
    //Give good result return.
    return TRUE;
}

int main(int argc, char*argv[])
{
    char value[MAX_STRING];

    if(argc != 2){
        printf("Opps Something Went Wrong.\n");
        exit(1);
    }else{
        //Copy file name
        strcpy(lzFilename,argv[1]);

    }
    //Load the key pair database
    if(_loadDb(lzFilename) == FALSE){
        printf("There Was A Error Reading The Database.\n");
        exit(1);
    }

    //EXAMPLE //EXAMPLE //EXAMPLE

    //Read in name
    strcpy(value,_getKeyValue("Name"));
    //Print value found.
    printf("Hello From: %s\n",value);

    //Read in the Message Of the day key value.
    strcpy(value,_getKeyValue("Msg"));
    //Print value found.
    printf("Message of the Day: %s\n",value);

    printf("\n--Changing MOD message--\n\n");
    //Set key value
    if(_setKeyValue("Msg","All admins login to computer 2") == FALSE){
        printf("There Was A Error Updating the Key Value\n");
    }
    //Gt new key value
    strcpy(value,_getKeyValue("Msg"));
    //Print value found.
    printf("Message of the Day: %s\n",value);
    //Add new

    _addNew("KeyTest","New Test Key Added.");
    //Show the new key
    strcpy(value,_getKeyValue("KeyTest"));
    //Print out found value
    printf("%s\n",value);

    if(_Update() == FALSE){
        printf("There Was An Error Updating The Database.\n");
    }

    //Clear up
    memset(KeyPair,0,sizeof KeyPair);
    memset(value,0,sizeof value);
    return 0;
}
