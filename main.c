#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexical_analyser.h"




int error=0;

int auxright = 0;
int auxleft = 0 ;

int left = 0, right = 0;
int codeLen = 0 ;
char str[200];

/* Grammaire :

    * P       ->  Program id S_DCL Debut S_INST Fin
    * S_DCL   ->  DCL D
    D       ->  Epsilon | S_DCL
    * DCL     ->  Var L_id : TYPE;
    L_id    ->  id B
    B       ->  Epsilon | , L_id
    TYPE    ->  entier | reel
    S_INST  ->  INST C
    C       ->  Epsilon | S_INST
    INST    ->  id := Expa; | Si Expb alors S_INST E
    E       ->  Fin Si | Sinon S_INST Fin Si
    Expb    ->  Expr A | non(Expb) A
    A       ->  et Expb A | ou Expb A | Epsilon
    Expr    ->  Expa oprel Expa | (Expr)
    Expa -> (Expa) Eprime | id Eprime | nb Eprime
    Eprime -> oparith Expa Eprime | epsilon


*/


void saveState(void){
    auxright = right;
    auxleft = left;
}

void validateMove(void){
    right = auxright;
    left = auxleft;
}


bool isValidSpace(char ch) {
   if (ch == ' ' )
   return (true);
   return (false);
}


char* nextToken(char* str) {

   saveState();
   int length = strlen(str);
   while (auxright <= length && auxleft <= auxright) {
      if (isValidSpace(str[auxright]) == false)
      {
          auxright++;
      }

      if (isValidSpace(str[auxright]) == true && auxleft == auxright)
      {
         auxright++;
         auxleft = auxright;

      } else if (isValidSpace(str[auxright]) == true && left != auxright || (auxright == length && auxleft != auxright))
      {
         char* subStr = subString(str, auxleft, auxright - 1);
         auxleft = auxright;
         return subStr ;
      }
   }
   return;
}



/*

Grammar procedures


*/

//P -> Program id S_DCL Debut S_INST Fin
void P(void){

    if (strcmp(nextToken(str),"Program" )==0){
        validateMove();
        if(strcmp(nextToken(str),"id" )==0){
            validateMove();
           S_DCL();
           if(error == 1) return;
           if (strcmp(nextToken(str),"Debut" )==0){
               validateMove();
               S_INST();
               if(error == 1) return;
               if (strcmp(nextToken(str),"Fin" )==0){
                   validateMove();

               }else{



                   printf("error : expected Fin\n");error=1;
                   printf("  \"%.*s\n", right, str);
                   return ;
               }

           }else{
               printf("error : expected Debut\n");error=1;
               printf("  \"%.*s\n", right, str);
               return ;
           }



        }else{
            printf("error : expected id\n");error=1;
            printf("  \"%.*s\n", right, str);
            return ;
        }


    } else{
        error = 1 ;
        printf("error : expected Program\n");error=1;
        printf("  \"%.*s\n", right, str);
        return ;
    }

}

//S_DCL -> DCL D
void S_DCL(void){
    DCL();
    if (error ==1 ) return;
    D();
    if (error ==1 ) return;
}

//D -> Epsilon | S_DCL
void D(void){

        if (strcmp(nextToken(str),"var")==0){
                validateMove();
                S_DCL();
                if (error ==1 ) return;
        }else{
        }

}

//DCL -> Var L_id : TYPE ;
void DCL(void){
    if (strcmp(nextToken(str),"var")==0){
            validateMove();
        L_id();
        if (error ==1 ) return;
        if (strcmp(nextToken(str),":")==0){
            validateMove();
                TYPE();
                if (error ==1 ) return;
                if (strcmp(nextToken(str),";")==0){
                    validateMove();
                }else{
                    error = 1;
                    printf("error : expected ;  \n");
                    printf("  \"%.*s\n", right, str);
                    return ;
                }
        }else{
            error = 1;
            printf("error : expected :  \n");
            printf("  \"%.*s\n", right, str);
            return ;
        }



    } else {
        error = 1;
        printf("error : expected var  \n");
        printf("  \"%.*s\n", right, str);
        return ;
    }
}

