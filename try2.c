#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

typedef struct node{
	char c;
	struct node* l;
	struct node* r;
}tree;


//loading
void input(char []);

//parsing
tree* par(char**,tree**);
tree* addnum(char**,tree **);
tree* addsign(char**,tree **);
tree* addneg(char**,tree**);

//computing
double cal(tree*);
double getnum(double ,tree*);
double getdeci(tree* );


void infix(tree* root){
	if(root==NULL)return;
	infix(root->l);
	printf("%c",root->c);
	infix(root->r);
}

void postfix(tree* root){
	if(root==NULL)return;
	postfix(root->l);
	postfix(root->r);
	printf("%c",root->c);
}

void main(){
	char ar[500];
	input(ar);
	for(char *p=ar;*p;p++){
		tree* root=NULL;
		printf("\nBUFFER: ");
		for(char* c=p;*c!=';';c++)
			printf("%c",*c);
		par(&p,&root);
		printf("\nTree Infix: ");
		infix(root);
		printf("\nTree Postfix: ");
		postfix(root);
		printf(" = %f\n",cal(root));
	}
}



//------------------------Loading------------------------------------------------------------------------------
void input(char ar[]){
	FILE *ip;
    char ch;
    ar[0]='(';
    char *p=ar;
    p++;
    ip=fopen("in.txt","r");
    if(ip!=NULL){
        while((ch=getc(ip))!=EOF){
            if(ch!=';')
            	*p=ch;
            else{
            	*(p++)=')';
            	*(p++)=';';
            	*p='(';
            }
            p++;
        }
        *(p-2)='\0';
    }
}
//-----------------------Parsing-------------------------------------------------------------------------------
tree* addnum(char **p,tree** root){
	tree *l = (tree*)malloc(sizeof(tree));
	l->c=**p;
	l->l=l->r=NULL;
	if(*root==NULL)
		*root=l;
	else{
		tree* temp=*root;
		while(isdigit(**p)||**p=='.'){
			tree *l = (tree*)malloc(sizeof(tree));
			l->c=**p;
			l->l=l->r=NULL;
			//printf("insl %c \n",l->c);
			temp->r=l;
			temp=temp->r;
			//printf("instemp %c \n",temp->c);
			++(*p);
		}
		--(*p);
	}
}
//------------------------------------------------------------------------------------------------------
tree* addneg(char** p,tree** root){
	tree *l = (tree*)malloc(sizeof(tree));
	l->c='=';
	l->r=NULL;
	if(*root!=NULL)
		(*root)->r=l;
	else 
		*root=l;
	++(*p);
	addnum(p,&l);
}
//------------------------------------------------------------------------------------------------------
tree* addsign(char **p,tree** root){
	tree *l = (tree*)malloc(sizeof(tree));
	l->c=**p;
	l->r=NULL;
	if(**p=='+'||**p=='-'){
		l->l=*root;
		*root=l;
	}
	else{
		if(isdigit((*root)->c)){
			l->l=*root;
		    *root=l;
		}
		else{
			if((*root)->r!=NULL){
				tree* temp=(*root)->r;
				l->l=temp;
			}
			(*root)->r=l;
			//printf("\nInfix: ");
			//infix(*root);
			//printf("\nPostfix: ");
			//postfix(*root);
			++(*p);
			if(isdigit(**p) || **p=='.' ){ 
				addnum(p,&(l->r));
				++(*p);
				if(isdigit(**p) || **p=='.' ) 
					addnum(p,&(l->r));
				else
					--(*p);
			}
			else if(**p=='-'){
				addneg(p,&(l->r));
				--(*p);
			}
			else
				par(p,&(l->r));
			//printf("\nInfix: ");
			//infix(*root);
			//printf("\nPostfix: ");
			//postfix(*root);
		}
	}
}
//------------------------------------------------------------------------------------------------------
tree* par(char **p,tree** root){
	//if(*root=+6=NULL)
		//printf("\nRoot NULL Enter: %c",**p);
	for(;**p!=';';(*p)++){
		//printf("\nprevious %c=%c",*(*p-1),**p);
		//printf("loop p val: %c \n",**p);
		if(isdigit(**p)||**p=='.')
			addnum(p,root);
		else if(**p=='-' && ((*(*p-1)=='+')||(*(*p-1)=='-')||(*(*p-1)=='*')||(*(*p-1)=='/')||(*(*p-1)=='('))){
			addneg(p,root);
			//printf(" Neg added %c",**p);
		}
		else if(**p=='+' || **p=='-'|| **p=='*'||**p=='/'){
			//printf("\n %c Infix: ",**p);
			//infix(*root);
			addsign(p,root);
			//printf("\n %c Infix: ",**p);
			//infix(*root);
			//printf("\n========");
		}
		else if(**p=='('){
			(*p)++;
			tree* temp=NULL;
			if(*root!=NULL){
				//printf("\nCalling root %c",(*root)->c);
				(*root)->r=par(p,&temp);
				//printf("\nReturn root->r %c",(*root)->r->c);
			}
			else 
				*root=par(p,&temp);
		}
		else if(**p=')')
			break;
	}
	//printf("\nRet: ");
	//postfix(*root);
	return *root;
}
//---------------------------Computing---------------------------------------------------------------------------
double getdeci(tree* root){	
	double num=0;
	for(int i=10;root!=NULL;root=root->r,i*=10){
		num+=(double)(atof(&(root->c)))/i;
	}
	return num;
}
//------------------------------------------------------------------------------------------------------
double getnum(double num,tree* root){
	if(root==NULL)
		return (double)num;
	else if(root->c!='.')
		return getnum(10*num+(double)(atof(&(root->c))),root->r);
	else
		return num+getdeci(root->r);
}
//------------------------------------------------------------------------------------------------------
double cal(tree * root){
	if(isdigit(root->c)||root->c=='.')
		return getnum((double)(atof(&(root->c))),root->r);
	else if (root->c=='=')
		return (-1)*getnum((double)(atof(&(root->r->c))),root->r->r);
	else if(root->c=='+')
		return cal(root->l)+cal(root->r);
	else if(root->c=='-')
		return cal(root->l)-cal(root->r);
	else if(root->c=='/')
		return cal(root->l)/cal(root->r);
	else if(root->c=='*')
		return cal(root->l)*cal(root->r);
}
