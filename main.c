#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_STRING 30
#define VAL_PROTEINS 4
#define VAL_FATS 9
#define VAL_CARBOHYDRATES 4

typedef enum {EXIT,INSERT,REMOVE,SEARCH,PRINT,MODIFY,CALCULATE} Choice;
typedef struct {
    char name[DIM_STRING+1];
    float price;
    float proteins;
    float fats;
    float carbohydrates;
    float energy_score; //Formula ENERGY = VAL_PROTEINS * proteins + VAL_FATS * fats + VAL_CARBOHYDRATES * carbohydrates
    float nutritional_value_score; //Formula = ENERGY * QUALITY, Formula QUALITY = proteins / (proteins + fats + carbohydrates)
    float unique_score; //Formula = NUTRITIONAL VALUE / price
} Product;

Product* add_product(int* numproducts,Product* product);
void print_product(Product* product);
Product* search_product(Product* products,const char name[DIM_STRING+1],int numproducts);
void print_ladder(int numproducts,Product products[numproducts]);
void calculate_data();
void remove_product(Product** products,int* numproducts,const char name[DIM_STRING+1]);
void rewrite_file(Product* products,int numproducts);
void modify_product(Product* products,const char name[DIM_STRING+1],int numproducts);
void print_banner();

Product* add_product(int* numproducts,Product* product) {
    if (*numproducts == 0) {
        product = (Product*) malloc(sizeof(Product));
    }
    else {
        product = (Product*) realloc(product,(*numproducts + 1)* sizeof(Product));
    }
    if (product == NULL) {
        exit(EXIT_FAILURE);
    }
    do {
        printf("\nEnter product name(no spaces): ");
        scanf("%s",product[*numproducts].name);
    }while (strlen(product[*numproducts].name) == 0);
    while (getchar() != '\n');
    printf("Enter product price(100g): ");
    scanf("%f",&product[*numproducts].price);
    printf("Enter grams of proteins(100g): ");
    scanf("%f",&product[*numproducts].proteins);
    printf("Enter grams of fats(100g): ");
    scanf("%f",&product[*numproducts].fats);
    printf("Enter grams of carbohydrates(100g): ");
    scanf("%f",&product[*numproducts].carbohydrates);
    product[*numproducts].energy_score = (VAL_PROTEINS * product[*numproducts].proteins) + (VAL_FATS * product[*numproducts].fats) + (VAL_CARBOHYDRATES * product[*numproducts].carbohydrates);
    product[*numproducts].nutritional_value_score = product[*numproducts].energy_score * (product[*numproducts].proteins / (product[*numproducts].proteins + product[*numproducts].fats + product[*numproducts].carbohydrates));
    product[*numproducts].unique_score = product[*numproducts].nutritional_value_score / product[*numproducts].price;
    printf("\nProduct added successfully!\n");
    (*numproducts) += 1;
    return product;
}
void print_product(Product* product) {
    printf("\nName: %s",product->name);
    printf("\nPrice: %.2f",product->price);
    printf("\nEnergy Score: %.2f",product->energy_score);
    printf("\nNutritional Value Score: %.2f",product->nutritional_value_score);
    printf("\nFinal Score: %.2f\n\n",product->unique_score);
}
Product* search_product(Product* products,const char name[DIM_STRING+1],int numproducts) {
    int i;
    printf("\nSearching for product...\n");
    for (i = 0;i < numproducts;i++) {
        if (strcmp(name,products[i].name) == 0) {
            return &products[i];
        }
    }
    printf("\nProduct not found!\n\n");
    return NULL;
}
void print_ladder(int numproducts,Product products[numproducts]) {
    int i,j;
    Product temp_product;
    Product* products_copy = products;

    for (i = 0;i < numproducts;i++) {
        for (j = 0;j < numproducts;j++) {
            if (products[i].unique_score > products[j].unique_score) {
                temp_product= products_copy[i];
                products_copy[i] = products_copy[j];
                products_copy[j] = temp_product;
            }
        }
    }
    printf("\n-------Ranking------- \n");
    for (i = 0;i < numproducts;i++) {

        printf("%s: %.2f\n",products_copy[i].name,products[i].unique_score);
    }
    printf("-------Ranking------- \n\n");

}
void calculate_data() {
    float absolute_price;
    float absolute_grams;
    float final_price;
    printf("Enter absolute price: ");
    scanf("%f",&absolute_price);
    printf("Enter number of grams: ");
    scanf("%f",&absolute_grams);
    final_price = absolute_price * 100 / absolute_grams;
    printf("Price per 100g: %.2f\n\n",final_price);


}
void remove_product(Product** products,int* numproducts,const char name[DIM_STRING+1]) {
    Product* found = NULL;
    found = search_product(*products,name,*numproducts);
    int index = 0;
    if (found) {
        printf("\nRemoving product...\n");
        int i;
        for (i = 0;i < *numproducts;i++) {
            if (strcmp(found->name,(*products)[i].name) == 0) {
                index = i;
            }
        }
        for (i = index;i < *numproducts -1 ;i++) {
            (*products)[i] = (*products)[i+1];
        }
        (*numproducts)--;
        if (*numproducts > 0) {
            Product *temp = realloc(*products, (*numproducts) * sizeof(Product));
            if (temp != NULL) {
                *products = temp;
            }
        } else {
            free(*products);
            *products = NULL;
        }
        printf("\nProduct removed successfully\n");
    }

}
void rewrite_file(Product* products,int numproducts) {
    int i;
    FILE *pf = NULL;
    pf = fopen("Data.txt","w");
    if (pf == NULL) {
        printf("File opening error!");
    }
    for (i = 0;i< numproducts;i++) {
        fprintf(pf,"%s\n",products[i].name);
        fprintf(pf,"%.2f\n",products[i].price);
        fprintf(pf,"%.2f\n",products[i].energy_score);
        fprintf(pf,"%.2f\n",products[i].nutritional_value_score);
        fprintf(pf,"%.2f\n\n",products[i].unique_score);
    }
    fclose(pf);
}
void modify_product(Product* products,const char name[DIM_STRING+1],int numproducts) {
    Product* found = NULL;
    found = search_product(products,name,numproducts);
    int index = 0;
    if (found) {
        int i;
        for (i = 0;i < numproducts;i++) {
            if (strcmp(found->name,products[i].name) == 0) {
                index = i;
            }
        }
        printf("Enter product price(100g): ");
        scanf("%f",&products[index].price);
        printf("Enter grams of proteins(100g): ");
        scanf("%f",&products[index].proteins);
        printf("Enter grams of fats(100g): ");
        scanf("%f",&products[index].fats);
        printf("Enter grams of carbohydrates(100g): ");
        scanf("%f",&products[index].carbohydrates);
        products[index].energy_score = (VAL_PROTEINS * products[index].proteins) + (VAL_FATS * products[index].fats) + (VAL_CARBOHYDRATES * products[index].carbohydrates);
        products[index].nutritional_value_score = products[index].energy_score * (products[index].proteins / (products[index].proteins + products[index].fats + products[index].carbohydrates));
        products[index].unique_score = products[index].nutritional_value_score / products[index].price;
        printf("\n\nProduct modified successfully!\n");
    }
}
void print_banner() {
    printf(
        "\n"
        "\n"
        "███████╗ ██████╗  ██████╗ ██████╗ ██████╗  █████╗ ████████╗███████╗██████╗\n"
        "██╔════╝██╔═══██╗██╔═══██╗██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗\n"
        "█████╗  ██║   ██║██║   ██║██║  ██║██████╔╝███████║   ██║   █████╗  ██████╔╝\n"
        "██╔══╝  ██║   ██║██║   ██║██║  ██║██╔══██╗██╔══██║   ██║   ██╔══╝  ██╔══██╗\n"
        "██║     ╚██████╔╝╚██████╔╝██████╔╝██║  ██║██║  ██║   ██║   ███████╗██║  ██║\n"
        "╚═╝      ╚═════╝  ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝\n"
        "\n"
        "                          by ShiroiShi\n"
        "\n"
        "FoodRater - Evaluate food convenience based on\n"
        "energy, nutritional quality and price.\n"
        "\n"
    );
}


