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
    wchar_t model[MAX_LEN];                             //марка ЛА
    wchar_t number[MAX_LEN];                            //бортовой номер ЛА
    double height;                                      //высота ЛА
    int echelon;                                        //номер эшелона
};

//ПРОТОТИПЫ ФУНКЦИЙ

/*КОПИРОВАНИЕ СТРОКИ
dest - указатель на целевой массив
depp - исходная строка, которую нужно скопировать*/
bool strcopy(wchar_t (&dest)[MAX_LEN], wchar_t (&dep)[MAX_LEN]);

/*ВЫВОД СООБЩЕНИЯ ОБ ОШИБКЕ ПО КОДУ ОШИБКИ
er - код ошибки
str - номер строки, в которой возникла ошибка при вводе данных*/
void errors(int code, int str);

/*ПРЕОБРАЗОВАНИЕ СТРОКИ В ЧИСЛО
dest - указатель на целевое значение
dep - строка для преобразования
i - строка файла, из которой читаются данные*/
bool str_to_double(double (&dest), wchar_t dep[MAX_LEN], int i);

/*ПРОВЕРКА ВЫСОТЫ ЛА
height - прочитанное из файла значение высоты*/
bool height_check(double height);

/*ИНДЕКСНАЯ СОРТИРОВКА МЕТОДОМ ВСТАВОК ПО ЭШЕЛОНАМ
aircrafts - указатели на ЛА
n - количество ЛА*/
int* index_sort(Aircraft *aircrafts, int n);


void print_sorted(Aircraft* aircrafts, int* sorted, int n);

void print_fixedwide(wstring word, int wide);

void table_header();

wstring int_towstring(int value);

wstring double_towstring(double value);



