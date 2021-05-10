#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*#include <time.h>*/

struct relazione {
	char *id_rel;
	struct entita *to;
	struct relazione *prev;
	struct relazione *next;
};
typedef struct relazione *ptrel;

struct entita {
	char *id_entita;
	ptrel relaz;
	ptrel last_relaz;
	struct entita *prev;
	struct entita *next;
};
typedef struct entita *ptr;


struct report_struct {
	char *rel_name;
	int count;
	struct report_struct *prev;
	struct report_struct *next;
};
typedef struct report_struct *report_struct_type;

int in_array(char elem[], char *array[], int num_elements);
void inserisci_entita(char *name);
ptr ricerca_entita(char *name);
void rimuovi_entita(char *name);
void inserisci_relazione(char *from, char *to, char *type);
void rimuovi_relazione(char *from, char *to, char *type);
void add_in_report(char *rel_name, char *to);
void remove_from_report(char *rel_name, char *to);
void print_report();
/*void dump_report();*/
void swap(char **ptr1, char **ptr2);
void remove_entity_from_report(char *entity);
/*void dump_entities();
void dump_relationships(ptr source);*/
void delete_all_relationships(ptr source);
ptr entities = NULL;
ptr last_entity = NULL;
report_struct_type report = NULL;
report_struct_type ptr_last_report = NULL;

int main() {
/*	clock_t start, end;
	double cpu_time_used;
	start = clock();*/

	char op[7];
	char name[50];
	char to[50];
	char type [50];
	while(1) {
		scanf("%s", op);
		if((strcmp(op,"end")) == 0) {
			break;
		}
		else if((strcmp(op,"addent")) == 0) {
			scanf("%s%*c", name);
			inserisci_entita(name);
		}
		else if((strcmp(op,"delent")) == 0) {
			scanf("%s%*c", name);
			rimuovi_entita(name);
		}
		else if((strcmp(op,"addrel")) == 0) {
			scanf("%s %s %s%*c", name, to, type);
			inserisci_relazione(name, to, type);
		}
		else if((strcmp(op,"delrel")) == 0) {
			scanf("%s %s %s%*c", name, to, type);  //name=mittente, to=destinatario, type=id_rel
			rimuovi_relazione(name, to, type);
		}
		else if((strcmp(op,"report")) == 0) {
			print_report();
		}
		/*else if((strcmp(op,"dumpent")) == 0) {
			dump_entities();
		}
		else if((strcmp(op,"dumprep")) == 0) {
			dump_report();
		}*/
	}
/*	end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Exec time: %f secs\n", cpu_time_used);*/
	return 0;
}

void inserisci_entita(char *name) {
	if(entities == NULL) {
		ptr temp = malloc(sizeof(struct entita));
		temp->id_entita = malloc(strlen(name)+1);
		strcpy(temp->id_entita, name);
		temp->prev = NULL;
		temp->next = NULL;
		temp->relaz = NULL;
		entities = temp;
		last_entity = temp;
	}
	else {
		ptr check = ricerca_entita(name);
		if(check == NULL) {
			ptr temp = malloc(sizeof(struct entita));
			temp->id_entita = malloc(strlen(name)+1);
			strcpy(temp->id_entita, name);
			temp->next = NULL;
			temp->relaz = NULL;
			temp->prev = last_entity;
			last_entity->next = temp;
			last_entity = temp;
		}
	}
}

ptr ricerca_entita(char *name) {
	for(ptr p = entities; p != NULL; p = p->next) {
		if((strcmp(p->id_entita,name)) == 0) {
			return p;
		}
	}
	return NULL;
}

void rimuovi_entita(char *name) {
	ptr todel;
	todel = ricerca_entita(name);
	if(todel == NULL)
		return;
	else {
		if(todel == entities) {
			entities = todel->next;
			entities->prev = NULL;
		}
		else if(todel == last_entity) {
			last_entity = last_entity->prev;
			last_entity->next = NULL;
		}
		else {
			ptr prev = todel->prev;
			ptr next = todel->next;
			prev->next = next;
			next->prev = prev;
		}

		for(ptrel tmp = todel->relaz; tmp != NULL; tmp = tmp->next) {
			remove_from_report(tmp->id_rel, tmp->to->id_entita);
		}
		remove_entity_from_report(todel->id_entita);

		int i = 0;
		ptrel reltodel[128];
		for(ptr tmp = entities; tmp != NULL; tmp = tmp->next) {
			for(ptrel rel = tmp->relaz; rel != NULL; rel = rel->next) {
				if(rel->to == todel) {
					reltodel[i] = rel;
					i++;
					if(rel == tmp->relaz && rel == tmp->last_relaz) {
						tmp->relaz = NULL;
						tmp->last_relaz = NULL;
					}
					else if(rel == tmp->relaz) {
						tmp->relaz = rel->next;
						tmp->relaz->prev = NULL;
					}
					else if(rel == tmp->last_relaz) {
						tmp->last_relaz = rel->prev;
						tmp->last_relaz->next = NULL;
					}
					else {
						rel->prev->next = rel->next;
						rel->next->prev = rel->prev;
					}
				}
			}
		}

		for(int j = 0; j < i; j++) {
			free(reltodel[j]->id_rel);
			free(reltodel[j]);
		}

		delete_all_relationships(todel);
		free(todel->id_entita);
		free(todel);
	}
}

