#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "inttypes.h"

int add_last(void **arr, int *len, data_structure *data)
{	if(len<0)
		return 1;
		
	char* i = (char*)(*arr); 
	int nrb = 0, len_data = data->header->len + 5;
	// daca arr este gol, ii aloc memorie pentru ce e in data
	if(*arr == NULL || len == 0) {
		*arr = malloc(len_data);
	} else { // daca arr nu este gol, il parcug pana la final si apoi ii realoc memorie
		for(int pos = 0; pos < *len; pos++) {
			int* lenc = (int*)(i+1);
			i+= *lenc+5;
			nrb = nrb + *lenc + 5;
		}
		*arr = realloc(*arr, nrb + len_data);
	}
	// copiez la finalul lui arr ceea ce se gasea in data 
	memcpy(*arr+nrb, &data->header->type, 1);
	nrb++;
	memcpy(*arr+nrb, &data->header->len, 4);
	nrb+=4;
	memcpy(*arr+nrb, data->data, data->header->len);

	(*len)++; // actualizez len 
	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{	if(*len < 0)
		return 1;

	char* i = (char*)(*arr);
	int nrb = 0, len_data = data->header->len + 5, nrb_prev = 0;


	if(index >= *len) { // daca trebuie adaugat data la finalul lui arr
		add_last(&(*arr), &(*len), data);
		return 0;
	} else { // parcurg arr pana la final, pentru a afla nr de acteti pentru a putea realoca memorie
		for(int pos = 0; pos < *len; pos++) {
			// retinerea pozitiei pe care trebuie introdus noua structura
			int* lenc = (int*)(i+1);
			i+= *lenc+5;
			if(pos < index){
				nrb_prev+= *lenc+5;
			}
			nrb = nrb + *lenc + 5;
		}
		*arr = realloc(*arr, nrb + len_data);
	}

	// fac loc pentru a introduce data in arr
	memcpy(*arr+nrb_prev+data->header->len+5, *arr+nrb_prev, nrb - nrb_prev);
	
	// copiez noua structura acolo unde i-am facut loc
	memcpy(*arr+nrb_prev, &data->header->type, 1);
	nrb_prev++;
	memcpy(*arr+nrb_prev, &data->header->len, 4);
	nrb_prev+=4;
	memcpy(*arr+nrb_prev, data->data, data->header->len);

	(*len)++; // iterez len
	return 0;
}

void find(void *data_block, int len, int index) 
{	if(index >= len || index < 0)
		return;

	char* i = (char*)(data_block);

	// parcurg arr pana la pozitia la care trebuie sa afisez
	for(int poz = 0; poz < index; poz++) {
		int *lenc = (int*)(i+1);
		i+= *lenc+5;
	}

	int type = 0;
	int8_t *suma1_8 = 0, *suma2_8 = 0;
	int16_t *suma_16 = 0;
	int32_t *suma_32 = 0, *suma1_32 = 0, *suma2_32 = 0;

	// afisare tip
		if(*i == '1')
			type = 1;
		else if(*i == '2')
			type = 2;
		else 
			type = 3;

		printf("Tipul %d\n", type);

		i+=5;

		// afisarea primului nume
		while(*i != '\0') {
			printf("%c", *i);
			i++;
		}
		i++;
		printf(" pentru ");

		if(type == 1) {
			suma1_8 = (int8_t*)(i);
			i++;
			suma2_8 = (int8_t*)(i);
			i++;
		} 
		  if(type == 2) {
			suma_16 = (int16_t*)(i);
			i+=2;
			suma_32 = (int32_t*)(i);
			i+=4;
		  } 
		   if(type == 3) {
			suma1_32 = (int32_t*)(i);
			i = i+4;
			suma2_32 = (int32_t*)(i);
			i = i+4;
		  }
		// afisarea celui de-al doilea nume
		while(*i != '\0') {
			printf("%c", *i);
			i++;
		}
		printf("\n");

		// afisarea sumelor
		if(type == 1) { 
			printf("%"PRId8"\n%"PRId8"", *suma1_8, *suma2_8);
		} if(type == 2) {
			printf("%"PRId16"\n%"PRId32"", *suma_16, *suma_32);
		} if(type == 3) {
			printf("%"PRId32"\n%"PRId32"", *suma1_32, *suma2_32); 
		}
		printf("\n\n");
}

int delete_at(void **arr, int *len, int index)
{	if(*len < 0 || index >= *len || *arr == NULL)
		return 1;

	// daca trebuie sters ultimul element din arr
	if(*len-1 == index) {
		(*len)--;
		return 0;
	}

	char* i = (char*)(*arr);
	int nrb = 0, nrb_prev = 0, len_data = 0;
	// gasesc pozitia structurii care trebuie sterse
	for(int pos = 0; pos < *len; pos++) {
			// retinerea pozitiei pe care trebuie introdus noua structura
			int* lenc = (int*)(i+1);
			i+= *lenc+5;
			if(pos <= index){
				nrb_prev+= *lenc+5;
				len_data = *lenc+5;
			}
			nrb = nrb + *lenc + 5;
		}

	// mut urmatoareele structuri cu o pozitie la stanga
	memcpy(*arr+nrb_prev-len_data, *arr+nrb_prev, nrb-nrb_prev);

	(*len)--; // decrementez len
	return 0;
}

void Print(void *arr, int len) {
	// folosesc functia de find pentru a face o afisare mai usoara
	int i;
	for(i = 0; i<len; i++) {
		find(arr, len, i);
	}
}

int main() {
	void *arr = NULL;
	int len = 0;
	char s[1001];
	// cat timp mai am instructiuni de citit
	while(fgets(s, sizeof(s), stdin)){ 
		// daca suntem in cazul in care avem exit
		if(strstr(s, "exit") != 0) {
			free(arr);
			break;
		}
		// daca suntem in cazul in care avem print
		if(strstr(s, "print") != 0) {
			Print(arr, len);
		}
		// daca suntem in cazul in care avem cele doua tipuri de insert
		if(strstr(s, "insert") != 0) {
			data_structure *data = (data_structure*) malloc(sizeof(data_structure));
			data->header = (head*) malloc(sizeof(head));
			int8_t suma81, suma82; int16_t suma16; int32_t suma321, suma322;
			int sz_nume1 = 0, sz_nume2 = 0, sz_max = 0, caz = 0, pozitie = 0, type = 0;
			char* ptr = strtok(s, " ");

			// daca avem cazul cu insert_at caz = 1 si caz = 0 in caz contrar
			if(strcmp(ptr, "insert_at") == 0) {
				caz = 1;
				ptr = strtok(NULL, " \n");
				pozitie = atoi(ptr);
			}
			ptr = strtok(NULL, " \n");
			
			// retinerea tipului 
			if(atoi(ptr) == 1) 
				type = 1;
			else if(atoi(ptr) == 2) 
					type = 2;
				 else 
					type = 3;
			
			ptr = strtok(NULL, " \n"); // primul nume 
			char *nume1 = (char*) malloc((strlen(ptr)+1) * sizeof(char));
			strcpy(nume1, ptr);
			nume1[strlen(ptr) + 1] = '\0';
			sz_nume1 = strlen(ptr)+1;

			sz_max += sz_nume1;
			ptr = strtok(NULL, " \n"); 

			if(type == 1) { // sumele de tipul 1
				suma81 = (int8_t)atoi(ptr); // prima suma
				sz_max++;
				ptr = strtok(NULL, " "); 
				suma82 = (int8_t)atoi(ptr);// a doua suma
				sz_max++;
			} else if(type == 2) { // sumele de tipul 2
				suma16 = (int16_t)atoi(ptr); // prima suma
				sz_max+= 2;
				ptr = strtok(NULL, " ");
				suma322 = (int32_t)atoi(ptr); // a doua suma
				sz_max+= 4;
			} else { // sumele de tipul 3
				suma321 = (int32_t)atoi(ptr); // prima suma
				sz_max+= 4;
				ptr = strtok(NULL, " ");
				suma322 = (int32_t)atoi(ptr); // a doua suma
				sz_max+= 4;
			}
			
			ptr = strtok(NULL, " \n"); // al doilea nume 
			char *nume2 = (char*) malloc((strlen(ptr)+1) * sizeof(char));
			strcpy(nume2, ptr);
			sz_nume2 = strlen(ptr)+1;
			nume2[strlen(ptr) + 1] = '\0';
			sz_max += sz_nume2;

			// alocarea memoriei pt data
			data->data = malloc(sz_max);
			data->header->type = (type + '0');
			data->header->len = sz_max;

			// copierea datelor in data 
			memcpy(data->data, nume1, sz_nume1);
			int pos = sz_nume1;
			free(nume1);

			if(type == 1) {
				memcpy((data->data)+pos, &suma81, 1);
				pos++;
				memcpy((data->data)+pos, &suma82, 1);
				pos++;
			} else if(type == 2) {
				memcpy((data->data)+pos, &suma16, 2);
				pos+=2;
				memcpy((data->data)+pos, &suma322, 4);
				pos+=4;
			} else {
				memcpy((data->data)+pos, &suma321, 4);
				pos+=4;
				memcpy((data->data)+pos, &suma322, 4);
				pos+=4;
			}

			memcpy(data->data+pos, nume2, sz_nume2);
			free(nume2);

			if(caz == 0) {
				add_last(&arr, &len, data);
			} else {
				add_at(&arr, &len, data, pozitie);
			}

			// dezalocare memorie
			free(data->header); free(data->data); 
			free(data);
		}

		// daca suntem in cazul in care avem find
		if(strstr(s, "find") != 0) {
			void* ptr = strtok(s, " ");
			ptr = strtok(NULL, "\n"); // retinerea pozitiei pe care trebuie sa o afisam
			int poz = atoi(ptr);
			find(arr, len, poz);
		}

		// daca suntem in cazul in care avem delete
		if(strstr(s, "delete_at")!=0) {
			void* ptr = strtok(s, " ");
			ptr = strtok(NULL, "\n");
			int poz = atoi(ptr); // retinerea pozitiei de la care stergem
			delete_at(&arr, &len, poz);
		}
	}
	return 0;
}
