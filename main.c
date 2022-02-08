#include "passport.h"
typedef
struct pass {
    char *first_name;
    char *last_name;
    int dob_Year;
    int dob_Month;
    int dob_Day;
    unsigned long int passport_id;
} Passport;

typedef struct {
    int (*comparePass)(const void* a,const void* b);
    Passport* data;
}funcpoint;

typedef struct ll_node_struct{
    unsigned long long id;
    struct ll_node_struct* next;
    struct ll_node_struct* previous;
    void* data;
}Node;
typedef struct ll_struct{
    unsigned long long id;
    struct ll_node_struct* head;
    struct ll_node_struct* tail;
    unsigned long long count;
}LinkedList;
typedef enum {FIRST_NAME, LAST_NAME} E_NAME;
#define MAX_DATE_LENGTH 12
#define ONE_ROUND_SCAN_SUCCESS 1
#define ERR_MSG "Sorry, the input is not valid! Please try again!"
#define ERR_MEM_ALLOC__PASSPORT "ERR_MEM_ALLOC__PASSPORT"
#define ERR_MEM_ALLOC__PASSPORT_NAME "ERR_MEM_ALLOC__PASSPORT_NAME"
#define ERR_MEM_ALLOC__BUFF "ERR_MEM_ALLOC__BUFFER"
#define BUFF_SIZE 4096
#define MAX_NAME_LENGTH 512
#define SCANF_NAME_PATTERN "%511[^\n]s"
#define MIN_NAME_LENGTH 2
#define ERR_MEM_ALLOC__NODE "ERR_MEM_ALLOC__NODE"
#define ERR_MEM_ALLOC__PASSPORT_NAME "ERR_MEM_ALLOC__PASSPORT_NAME"

void fatal_err (const char* msg ){
    perror(msg);
    exit(errno);
}
void free_passport(Passport* passport ){
    if (passport != NULL) {
        if (passport->first_name != NULL) free(passport->first_name);
        if (passport->last_name != NULL) free(passport->last_name);
        free(passport); passport = NULL;
    }
}
bool entry_err(const char* msg, char* buffer, Passport* p){
    if (buffer) memset(buffer,0, BUFF_SIZE);
    if (msg) fprintf(stdout, "%s\n", msg);
    free_passport(p);
    return false;

}
bool check_entry(const char* name){
    const int length = strnlen(name,MAX_NAME_LENGTH);
    if(!name) return false;
    if (length < MIN_NAME_LENGTH) return false;
    for (int i = 0; i < length; ++i) {
        if (!isalpha(name[i]) && !isdigit(name[i]) && !isspace(name[i])) return false;
    }
    return true;
}
Node* node_alloc(unsigned long long id, Passport* p){
 Node* node =(Node*) calloc(1, sizeof(Node));
    if (node==NULL) fatal_err(ERR_MEM_ALLOC__NODE);
    node->id = id;
    node->data = calloc(1, sizeof(Passport));
    if (node->data == NULL) fatal_err(ERR_MEM_ALLOC__PASSPORT);
    p = (Passport*) node->data;
    p->passport_id = id;
    return node;
}
void passport_node_free(Node* node){
    if (node == NULL) return;
    Passport* passport = (Passport*) node->data;
    if (passport != NULL) {
        char* name = (char*) passport->last_name;
        char* fname = (char*) passport->first_name;
        if (name != NULL) free(name);
        if(fname!=NULL) free(fname);
        free(passport);
    }
    free(node); node = NULL;
}
bool get_name(const char* mesg, char* buffer, Passport* p, E_NAME e){
    if (buffer == NULL) return false;
    char* name;
    char ch ='\0';
    if(mesg) printf("%s",mesg);
    if (scanf(SCANF_NAME_PATTERN, buffer) != ONE_ROUND_SCAN_SUCCESS || scanf("%c",&ch)
    != ONE_ROUND_SCAN_SUCCESS || !check_entry(buffer)) {
        return entry_err(ERR_MSG, buffer, p);
    }
    name = strndup(buffer,MAX_NAME_LENGTH);
    if (!name) return entry_err(ERR_MEM_ALLOC__PASSPORT_NAME , buffer, p);
    if (e == FIRST_NAME) p->first_name =name;
    else if (e== LAST_NAME) p->last_name = name;
    memset(buffer,0, BUFF_SIZE);
    return true;
}
bool check_DOB(const char* buffer) {
    if (buffer == NULL) return false;
    unsigned long long len = strnlen(buffer, MAX_DATE_LENGTH);
    for (int i = 0; i < len; ++i) {
        if (isdigit(buffer[i]) && ispunct(buffer[i])) return false;
    }
    return true;
}
bool get_DOB(const char* msg,char* buffer, Passport* p){
    if (buffer == NULL) return false;
    if(msg) printf("%s", msg);
    fgets(buffer,MAX_DATE_LENGTH, stdin);
    if(!check_DOB(buffer)) return entry_err(ERR_MSG,buffer,p);
    sscanf(buffer,"%d %*c %d %*c %d",&p->dob_Year,&p->dob_Month,&p->dob_Day);
    if(p->dob_Month < 1 || p->dob_Month > 12 || p->dob_Day < 1 || p->dob_Day >31) return entry_err(ERR_MSG,buffer,p);
    memset(buffer,0,BUFF_SIZE);
    return true;
}
void get_data(Passport* passport, char* buffer, unsigned long long id){
    if(buffer== NULL) exit(EXIT_FAILURE);
    printf("Issuing a passport with id:%llu\n",id);
    while (true) {
        Passport *p = (Passport*) calloc(ONE_ROUND_SCAN_SUCCESS, sizeof(Passport));
        if (!p) fatal_err(ERR_MEM_ALLOC__PASSPORT);
        p->passport_id = id;
        if (!get_name("\t\tPlease enter the first name of the applicant:", buffer, p, FIRST_NAME))continue;
        if (!get_name("\t\tPlease enter the last name of the applicant:", buffer, p, LAST_NAME))continue;
        if(!get_DOB("\t\tPlease enter the applicant's date of birth (yyyy/mm/dd):",buffer,p))continue;
        passport->dob_Day= p->dob_Day;
        passport->dob_Month = p->dob_Month;
        passport->dob_Year =p->dob_Year;
        passport->last_name = p->last_name;
        passport->first_name = p->first_name;
        passport->passport_id =p->passport_id;
        return;

    }

}
int compare_first_names_asc(const void* a, const void* b) {
    char *f_name = (*(Passport **) a)->first_name;
    char *f_name2 = (*(Passport **) b)->first_name;
    return strcmp(f_name2, f_name);
}
int compare_last_names_asc(const void* a, const void* b) {
    char *l_name = (*(Passport **) a)->last_name;
    char *l_name2 = (*(Passport **) b)->last_name;
    return strcmp(l_name2, l_name);
}

