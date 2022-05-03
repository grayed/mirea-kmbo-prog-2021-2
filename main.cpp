//  Kursovaya_Romantsov_Andrey_KMBO-01-20

#include <iostream>
#include <cmath>

using namespace std;

class Exception : public exception
{
protected:
    //сообщение об ошибке
    char* str;
public:
    Exception(const char* s)
    {
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }
    Exception(const Exception& e)
    {
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
    }
    ~Exception()
    {
        delete[] str;
    }
    
    //функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
    virtual void print()
    {
        cout << "Исключение: " << str << "\n";
    }
};


class WrongEnterException : public Exception //неправильный ввод функции
{
public:
    WrongEnterException(const char* s) : Exception(s){}
    void print()
    {
        cout << str << "; неправильно введена функция\n";
    }
};


class UnknownSymbolException : public WrongEnterException //неизвестный символ при вводе функции
{
protected:
    char h;
public:
    UnknownSymbolException(const char* s, char symbol) : WrongEnterException(s)
    {
        h = symbol;
    }
    
    void print()
    {
        cout << str << "; неизвестный символ: " << h << "\n";
    }
};


class InaccessibleStepException : public Exception //степень не равна целому неотрицательному числу
{
protected:
    double step;
public:
    InaccessibleStepException(const char* s, double pow) : Exception(s)
    {
        step = pow;
    }
    
    void print()
    {
        cout << str << "; невозможная степень" << step << "\n";
    }
};


class WrongBoundariesException : public Exception //на данном промежутке функция не имеет решений
{
public:
    WrongBoundariesException(const char* s) : Exception(s){}
    
    void print()
    {
        cout << str << "; данная функция не имеет решений\n";
    }
};



int len_func(const char* p)
{
    int len = 0;
    if (p != nullptr)
        while (p[len] != '\0')
            len++;
    else
        throw WrongEnterException("\n введена пустая строка");
    if (len == 0)
        throw WrongEnterException("\n введена пустая строка");
    return len;
}


double take_koef(const char* pl, int i)
{
    while (!isdigit(pl[i]))
        i--;
    string koef = "";
    double poly_length = len_func(pl);
    bool flag_dot = 0;
    bool flag_minus = 0;
    while ((i >= 0) && (pl[i] != ' ')){
        if(isdigit(pl[i]) || ((i - 1 >= 0) && (i + 1 < poly_length) && (isdigit(pl[i - 1])) && (isdigit( pl[i + 1])) && (pl[i] == '.') && (!flag_dot))){
            if (pl[i] == '.')
                flag_dot = 1;
            koef = pl[i] + koef;
            i--;
        }
        else if ((pl[i] == '-') && (flag_minus == 0)){
            koef = pl[i] + koef;
            flag_minus = 1;
            i--;
        }
        else
            throw UnknownSymbolException("\n введён нечитаемый коэффициент", pl[i]);
    }
    if ((i > 0) && (pl[i - 1] == '-') && (flag_minus == 0))
        koef = pl[i - 1] + koef;
    else if ((i > 0) && (pl[i - 1] != '+'))
        throw UnknownSymbolException("\nвстречен бессмысленный символ", pl[i - 1]);
    double koef_db = stod(koef);
    if ((koef_db == 0) || (koef_db == 1))
        throw WrongEnterException ("коэффициент равен нулю или единице");
    return koef_db;
}


double take_step(const char* pl, int &i)
{
    double poly_length = len_func(pl);
    bool flag0 = 0; //происходит считывание степени
    if ((i + 2 <= poly_length) && (pl[i + 1] == ' ') && (isdigit(pl[i + 2])))
        i += 2;
    else if (isdigit(pl[i]))
        flag0 = 1; //происходит считывание свободного коэффициента
    else
        throw WrongEnterException ("\nНеверно введена степень");
    string step = "";
    if ((flag0) && (((i > 0) && (pl[i - 1] == '-')) || ((i > 1) && (pl[i - 2] == '-'))))
       step = '-' + step;
    while ((i < poly_length) && (pl[i] != ' ')){
        if (isdigit(pl[i]))
        {
            step += pl[i];
            i++;
        }
        else if ((flag0 == 1) && (pl[i] == '.')){
            step += pl[i];
            i++;
        }
        else
            throw WrongEnterException ("\nНеверно введена степень");
    }
    i--;
    double step_db = stod(step);
    if ((step_db == 0) || ((step_db == 1) && (flag0 == 0)))
        throw InaccessibleStepException ("\nвведена нулевая или первая степень или свободный член равен нулю", step_db);
    return step_db;
}