int main(void) {

    Choice choice = 0;
    Product* products = NULL;
    int numproducts = 0;
    Product* found_product = NULL;
    char name[DIM_STRING+1];
    FILE *pf = NULL;
    pf = fopen("Data.txt","a");
    if (pf == NULL) {
        printf("File opening failed!");
    }
    fclose(pf);

    pf = fopen("Data.txt","r");
    if (pf == NULL) {
        printf("File opening failed!");
    }
    char product_name[DIM_STRING+1];
    char product_line[DIM_STRING+1];
    float product_price;
    float product_energy_score;
    float product_nutritional_score;
    float product_final_score;
    while (1) {
        if (!fgets(product_name, DIM_STRING+1, pf)) break;
        product_name[strcspn(product_name, "\n")] = '\0';
        products = (Product*) realloc(products,(numproducts+1) * sizeof(Product));
        strcpy(products[numproducts].name,product_name);

        if (!fgets(product_line, DIM_STRING+1, pf)) break;
        product_price = strtof(product_line, NULL);
        products[numproducts].price = product_price;

        if (!fgets(product_line, DIM_STRING+1, pf)) break;
        product_energy_score = strtof(product_line, NULL);
        products[numproducts].energy_score = product_energy_score;

        if (!fgets(product_line, DIM_STRING+1, pf)) break;
        product_nutritional_score = strtof(product_line, NULL);
        products[numproducts].nutritional_value_score = product_nutritional_score;

        if (!fgets(product_line, DIM_STRING+1, pf)) break;
        product_final_score = strtof(product_line, NULL);
        products[numproducts].unique_score = product_final_score;

        fgets(product_line, DIM_STRING+1, pf);
        numproducts++;
    }
    fclose(pf);
    print_banner();
    do {
        printf("-------Menu-------\n");
        printf("0: Exit\n");
        printf("1: Insert new product\n");
        printf("2: Remove a product\n");
        printf("3: Search product by name\n");
        printf("4: Print product ranking\n");
        printf("5: Modify product data\n");
        printf("6: Calculate proportions per 100g\n");
        printf("-------Menu-------\n");
        printf("Choice: ");
        scanf("%d",&choice);

        switch (choice) {
            case EXIT:
                printf("Bye!");
                break;
            case INSERT:
                products = add_product(&numproducts,products);
                pf = fopen("Data.txt","a");
                if (pf == NULL) {
                    printf("File opening failed!");
                }
                fprintf(pf,"%s\n%.2f\n%.2f\n%.2f\n%.2f\n\n",products[numproducts-1].name,products[numproducts-1].price,products[numproducts-1].energy_score,products[numproducts-1].nutritional_value_score,products[numproducts-1].unique_score);
                fclose(pf);
                break;
            case REMOVE:
                printf("Enter product name to remove: ");;
                scanf("%s",name);
                remove_product(&products,&numproducts,name);
                rewrite_file(products,numproducts);
                break;
            case SEARCH:
                printf("Enter product name to search: ");
                scanf("%s",name);
                found_product = search_product(products,name,numproducts);
                if (found_product != NULL) {
                    print_product(found_product);
                }
                break;
            case PRINT:
                print_ladder(numproducts,products);
                break;
            case MODIFY:
                printf("Enter product name to search: ");
                scanf("%s",name);
                modify_product(products,name,numproducts);
                rewrite_file(products,numproducts);
                break;
            case CALCULATE:
                calculate_data();
                break;
            default:
                printf("\nEnter a valid choice!\n\n");
                break;
        }


    }while (choice != EXIT);

    free(products);
    exit(EXIT_SUCCESS);
}