int compare_first_names(const void* a, const void* b){
    char *f_name= (*(Passport**) a)->first_name;
    char *f_name2= (*(Passport**) b)->first_name;
    return strcmp(f_name,f_name2);
}
int compare_last_names(const void* a, const void* b){
    char *l_name= (*(Passport**) a)->last_name;
    char *l_name2= (*(Passport**) b)->last_name;
    return strcmp(l_name, l_name2);
}
int compare_id_asc(const void* a, const void* b){
    unsigned long long num1 = (*(Passport**) a)->passport_id;
    unsigned long long num2 = (*(Passport**) b)->passport_id;
    if (num1 > num2) return 1;
    if (num1==num2) return 0;
    else return -1;
}
int compare_id_desc(const void* a, const void* b){
    unsigned long long num1 = (*(Passport**) a)->passport_id;
    unsigned long long num2 = (*(Passport**) b)->passport_id;
    if (num1 > num2) return -1;
    if (num1==num2) return 0;
    else return 1;
}

int main() {

    char* buff =(char*) calloc(BUFF_SIZE+1, sizeof (char));
    if (buff== NULL) fatal_err(ERR_MEM_ALLOC__BUFF);
    unsigned long long num_of_passp;
    printf("How many passport you like to generate: ");
    scanf("%llu",&num_of_passp);
    Passport** passports_ppoint = (Passport**) calloc(num_of_passp, sizeof (Passport*));
    if(passports_ppoint == NULL){
        free(buff);
        fatal_err(ERR_MEM_ALLOC__PASSPORT);
    }
   LinkedList linked;
   linked.id = 100L;
   Node* pre = NULL; Node* nex =NULL; Node* curr= NULL;
   unsigned long long base = 0;
    printf("Please enter the base ID for your Passports: ");
     scanf("%llu",&base);
     getchar();
    for(int i= 0; i<num_of_passp; pre = curr, linked.count++,i++)
    {
        curr = node_alloc(base+(i), passports_ppoint[i]);
        linked.tail = curr;
        if (i == 0) linked.head = curr;
        curr->previous = pre;
        if (pre) pre->next = curr;
        get_data(curr->data, buff,base+(i));
        passports_ppoint[i] = (Passport*)curr->data;
    }
    funcpoint pass_sort;
    int sorting_option;
    struct tm t;
    time_t birthtime;
    t.tm_min = 0;
    t.tm_hour =0;
    t.tm_sec =0;

    printf("\nBEFORE SORT:\n");
    for(curr=linked.head; curr != NULL; curr= curr->next){
        Passport* passport = curr->data;
        t.tm_year = passport->dob_Year -1900;
        t.tm_mon = passport->dob_Month -1;
        t.tm_mday = passport->dob_Day;
        birthtime = mktime(&t);
        printf("Passport id %lu belongs to %s %s with the date of birth: %d-%d-%d", passport->passport_id, passport->first_name
        ,passport->last_name, passport->dob_Year, passport->dob_Month,passport->dob_Day);
        printf(" (%s)\n",ctime(&birthtime));
    }
    printf("How would you like to sort your function?\nEnter 1 for First Name\n"
           "Enter 2 for Last Name\nEnter 3 to sort by id:\n");
    scanf("%d",&sorting_option);
    if (sorting_option != 1 && sorting_option != 2 && sorting_option !=3){
        printf("Sorry not a valid option, extiting...");
        exit(EXIT_FAILURE);
    }
    if (sorting_option == 1){
        printf("Enter 1 for ascending\nEnter 2 for descending:\n");
        scanf("%d",&sorting_option);
        if (sorting_option != 1 && sorting_option != 2){
            printf("Sorry not a valid option, extiting...");
            exit(EXIT_FAILURE);
        }
        if(sorting_option == 1)pass_sort.comparePass =&compare_first_names_asc;
        if(sorting_option == 2)pass_sort.comparePass =&compare_first_names;
    }
    if (sorting_option == 2) {
        printf("Enter 1 for ascending\nEnter 2 for descending:\n");
        scanf("%d",&sorting_option);
        if (sorting_option != 1 && sorting_option != 2){
            printf("Sorry not a valid option, extiting...");
            exit(EXIT_FAILURE);
        }
        if(sorting_option == 1)pass_sort.comparePass =&compare_last_names_asc;
        if(sorting_option == 2)pass_sort.comparePass =&compare_last_names;

    }
    if(sorting_option == 3)
    {
        printf("Enter 1 for ascending\nEnter 2 for descending:\n");
        scanf("%d",&sorting_option);
        if (sorting_option != 1 && sorting_option != 2){
            printf("Sorry not a valid option, extiting...");
            exit(EXIT_FAILURE);
        }
        if(sorting_option == 1) pass_sort.comparePass = &compare_id_asc;
        if(sorting_option == 2) pass_sort.comparePass = &compare_id_desc;

    }

    qsort(passports_ppoint, num_of_passp, sizeof(Passport*), pass_sort.comparePass);

    printf("AFTER SORT:\n");
    for (int i = 0; i < num_of_passp; i++) {
        t.tm_year = passports_ppoint[i]->dob_Year -1900;
        t.tm_mon = passports_ppoint[i]->dob_Month -1;
        t.tm_mday = passports_ppoint[i]->dob_Day;
        birthtime = mktime(&t);
        printf("Passport id %lu belongs to %s %s with the date of birth: %d-%d-%d", passports_ppoint[i]->passport_id, passports_ppoint[i]->first_name
                , passports_ppoint[i]->last_name, passports_ppoint[i]->dob_Year, passports_ppoint[i]->dob_Month, passports_ppoint[i]->dob_Day);
        printf(" (%s)\n",ctime(&birthtime));
    }

    free(pass_sort.data);
    pass_sort.data = NULL;

    for (int i = 0; i < num_of_passp ; i++) free_passport(passports_ppoint[i]);
    free(passports_ppoint); passports_ppoint = NULL;
    free(buff); buff = NULL;

    for(curr = linked.head; curr != NULL; curr = nex){
        nex = curr->next;
        passport_node_free(curr);
    }
}