int max_step(const char* pl)
{
    int n = 0;
    int step_int = 0;
    int i = 0;
    double poly_length = len_func(pl);
    while (i < poly_length){
        
        if ((pl[i] != '^') && (pl[i] != '*') && (pl[i] != ' ') && (pl[i] != '+') && (pl[i] != '-') && (!isdigit(pl[i])) && (pl[i] != 'x') && (pl[i] != '.')) //проверка на существование лишних знаков
            throw UnknownSymbolException ("\nвведён неизввестный символ", pl[i]);
        if ((pl[i] == '-') && (!((i - 2 >= 0) && (pl[i - 1] == ' ') && ((pl[i - 2] == 'x') || (isdigit(pl[i - 2]))) && (i + 2 < poly_length) && (pl[i + 1] == ' ') && ((pl[i + 2] == 'x') || (isdigit(pl[i + 2]))) ) && !((i == 0) && (i + 1 < poly_length) && ((pl[i + 1] == 'x') || (isdigit(pl[i + 1]))) )) )
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около знака '-'");
        if ( (pl[i] == '+') && !((i - 2 >= 0) && (pl[i - 1] == ' ') && ((pl[i - 2] == 'x') || (isdigit(pl[i - 2]))) && (i + 2 < poly_length) && (pl[i + 1] == ' ') && ((pl[i + 2] == 'x') || (isdigit(pl[i + 2])))) )
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около знака '+'");
        if ( (pl[i] == '*') && !((i - 2 >= 0) && (pl[i - 1] == ' ') && (isdigit(pl[i - 2])) && (i + 2 < poly_length) && (pl[i + 1] == ' ') && (pl[i + 2] == 'x')))
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около знака '*'");
        if ( (pl[i] == '^') && !((i - 2 >= 0) && (pl[i - 1] == ' ') && (pl[i - 2] == 'x') && (i + 2 < poly_length) && (pl[i + 1] == ' ') && (isdigit(pl[i + 2]))) )
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около знака '^'");
        //а что с x пробелом числом и точкой
        if ((pl[i] == ' ') && ((i == 0) || (i == poly_length - 1) || (pl[i - 1] == ' ') || (pl[i + 1] == ' ') || (pl[i - 1] == '.') || (pl[i + 1] == '.') || ((isdigit(pl[i - 1])) && (isdigit(pl[i + 1]))) || (pl[i - 1] == pl[i + 1])))
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около пробела");
        if ((pl[i] == '.') && !((i > 0) && (i - 1 < poly_length) && (isdigit(pl[i - 1])) && (i + 1 < poly_length) && (isdigit(pl[i + 1]))))
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около знака '.'");
        if ((pl[i] == 'x') && !((((i == 0) || ((i - 1 > 0) && (pl[i - 1] == ' '))) && ((i + 1 == poly_length) || ((i + 1 < poly_length) && (pl[i + 1] == ' ')))) || ((i == 1) && (pl[i - 1] == '-'))))
            throw WrongEnterException ("\nвведённая функция не соответствует правилам ввода около знака 'x'");
        
        if (pl[i] == '^'){
            step_int = take_step(pl, i);
            if (step_int > n)
                n = step_int;
        }
        i++;
    }
    i = 0;
    if (n == 0)
        while (i < poly_length){
            if (pl[i] == 'x')
                n = 1;
            i++;
        }
    return n;
}


void null_first_step_check(const char* Poly, double &koef_first, double &koef_null)
{
    int len = len_func(Poly);
    double k = 0;
    for (int i = 0; i < len; i++){
        if (Poly[i] == 'x'){
            if (((i + 2 < len) && (Poly[i + 2] != '^')) || (i + 1 == len)){
                if ((i - 2 >= 0) && (Poly[i - 2] == '*') && (isdigit(Poly[i - 4])))
                    koef_first += take_koef(Poly, i - 2);
                else if (((i - 2 > 0) && (Poly[i - 2] == '-')) || ((i - 1 == 0) && (Poly[i - 1] == '-')))
                    koef_first--;
                else
                    koef_first++;
            }
        }
        else if (isdigit(Poly[i]))
            if ((((i - 2 >= 0) && (Poly[i - 2] != '^'))  && (!isdigit(Poly[i - 1])) && (Poly[i - 1] != '.')) || ((i == 1) && (!isdigit(Poly[i - 1]))) || (i == 0)){
                k = take_step(Poly, i); //число считаывается с помощью этой функции, так как необходимо дойти до правой границы числа, чтобы удобно взглянуть на наличие знака умножения после него.
                if (((i + 2 < len) && (Poly[i + 2] != '*')) || (i + 1 == len))
                    koef_null += k;
            }
    }
}