//ОСНОВНАЯ ПРОГРАММА
int main(){
    
    wifstream input("input1.txt");           //входной файл

    if(!input.is_open()){                   //попытка открытия файла
        errors(1, 0);                       //вывод сообщения об ошибке, если файл не открылся
        return 1;
    }

    int n;                                  //количество записей
    input>>n;

    wchar_t e;
    input.get(e);                           //считывание перевода строки после ввода n

    //ОБЪЯВЛЕНИЕ ПЕРЕМЕННЫХ
    Aircraft *aircrafts=new Aircraft[n],    //массив ЛА
              temp_aircraft;                //переменная для записи текущей строки данных
    wchar_t current='\0',                   //символ для считывания входных данных для текущего ЛА
         word[MAX_LEN];                     //строка для считывания входных данных для текущего ЛА
    double height;                          //высота ЛА
    int cnt=0,                              //количество ЛА 
        cnt_word=0;                         //количество символов в word
    bool f1=false,f2=false,f3=false;        //флаги, отображающие корректность данных во всех полях структуры

    for(int i=0;i<n;++i){                   //цикл по строкам файла
        for(int k=0;k<3;++k){                   //цикл по 3м словам в строке, разделенным пробелами

            //проверка введенного слова
            word[0]='\0';                           //обнуление word
            cnt_word=0;                             //и длины word

            //игнорирование пробелов в начале строки
            do {
                input.get(current);                 //считывание символа
                if(input.eof())                     //проверка конца файла
                    break;
            } while(current == ' ' && k==0);

            if(current=='\n' && word[0]=='\0'){
                errors(2,i);                        //недостаточно данных(пустая строка)
                continue;
            }
            while(current!=' ' && current!='\n' && !input.eof()){
                word[cnt_word++]=current;
                word[cnt_word]='\0';
                input.get(current);
            }
            //проверка введенного слова

            switch(k){                          //инициализация поля структуры
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

                                                     //если в строке больше слов, чем 3
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

    int* aircrafts_sorted = index_sort(aircrafts, cnt);     //индексная сортировка
                                                            //aircrafts_sorted - массив индесов ЛА, отсортированных по эшелону
    
    table_header();
    print_sorted(aircrafts, aircrafts_sorted, n);

    return 0;
}

//КОПИРОВАНИЕ СТРОКИ
bool strcopy(wchar_t (&dest)[MAX_LEN], wchar_t (&dep)[MAX_LEN]){

    if(dep[0]=='\0')                    //проверка на пустую строку
        return false;

    int i=0;
    for(;dep[i]!='\0';++i)             //цикл по символам строки dep
        dest[i]=dep[i];                //копирование слова посимвольно
    dest[i]='\0';

    return true;
}

//ВЫВОД СООБЩЕНИЯ ОБ ОШИБКЕ ПО КОДУ ОШИБКИ
void errors(int code, int str){
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

//ПРЕОБРАЗОВАНИЕ СТРОКИ В ЧИСЛО
bool str_to_double(double (&dest), wchar_t dep[MAX_LEN], int i){

    wchar_t* ptr = nullptr;             //указатель для проверки преобразования
    double v = wcstod(dep, &ptr);       //попытка преобразования dep в число
    if(*ptr=='\0'){                     //если преобразована вся строка
        if(!height_check(v)){           //проверка высоты: от 0 км до 15 км
            errors(5, i);
            return false;
        }
        dest=v;                         //проверку прошла успешно - инициализируем поле структуры
        return true;
    }
    else
        errors(6, i);                  //вывод ошибки о некорректном значении
    return false;
}

//ПРОВЕРКА ВЫСОТЫ ЛА
bool height_check(double height){

    if(height>15 || height<0){            //если высота не находиться впределах от 0 до 15 км - ошибка
        return false;
    }
    return true;
}

//ИНДЕКСНАЯ СОРТИРОВКА МЕТОДОМ ВСТАВОК ПО ЭШЕЛОНАМ
int* index_sort(Aircraft *aircrafts, int n){

    int *ind = new int[n];                //массив номеров ЛА в исходном списке
    for(int i=0;i<n;++i)                  //заполнение массива номерами ЛА
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

void table_header(){                                    //функция печати шапки таблицы
    for(int i=0;i<70;++i){
        wcout<<L'=';
    }
    wcout<<L'\n'<<L'|'
         <<L"N"<<L"   |";

    wcout<<"Марка ЛА";
    for(int i=0;i<8;++i){
        wcout<<L' ';                                    //в сумме 16
    }
    wcout<<L'|'<<"Бортовой номер";
    for(int i=0;i<4;++i){
        wcout<<L' ';
    }
    wcout<<L'|'<<"Высота";
    for(int i=0;i<4;++i){
        wcout<<L' '; 
    }                                                  //в сумме 10
    wcout<<L'|'<<"Эшелон";
    for(int i=0;i<4;++i){
        wcout<<L' ';
    }

    wcout<<L'|';
    //печать номера ЛА в исходном массиве
    wcout<<L"ID";
    for(int i=0;i<3;++i)
        wcout<<" ";
    wcout<<L'|'<<L'\n';

    for(int i=0;i<70;++i)
        wcout<<L'=';
    wcout<<'\n';
}

void print_sorted(Aircraft* aircrafts, int* sorted, int n){
    if(n==0){
        errors(4,0);
        return;
    }
    int ind;
    
    table_header();                         //печать шапки таблицы

    //цикл по всем ЛА, печатающий информацию о ЛА
    for(int i=0;i<n;++i){
        ind=sorted[i];                      //номер ЛА в исходном массиве

        wstring model=aircrafts[ind].model,
                number=aircrafts[ind].number,
                height, 
                echelon, 
                ind1, 
                ind2;
        
        //преобразование чисел в строки для вычисления длины и печати
        ind1 = int_towstring(i+1);                                        //номер ЛА в отсортированном массиве
        ind2 = int_towstring(ind+1);                                      //номер ЛА в исходном массиве
        height = double_towstring(aircrafts[ind].height);          //высоту берем с точностью до 1 знака после запятой
        echelon = int_towstring(aircrafts[ind].echelon);

        //печать столбца номера ЛА в отсортированном массиве
        print_fixedwide(ind1, 4);
        //печать модели ЛА
        print_fixedwide(model, 16);
        //печать бортового номера
        print_fixedwide(number, 18);
        //печать высоты ЛА
        print_fixedwide(height, 10);
        //печать номера эшелона
        print_fixedwide(echelon, 10);
        //печать столбца номера ЛА в исходной таблице
        print_fixedwide(ind2, 5);

        wcout<<L'|'<<L'\n';

        //печать разделителя строк
        for(int j=0;j<70;++j){
            wcout<<L'-';
        }
        wcout<<L'\n';
    }
}

void print_fixedwide(wstring word, int wide){
    int l=word.size();                      //длина слова
    wcout<<L'|';
    wcout<<word;
    for(int i=0;i<wide-l;++i){
        wcout<<L' ';
    }
}

wstring int_towstring(int value){
    wstring res=L"";                           //создание пустой строки
    wchar_t word[MAX_LEN];                     //массив символов для преобразование из double в char[]
    swprintf(word, MAX_LEN, L"%d", value);    //преобразование double в wchar_t
    for(int i=0;word[i]!='\0';++i){
        res = res + word[i];                   //добавление символа в строку результата
    }
    return res;
}

wstring double_towstring(double value){
    wstring res=L"";                           //создание пустой строки
    wchar_t word[MAX_LEN];                     //массив символов для преобразование из double в char[]
    swprintf(word, MAX_LEN, L"%.1f", value);    //преобразование double в wchar_t
    for(int i=0;word[i]!='\0';++i){
        res = res + word[i];                   //добавление символа в строку результата
    }
    return res;
}