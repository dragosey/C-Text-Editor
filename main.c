#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element {
	char caracter;
	struct element *prev;
	struct element *next;
} element;

//definire element din stiva
typedef struct element_stiva {
	char *op;
	int val;
	char *string;
	struct element_stiva *next;
} element_stiva;

//definirea stivei
typedef struct stiva_t {
	struct element_stiva *head;
	unsigned int lungime_stiva;
} stiva_t;

//functie pentru initializare stive
stiva_t *creare_stiva() {
	stiva_t *stiva = NULL;
	stiva=(stiva_t *)malloc(sizeof(stiva_t));
	stiva->head=NULL;
	return stiva;
}

//functie pentru crearea unui element nou din stiva si asocierea valorilor corespunzatoare
element_stiva *creaza_nod(int val, char* string, char *operatie, int string_length, int op_length)
{
	element_stiva *nod = NULL;
	nod=(element_stiva *)malloc(sizeof(element_stiva));
	nod->op=(char *)malloc(op_length*sizeof(char)+1);
	nod->string=(char *)malloc(string_length*sizeof(char)+1);
	nod->val=val;
	strcpy(nod->op,operatie);
	strcpy(nod->string,string);
	nod->next=NULL;
	return nod;
}

//functie pentru inserarea unui element in stiva
void insert_el_stiva(stiva_t *stiva, int val, char* string, char *operatie, int string_length, int op_length)
{
	element_stiva *nod = NULL;
	nod = creaza_nod(val,string,operatie,string_length,op_length);
	nod->next=stiva->head;
	stiva->head=nod;
	stiva->lungime_stiva++;
}

//functie pentru eliminarea unui element din stiva
void eliminare_el_stiva(stiva_t *stiva)
{
	element_stiva *aux;
	if(stiva->head==NULL)
		printf("Lista vida\n");
	else
	{
		aux=stiva->head;
		free(stiva->head);
		stiva->head=aux->next;
		stiva->lungime_stiva--;
	}
}

//Variabile globale
element *list_head=NULL;
element *poz_curenta=NULL;
int dim_lista=0;
char *copy_string;
char *prev_operatie;
char *prev_undo_operatie;
char *deleted_string;
char *deleted_backspace;		

//functie pentru adaugarea unui nou nod in lista dublu inlantuita ce contine caracterele din fisierul text.
void adauga_nod(char car) {
	element *node, *parcurgator;
	node=(element *)malloc(sizeof(element));
	node->caracter=car;
	node->next=NULL;
	node->prev=NULL;

	if(list_head==NULL)
	{
		list_head=node;
		poz_curenta=list_head;
	}
	else
	{
		parcurgator=list_head;
		while(parcurgator->next!=NULL)
			parcurgator=parcurgator->next;
		parcurgator->next=node;
		node->prev=parcurgator;
		poz_curenta=list_head;
	}
}