void delete_all_relationships(ptr source) {
	ptrel todel = NULL;
	for(ptrel tmp = source->relaz; tmp != NULL; ) {
		todel = tmp;
		tmp = tmp->next;
		free(todel->id_rel);
		free(todel);
	}
	source->relaz = NULL;
}

/*void dump_relationships(ptr source) {
	for(ptrel tmp = source->relaz; tmp! = NULL; tmp = tmp->next) {
		printf("- %s %s\n", tmp->id_rel, tmp->to->id_entita);
	}
	printf("---\n");
}*/

void remove_entity_from_report(char *entity) {
	report_struct_type todel[128];
	int i = 0;
	for(report_struct_type tmp = report; tmp != NULL; tmp = tmp->next) {
		if(strstr(tmp->rel_name, entity) != NULL) {
			if(tmp == report && tmp == ptr_last_report) {
				report = NULL;
				ptr_last_report = NULL;
			}
			else if(tmp == report) {
				report = report->next;
				report->prev = NULL;
			}
			else if(tmp == ptr_last_report) {
				ptr_last_report = ptr_last_report->prev;
				ptr_last_report->next = NULL;
			}
			else {
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;
			}
			todel[i] = tmp;
			i++;
		}
	}
	for(int j = 0; j < i; j++) {
		free(todel[j]->rel_name);
		free(todel[j]);
	}
}

void inserisci_relazione(char *from, char *to, char *type) {
	ptr source = ricerca_entita(from);
	ptr dest = ricerca_entita(to);
	if(source != NULL && dest != NULL) {
		if(source->relaz == NULL) {	//mittente senza relazioni
			ptrel rel = malloc(sizeof(struct relazione));
			rel->id_rel = malloc(strlen(type)+1);
			strcpy(rel->id_rel,type);
			rel->next = NULL;
			rel->prev = NULL;
			rel->to = dest;
			source->relaz = rel;
			source->last_relaz = rel;
		}
		else { //mittente con relazioni
			for(ptrel tmp = source->relaz; tmp != NULL; tmp=tmp->next) {
				if(tmp->to == dest && strcmp(tmp->id_rel,type) == 0) {
					return;
				}
			}

			ptrel rel = malloc(sizeof(struct relazione));
			rel->id_rel = malloc(strlen(type)+1);
			strcpy(rel->id_rel,type);
			rel->next = NULL;
			rel->to = dest;
			rel->prev = source->last_relaz;
			source->last_relaz->next = rel;
			source->last_relaz = rel;
		}
		add_in_report(type, to);
	}
}

void add_in_report(char *rel_name, char *to) {
	char *key = malloc(strlen(to)+strlen(rel_name)+2);
	sprintf(key, "%s$%s", rel_name, to);
	if(report == NULL) {
		report = malloc(sizeof(struct report_struct));
		report->rel_name = malloc(strlen(key)+1);
		strcpy(report->rel_name, key);
		report->prev = NULL;
		report->next = NULL;
		report->count = 1;
		ptr_last_report = report;
	}
	else {
		for(report_struct_type tmp = report; tmp != NULL; tmp = tmp->next) { //cerco nelle relazioni
			int compare = strcmp(tmp->rel_name, key);
			if(compare < 0) {
				continue;
			}
			else if(compare == 0) { //match
				tmp->count++;
				free(key);
				return;
			}
			else { //chiave analizzata > chiave da inserire
				report_struct_type middleptr = malloc(sizeof(struct report_struct));
				middleptr->rel_name = malloc(strlen(key)+1);
				strcpy(middleptr->rel_name, key);
				middleptr->count = 1;

				if(tmp == report) { //è il primo elemento
					middleptr->next = tmp;
					middleptr->prev = NULL;
					tmp->prev = middleptr;
					report = middleptr;
				}
				else {
					middleptr->next = tmp;
					middleptr->prev = tmp->prev;
					tmp->prev->next = middleptr;
					tmp->prev = middleptr;
				}
				free(key);
				return;
			}
		}
		report_struct_type tmp = malloc(sizeof(struct report_struct));
		tmp->rel_name = malloc(strlen(key)+1);
		strcpy(tmp->rel_name, key);
		tmp->count = 1;
		tmp->next = NULL;
		tmp->prev = ptr_last_report;
		ptr_last_report->next = tmp;
		ptr_last_report = tmp;
	}
	free(key);
}

void rimuovi_relazione(char *from, char *to, char *type) {
	ptr source = ricerca_entita(from);
	ptr dest = ricerca_entita(to);
	if(source != NULL && dest != NULL) {
		for(ptrel tmp = source->relaz; tmp != NULL; tmp = tmp->next) {
			if(tmp->to == dest && strcmp(tmp->id_rel, type) == 0) {
				if(tmp == source->relaz && tmp == source->last_relaz) {
					source->relaz = NULL;
					source->last_relaz = NULL;
				}
				else if(tmp == source->relaz) {
					source->relaz = tmp->next;
					source->relaz->prev = NULL;
				}
				else if(tmp == source->last_relaz) {
					source->last_relaz = tmp->prev;
					source->last_relaz->next = NULL;
				}
				else {
					tmp->next->prev = tmp->prev;
					tmp->prev->next = tmp->next;
				}
				free(tmp->id_rel);
				free(tmp);
				remove_from_report(type, to);
				break;
			}
		}
	}
}