class Polynomial{
protected:
    double* kfcs; // коэффициенты при x
    int n; // степень функции
    double LeftX;//нижняя граница
    double RightX;//верхняя граница
public:
    Polynomial(double step = 2, double l = -100, double r = 100){ //первый конструктор
        n = step;
        LeftX = l;
        RightX = r;
        kfcs = new double[n + 1];
        for (int i = 0; i < n + 1; i++)
            kfcs [i] = 0;
    }
    
    Polynomial(char* Poly, double l = -100, double r = 100){ //второй конструктор
        n = max_step(Poly);
        LeftX = l;
        RightX = r;
        if (LeftX > RightX)
            throw WrongBoundariesException("\nЛевая граница поиска решений находится правее правой");
        kfcs = new double[n + 1];
        for (int i = 0; i < n + 1; i++)
            kfcs[i] = 0;
        
        int i = 0;
        int step_int = 0;
        double koef_db = 1;
        double poly_length = len_func(Poly);
        while (i < poly_length){ //+1?
            if (Poly[i] == '*')
            {
                koef_db = take_koef(Poly, i);
            }
            if (Poly[i] == '^')
            {
                if (((i - 4 > 0) && (koef_db == 1) && (Poly[i - 4] == '-')) || ((i == 3) && (koef_db == 1) && (Poly[i - 3] == '-')))
                    koef_db = -1;
                step_int = take_step(Poly, i);
                kfcs[step_int] += koef_db;
                koef_db = 1;
            }
            i++;
        }
        null_first_step_check(Poly, kfcs[1], kfcs[0]);
    }
    
    Polynomial(const char* Poly, double l = -100, double r = 100){ //третий конструктор
        n = max_step(Poly);
        LeftX = l;
        RightX = r;
        if (LeftX > RightX)
            throw WrongBoundariesException("\nЛевая граница поиска решений находится правее правой");
        kfcs = new double[n + 1];
        for (int i = 0; i < n + 1; i++)
            kfcs[i] = 0;
        
        int i = 0;
        int step_int = 0;
        double koef_db = 1;
        double poly_length = len_func(Poly);
        while (i < poly_length){
            if (Poly[i] == '*')
            {
                koef_db = take_koef(Poly, i);
            }
            if (Poly[i] == '^')
            {
                if (((i - 4 > 0) && (koef_db == 1) && (Poly[i - 4] == '-')) || ((i == 3) && (koef_db == 1) && (Poly[i - 3] == '-')))
                    koef_db = -1;
                step_int = take_step(Poly, i);
                kfcs[step_int] += koef_db;
                koef_db = 1;
            }
            i++;
        }
        null_first_step_check(Poly, kfcs[1], kfcs[0]);
    }
    
    
    void get_kfcs(){
        for (int i = n; i >= 0; i--)
            if (kfcs[i] != 0)
                cout << "\nСтепень: " <<  i << " коэффициент: " << kfcs[i];
    }
    
    ~Polynomial() //деструктор
    {
        if (kfcs != NULL)
            delete[] kfcs;
        n = 0;
    }
};





class D_Polynom : public Polynomial
{
public:
    D_Polynom(char* D_Poly, double l = -100, double r = 100) : Polynomial(D_Poly, l, r) {}
    D_Polynom(const char* D_Poly, double l = -100, double r = 100) : Polynomial(D_Poly, l, r) {}
    D_Polynom(double step) : Polynomial(step) {}
    ~D_Polynom() {}
    
    D_Polynom(D_Polynom &P2){
        n = P2.n;
        kfcs = new double[n + 1];
        LeftX = P2.LeftX;
        RightX = P2.RightX;
        for (int i = 0; i < n + 1; i++)
            kfcs[i] = P2.kfcs[i];
    }

    
    double operator()(double x) //возвращает значение функции в точке x
    {
        double result = 0;
        for (int i = n; i >= 0; i--)
            result = x * result + kfcs[i];
        return result;
    }
    D_Polynom derivative() //возвращает продифференцированную функцию
    {
        D_Polynom P2(*this);
        for (int i = 0; i < P2.n; i++)
            P2.kfcs[i] = P2.kfcs[i+1] * (i+1);
        P2.n--;
        return P2;
    }
    
