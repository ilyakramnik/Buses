#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_buses 11
#define max_stops 31

typedef struct{
    char name[50]; //название каждой остановки
    int amount_of_buses; //число автобусов
    char buses[11][50]; //массив автобусов с их названиями
}Stops;

typedef struct{
    char name[50]; //название каждого автобуса
    int amount_of_stops; //число остановок
    char stops[31][50]; //массив остановок с их названиями
}Buses;

Buses *arr;
Stops *arr_stops;
void InitArrayStruct() {
    arr = (Buses *) malloc(max_buses*sizeof(Buses));
    for (int i=0;i<max_buses;i++){
        strcpy(arr[i].name, "");
        arr[i].amount_of_stops = 1;
    }
    arr_stops = (Stops *) malloc(max_stops*sizeof(Stops));
    for (int i=0;i<max_stops;i++){
        strcpy(arr_stops[i].name, "");
        arr_stops[i].amount_of_buses = 1;
    }
}

int IsBusInArray(char mas[31][50], int n, char *team){
    if (n==0){
        return 0;
    }
    else{
        for (int i=0;i<n;i++){
            if (strcmp(mas[i], team)==0)
                return i;
        }
    }
    return 0;
}

int len_depot=1;

void AddBus(char *bus_number, char depot[11][50], FILE *file1) {
    if (len_depot > 10)
        fprintf(file1, "%s %s%s\n", "CAN'T ADD BUS", bus_number, ". BUSPARK IS FULL");
    else if (!IsBusInArray(depot, len_depot, bus_number)) {
        strcpy(depot[len_depot], bus_number);
        strcpy(arr[len_depot].name, bus_number);
        len_depot++;
    }
    else {
        fprintf(file1, "%s %s %s\n", "BUS", bus_number, "ALREADY EXISTS");
    }
}

int IndexOfBusInArray, len_stops=1, IndexOfStopInArray;

void AddStop(char *bus_number, char *bus_stop, char depot[11][50], char all_stops[31][50], FILE *file1) {
    IndexOfBusInArray = IsBusInArray(depot, len_depot, bus_number);
    if (arr[IndexOfBusInArray].amount_of_stops > 30)
        fprintf(file1, "%s %s %s %s %s\n", "CAN'T ADD STOP", bus_stop, " TO BUS", bus_number, ". BUS IS FULL");
    else if(IsBusInArray(depot, len_depot, bus_number)){ //если такой автобус есть
        if (!IsBusInArray(arr[IndexOfBusInArray].stops, arr[IndexOfBusInArray].amount_of_stops, bus_stop)){ //если у него нет выбранной остановки
            strcpy(arr[IndexOfBusInArray].stops[arr[IndexOfBusInArray].amount_of_stops], bus_stop); //добавление остановки в конец списка
            arr[IndexOfBusInArray].amount_of_stops++;

            IndexOfStopInArray = IsBusInArray(all_stops, len_stops, bus_stop);
            if (!IndexOfStopInArray) { //если такой остановки еще не было вообще
                strcpy(arr_stops[len_stops].buses[arr_stops[len_stops].amount_of_buses], bus_number); //добавления автобуса в массив для только что добавленной остановки
                arr_stops[len_stops].amount_of_buses++;
                strcpy(arr_stops[len_stops].name, bus_stop);
                strcpy(all_stops[len_stops], bus_stop);
                len_stops++;
            }
            else{
                strcpy(arr_stops[IndexOfStopInArray].buses[arr_stops[IndexOfStopInArray].amount_of_buses], bus_number); //добавления автобуса в массив для имеющийся остановки
                arr_stops[IndexOfStopInArray].amount_of_buses++;
            }
        }
        else
            fprintf(file1, "%s %s %s %s\n", "BUS", bus_number, "ALREADY HAS STOP", bus_stop);
    }
    else
        fprintf(file1, "%s %s %s\n", "BUS", bus_number, "DOESN'T EXIST");
}

