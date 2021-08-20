#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(){
    FILE *fileLex,*file;
    file = fopen("code.psi", "r");
    fileLex = fopen("code.lex", "w");

    char keyword[18][10]={"break","goto","case","char","const","continue","do","else","enum","float","for","if","int","long","record","return","static","while"};


    int i,j,strSize,m;
    i=j=strSize=m=0;
    char word[20];
    char strConst[100];

    if(file!=NULL){

      char chr = getc(file);

      while(!feof(file)){

        if(isspace(chr)){ // Space and other similar types will be ignored.
            chr = getc(file);
        }
        // identifier and keyword control
        // Is the character read in alphabetical order?
        if((isalpha(chr))){
          int wordLen=0;
          bool isKeyword=true;
          while(isalpha(chr) || isdigit(chr) || chr == 95){ // Is it suitable for identifier writing?
          // Over 20 will not be accepted.
            if(wordLen > 20){
                printf("Identifier names can't be longer than 20 characters!");
                return 1;
            }
            if(isdigit(chr) || chr == 95){
              isKeyword=false;
            }
            //
            if(isupper(chr)) chr = tolower(chr);
            word[wordLen++] = chr;
            chr = getc(file);
          }
        word[wordLen] = '\0'; // Added null to the end of the word to make it readable.

        /*If the word consists of all alphabetic characters,
        keyword control is performed.*/
        if (isKeyword){
          for (jsd=0;j<18;j++){
            if (strcmp(word, keyword[j]) == 0){ // Eslesme varsa
                fputs("Keyword(", fileLex);
                isKeyword=true;
              for(j=0; j < wordLen; j++){
                fputc(word[j], fileLex);
              }
              fputs(")\n", fileLex);
              break;
            }
            else{
              isKeyword=false;
            }
          }
        }
            if (!isKeyword){ // If it's not a keyword, it's an identifer.
              fputs("Identifier(", fileLex);
              for (j=0; j < wordLen; j++){
                  fputc(word[j], fileLex);
              }
              fputs(")\n", fileLex);
            }

        }
        // operators
        if (chr == 43){ // Status of being + or ++ for the + operator
            chr = getc(file);
            if (chr == 43){
                fputs("Operator(++)\n", fileLex);
            }
            else{
                fputs("Operator(+)\n", fileLex);
            }
        }
        if (chr == 45){ // Status of being - or -- for the - operator
          chr = getc(file);
          if (chr == 45){
            fputs("Operator(--)\n", fileLex);
          }
          else{
            fputs("Operator(-)\n", fileLex);
          }
        }
        // star character control
        if (chr == 42){
          chr=getc(file);
          if (chr!=41){
            fputs("Operator(*)\n", fileLex);
          }
        }
        // := equality operator check
        if (chr == 58){
            chr = getc(file);
            if (chr != 61){
              printf("Undefined input error...");
              return 1;
            }
            else{
              fputs("Operator(:=)\n",fileLex);
            }
        }
        // Slash check '/'
        if (chr == 47){
          chr = getc(file);
          fputs("Operator(/)\n", fileLex);
        }
        // state of being string
        if (chr == 34){ // " check
            chr = getc(file);
            strConst[strSize++]=chr;
            // In order to be defined as a string,
            // we read it until the double quotes come.
            while (chr != 34 && chr != EOF){
              chr = getc(file);
              strConst[strSize++]=chr;
            }
            strConst[strSize]='\0';
            if (chr == 34){ //When the quotation mark is closed
              fputs("StrConst(", fileLex);
              for (m=0; m < (strlen(strConst)-1); m++){
                  fputc(strConst[m], fileLex);
              }
              fputs(")\n", fileLex);
            }
            // If you reach the end of the file without
            // closing the quotation marks:
            else if (chr == EOF){
                printf("Double quote error!");
                return 1;
            }
        }
        // integer check
        if(isdigit(chr)){
        int int_len=0;
          fputs("IntConst(", fileLex);
          while (isdigit(chr)){
            if(int_len > 10){
              printf("Integers can't be longer than 10 characters!");
              return 1;
            }
            fputc(chr, fileLex);
            chr = getc(file);
            int_len++;
          }
            fputs(")\n", fileLex);
        }

        // ';' check
        if (chr == 59){
            fputs("EndOfLine\n",fileLex);
        }

        // Bracket characters blocks
        if (chr == 40){
          chr = getc(file);
          // Checking whether there is a comment

          if (chr == 42){
            // '(' does the * star come after the left parenthesis?
            while (!feof(file)){
              chr = getc(file);
              if (chr == 42){ // If an star is followed,
                              //the next character is ')', it's the end of the comment.
                chr = getc(file);
                if (chr == 41) // did ')' come after the second star
                chr = getc(file);
                break;
              }
              if(chr == EOF){
                printf("Comment error.");
                return 1;
              }
            }
          }
          // If it is not a comment blog,
          // it will be read as a straight left parenthesis.
          else{
            fputs("LeftParantheses\n", fileLex);
          }
        }
        // Right Parantheses
        if (chr == 41){
          fputs("RightParantheses\n", fileLex);
        }
        // LeftSquareBracket
        if (chr == 91){
           fputs("LeftSquareBracket\n", fileLex);
        }
        // RightSquareBracket
        if (chr == 93){
           fputs("RightSquareBracket\n", fileLex);
        }
        // LeftCurlyBracket
        if (chr == 123){
           fputs("LeftCurlyBracket\n", fileLex);
        }
        // RightCurlyBracket
        if (chr == 125){
           fputs("RightCurlyBracket\n", fileLex);
        }
        chr = getc(file);
      }
    }
    else{
        printf("File not found...");
        return 1;
    }

    fclose(file);
    fclose(fileLex);
    fflush(stdin);
    return 0;
}
