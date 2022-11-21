#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// #include "WEEK1.c"
typedef struct ND
{
    char username[100]; 
    char password[100];
    int status;
    struct ND *next;
} USER; 
typedef struct NDLIST
{
   USER* list;
   int size; 
   char inputFilename[255];
   USER* NDN;
} USERLIST;
// void clearenter(char* input)
// {
//   if(input[strlen(input)-1]=='\n')
//       input[strlen(input)-1] = '\0';
// }
int demdong(FILE* fptr){
  char s[100];
  int dem=0;
  while(fgets(s,100,fptr))
    dem++;
  
  return dem;
}
void readInsertEnd(USER* head, FILE* fptr)
{
    char s[100];
  while(head->next != NULL) head=head->next;
  USER* New = (USER*) malloc(sizeof(USER));
  fscanf(fptr,"%s ",s );
  strcpy(New->username,s);
  fscanf(fptr,"%s ",s );
  strcpy(New->password,s); 
  fscanf(fptr,"%s ",s );
  New->status=atoi(s);
  New->next=NULL;
  head->next=New;
  return ;
}
//USER* KTTonTaiTK(char name [100], const char* filename){

void NHAP(USERLIST *NDList,const char* filename){
  FILE *fptr;
	fptr = fopen(filename, "r");
  NDList->size=demdong(fptr);
  fclose(fptr);

  fptr = fopen(filename, "r");
	if (fptr == NULL){
		printf("Cannot open %s.\n", filename);
		return;
	} 
  strcpy(NDList->inputFilename,filename);
	char s[100];
  
	USER* head = (USER*) malloc(sizeof(USER));
	head->next=NULL;
  NDList->list=head;
  fscanf(fptr,"%s ",s );
  strcpy(head->username,s);
  fscanf(fptr,"%s ",s );
  strcpy(head->password,s);  
  fscanf(fptr,"%s ",s );
  head->status=atoi(s);
	for(int i=0; i<NDList->size-1; i++)
	  {
	    readInsertEnd(head,fptr);
    }

  fclose(fptr);
  
  return;

}
void printContactList(USERLIST NDList)
{
  USER* p= NDList.list;
  while(p!=NULL){
     printf("Usename: %s\n",p->username);
      printf("Password: %s\n",p->password);
      printf("Status: %d\n",p->status);
      printf("===================\n");
      p=p->next;
  }
  printf("\n-------------------------\n");
  return;
}
USER* searchTK(USERLIST NDList, char name[100]){
  USER* q= NDList.list;
  while (q!=NULL)
  {
      if (strcmp(q->username,name)==0)
         return q;
      q=q->next;
  }
  return NULL;
}
void addTK(USER* head,char s1[100],char s2[100],int x){
  while(head->next!= NULL) head=head->next;
  USER* New = (USER*) malloc(sizeof(USER));
  New->next=NULL;
  strcpy(New->username,s1);
  strcpy(New->password,s2);
  New->status=x;
  head->next=New;
  return;
}
void printinfile(USERLIST* NDList){
      FILE*fp;
      USER*firt=NDList->list;
  char cach[10]=" ";
  char enter[10]="\n";
   fp = fopen(NDList->inputFilename, "w");
   while(firt != NULL){
   fprintf(fp, firt->username);
   fprintf(fp, cach);
   fprintf(fp, firt->password);
   fprintf(fp, cach);
   fprintf(fp,"%d",firt->status);
   fprintf(fp, enter);
   firt=firt->next;
   }
      fclose(fp);
      return;
}
void Register(USERLIST *NDList){
  char s1 [100],s2 [100];
  FILE*fp;
  USER*head=NDList->list;
  printf("moi nhap usename:");
    scanf("%s",s1);
    USER*p = searchTK(*NDList,s1);
    if(p != NULL){
        printf("Tai khoan da ton tai!\n");
    }
    else{
       printf(" moi nhap password: ");
       scanf("%s",s2);
       addTK(head,s1,s2,2);
   
  // USER*firt=NDList->list;
  // char cach[10]=" ";
  // char enter[10]="\n";
  //  fp = fopen("nguoidung.txt", "w");
  //  while(firt != NULL){
  //  fprintf(fp, firt->username);
  //  fprintf(fp, cach);
  //  fprintf(fp, firt->password);
  //  fprintf(fp, cach);
  // fprintf(fp,"%d",firt->status);
  //  fprintf(fp, enter);
  //  firt=firt->next;
  //  }
   printinfile(NDList);
   NDList->size=NDList->size+1;
      fclose(fp);
    }
}
void khoaTK(USERLIST *NDList, USER* p){
    p->status=0;
    printinfile(NDList);

}
void Activate(USERLIST *NDList){
     char s1[100],s2[100],s3[10];
     int dem=0;
     printf("moi nhap tai khoan muon kinh hoat.\n");
     printf("Username:");
     scanf("%s",s1);
     USER *p= searchTK(*NDList,s1);
     if(p==NULL){
      printf("Tai khoan khong ton tai\n!");
      return;
     }
     printf("Password:");
     scanf("%s",s2);
     if(strcmp(s2,p->password)!= 0){
      printf("sai mat khau!\n");
      return;
     }
     while(dem<5){
        printf("Ma kich hoat:");
        scanf("%s",s3);
        if (strcmp(s3,"20194574")==0){
          //kich hoat
          printf("tai khoan cua ban da duoc kich hoat!\n");
          p->status=1;
          printinfile(NDList);
          break;
        }
        dem++;
     }
     if(dem==5){
      printf("tai khoan ban da bi khoa!\n");
      khoaTK(NDList,p);
     }
     return;
}
void signin(USERLIST *NDList){
  char s1[100],s2[100];
  int dem=0;
  if(NDList->NDN != NULL){
    printf("da co tai khoan dang dang nhap!");
    return;
  }
  printf("moi nhap thong tin tai khoan dang nhap.\n");
  printf("Username:");
  scanf("%s",s1);
  USER *p= searchTK(*NDList,s1);
     if(p==NULL){
      printf("Tai khoan khong ton tai\n!");
      return;
     }
  while(dem<4){
    printf("Password:");
    scanf("%s",s2);
    if(strcmp(s2,p->password)==0){
      if(p->status==0){
        printf("Tai khoan dang bi khoa!\n");
        return;
      }
      if(p->status==2){
        printf("Tai khoan chua kich hoat!\n");
        return;
      }
      printf("Chuc mung ban da dang nhap thanh cong!\n");
      NDList->NDN=p;
    return;
    }
    dem++;
  }
  printf("tai khoan ban da bi khoa!\n");
  khoaTK(NDList,p);
}
void SearchTKONLINE(USERLIST NDList){
  char s1[100];
      printf("moi nhap Username TK muon tim kiem:");
     scanf("%s",s1);
     USER *p= searchTK(NDList,s1);
     if(p==NULL){
      printf("Tai khoan khong ton tai\n!");
      return;
     }

     if(NDList.NDN == NULL){
          printf("Ban chua dang nhap khong the tim kiem!\n");
     }
     else{
        printf("%s\n",p->username);
        if(p->status==0)
            printf("Trang thai tai khoan la blocked.\n");
        if(p->status==1)
            printf("Trang thai tai khoan la active.\n");
        if(p->status==2)
            printf("Trang thai tai khoan la idle.\n");
      
     }
}
void Changepassword(USERLIST *NDList){
   char s1[100],s2[100],s3[100];
       if(NDList->NDN == NULL){
          printf("Ban chua dang nhap!\n");
     }
     else{
          printf("Moi nhap thong tin tai khoan can doi mat khau.\n");
          printf("Username:");
          scanf("%s",s1);
          if(strcmp((NDList->NDN)->username,s1)!= 0){
              printf("Sai Tai Khoan!\n");
              return;
          }
          printf("Password:");
          scanf("%s",s2);
          if(strcmp((NDList->NDN)->password,s2)!= 0){
              printf("Sai Mat Khau!\n");
              return;
          }
          printf("Moi nhap mat khau moi:");
          scanf("%s",s3);
          printf("Chuc mung ban da doi mat khau thanh cong.\n");
          strcpy((NDList->NDN)->password,s3);
          printinfile(NDList);

     }
}
void Signout(USERLIST* NDList){
  char s[100];
     if(NDList->NDN == NULL){
          printf("Chua co tai khoan dang dang nhap!\n");
     }
     else{
          printf("Moi nhap tai khoan muon dang xuat.\n");
          printf("Username:");
          scanf("%s",s);
          USER *p= searchTK(*NDList,s);
          if(p==NULL){
          printf("Tai khoan khong ton tai\n!");
          return;
          }
          else{
            if(strcmp((NDList->NDN)->username,s)!=0){
                printf("Tai khoan nay chua dang nhap!\n");
                return;
            }
            NDList->NDN=NULL;
            printf("Ban da dang xuat tai khoan thanh cong.\n");
          }
     }
}
int main()
{
  USERLIST nguoidung;
  int check=0;
  NHAP(&nguoidung,"nguoidung.txt");
  nguoidung.NDN=NULL;
  
 
  int chonmuc;
  while(chonmuc!=7){
    printf("USER MANAGEMENT PROGRAM\n");
    printf("1.Register\n");
    printf("2.Activate\n");
    printf("3.Sign in\n");
    printf("4.Search\n");
    printf("5.Change password\n");
    printf("6.Sign out\n");
    printf("7.Thoat chuong trinh\n");
    printf("moi chon muc:");
    scanf("%d",&chonmuc);
    if((chonmuc<1)||(chonmuc>6))
      printf("khong ton tai muc da chon hay chon lai!!\n");
    else
      switch(chonmuc){
        case 1 :
          Register(&nguoidung);
          break;
        case 2 :
          Activate(&nguoidung);
          break;
        case 3 :
          signin(&nguoidung);
          break;
        case 4 :
          SearchTKONLINE(nguoidung);
          break;
        case 5 :
          Changepassword(&nguoidung);
          break;
        case 6 :
          Signout(&nguoidung);
          break;
           
      }
  }
    return 0;
}