void PrintBus(char *bus_number, char depot[11][50], FILE *file1){
    if(IsBusInArray(depot, len_depot, bus_number)){
        IndexOfBusInArray = IsBusInArray(depot, len_depot, bus_number);
        fprintf(file1, "%s\n", arr[IndexOfBusInArray].name);
        fprintf(file1, "STOPS:");
        for (int i=0;i<arr[IndexOfBusInArray].amount_of_stops;i++){
            fprintf(file1, "%s ", arr[IndexOfBusInArray].stops[i]);
        }
        fprintf(file1, "\n");
    }
    else
        fprintf(file1, "%s %s %s\n", "BUS", bus_number, "DOESN'T EXIST");
}

void PrintStop(char *bus_stop, char all_stops[31][50], FILE *file1){
    IndexOfStopInArray = IsBusInArray(all_stops, len_stops, bus_stop);
    fprintf(file1, "%s%s", bus_stop, ":");
    if (!IndexOfStopInArray)
        fprintf(file1, " NO BUSES GO TO STOP\n");
    else {
        for (int i = 0; i < arr_stops[IndexOfStopInArray].amount_of_buses; i++) {
            fprintf(file1, "%s ", arr_stops[IndexOfStopInArray].buses[i]);
        }
        fprintf(file1, "\n");
    }
}

void PrintAll(FILE *file1){
    for (int j=1;j<len_depot;j++) {
        fprintf(file1, "%s\n", arr[j].name);
        fprintf(file1, "STOPS:");
        for (int i = 0; i < arr[j].amount_of_stops; i++) {
            fprintf(file1, "%s ", arr[j].stops[i]);
        }
        fprintf(file1, "\n");
    }
}

int IndexOfStopInArray1, IndexOfStopInArray2, flag,  IndexOfBusInArray1,  IndexOfBusInArray2, transfers[100]={-1}, amount_of_transfers=0;

int WithoutTransfers(char *stop1, char *stop2, char all_stops[31][50], FILE *file1){
    flag=0;
    IndexOfStopInArray1 = IsBusInArray(all_stops, len_stops, stop1);
    IndexOfStopInArray2 = IsBusInArray(all_stops, len_stops, stop2);
    if (IndexOfStopInArray1==0 || IndexOfStopInArray2==0) {
        fprintf(file1, "%s %s %s %s\n", "THERE ARE NO BUSES FROM", stop1, "TO", stop2);
        return 1;
    }
    else if(strcmp(stop1,stop2)==0) {
        fprintf(file1, "%s %s %s\n", "STOPS", stop1, "ARE THE SAME");
        return 1;
    }
    else {
        for (int i = 1;
             i < arr_stops[IndexOfStopInArray1].amount_of_buses; i++) { //все автобусы, проходящие через остановку 1
            for (int j = 1; j < arr_stops[IndexOfStopInArray2].amount_of_buses; j++) {
                if (strcmp(arr_stops[IndexOfStopInArray1].buses[i], arr_stops[IndexOfStopInArray2].buses[j]) == 0) {
                    if (!flag) {
                        fprintf(file1, "%s %s %s %s%s\n", "BUSES FROM", stop1, "TO", stop2, ":");
                        flag = 1;
                    }
                    fprintf(file1, "%s\n", arr_stops[IndexOfStopInArray1].buses[i]);
                }
            }
        }
    }
    if (flag > 0) {
        return 1;
    }
    return 0;
}

int IsNeighborsInArray(int *mas, int n, int num1, int num2){
    for (int i=0; i<n; i++){
        if (mas[i]==num1)
            if (mas[i+1]==num2)
                return 1;
    }
    return 0;
}