    D_Polynom& operator=(D_Polynom& P2)
    {
        if (kfcs != NULL)
            delete[] kfcs;
        n = P2.n;
        kfcs = new double[n + 1];
        LeftX = P2.LeftX;
        RightX = P2.RightX;
        for (int i = 0; i < n + 1; i++)
            kfcs[i] = P2.kfcs[i];
        return *this;
    }
    
    
    D_Polynom& operator/(double x) //удаляет один из корней
    {
        D_Polynom P2(n - 1);
        P2.kfcs[n - 1] = kfcs[n];
        for (int i = n - 2; i >= 0; i--){
            P2.kfcs[i] = P2.kfcs[i + 1] * x + kfcs[i + 1];
            P2.kfcs[i] = round(P2.kfcs[i] * 100000)/100000;
        }
        *this = P2;
        return *this;
    }
    

    
    bool right_ans(double func, D_Polynom& Poly, double e, double ans){ //проверяет значение функции на равенство нулю
        if (abs(func) <= e){
            Poly = Poly/ans;
            cout <<"\nОтвет: " << ans;
            Poly.get_kfcs();
            Poly.dichotomy_method();
            return 1;
        }
        return 0;
    }

    bool dichotomy_method(double l = 100, double r = -100, double e = 0.0001) //метод половинного деления (метод дихотомии)
    {
        if (n > 1) {
            if ((n == 2) && ((kfcs[1] * kfcs[1] - 4 * kfcs[0] * kfcs[2]) < - 4 * e))
                return 0;
            LeftX = l;
            RightX = r;
            double m = (l + r)/2;
            double f1 = (*this)(l);
            double f2 = (*this)(r);
            double f3 = (*this)(m);
            bool flag_ans = 0;
            cout << "\nl = " << l << ", r = " << r << ", f1 = " << f1 << ", f2 = " << f2;
            if (!((right_ans(f1, *this, e, l)) || (right_ans(f2, *this, e, r))))
            {
                if (f1 * f3 <= 0)
                    flag_ans = dichotomy_method(l, m, e);
                else if (f2 * f3 <= 0)
                    flag_ans = dichotomy_method(m, r, e);
                else if ((abs(l - r) > e) && (!flag_ans)){
                    if (f1 < f2)
                        flag_ans = dichotomy_method(l, m, e);
                    if ((f1 > f2) && (!flag_ans))
                        dichotomy_method(m, r, e);
                    //двусторонняя рекурсия, пока f(l)*f(r) > 0
                }
            }
            else
                return 1;
        }
        else if (n == 1){
            cout << "\nОтвет: " << - kfcs[0]/kfcs[1];
            n = 0;
            kfcs[0] = 0;
            return 1;
        }
        return 0;
    }
    
    
    void Newtons_method(double x = 0, double e = 0.000000000000001) //метод Ньютона (метод касательных)
    {
        double f1;
        double f2;
        bool dis_flag = 0;
        int num_of_iter = 0;
        while ((n > 1) && (!dis_flag)){
            if ((x <= LeftX) || (x >= RightX))
                throw WrongBoundariesException ("неверно введена точка опоры, или алгоритм вышел за пределы указанных границ");
            if ((n == 2) && ((kfcs[1] * kfcs[1] - 4 * kfcs[0] * kfcs[2]) < - 4 * e)){
                cout << "\nдискриминант меньше нуля, больше корней нет";
                dis_flag = 1;
            }
            else if ((n > 2) && (n % 2 == 0) && (num_of_iter > 100))
                dis_flag = 1;
            else{
                num_of_iter++;
                while ((this->derivative())(x) == 0)
                    x = x + 1;
                f1 = (*this)(x);
                f2 = (this->derivative())(x);
                cout << "\nx = " << x  << ", f1 = " << f1 << ", f2 = " << f2;
                if (abs(f1) < e){
                    *this = (*this)/x;
                    cout <<"\nОтвет: " << x;
                    this->get_kfcs();
                    num_of_iter = 0;
            }
            x = x - f1/f2;
            }
        }
        if (n == 1)
            cout << "\nОтвет: " << - kfcs[0]/kfcs[1];
    }
    
    
    
    void secant_method(double e = 0.0000000000001) //метод секущих
    {
        double f0 = (*this)(LeftX);
        double f1 = (*this)(RightX);
        double x0 = LeftX;
        double x1 = RightX;
        double x2 = 0;
        bool dis_flag = 0;
        int num_of_iter = 0;
            
        while ((n > 1) && (!dis_flag)){
            if ((n == 2) && ((kfcs[1] * kfcs[1] - 4 * kfcs[0] * kfcs[2]) < - 4 * e)){
                cout << "\nдискриминант меньше нуля";
                dis_flag = 1;
            }
            else if ((n > 2) && (n % 2 == 0) && (num_of_iter > 100))
                dis_flag = 1;
            else{
                if (abs(f1) < e){
                    num_of_iter = 0;
                    *this = (*this)/x2;
                    cout <<"\nОтвет: " << x2;
                    this->get_kfcs();
                    x0 = LeftX;
                    x1 = RightX;
                    f0 = (*this)(x0);
                    f1 = (*this)(x1);
                }
                if (f1 != f0){
                    num_of_iter++;
                    x2 = x1 - f1 * (x1 - x0)/(f1 - f0);
                    f0 = f1;
                    f1 = (*this)(x2);
                    x0 = x1;
                    x1 = x2;
                }
                else{
                    while (f0 == f1){
                        x0 += 0.00001;
                        f0 = (*this)(x0);
                    }
                }
                cout << "\nx = " << x2  << ", f0 = " << f0 << ", f1 = " << f1;
            }
        }
        if (n == 1)
            cout << "\nОтвет: " << - kfcs[0]/kfcs[1];
    }
    
    
    
