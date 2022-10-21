#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IDEI:
// Functia de comparare nume ca sa nu mai modifice numele din lista
//verificare input cantitate = 0 sau < 0

typedef struct structura
{
    int id_produs;
    char name_produs[20];
    int cant_produs;
    float pret_produs;
    struct structura *urmator;
} STR; // p q cap_lista

typedef struct cumparaturi
{
    int id_produs;
    char nume_produs[10];
    int cantitate;
    struct cumparaturi *urmator;
} CUMP; // cump1 cump2 cmp_lista

int name_compare(char *name, char *name2)
{
    // exluding the input mistake by uppering both strings
    for (int i = 0; name[i] != NULL; i++)
    {
        if (name[i] >= 'a' && name[i] <= 'z')
            name[i] = name[i] - 32;
        if (name2[i] >= 'a' && name2[i] <= 'z')
            name2[i] = name2[i] - 32;
    }

    // comparing the strings
    for (int i = 0; i < strlen(name2); i++)
    {
        if (name[i] != name2[i])
        {
            // printf("Product not found!\nPlease try again.");
            return 0;
        }
    }
    return 1;
}

void Verify_file(FILE *file)
{
    if (file == NULL)
    {
        printf("Stock file could not open properly!\nPlease check the file and try again.");
        exit(-1);
    }
}