//L_id -> id B
void L_id(void){
    if (strcmp(nextToken(str),"id")==0){
            validateMove();
        B();
        if (error ==1 ) return;
    }else{
        printf("error : expected id");
        printf("  \"%.*s\n", right, str);
        error = 1;
        return ;
    }
}

//B -> Epsilon | , L_id
void B(void){

    //save previous satus of right and left in case of failure


    if(strcmp(nextToken(str),",")==0){
            validateMove();
        L_id();
        if (error ==1 ) return;
    }
    else{
        //restore in case of failure

    }
}

//TYPE -> entier | reel
void TYPE(void){
    if (strcmp(nextToken(str),"entier")==0 || strcmp(nextToken(str),"reel")==0){
        validateMove();
    }else{
        error = 1;
        printf("error : expected type \n");
        printf("  \"%.*s\n", right, str);
        return ;
    }
}

//S_INST -> INST C
void S_INST(void){
    INST();
    if (error ==1 ) return;
    C();
    if (error ==1 ) return;
}

//C -> Epsilon | S_INST
void C(void){

    if (strcmp(nextToken(str),"Fin")!=0 && strcmp(nextToken(str),"Finsi")!=0){
            //validateMove();
            S_INST();
            if (error ==1 ) return;
    }

}




//INST -> id := Expa ; | Si Expb alors S_INST E
void INST(void){
    if (strcmp(nextToken(str),"id")==0){
            validateMove();
            if (strcmp(nextToken(str),":=")==0){
                validateMove();
                    Expa();
                    if (error ==1 ) return;
                    if (strcmp(nextToken(str),";")==0){
                            validateMove();
                    }else{
                        error = 1;
                        printf("error : expected ; \n");
                        printf("  \"%.*s\n", right, str);
                        return ;
                    }
            }else{
                error = 1;
                printf("error : expected := \n");
                printf("  \"%.*s\n", right, str);
                return ;
            }
    }
    else if (strcmp(nextToken(str),"si")==0){  //Si Expb alors S_INST E
            validateMove();
             Expb();
             if (error ==1 ) return;
             if (strcmp(nextToken(str),"alors")==0){
                 validateMove();
                 S_INST();
                 if (error ==1 ) return;
                 E();
                 if (error ==1 ) return;
             }else{
                 error = 1;
                 printf("error : expected alors \n");
                 printf("  \"%.*s\n", right, str);
                 return ;
             }


    }
    else{
        error = 1;
        printf("error : expected id or si  \n");
        printf("  \"%.*s\n", right, str);
        return ;
    }




}



//E -> Finsi | Sinon S_INST FinSi
void E(void){
    if (strcmp(nextToken(str),"Finsi")==0){
            validateMove();
    }else if (strcmp(nextToken(str),"sinon")==0){
        validateMove();
              S_INST();
              if (error ==1 ) return;
              if (strcmp(nextToken(str),"Finsi")==0){
                  validateMove();
              }else{
                  error = 1;
                  printf("error : expected Finsi \n");
                  printf("  \"%.*s\n", right, str);
                  return ;
              }

    }
    else{
        error = 1;
        printf("error : expected Finsi or Sinon");
        printf("  \"%.*s\n", right, str);
        return ;
    }

}




//Expb -> Expr A | non(Expb) A
void Expb(void){

    if (strcmp(nextToken(str),"non")==0){
            validateMove();
            if (strcmp(nextToken(str),"(")==0){
                validateMove();
                    Expb();
                    if (error ==1 ) return;
                    if (strcmp(nextToken(str),")")==0){
                        validateMove();
                            A();
                    }else{
                        error = 1;
                        printf("error : expected ) \n");
                        printf("  \"%.*s\n", right, str);
                        return ;
                    }

            }else{
                error = 1;
                printf("error : expected ( \n");
                printf("  \"%.*s\n", right, str);
                return ;
            }
    }
    else{
        Expr();
        if (error ==1 ) return;
        A();
        if (error ==1 ) return;
    }
}

//A -> et Expb A | ou Expb A | Epsilon
void A(void){

    if (strcmp(nextToken(str),"et") ==0 || strcmp(nextToken(str),"ou") ==0 ){
            validateMove();
        Expb();
        if (error ==1 ) return;
        A();
    }
}