    void iteration_method(double e = 0.00000000000001) //метод простых итераций
    {
        int low_step = 1;
        while (kfcs[0] == 0){
            *this = (*this)/0;
            cout <<"\nОтвет: " << 0;
            this->get_kfcs();
        }
        
        bool dis_flag = 0;
        int num_of_iter = 0;
        
        while ((n > 1) && (!dis_flag)){
            D_Polynom P2(*this);
            if ((n == 2) && ((kfcs[1] * kfcs[1] - 4 * kfcs[0] * kfcs[2]) < - 4 * e)){
                cout << "\nдискриминант меньше нуля";
                dis_flag = 1;
            }
            else if ((n > 2) && (n % 2 == 0) && (num_of_iter > 1000))
                dis_flag = 1;
            else{
                if (P2.kfcs[1] != 0){
                    for (int i = 0; i < P2.n + 1; i++)
                        if (i != 1)
                            P2.kfcs[i] = -P2.kfcs[i] / P2.kfcs[1];
                    P2.kfcs[1] = 0;
                    low_step = 1;
                }
                else{
                    low_step = 2;
                    while (P2.kfcs[low_step] == 0)
                        low_step++;
                    for (int i = 0; i < P2.n + 1; i++)
                        if (i != low_step)
                            P2.kfcs[i] = -P2.kfcs[i] / P2.kfcs[low_step];
                    P2.kfcs[low_step] = 0;
                }
                double x = P2((LeftX + RightX) / 2);
                while ((abs(pow(x, low_step) - P2(x)) > e) && (dis_flag == 0)){
                    if (low_step > 1){
                        if (x != 0)
                            x = P2(x) / pow(x, low_step - 1);
                        else
                            throw Exception("попытка разделить на ноль");
                    }
                    else
                        x = P2(x);
                    if ((n > 2) && (n % 2 == 0) && (num_of_iter > 1000))
                        dis_flag = 1;
                    num_of_iter++;
                    if (x == P2((LeftX + RightX) / 2))
                        x = P2((LeftX + RightX) / 2) - 0.5;
                }
                *this = (*this)/x;
                cout <<"\nОтвет: " << x;
                this->get_kfcs();
            }
        }
        if (n == 1)
            cout << "\nОтвет: " << - kfcs[0]/kfcs[1];
    }
    
};





int main(int argc, const char * argv[]) {
    try
    {
        D_Polynom poly_2("x ^ 3 - 3 * x ^ 2 + 3 * x - 1");
        //D_Polynom poly_2("7 * x ^ 6 - 32 * x ^ 5 - 15 * x ^ 4 - 7 * x ^ 2 + 32 * x + 15"); //работают все 3
        //D_Polynom poly_2("x ^ 6 - x - 1"); //не работает метод итераций
        //D_Polynom poly_2("x ^ 2 - 1");
        //cout << poly_2(-0.5);
        poly_2.Newtons_method(); // 4/4
        //poly_2.secant_method(); // 4/4
        //poly_2.iteration_method(); // 3/4
        //poly_2.dichotomy_method(-100, 100, 0.0000001); // 4/4
        //poly_2.get_kfcs();
    }
    catch (WrongBoundariesException e)
    {
        cout << "\nWrongBoundariesException is caught: ";
        e.print();
    }
    catch (InaccessibleStepException e)
    {
        cout << "\nInaccessibleStep is caught: ";
        e.print();
    }
    catch (UnknownSymbolException e)
    {
        cout << "\nUnknownSymbolException is caught: ";
        e.print();
    }
    catch (WrongEnterException e)
    {
        cout << "\nWrongEnterException is caught: ";
        e.print();
    }
    catch (Exception e)
    {
        cout << "\nException is caught: ";
        e.print();
    }
    catch (exception e)
    {
        cout << "\exception is caught: ";
        cout << e.what();
    }
    return 0;
}
