#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct structura
{
    int id_produs;
    char name_produs[20];
    int cant_produs;
    float pret_produs;
    struct structura *next;
} STR; // p q cap_lista

typedef struct cumparaturi
{
    int id_produs;
    char nume_produs[10];
    int cantitate;
    struct cumparaturi *next;
} CUMP; // cump1 cump2 cmp_lista

int name_compare(char n[], char m[])
{
    char name[20], name2[20];
    strcpy(name, n);
    strcpy(name2, m);
    // exluding the input mistake by uppering both strings
    for (int i = 0; i < strlen(name); i++)
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
        *head_ref = temp->next; // Changed head
        free(temp);                // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->id_produs != key)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
}

void deleteNode_NAME(CUMP **head_ref, char *key)
{
    // Store head node
    CUMP *temp = *head_ref, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && name_compare(temp->nume_produs, key))
    {
        *head_ref = temp->next; // Changed head
        free(temp);                // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && !name_compare(temp->nume_produs, key))
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

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
    new_node->next = NULL;

    /* 4. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 5. Change the next of last node */
    last->next = new_node;
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

    // Stock list
    STR *cap_lista, *p, *q;
    int no_stock_products = 0, nr;
    int i;

    p = (STR *)malloc(sizeof(STR));
    if (p == NULL)
    {
        printf("Memory allocation failed!");
        exit(-1);
    }

    fscanf(stock_input, "%d", &p->id_produs);
    fscanf(stock_input, "%s", &p->name_produs);
    fscanf(stock_input, "%d", &p->cant_produs);
    fscanf(stock_input, "%f", &p->pret_produs); // (*p).valoare
    p->next = NULL;
    cap_lista = p;

    while (!feof(stock_input))
    {
        q = (STR *)malloc(sizeof(STR));
        if (q == NULL)
        {
            printf("Memory allocation failed!");
            exit(-1);
        }
        
        fscanf(stock_input, "%d", &(q->id_produs));
        fscanf(stock_input, "%s", (q->name_produs));
        fscanf(stock_input, "%d", &(q->cant_produs));
        fscanf(stock_input, "%f", &(q->pret_produs));
        q->next = NULL;
        p->next = q;
        p = q;
        no_stock_products++;
    }
    no_stock_products++;

    for (p = cap_lista, i = 1; p != NULL; p = p->next, i++)
    {
        printf("Element with id: %d called: %s is in stock with quantity: %d at %.2f EUR\n", p->id_produs, p->name_produs, p->cant_produs, p->pret_produs);
    }

    // Cart list
    CUMP *cmp_lista, *cump1, *cump2;
    int numar_prod_cump = 0, j;



    // First element of Cart
    cump1 = (CUMP *)malloc(sizeof(CUMP));
    if (cump1 == NULL)
    {
        printf("Memory allocation failed");
        exit(1);
    }

    char mask[10];
    int id2; // knows if the user works with ID or name. If 1, user works with ID

    printf("Please enter the name or ID of the product you want: ");

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
    printf("Enter the quantity: ");
    scanf("%d", &cump1->cantitate);
    numar_prod_cump = 1;
    int ok = 0;

    if (id2 == 1)
    {
        for (p = cap_lista; p != NULL; p = p->next)
        {
            if (cump1->id_produs == p->id_produs)
            {
                while (cump1->cantitate > p->cant_produs)
                {
                    printf("Quantity not available in stock. Please re-enter a smaller than %d quantity: ", p->cant_produs + 1);
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
        for (p = cap_lista; p != NULL; p = p->next)
        {
            if (name_compare(cump1->nume_produs, p->name_produs))
            {
                while (cump1->cantitate > p->cant_produs)
                {
                    printf("Quantity not available in stock. Please re-enter a smaller than %d quantity: ", p->cant_produs + 1);
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
            printf("Product not found in list.\nPlease re-enter the name or ID: ");
            goto buy;
        }
    }

    cump1->next = NULL;
    cmp_lista = cump1;

    printf("\nProduct with ID: %d added in cart in %d quantity\n", cump1->id_produs, cump1->cantitate);

    char r; // User answer
    int id, cantitate;

    // Modify Cart
    printf("If you want to add or modify products in your cart, press a.\nIf you want to delete products from cart, press s.\nOtherwise, press n: ");

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

        printf("Please enter the name or ID of the product you want: ");
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
            id2 = 0;
        }

        if (id2 == 1)
        {
            // checking if the product is already in the basket
            for (cump1 = cmp_lista, i = 1; cump1 != NULL || i < numar_prod_cump; cump1 = cump1->next, i++)
            {
                if (cump1->id_produs == cump2->id_produs)
                {
                    printf("Product already in cart. Enter the new quantity: ");
                    scanf("%d", &cump1->cantitate);
                    for (p = cap_lista, i = 1; p != NULL; p = p->next, i++)
                    {
                        if (cump1->id_produs == p->id_produs)

                        {
                            while (cump1->cantitate > p->cant_produs)
                            {
                                printf("Quantity not available in stock. Please re-enter a smaller than %d quantity: ", p->cant_produs + 1);
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
                for (p = cap_lista, i = 1; p != NULL; p = p->next, i++)
                {
                    if (p->id_produs == cump2->id_produs)
                    {
                        printf("Enter the quantity: ");
                        scanf("%d", &cantitate);
                        while (cantitate > p->cant_produs)
                        {
                            printf("Quantity not available in stock. Please re-enter a smaller than %d quantity: ", p->cant_produs + 1);
                            scanf("%d", &cantitate);
                        }

                        append(&cmp_lista, cump2->id_produs, cantitate, p->name_produs);
                        numar_prod_cump++;
                        printf("Do you want to make other changes to the cart? a->add/modify, s->delete, n->no: ");

                        goto input;
                    }
                }
                printf("Wrong input. Cart has not been modified!\n");
                printf("If you want to add or modify products in your cart, press a.\nIf you want to delete products from cart, press s.\nOtherwise, press n: ");

                goto input;
            }

            printf("Do you want to make other changes to the cart? a->add/modify, s->delete, n->no: ");
            goto input;
        }
        else
        {
            for (cump1 = cmp_lista, i = 1; cump1 != NULL || i < numar_prod_cump; cump1 = cump1->next, i++)
            {
                if (name_compare(cump1->nume_produs, cump2->nume_produs))
                {
                    printf("Product already in cart. Enter the new quantity: ");
                    scanf("%d", &cump1->cantitate);
                    for (p = cap_lista, i = 1; p != NULL; p = p->next, i++)
                    {
                        if (cump1->id_produs == p->id_produs)
                            while (cump1->cantitate > p->cant_produs)
                            {
                                printf("Quantity not available in stock. Please re-enter a smaller than %d quantity: ", p->cant_produs + 1);
                                scanf("%d", &cump1->cantitate);
                                cump1->id_produs = p->id_produs;
                            }
                    }
                    break;
                }
            }

            if (cump1 == NULL)
            {
                for (p = cap_lista, i = 1; p != NULL; p = p->next, i++)
                {
                    if (name_compare(p->name_produs, cump2->nume_produs))
                    {
                        printf("Enter the quantity: ");
                        scanf("%d", &cantitate);
                        while (cantitate > p->cant_produs)
                        {
                            printf("Quantity not available in stock. Please re-enter a smaller than %d quantity: ", p->cant_produs + 1);
                            scanf("%d", &cantitate);
                        }

                        append(&cmp_lista, p->id_produs, cantitate, p->name_produs);
                        printf("Do you want to make other changes to the cart? a->add/modify, s->delete, n->no: ");

                        goto input;
                    }
                }
                printf("Wrong input. Cart has not been modified!\n");
                printf("If you want to add or modify products in your cart, press a.\nIf you want to delete products from cart, press s.\nOtherwise, press n: ");

                goto input;
            }

            printf("Do you want to make other changes to the cart? a->add/modify, s->delete, n->no: ");
            goto input;
        }

    // Remove from cart
    case 's':

        printf("Enter the ID or name of the product you want to remove from cart: ");
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

        if (id2 == 1)
        {
            for (cump1 = cmp_lista; cump1 != NULL; cump1 = cump1->next)
            {
                if (cump1->id_produs == atoi(mask))
                {
                    deleteNode_ID(&cmp_lista, atoi(mask));
                    printf("Product with ID %s has been removed from cart!\n", mask);
                    printf("Do you want to make other changes to the cart? a->add/modify, s->delete, n->no: ");

                    del = 1;
                    goto input;
                }
            }
        }
        else
        {
            for (cump1 = cmp_lista; cump1 != NULL; cump1 = cump1->next)
            {
                if (name_compare(cump1->nume_produs, mask))
                {
                    deleteNode_NAME(&cmp_lista, mask);
                    printf("Product with ID %s has been removed from cart!\n", mask);
                    printf("Do you want to make other changes to the cart? a->add/modify, s->delete, n->no: ");

                    del = 1;
                    goto input;
                }
            }
        }

        if (del == 0)
        {
            printf("Product not in cart!");
            goto input;
        }

    case 'n':
        break;

    // Wrong input
    default:
        printf("Wrong input.\nPlease try again a/s/n: ");
        goto input;
    }
    printf("Current cart:  \n");

    // Total sum
    float suma = 0;
    for (cump1 = cmp_lista, i = 1; cump1 != NULL; cump1 = cump1->next, i++)
    {
        printf("Product with ID: %d name: %s quantity:  %d\n", cump1->id_produs, cump1->nume_produs, cump1->cantitate);
        for (p = cap_lista, j = 1; p != NULL; p = p->next, j++)
        {
            if (cump1->id_produs == p->id_produs)
            {
                suma = suma + (cump1->cantitate) * (p->pret_produs);
            }
        }
    }
    printf("Checkout Total: %.2f EUR. Have a nice day!\n\n", suma);

    
    char wait_for_input;

    printf("Press enter to exit");
    scanf("%c", &wait_for_input);
}