void WithTransfers(char *stop1, char *stop2, char depot[11][50], FILE *file1) {
    flag=0;
    for (int i = 1; i < arr_stops[IndexOfStopInArray1].amount_of_buses; i++) { //все автобусы через остановку 1
        IndexOfBusInArray1 = IsBusInArray(depot, len_depot, arr_stops[IndexOfStopInArray1].buses[i]);
        for (int j = 1; j < arr_stops[IndexOfStopInArray2].amount_of_buses; j++) { //все автобусы через остановку 2
            IndexOfBusInArray2 = IsBusInArray(depot, len_depot, arr_stops[IndexOfStopInArray2].buses[j]);
            for (int k = 1;
                 k < arr[IndexOfBusInArray1].amount_of_stops; k++) { //идем по остановкам первого автобуса
                for (int m = 1; m < arr[IndexOfBusInArray2].amount_of_stops; m++) {
                    if (strcmp(arr[IndexOfBusInArray1].stops[k], arr[IndexOfBusInArray2].stops[m]) == 0 &&
                        strcmp(arr[IndexOfBusInArray1].stops[k], "") !=0) { //если есть общая остановка и название остановок не ""
                        if (!flag) {
                            fprintf(file1, "%s %s %s %s%s\n", "BUSES FROM", stop1, "TO", stop2, ":");
                            flag = 1;
                        }
                        if (!IsNeighborsInArray(transfers, amount_of_transfers, IndexOfBusInArray1,
                                                   IndexOfBusInArray2)) { //если хотя бы один автобус новый, то берем всю пару
                            if (IndexOfBusInArray1 < IndexOfBusInArray2)
                                fprintf(file1, "%s %s\n", arr[IndexOfBusInArray1].name,
                                        arr[IndexOfBusInArray2].name);
                            else
                                fprintf(file1, "%s %s\n", arr[IndexOfBusInArray2].name,
                                        arr[IndexOfBusInArray1].name);
                            transfers[amount_of_transfers++] = IndexOfBusInArray1;
                            transfers[amount_of_transfers++] = IndexOfBusInArray2;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < amount_of_transfers; i++) { //обнуление массива пересадок
        transfers[i] = 0;
    }
    if (!flag)
        fprintf(file1, "%s %s %s %s\n", "THERE ARE NO BUSES FROM", stop1, "TO", stop2);
}

void Find(char *stop1, char *stop2, char all_stops[31][50], char depot[11][50], FILE *file1) {
    if (!WithoutTransfers(stop1, stop2, all_stops, file1))
        WithTransfers(stop1, stop2, depot, file1);
}

int main() {
    FILE *file;
    FILE *file1;
    file = fopen("input.txt", "rt");
    file1 = fopen("output.txt", "w+");
    if (!file) {
        puts("Error opening file");
        return -1;
    }

    char str[100], action1[10], action2[50], bus_number[50], bus_stop[50]="", depot[11][50], all_stops[31][50];
    InitArrayStruct();
    
    while (fgets(str, sizeof(str), file)){
        sscanf(str, "%s %s %s %s",action1, action2, bus_number, bus_stop);
            if (strcmp(action1, "ADD") == 0) {
                if (strlen(bus_number) <= 4 && strcmp(action2, "")!=0) { //проверка на неправильный ввод
                    if (strcmp(action2, "BUS") == 0) {
                        if (strlen(bus_stop)==0)
                            AddBus(bus_number, depot, file1);
                        else
                            fprintf(file1, "%s\n", "WRONG ADD FORMAT");
                    }
                    else if (strcmp(action2, "STOP") == 0)
                        AddStop(bus_number, bus_stop, depot, all_stops, file1);
                }
                else
                    fprintf(file1, "%s\n", "WRONG ADD FORMAT");
            }
            else if(strcmp(action1, "PRINT") == 0){
                if (strcmp(action2, "")!=0) { //проверка на правильность ввода
                    if (strcmp(action2, "BUS") == 0) {
                        PrintBus(bus_number, depot, file1);
                    } else if (strcmp(action2, "STOP") == 0) {
                        PrintStop(bus_number, all_stops, file1);
                    } else if (strcmp(action2, "ALL") == 0 && len_depot > 1) {
                        PrintAll(file1);
                    }
                }
                else
                    fprintf(file1, "%s\n", "WRONG PRINT FORMAT");
            }
            else if(strcmp(action1, "FIND") == 0){
                if (strcmp(bus_number, "")!=0 && strcmp(bus_stop, "")==0) //проверка на правильность ввода
                    Find(action2, bus_number, all_stops, depot, file1);
                else
                    fprintf(file1, "%s\n", "WRONG FIND FORMAT");
            }

        action1[0]='\0';action2[0]='\0';bus_number[0]='\0';bus_stop[0]='\0';
    }
}
