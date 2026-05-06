#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <cstdio>
using namespace std;

const int MAX_LEN=100;

//СТРУКТУРА ЛЕТАТЕЛЬНОГО АППАРАТА
struct Aircraft{
    wchar_t model[MAX_LEN];                            //марка ЛА
    wchar_t number[MAX_LEN];                           //бортовой номер ЛА
    double height;                                  //высота ЛА
    int echelon;                                    //номер эшелона
};

//ПРОТОТИПЫ ФУНКЦИЙ
bool strcopy(wchar_t (&dest)[MAX_LEN], wchar_t (&dep)[MAX_LEN]);
void errors(int code, int str);
bool str_to_double(double (&dest), wchar_t dep[MAX_LEN], int i);
int* index_sort(Aircraft *aircrafts, int n);                //индексная сортировка


int main(){
    
    wifstream input("input1.txt");            //входной файл

    if(!input.is_open()){
        errors(1, 0);
        return 1;
    }

    int n;                                  //количество записей
    input>>n;

    wchar_t e;
    input.get(e);                           //считает перевод строки после n

    Aircraft *aircrafts=new Aircraft[n],        //массив ЛА
              temp_aircraft;                    //для записи текущей строки данных
    wchar_t current='\0',                          //входные данные для текущего ЛА
         word[MAX_LEN];
    double height;                          //высота ЛА
    int cnt=0,                              //количество ЛА 
        cnt_word=0;                         //количество символов в word
    bool f1=false,f2=false,f3=false;        //флаги, отображающие корректность данных во всех полях структуры

    for(int i=0;i<n;++i){                   //цикл п строкам файла
        for(int k=0;k<3;++k){                   //цикл по 3м словам в строке, разделенным пробелами

            wcin>>word;

            switch(k){                              //инициализация поля структуры
            case 0:
                f1 = strcopy(temp_aircraft.model, word);
                break;
            case 1:
                f2 = strcopy(temp_aircraft.number, word);
                break;
            case 2:
                f3 = str_to_double(temp_aircraft.height, word, i);
                break;
            }
            if(k==2 && current!='\n')
                while(current!='\n' && !input.eof())
                    input.get();
            //wcout<<word<<'\n';                     //проверка: печать считанного слова
        }

        
       if ((current != '\n' && i!=n-1) || (!input.eof() && i == n - 1)) {                          //в строке больше слов
            errors(3, i);                            //выводим предупреждение, но проверяем первые три слова
            continue;
        }

        //если все поля заполнены правильно
        if(f1 && f2 && f3){
            temp_aircraft.echelon=ceil(temp_aircraft.height/1.5);     //вычислить номер эшелона по высоте
            aircrafts[cnt++]=temp_aircraft;                           //добавить ЛА в массив
        }
    }

    int* aircrafts_sorted = index_sort(aircrafts, cnt);
    return 0;
}

bool strcopy(wchar_t (&dest)[MAX_LEN], wchar_t (&dep)[MAX_LEN]){    //копирование строк
    if(dep[0]=='\0') 
        return false;
    int i=0;
    for(;dep[i]!='\0';++i)
        dest[i]=dep[i];
    dest[i]='\0';
    return true;
}

void errors(int code, int str){                         //функция печати сообщения об ошибке по коду ошибки
    switch(code){
    case 1:
        wcout<<"Ошибка: не удалось открыть файл\n";
        break;
    case 2:
        wcout<<"Ошибка: недостаточно данных в строке "<<str+1<<'\n';
        break;
    case 3:
        wcout<<"Предупреждение: в строке "<<str+1<<" больше данных, чем требуется\n";
        break;
    case 4:
        wcout<<"Нет данных о летательных аппаратах\n";
        break;
    }
}

bool str_to_double(double (&dest), wchar_t dep[MAX_LEN], int i){    //преобразование прочитанного слова в double + проверка высоты
    wchar_t* ptr = nullptr;
    double v = wcstod(dep, &ptr);
    if(*ptr=='\0'){
        if(v>15 || v<0){
            errors(5, i);
            return false;
        }
        dest=v;
        return true;
    }
    else 
        errors(6, i);
    return false;
}

int* index_sort(Aircraft *aircrafts, int n){
    int *ind = new int[n];                              //массив номеров ЛА в исходном списке
    for(int i=0;i<n;++i)                                //заполнение массива номерами ЛА
        ind[i]=i;

    //сортировка вставками
    for(int i=1; i<n; ++i){
        int now=ind[i];
        int j=i-1;
        while(j>=0 && aircrafts[ind[j]].echelon < aircrafts[now].echelon){
            ind[j+1]=ind[j];
            --j;
        }
        ind[j+1]=now;
    }
    return ind;
}