//Expr -> Expa oprel Expa | (Expr)
void Expr(void){
     if (strcmp(nextToken(str),"(") ==0 ){
            validateMove();
            Expr();
            if (strcmp(nextToken(str),")") ==0 ){
                validateMove();
            }else{
                error = 1;
                printf("error : expected ) \n");
                printf("  \"%.*s\n", right, str);
                return ;
            }


     }else{
         Expa();
         if (error ==1 ) return;
         if (strcmp(nextToken(str),"oprel") ==0 ){
                validateMove();
                Expa();
                if (error ==1 ) return;
         }else{
             error = 1;
             printf("error : expected oprel \n");
             printf("  \"%.*s\n", right, str);
             return ;
         }
     }
}

//Expa -> (Expa) Eprime | id Eprime | nb Eprime

void Expa(void){
    if (strcmp(nextToken(str),"(") ==0 ){
            validateMove();
            Expa();
            if (error ==1 ) return;
            if (strcmp(nextToken(str),")") ==0 ){
                validateMove();
                    Eprime();
                    if (error ==1 ) return;
            }else{
                error = 1;
                printf("error : expected token ) \n");
                printf("  \"%.*s\n", right, str);
                return ;
            }
    }else if (strcmp(nextToken(str),"id") ==0 ){
        validateMove();
        Eprime();
        if (error ==1 ) return;
    }else if (strcmp(nextToken(str),"nb") ==0 ){
        validateMove();
        Eprime();
        if (error ==1 ) return;
    }else{
        error = 1 ;
        printf("error : expected ( or id or nb \n ");
        printf("  \"%.*s\n", right, str);
        return ;
    }
}

//Eprime -> oparith Expa Eprime | epsilon
void Eprime(void){
    if (strcmp(nextToken(str),"oparith") ==0 ){
            validateMove();
        Expa();
        if (error ==1 ) return;
        Eprime();
        if (error ==1 ) return;

    }
}



/*

end of Grammar procedures

*/



int main()
{




    //testing first program
    char code[200] = "Program test var a, b : anything_instead_of_type ; Debut a := 10; si a >= 10 alors a := 20 ; Finsi Fin";


    printf("The first Program to test is :\n       ----   '%s' \n", code);

    detectTokens(code);
    printf("\n\n");
    strcpy(str,token);


    printf("\n");
    P();
    printf("\n\n");

    if(error == 1){
        printf("----------------------\n");
        printf("  program not valid \n");
        printf("----------------------\n");
    }

    else{
        printf("----------------------\n");
        printf("  valid program !!!\n");
        printf("----------------------\n");
    }

    //reinitialize params
   memset(str, 0, 200);
   memset(token, 0, 200);
   right = 0;
   left = 0;
   auxleft = 0;
   auxright = 0;
    error = 0;

   printf("\n\n\n\n");

   //testing second program
    char code2[200] = "Program test var a, b : entier ; Debut a := 10; si a >= 10 anything_instead_of_alors a := 20 ; Finsi Fin";


    printf("The second Program to test is :\n       ----   '%s' \n", code2);

    detectTokens(code2);
    printf("\n\n");
    strcpy(str,token);


    printf("\n");
    P();
    printf("\n\n");

    if(error == 1){
        printf("----------------------\n");
        printf("  program not valid \n");
        printf("----------------------\n");
    }

    else{
        printf("----------------------\n");
        printf("  valid program !!!\n");
        printf("----------------------\n");
    }





    //reinitialize params
   memset(str, 0, 200);
   memset(token, 0, 200);
   right = 0;
   left = 0;
   auxleft = 0;
   auxright = 0;
    error = 0;

   printf("\n\n\n\n");

   //testing third program (correct program)
    char code3[200] = "Program test var a, b : entier ; Debut a := 10; si a >= 10 alors a := 20 ; Finsi Fin";


    printf("The third Program to test is :\n       ----   '%s' \n", code3);

    detectTokens(code3);
    printf("\n\n");
    strcpy(str,token);


    printf("\n");
    P();
    printf("\n\n");

    if(error == 1){
        printf("----------------------\n");
        printf("  program not valid \n");
        printf("----------------------\n");
    }

    else{
        printf("----------------------\n");
        printf("  valid program !!!\n");
        printf("----------------------\n");
    }


    printf("\n\n\nEnter q to exit ");
    scanf("%s",str);

    return(0);



}