void deleteNode_ID(CUMP **head_ref, int key)
{
    // Store head node
    CUMP *temp = *head_ref, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->id_produs == key)
    {
        *head_ref = temp->urmator; // Changed head
        free(temp);                // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->id_produs != key)
    {
        prev = temp;
        temp = temp->urmator;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->urmator = temp->urmator;

    free(temp); // Free memory
}

void deleteNode_NAME(CUMP **head_ref, char *key)
{
    // Store head node
    CUMP *temp = *head_ref, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && name_compare(temp->nume_produs, key))
    {
        *head_ref = temp->urmator; // Changed head
        free(temp);                // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && !name_compare(temp->nume_produs, key))
    {
        prev = temp;
        temp = temp->urmator;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->urmator = temp->urmator;

    free(temp); // Free memory
}

void append(CUMP **head_ref, int new_id, int new_cantitate, char nume[])
{
    /* 1. allocate node */
    CUMP *new_node = (CUMP *)malloc(sizeof(CUMP));

    CUMP *last = *head_ref; /* used in step 5*/

    /* 2. put in the data  */
    new_node->id_produs = new_id;
    new_node->cantitate = new_cantitate;
    strcpy(new_node->nume_produs, nume);

    /* 3. This new node is going to be the last node, so make next
          of it as NULL*/
    new_node->urmator = NULL;

    /* 4. Else traverse till the last node */
    while (last->urmator != NULL)
        last = last->urmator;

    /* 5. Change the next of last node */
    last->urmator = new_node;
    return;
}

int name_or_id(char input[]) // if the user input is ID or name
{
    if (input[0] >= '0' && input[0] <= '9')
    {
        return 1;
    }
    return 0;

    // 1 -> ID, 0 -> Product_name
}

void main()
{
    FILE *stock_input;
    stock_input = fopen("stock.txt", "rt");
    Verify_file(stock_input);

    // Lista stoc
    STR *cap_lista, *p, *q;
    int no_stock_products = 0, nr;
    int i;

    p = (STR *)malloc(sizeof(STR));
    if (p == NULL)
    {
        printf("Alocare esuata");
        exit(-1);
    }

    fscanf(stock_input, "%d", &p->id_produs);
    fscanf(stock_input, "%s", &p->name_produs);
    fscanf(stock_input, "%d", &p->cant_produs);
    fscanf(stock_input, "%f", &p->pret_produs); // (*p).valoare
    p->urmator = NULL;
    cap_lista = p;

    while (!feof(stock_input))
    {
        q = (STR *)malloc(sizeof(STR));
        if (q == NULL)
        {
            printf("Alocarea a esuat");
            exit(-1);
        }
        // printf("Tastati id ul produsului, cantitatea si pretul: ", i + 1);
        fscanf(stock_input, "%d", &(q->id_produs));
        fscanf(stock_input, "%s", (q->name_produs));
        fscanf(stock_input, "%d", &(q->cant_produs));
        fscanf(stock_input, "%f", &(q->pret_produs));
        q->urmator = NULL;
        p->urmator = q;
        p = q;
        no_stock_products++;
    }
    no_stock_products++;

    for (p = cap_lista, i = 1; p != NULL; p = p->urmator, i++)
    {
        printf("Elementul cu id ul: %d si numele: %s se gaseste in cantitatea: %d la pretul %.2f EUR\n", p->id_produs, p->name_produs, p->cant_produs, p->pret_produs);
    }

    // Lista cos cumparaturi
    CUMP *cmp_lista, *cump1, *cump2;
    int numar_prod_cump = 0, j;

    // MODIFICA ASTA CA SA NU MAI CEARA NUMAR DE PRODUSE
    //  printf("Scrieti cate tipuri de produse doriti sa cumparati: ");
    //  scanf("%d", &numar_prod_cump);

    // Primul element al cosului
    cump1 = (CUMP *)malloc(sizeof(CUMP));
    if (cump1 == NULL)
    {
        printf("alocarea a esuat");
        exit(1);
    }

    char mask[10];
    int id2; // knows if the user works with ID or name. If 1, user works with ID

    printf("Tastati numele sau ID ul produsului dorit:");

buy:

    id2 = 1;
    fflush(stdin);
    scanf("%s", &mask);

    if (name_or_id(mask))
    {
        cump1->id_produs = mask[0] - '0';
    }
    else
    {
        strcpy(cump1->nume_produs, mask);
        id2 = 0;
    }

    fflush(stdin);
    printf("Tastati cantitatea pe care o doriti: ");
    scanf("%d", &cump1->cantitate);
    numar_prod_cump = 1;
    int ok = 0;

    if (id2 == 1)
    {
        for (p = cap_lista; p != NULL; p = p->urmator)
        {
            if (cump1->id_produs == p->id_produs)
            {
                while (cump1->cantitate > p->cant_produs)
                {
                    printf("Numarul de produse nu se afla pe stoc. Introduceti un numar mai mic de %d produse: ", p->cant_produs + 1);
                    scanf("%d", &cump1->cantitate);
                    //break;
                }
                strcpy(cump1->nume_produs, p->name_produs);
                break;
            }
        }
    }
    else
    {
        for (p = cap_lista; p != NULL; p = p->urmator)
        {
            if (name_compare(cump1->nume_produs, p->name_produs))
            {
                while (cump1->cantitate > p->cant_produs)
                {
                    printf("Numarul de produse nu se afla pe stoc. Introduceti un numar mai mic de %d produse: ", p->cant_produs + 1);
                    scanf("%d", &cump1->cantitate);
                    cump1->id_produs = p->id_produs;
                    ok = 1;
                    //break;
                }
                ok = 1; // if product found, set ok to 1
                cump1->id_produs = p->id_produs;
                break;
            }
        }
        // checks if product was found or not
        if (ok == 0)
        {
            printf("Produsul nu a fost gasit in lista.\nTastati din nou numele sau ID ul produsului: ");
            goto buy;
        }
    }

    cump1->urmator = NULL;
    cmp_lista = cump1;

    printf("\nProdusul cu id ul: %d a fost adaugat in cos in cantitate de: %d\n", cump1->id_produs, cump1->cantitate);

    char r; // raspunsul
    int id, cantitate;

    // Modificare cos de cumparaturi
    printf("Daca doriti sa adaugati/modificati produse in cos, apasati a. Daca doriti sa stergeti produse din cos, apasati s. Altfel, apasati n : ");

input:

    id2 = 1;
    int del = 0; // check for delete
    fflush(stdin);
    scanf("%c", &r);
    fflush(stdin);

    switch (r)
    {

    // adding
    case 'a':

        printf("Tastati id ul sau numele produsului dorit: ");
        scanf("%s", &mask);

        cump2 = (CUMP *)malloc(sizeof(CUMP));

        id2 = 1;
        if (name_or_id(mask))
        {
            cump2->id_produs = mask[0] - '0';
        }
        else
        {
            strcpy(cump2->nume_produs, mask);
            // printf("cump2->nume =  %s\n", cump2->nume_produs);
            id2 = 0;
        }

        if (id2 == 1)
        {
            // checking if the product is already in the basket
            for (cump1 = cmp_lista, i = 1; cump1 != NULL || i < numar_prod_cump; cump1 = cump1->urmator, i++)
            {
                if (cump1->id_produs == cump2->id_produs)
                {
                    printf("Produsul este deja in cos. Tastati noua cantitate dorita: ");
                    scanf("%d", &cump1->cantitate);
                    for (p = cap_lista, i = 1; p != NULL; p = p->urmator, i++)
                    {
                        if (cump1->id_produs == p->id_produs)

                        {
                            while (cump1->cantitate > p->cant_produs)
                            {
                                printf("Numarul de produse nu se afla pe stoc. Introduceti un numar mai mic de %d produse: ", p->cant_produs + 1);
                                scanf("%d", &cump1->cantitate);
                                strcpy(cump1->nume_produs, p->name_produs);
                            }
                            strcpy(cump1->nume_produs, p->name_produs);
                        }
                    }
                    break;
                }
            }

            if (cump1 == NULL)
            {
                for (p = cap_lista, i = 1; p != NULL; p = p->urmator, i++)
                {
                    if (p->id_produs == cump2->id_produs)
                    {
                        printf("Tastati cantitatea dorita: ");
                        scanf("%d", &cantitate);
                        while (cantitate > p->cant_produs)
                        {
                            printf("Numarul de produse nu se afla pe stoc. Introduceti un numar mai mic de %d produse: ", p->cant_produs + 1);
                            scanf("%d", &cantitate);
                        }

                        append(&cmp_lista, cump2->id_produs, cantitate, p->name_produs);
                        numar_prod_cump++;
                        printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu: ");

                        goto input;
                    }
                }
                printf("Produs introdus gresit. Nicio schimbare efectuata in cos.");
                printf("Daca doriti sa adaugati/modificati produse in cos, apasati a. Daca doriti sa stergeti produse din cos, apasati s. Altfel, apasati n : ");

                goto input;
            }

            printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu: ");
            goto input;
        }
        else
        {
            for (cump1 = cmp_lista, i = 1; cump1 != NULL || i < numar_prod_cump; cump1 = cump1->urmator, i++)
            {
                if (name_compare(cump1->nume_produs, cump2->nume_produs))
                {
                    printf("Produsul este deja in cos. Tastati noua cantitate dorita: ");
                    scanf("%d", &cump1->cantitate);
                    for (p = cap_lista, i = 1; p != NULL; p = p->urmator, i++)
                    {
                        if (cump1->id_produs == p->id_produs)
                            while (cump1->cantitate > p->cant_produs)
                            {
                                printf("Numarul de produse nu se afla pe stoc. Introduceti un numar mai mic de %d produse: ", p->cant_produs + 1);
                                scanf("%d", &cump1->cantitate);
                                cump1->id_produs = p->id_produs;
                            }
                    }
                    break;
                }
            }

            if (cump1 == NULL)
            {
                for (p = cap_lista, i = 1; p != NULL; p = p->urmator, i++)
                {
                    if (name_compare(p->name_produs, cump2->nume_produs))
                    {
                        printf("Tastati cantitatea dorita: ");
                        scanf("%d", &cantitate);
                        while (cantitate > p->cant_produs)
                        {
                            printf("Numarul de produse nu se afla pe stoc. Introduceti un numar mai mic de %d produse: ", p->cant_produs + 1);
                            scanf("%d", &cantitate);
                        }

                        append(&cmp_lista, p->id_produs, cantitate, p->name_produs);
                        printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu:  ");

                        goto input;
                    }
                }
                printf("Produs introdus gresit. Nicio schimbare efectuata in cos.");
                printf("Daca doriti sa adaugati/modificati produse in cos, apasati a. Daca doriti sa stergeti produse din cos, apasati s. Altfel, apasati n : ");

                goto input;
            }

            printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu: ");
            goto input;
        }

    // Stergere cos de cumparaturi
    case 's':

        printf("Tastati id ul sau numele produsului pe care doriti sa l stergeti: ");
        scanf("%s", &mask);
        id2 = 1;
        if (name_or_id(mask))
        {
            id = atoi(mask);
        }
        else
        {
            id2 = 0;
        }

        // scoate if ul name_or_id in fata pt eficienta ca la 'a'
        if (id2 == 1)
        {
            for (cump1 = cmp_lista; cump1 != NULL; cump1 = cump1->urmator)
            {
                if (cump1->id_produs == atoi(mask))
                {
                    deleteNode_ID(&cmp_lista, atoi(mask));
                    printf("Produsul cu ID ul %s a fost sters din cos!\n", mask);
                    printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu: ");

                    del = 1;
                    goto input;
                }
            }
        }
        else
        {
            for (cump1 = cmp_lista; cump1 != NULL; cump1 = cump1->urmator)
            {
                if (name_compare(cump1->nume_produs, mask))
                {
                    deleteNode_NAME(&cmp_lista, mask);
                    printf("Produsul %s a fost sters din cos!\n", mask);
                    printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu: ");

                    del = 1;
                    goto input;
                }
            }
        }

        if (del == 0)
        {
            printf("Produsul nu se afla in cos!");
            goto input;
        }

        // printf("Dorititi sa mai faceti adaugati produse in cosul de cumparaturi? a->adaugare/modificare, s->stergere, n->nu ");
        // goto input;

    case 'n':
        break;

    // input gresit
    default:
        printf("Comanda introdusa gresit. Incercati din nou: a/s/n");
        goto input;
    }
    printf("Cosul de cumparaturi dupa modificari contine:  \n");

    // Suma totala a cosului
    // Alta varianta pentru suma: adaugam in crearea listei cosului de cumparaturi si pretul, iar la suma avem un for mai putin
    float suma = 0;
    for (cump1 = cmp_lista, i = 1; cump1 != NULL; cump1 = cump1->urmator, i++)
    {
        printf("Produsul cu id ul: %d si numele %s a fost adaugat in cos in cantitate de: %d\n", cump1->id_produs, cump1->nume_produs, cump1->cantitate);
        for (p = cap_lista, j = 1; p != NULL; p = p->urmator, j++)
        {
            if (cump1->id_produs == p->id_produs)
            {
                suma = suma + (cump1->cantitate) * (p->pret_produs);
            }
        }
    }
    printf("Totalul de plata este: %.2f EUR. Va dorim o zi buna!\n\n", suma);

    
    char wait_for_input;

    printf("Press enter to exit");
    scanf("%c", &wait_for_input);
}