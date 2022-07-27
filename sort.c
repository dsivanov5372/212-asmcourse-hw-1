#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

static unsigned long long cmp_count = 0; //переменная в которой будем хранить число сравнений
static unsigned long long swap_count = 0;  //переменная в которой будем хранить число обменов
static int param = 0; //параметр для генерации массива
static int method = 0; //параметр отвечающий за метод сортировки

static void gen_array(int *arr, int size){ //функция для генерации массива в зависимости от заданного параметра
    if(param == 0){ //первый массив
        int num = INT_MIN+2; //порождающий элемент массива
        for(int i = 0; i < size; i++){ // генерация массива, в котором элементы уже отсортированы по неубыванию
            arr[i] = (rand()%2 == 0) ? num : ++num;
        }
        param++; //увеличиваем параметр для следующей генерации
    } else if(param == 1){ //второй массив
        int num = INT_MAX-2; //порождающий элемент массива
        for(int i = 0; i < size; i++){ // генерация массива, в котором элементы уже отсортированы по невозрастанию (обратный порядок)
            arr[i] = (rand()%2 == 0) ? num : --num;
        }
        param++; //увеличиваем параметр для следующей генерации
    } else {
        for(int i = 0; i < size; i++){ //генерация массива со случайной расстановкой элементов
            arr[i] = rand()%INT_MAX-10;
            arr[i] *= (rand()%2) == 1? -1 : 1;
        }
    }
}

static void arr_cpy(int *dest, int *src, int size){ //функция которая копирует массив
    for(int i = 0; i < size; i++){ //проходимся по всем элементам src
        dest[i] = src[i]; //копируем элемент из src в dest
    }
}

static int search(int *temp_arr, int *sorted_arr, int size, int i){
    int is_found = 0; //булевая переменная, означающая, найден ли элемент в исходном массиве
    for(int j = 0; j < size; j++){
        if(temp_arr[j] == INT_MIN){  //элемент уже был найден
            continue;
        }
        if(sorted_arr[i] == temp_arr[j]){
            is_found = 1; //элемент найден
            temp_arr[j] = INT_MIN; //минимальный инт, поскольку мы не должны снова попасть на этот элемент
            break;
        }
    }
    return is_found;
}

static void array_check(int *arr, int *sorted_arr, int size){ //функция, проверяющая, является ли отсортированный массив исходным массивом
    int *temp_arr = malloc(size*sizeof(int)); //создаём временный массив
    arr_cpy(temp_arr, arr, size); //копируем в временный массив элементы из основного массива
    method++;

    for(int i = 0; i < size; i++){
        if(i < size - 1){ //проверка, чтобы мы не вышли за пределы массива
            if(sorted_arr[i] > sorted_arr[i+1]){ //проверяем, чтобы элементы стояли по неубыванию
                printf("массив отсортирован не по неубыванию!\n");
                goto end;
            }
        }
        if(!search(temp_arr, sorted_arr, size, i)){ //ищем элемент из отсортированного массива в неотсортированном
            free(temp_arr); //элемент не найден - освобождаем память под массив и выходим из функции
            printf("массив отсортирован неправильно: элементы не совпадают!\n");
            goto end;
        }
    }

    if(method % 2 == 1){ //смотрим на метод, которым отсортировали массив
        printf("массив отсортирован методом пузырька корректно\n");
    } else {
        printf("массив отсортирован методом Шелла корректно\n");
    }

    printf("число сравнений: %llu\n", cmp_count);
    printf("число обменов: %llu\n", swap_count);

    end:
    free(temp_arr); //освобождаем память под временный массив
    swap_count = 0;
    cmp_count = 0;
}

static int cmp(int a, int b){ //функция которая сравнивает элементы
    cmp_count++; //увеличиваем число сравнений
    return a > b; //возвращаем результат сравнения
}

static void swap(int *a, int *b){ //функция выполняющая обмен элементов
    int temp = *a;
    *a = *b;
    *b = temp;
    swap_count++; //увеличиваем число обменов
}