void remove_from_report(char *rel_name, char *to) {
	char *key = malloc(strlen(to)+strlen(rel_name)+2);
	sprintf(key, "%s$%s", rel_name, to);
	for(report_struct_type tmp = report; tmp != NULL; tmp = tmp->next) { //cerco nelle relazioni
		if(strcmp(tmp->rel_name, key) == 0 ) { //match
			if(tmp->count > 1) {
				tmp->count--;
			}
			else {
				report_struct_type prev = tmp->prev;
				report_struct_type next = tmp->next;
				if(tmp == report && tmp == ptr_last_report) {
					report = NULL;
					ptr_last_report = NULL;
				}
				else if(tmp == report) {
					next->prev = NULL;
					report = next;
				}
				else if(tmp == ptr_last_report) {
					prev->next = NULL;
					ptr_last_report = prev;
				}
				else {
					prev->next = next;
					next->prev = prev;
				}
				free(tmp->rel_name);
				free(tmp);
			}
			break;
		}
	}
	free(key);
}

/*void dump_report() {
	if(report == NULL) {
		printf("Report vuoto\n\n");
		return;
	}
	printf("HEAD %s\n", report->rel_name);
	printf("LAST %s\n", ptr_last_report->rel_name);
	for(report_struct_type tmp = report; tmp != NULL; tmp = tmp->next) {
		printf("%s %d\n", tmp->rel_name, tmp->count);
	}
	printf("---\n\n");
}

void dump_entities() {
	for(ptr tmp = entities; tmp != NULL; tmp = tmp->next) {
		printf(">>> %s \n", tmp->id_entita);
	}
}*/

void print_report() {
	if(report == NULL) printf("none\n");
	else {
		int x = 0;
		int i;
		char *rel_tmpstr = NULL;
		char *rels[50];
		char *dest[50];
		char *rel = NULL;
		char *tmpstr = NULL;
		char *dest_tmpstr = NULL;
		char *tmpstr2 = NULL;
		int max = 0;

		for(report_struct_type tmp = report; tmp != NULL; tmp = tmp->next) {
			i = 0;
			tmpstr = malloc(strlen(tmp->rel_name)+1);
			strcpy(tmpstr, tmp->rel_name);
			rel = strtok(tmpstr,"$");
			rel_tmpstr = malloc(strlen(rel)+1);
			strcpy(rel_tmpstr, rel);
			dest_tmpstr = strtok(NULL, "$");
			max = tmp->count;
			if(in_array(rel_tmpstr, rels, x) == 0) {
				dest[i] = malloc(strlen(dest_tmpstr)+1);
				strcpy(dest[i], dest_tmpstr);
				for(report_struct_type tmp2 = report; tmp2 != NULL; tmp2 = tmp2->next) {
					if(tmp == tmp2) continue;
					if(strncmp(tmp2->rel_name, rel_tmpstr, strlen(rel_tmpstr)) == 0) { //match relazione
						tmpstr2 = malloc(strlen(tmp2->rel_name)+1);
						strcpy(tmpstr2, tmp2->rel_name);
						strtok(tmpstr2,"$");
						dest_tmpstr = strtok(NULL, "$");
						if(tmp2->count > max) {
							for(int j = 0; j <= i; j++) free(dest[j]);
							i = 0;
							max = tmp2->count;
						}
						else if(tmp2->count == max) {
							i++;
						}
						else if(tmp2->count < max) {
							free(tmpstr2);
							continue;
						}
						dest[i] = malloc(strlen(dest_tmpstr)+1);
						strcpy(dest[i], dest_tmpstr);
						free(tmpstr2);
					}
				}

				rels[x] = malloc(strlen(rel_tmpstr)+1);
				strcpy(rels[x], rel_tmpstr);
				x++;
				//sort dest array
				for(int k = 0; k <= i; k++) {
					for(int v = 0; v <= i; v++) {
						if(strcmp(dest[k], dest[v]) < 0) {
							swap(&dest[k], &dest[v]);
						}
					}
				}
				printf("%s ", rel_tmpstr);
				for(int k = 0; k <= i; k++) {
					printf("%s ", dest[k]);
					free(dest[k]);
				}
				printf("%d; ", max);
			}
			free(tmpstr);
			free(rel_tmpstr);
		}
		for(int w = 0; w < x; w++) free(rels[w]);
		printf("\n");
	}
}

int in_array(char elem[], char *array[], int num_elements) {
	for(int i = 0; i < num_elements; i++) {
		if(strcmp(elem, array[i]) == 0) return 1;
	}
	return 0;
}

void swap(char **ptr1, char **ptr2) {
    char *temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}