//functia cu ajutorul careia caracterele sunt preluate si inserate in lista. Am adaugat un element la finalul listei pentru a putea ajunge cu
//poz_curenta dupa ultimul caracter din fisierul text. 
void preluare_caractere(char* fisier_date)
{
	FILE * file;
	char c;
	if((file=fopen(fisier_date,"r"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis!", fisier_date);
		exit(1);
	}
	else
	{
		while((c=fgetc(file))!=EOF)
		{
			adauga_nod(c);
			dim_lista++;
		}
		adauga_nod('.');
	}
	fclose(file);
}

//Functiile pentru operatii
//Functia MOVE
void move(int deplasare)
{
	int i;
	if(list_head==NULL)
		exit(1);
	if(deplasare>0 && deplasare>dim_lista)
	{
		poz_curenta=list_head;
		for(i=0;i<deplasare;i++)
		{
			if(poz_curenta->next==NULL)
				break;
			else
				poz_curenta=poz_curenta->next;
		}
	}
	if(deplasare>0 && deplasare<dim_lista)
	{
		for(i=0;i<deplasare;i++)
		{
			if(poz_curenta->next==NULL)
				break;
			else
				poz_curenta=poz_curenta->next;
		}
	}
	if(deplasare<0 && (-1)*deplasare>dim_lista)
	{
		poz_curenta=list_head;
		while(poz_curenta->next!=NULL)
			poz_curenta=poz_curenta->next;

		for(i=0;i<(-1)*deplasare;i++)
		{
			if(poz_curenta->prev==NULL)
				break;
			else
				poz_curenta=poz_curenta->prev;
		}
	}
	if(deplasare<0 && (-1)*deplasare<dim_lista)
	{
		for(i=0;i<(-1)*deplasare;i++)
		{
			if(poz_curenta->prev==NULL)
				break;
			else
				poz_curenta=poz_curenta->prev;
		}
	}
}

//Functia INSERT STRING - insereaza caracter cu caracter de la poz_curenta
void insert_string(char* string)
{
	element *node, *parcurgator;
	int i;
	for(i=0;i<strlen(string);i++)
	{
		node=(element *)malloc(sizeof(element));
		node->caracter=string[i];
		node->next=NULL;
		node->prev=NULL;
		if(poz_curenta==list_head)
		{
			node->next=list_head;
			list_head->prev=node;
			list_head=node;
			dim_lista++;
			poz_curenta=node->next;
		}	
		if(poz_curenta!=list_head)
		{
			element *aux;
			parcurgator=poz_curenta;
			parcurgator->prev->next=node;
			aux=parcurgator->prev;
			parcurgator->prev=node;
			node->prev=aux;
			node->next=parcurgator;
			dim_lista++;
		}
	}
}	

//Functia DELETE 
void delete(int deplasare)
{
	element *delete;
	element *parcurgator;
	int i;
	int count_del=0;
	deleted_string=(char*)malloc(deplasare*sizeof(char)+1); //variabila ce stocheaza sirul de caractere sters.
	if(list_head==NULL)
		exit(1);
	for(i=0;i<deplasare;i++)
	{
		if(poz_curenta==list_head)
		{
			delete=list_head;
			list_head=list_head->next;
			list_head->prev=NULL;
			deleted_string[count_del]=delete->caracter;
			count_del++;
			free(delete);
			dim_lista--;
			poz_curenta=list_head;
		}
		if(poz_curenta!=list_head)
		{
			parcurgator=poz_curenta;
			parcurgator->prev->next=parcurgator->next;
			parcurgator->next->prev=parcurgator->prev;
			delete=parcurgator;
			deleted_string[count_del]=delete->caracter;
			count_del++;
			free(delete);
			dim_lista--;
			poz_curenta=parcurgator->next;
			if(poz_curenta->next==NULL)
				break;
		}
	}
	deleted_string[count_del]='\0';
}

//Functia COPY
void copy(int deplasare)
{
	element *parcurgator;
	int i;
	copy_string=(char *)malloc(deplasare*sizeof(char)+1);
	parcurgator=poz_curenta;
	for(i=0;i<deplasare;i++)
	{	
		if(parcurgator->next==NULL)
		{
			copy_string[i]=parcurgator->caracter;
			break;
		}
		else
		{
			copy_string[i]=parcurgator->caracter;
			parcurgator=parcurgator->next;
		}
	}
	copy_string[i]='\0'; //variabila ce retine sirul de caractere copiat.
}

//Functia BACKSPACE
void backspace()
{
	element *parcurgator, *desters;
	deleted_backspace=(char*)malloc(1*sizeof(char)+1);
	if(poz_curenta==list_head)
	{
		printf("Inceputul listei. Functia backspace nu va efectua nicio modificare\n");
		exit(1);
	}
	parcurgator=poz_curenta;
	if(parcurgator->prev==list_head)
		{
			desters=list_head;
			list_head=list_head->next;
			list_head->prev=NULL;
			deleted_backspace[0]=desters->caracter;
			free(desters);
			dim_lista--;
		}
	else if(parcurgator->prev!=NULL)
		{
			desters=parcurgator->prev;
			parcurgator->prev->prev->next=parcurgator;
			parcurgator->prev=parcurgator->prev->prev;
			deleted_backspace[0]=desters->caracter;
			free(desters);
			dim_lista--;
		}
	deleted_backspace[1]='\0'; //caracterul sters este memorat tot sub forma unui sir de caractere declarat global
}

//Functia REPLACE
void replace(char* old, char* new)
{
	element *parcurgator, *start, *node, *delete;
	int i,j;
	int nr=0;
	parcurgator=list_head;
	while(parcurgator->next!=NULL)
	{
		if(parcurgator->caracter==old[0]) //parcurge lista de caractere pana cand gaseste primul element din sirul care trebuie inlocuit
		{
			start=parcurgator; //memoreaza adresa unde a fost gasit primul caracter din string-ul de inlocuit
			//parcurge cat lungimea sirului de inlocuit si se porneste un contor; daca la finalul for-ului, valoarea contorului este egala
			//cu lungima sirului de inlocuit inseamna ca l-am gasit si urmeaza procesul efectiv de inlocuire, pe trei cazuri;
			for(i=0;i<strlen(old);i++) 
			{
				if(parcurgator->caracter==old[i]) 
				{
					nr++;
					parcurgator=parcurgator->next;
				}
				else
				{
					nr=0;
					break;
				}
			}
			if(nr==strlen(old) && strlen(old)==strlen(new)) //a), daca lungimea cuvantului nou si a celui vechi sunt egale.
			{
				parcurgator=start;
				for(i=0;i<strlen(new);i++)
				{
					parcurgator->caracter=new[i];
					parcurgator=parcurgator->next;
					poz_curenta=parcurgator;
				}
			}
			if(nr==strlen(old) && strlen(old)<strlen(new)) //b), daca lungimea cuvantului inlocuit este mai mica decat cea a cuvantului  nou
			{
				parcurgator=start;
				for(i=0;i<strlen(old);i++) //parcurgem si inlocuim caractere cat lungimea cuvantului inlocuit
				{
					parcurgator->caracter=new[i];
					parcurgator=parcurgator->next;
					poz_curenta=parcurgator;
				}
				for(i;i<strlen(new);i++) //pentru caracterele ramase trebuie sa adaugam intai noduri listei si sa le asociem caracterele ramase
				{
					node=(element *)malloc(sizeof(element));
					node->caracter=new[i];
					node->next=NULL;
					node->prev=NULL;

					parcurgator=poz_curenta;
					parcurgator->prev->next=node;
					parcurgator->prev=node;
					node->prev=parcurgator->prev;
					node->next=parcurgator;
					dim_lista++;
				}
				nr=0;
			}
			if(nr==strlen(old) && strlen(old)>strlen(new))//c), daca lungimea cuvantului nou este < decat cea a cuvantului vechi
			{
				parcurgator=start;
				for(i=0;i<strlen(new);i++) //introducem toate caracterele cuvantului nou
				{
					parcurgator->caracter=new[i];
					parcurgator=parcurgator->next;
					poz_curenta=parcurgator;
				}
				for(i;i<strlen(old);i++) //stergem caracterele ramase din cuvantul vechi si eliberam memoria
				{
					parcurgator=poz_curenta;
					parcurgator->prev->next=parcurgator->next;
					parcurgator->next->prev=parcurgator->prev;
					delete=parcurgator;
					free(delete);
					dim_lista--;
					poz_curenta=parcurgator->next;
					parcurgator=parcurgator->next;
				}
				nr=0;
			}
		}
		parcurgator=parcurgator->next;
	}
}

//functie ampla care citeste si executa fiecare operatie din fisier prin apelarea functiilor
//corespunzatoare
void preluare_operatii(char* fisier_operatii) 
{
	FILE * file;
	int nr_operatii=0, i,j;
	char nume_operatie[9];
	char *string_only, *word, *new_word;
	char *aux_backspace;
	int valoare_operatie;
	int count=0;
	stiva_t *stivaredo, *stivaundo; //doua stive folosite pentru operatia de undo si redo.
	stivaundo=creare_stiva();
	stivaredo=creare_stiva();
	
	if((file=fopen(fisier_operatii,"r"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis!", fisier_operatii);
		exit(1);
	}
	else
	{
		if(nr_operatii==0)
			fscanf(file,"%d", &nr_operatii);
		for(i=0;i<nr_operatii;i++)
		{
			valoare_operatie=0;
			fscanf(file,"%s %d", nume_operatie, &valoare_operatie); //preluam fiecare operatie si valoare asociata operatiei

			//caz special pentru operatia insert care are forma string string
			if(strcmp(nume_operatie,"insert")==0)
			{
				string_only=(char *)malloc(50*sizeof(char));
				fscanf(file,"%s", string_only);
				string_only=(char *)realloc(string_only, strlen(string_only)*sizeof(char));
				insert_string(string_only);
				insert_el_stiva(stivaundo,valoare_operatie,string_only,nume_operatie,sizeof(string_only),sizeof(nume_operatie));
			}

			if(strcmp(nume_operatie,"undo")!=0 && strcmp(nume_operatie,"insert")!=0) 
			//daca nu avem operatia undo sau insert inseram op. in stiva undo si initializam string-ul folosit pentru insert cu un caracter;
			{
				string_only=(char *)malloc(sizeof(char));
				string_only="-";
				insert_el_stiva(stivaundo,valoare_operatie,string_only,nume_operatie,1,strlen(nume_operatie));
			}

			//caz special pentru operatia replace care are forma string string string
			if(strcmp(nume_operatie,"replace")==0)
			{
				word=(char *)malloc(50*sizeof(char));
				new_word=(char *)malloc(50*sizeof(char));
				fscanf(file,"%s %s", word, new_word);
				word=(char *)realloc(word,strlen(word)*sizeof(char)); //realocare pentru a ocupa doar cata memorie este necesara
				new_word=(char *)realloc(new_word, strlen(new_word)*sizeof(char));
				replace(word,new_word);
			}

			if(strcmp(nume_operatie,"del")==0)
			{
				printf("Se executa operatia DELETE: ");
				delete(valoare_operatie);

			}
			if(strcmp(nume_operatie,"move")==0)
			{
				printf("Se executa operatia MOVE: ");
				move(valoare_operatie);
			}
			if(strcmp(nume_operatie,"copy")==0)
			{
				printf("Se executa operatia COPY: ");
				copy(valoare_operatie);
			}
			if(strcmp(nume_operatie,"paste")==0)
			{
				printf("Se executa operatia PASTE: ");
				insert_string(copy_string);
			}
			if(strcmp(nume_operatie,"backspace")==0)
			{
				printf("Se executa operatia BACKSPACE: ");
				backspace();
			}
			if(strcmp(nume_operatie,"undo")==0) //daca operatia citita este undo, se citeste capul stivei undo, aflandu-se astfel operatia anterioara
			{
				if(strcmp(stivaundo->head->op,"move")==0)
					move(-stivaundo->head->val);
				if(strcmp(stivaundo->head->op,"del")==0)
				{
					insert_string(deleted_string);
					move(-stivaundo->head->val);
				}
				if(strcmp(stivaundo->head->op,"backspace")==0)
					insert_string(deleted_backspace);
				if(strcmp(stivaundo->head->op,"paste")==0)
				{
					for(j=0;j<strlen(copy_string);j++) //aplic backspace de cate caractere are sirul inserat ori
						backspace();
				}
				if(strcmp(stivaundo->head->op,"insert")==0)
				{
					aux_backspace=(char *)malloc(sizeof(char)+1); 
					//retin sirul deleted_backspace care altfel se va pierde la executarea repetata a functiei backspace() 
					aux_backspace=deleted_backspace;
					for(j=0;j<strlen(stivaundo->head->string);j++)
						backspace();
					deleted_backspace=aux_backspace; //repun sirul initial deleted_backspace la locul lui.
				}
				//operatia pe care s-a executat undo este copiata in stiva de redo si apoi stearsa din stiva undo.
				insert_el_stiva(stivaredo,stivaundo->head->val,stivaundo->head->string,stivaundo->head->op,sizeof(stivaundo->head->string),sizeof(stivaundo->head->op));
				eliminare_el_stiva(stivaundo);
			}
			//operatiile corespunzatoare pentru redo. informatiile legate de operatii se preiau din capul stivei redo.
			if(strcmp(nume_operatie,"redo")==0)
			{
				if(strcmp(stivaredo->head->op,"move")==0)
					move(stivaredo->head->val);
				if(strcmp(stivaredo->head->op,"del")==0)
					delete(stivaredo->head->val);
				if(strcmp(stivaredo->head->op,"insert")==0) 
					insert_string(stivaredo->head->string);
				if(strcmp(stivaredo->head->op,"paste")==0)
					insert_string(copy_string);
				if(strcmp(stivaredo->head->op,"backspace")==0)
					backspace();
				eliminare_el_stiva(stivaredo); //este eliminat de fiecare data capul stivei redo.
			}
		}
	}
	//eliberam memoria string-urilor si stivelor pe care le-am folosit.
	free(copy_string);
	free(stivaredo);
	free(stivaundo);
	fclose(file);
}

//functie pentru a scrie in fisierul de output.
void scriere_output(char* fisier_output)
{
	FILE * file;
	element *parcurgator;
	int i;
	if((file=fopen(fisier_output,"w"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis!", fisier_output);
		exit(1);
	}
	else
	{
		parcurgator=list_head;
		while(parcurgator->next!=NULL) //verific daca parcurgator->next!=NULL pentru a nu insera in fisier si ultimul element adaugat manual.
		{
			fprintf(file,"%c",parcurgator->caracter);
			parcurgator=parcurgator->next;
		}
	}
	fclose(file);
}

int main(int argc, char *argv[])
{
	preluare_caractere(argv[1]);
	preluare_operatii(argv[2]);
	scriere_output(argv[3]);
	return 0;
}