//функция которая выполняет сортировку массива методом пузырька
static void bubble_sort(int *arr, int size){ //https://neerc.ifmo.ru/wiki/index.php?title=Сортировка_пузырьком
    for(int i = 0; i < size - 1; i++){ //проходимся по массиву
        for(int j = 0; j < size - i - 1; j++){ //проходимся по массиву, но при этом сравнивания элементы друг с другом
            if(cmp(arr[j], arr[j+1])){ //сравнение элементов
                swap(&arr[j], &arr[j+1]); //обмен элементами
            }
        }
    }
}

//функция которая выполняет сортировку массива методов шелла
static void shell_sort(int *arr, int size){  //medium.com/@dimko1/алгоритмы-сортировки-shell-sort-f7a5f75427c1
    int gap = 1; //интервал, по которому будем выбирать числа для вставки
    while(gap < size / 3){  //высчитываем интервал по формуле Кнута
        gap = 3*gap + 1;
    }

    while(gap > 0){
        for (int i = gap; i < size; i++){ //выберем число, которое нужно вставить
            for (int j = i; j >= gap && cmp(arr[j - gap], arr[j]); j -= gap){ //разбиваем массив на элементы с интервалом gap
                swap(&arr[j], &arr[j - gap]); //перемещаем элемент вправо, если он больше следующего
            }
        }
        gap /= 3; //уменьшаем интервал
    }
}

//static void print_arr(int *arr, int size){ //функция печатающая массив
//    for(int i = 0; i < size; i++){ //проходимся по каждому элементу массива
//        printf("%d ", arr[i]); //печатаем элемент
//    }
//    printf("\n"); //перевод строки, для лучшей читаемости
//}

//функция которая создаёт три массива - один исходный и два для сортировки (копирует исходный в два новых)
static void make_arrays(int *arr, int *bubble_array, int *shell_array, int size){
    gen_array(arr, size);
    //print_arr(arr, size);
    arr_cpy(bubble_array, arr, size);
    //print_arr(bubble_array, size);
    arr_cpy(shell_array, arr, size);
    //print_arr(shell_array, size);
}

int main(void) {
    srand(time(NULL)); //чтобы каждую секунду генерировались новые числа

    int size = 0; //размер массива

    if(scanf("%d", &size) == 0){ //введём размер массива
        fprintf(stderr, "Не удалось считать размер массива.\n"); // сообщение об ошибке
        return 1; //заканчиваем работу программы, если не удалось считать размер массива
    }

    if(size < 1){ //проверим, чтобы в массиве было не меньше одного элемента
        fprintf(stderr, "Размер массива должен быть не меньше единицы.\n"); //сообщение об ошибке
        return 2; //заканчиваем работу программы, если у нас в массиве меньше одного элемента
    }

    int *arr = malloc(size*sizeof(int)); //исходный массив
    int *bubble_array = malloc(size*sizeof(int)); //исходный массив, который будет сортироваться пузырьком
    int *shell_array = malloc(size*sizeof(int)); //исходный массив, который будет сортироваться методом Шелла

    char *str[4] = {"Первый массив",
                    "Второй массив",
                    "Третий массив",
                    "Четвёртый массив"};

    for(int i = 0; i < 4; i++){
        make_arrays(arr, bubble_array, shell_array, size); //создаём три массива расстановкой элементов по неубыванию

        printf("%s\n", str[i]); //Печатаем номер сортируемого массива
        bubble_sort(bubble_array, size); //cортировка пузырьком
        array_check(arr, bubble_array, size); //проверяем, что массив, отсортированный пузырьком - исходный массив

        shell_sort(shell_array, size); //сортировка методом Шелла
        array_check(arr, bubble_array, size); //проверяем, что массив, отсортированный методом шелла - исходный массив
        printf("\n");
    }

    free(arr); //освобождаем память выделенную под основной массив
    free(bubble_array); //освобождаем память выделенную под массив сортируемый пузырьком
    free(shell_array); //освобождаем память выделенную под массив сортируемый методом Шелла

    return 0;
}
