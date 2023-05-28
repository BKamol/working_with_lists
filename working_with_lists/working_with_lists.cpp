#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct elem
{
    int info;
    elem* next;
};

bool is_empt(elem* top)
{
    return top->next == NULL && top->info == -1;
}

elem* create_list(string file_name)
{
    ifstream in(file_name);
    //пустой первый элемент
    elem* top = new elem;
    top->info = -1;
    top->next = NULL;

    elem* p = top;
    int i = 0;
    while (in >> i)
    {
        p->next = new elem;
        p = p->next;
        p->info = i;
    }
    p->next = NULL;
    return top;
}

void show(elem* top)
{
    if (is_empt(top)) cout << "Empty list";
    top = top->next;
    while (top != NULL)
    {
        cout << top->info << ' ';
        top = top->next;
    }
    cout << endl;
}

elem* copy(elem* top)
{
    elem* top1 = new elem;
    top1->info = -1;
    top1->next = NULL;

    elem* p = top;
    elem* p1 = top1;

    while (p->next != NULL)
    {
        p1->next = new elem;
        p1 = p1->next;
        p1->info = p->next->info;
        p = p->next;
    }
    p1->next = NULL;

    return top1;
}

void del(elem* &top, int x)
{
    elem* p = top;
    elem* q;
    bool is_empty = is_empt(top);
    if (is_empty) cout << "Empty list" << endl;

    while (p->next != NULL && p->next->next != NULL)
    {
        if (p->next->info == x)
        {
            q = p->next;
            p->next = p->next->next;
            delete q;
        }
        else
        {
            p = p->next;
        }

    }
    if (p->next != NULL && p->next->next == NULL && p->next->info == x)
    {
        q = p->next;
        p->next = NULL;
        delete q;
    }

}

bool search(elem* top, int x)
{
    elem* p = top;
    bool res = false;
    while (p->next != NULL && !res)
    {
        if (p->next->info == x) res = true;
        p = p->next;
    }

    return res;
}

int len(elem* top)
{
    elem* p = top;
    int i = 0;
    while (p->next != NULL)
    {
        i++;
        p = p->next;
    }
    return i;
}

bool is_subset(elem* top1, elem* top2)
{
    bool res = true;
    elem* p1 = top1;
    elem* p2 = top2;
    bool found;

    while (p1->next != NULL && res)
    {
        p2 = top2;
        found = false;
        while (p2->next != NULL && !found)
        {
            if (p2->next->info == p1->next->info) found = true;
            p2 = p2->next;
        }
        if (!found) res = false;
        p1 = p1->next;
    }

    return res;
}

bool equal_without_order(elem* top1, elem* top2)
{
    return len(top1) == len(top2) && is_subset(top1, top2) && is_subset(top2, top1);
}

bool is_subset_with_order(elem* p1, elem* p2) //проверяет входит ли p1 в p2 сплошным фрагментом
{
    bool res = true;
    while (p1->next != NULL && p2->next != NULL && res)
    {
        if (p1->next->info != p2->next->info) res = false;
        p1 = p1->next;
        p2 = p2->next;
    }
    if (p2->next == NULL && p1->next != NULL) res = false;

    return res;
}

int is_sublist(elem* top1, elem* top2) //Возвращает индекс начала вхождения списка top1 в top2
{
    if (len(top1) > len(top2)) return -1; //Если top1 не входит в top2, то возвращает -1

    bool found = false;;
    int res = -1;
    elem* p1 = top1;
    elem* p2 = top2;

    while (p2->next != NULL && !found)
    {
        res++;
        if (p2->next->info == p1->next->info)
        {
            if (is_subset_with_order(p1, p2)) found = true;

        }
        p2 = p2->next;
    }
    if (!found) res = -1;
    return res;
}

void delete_top2_from_top1_once(elem* top2, elem* &top1)
{
    elem* p1 = top1;
    elem* q;
    int ind = is_sublist(top2, top1);
    if (ind != -1)
    {
        for (int i = 0; i < ind; i++) p1 = p1->next; //Перехожу к элементу, cразу после которого идет фрагмент, который нужно удалить

        for (int i = 0; i < len(top2); i++) // нужно удалить столько элементов, сколько их в top2
        {
            q = p1->next;
            p1->next = q->next;
            delete q;
        }
    }

}

void delete_top2_from_top1(elem* top2, elem* &top1)
{
    int ind = is_sublist(top2, top1);
    while (ind != -1)
    {
        delete_top2_from_top1_once(top2, top1);
        ind = is_sublist(top2, top1);
    }
    
}

void replace_once(elem* top1, elem* top2, elem* top3) //Заменяет первое вхождение top2 в top1 на top3
{
    elem* p1 = top1;
    elem* q;
    int ind = is_sublist(top2, top1); //Получаю индекс вхождения top2 в top1
    if (ind != -1)
    {
        for (int i = 0; i < ind; i++) p1 = p1->next; //Перехожу к элементу, cразу после которого идет фрагмент, который нужно заменить
        for (int i = 0; i < len(top2); i++) // удаляю старый фрагмент
        {
            q = p1->next;
            p1->next = q->next;
            delete q;
        }
        q = p1->next; //Элемент после p1 нужно соединить с последним элементом top1
        if (top3->next != NULL) p1->next = top3->next; //если top3 не пустой, то соединяю его начало с местом, куда его нужно вставить
        while (top3->next != NULL)
        {
            top3 = top3->next;
        }
        top3->next = q; //Соединяю конец top3 с остальной частью top1
    }
}

void add_top3_after_top2_once(elem* top1, elem* top2, elem* top3)
{
    elem* p1 = top1;
    elem* q;
    int ind = is_sublist(top2, top1);
    if (ind != -1)
    {
        for (int i = 0; i < ind + len(top2); i++) p1 = p1->next; //перехожу к концу top2
        q = p1->next;
        p1->next = top3->next; //склеиваю конец top2 и начало top3
        while (top3->next != NULL) top3 = top3->next; //перехожу к концу top3
        top3->next = q; //скеливаю конец top3 и оставшуюся часть top1
    }
}

void add_top3_before_top2_once(elem* top1, elem* top2, elem* top3)
{
    elem* p1 = top1;
    elem* q;
    int ind = is_sublist(top2, top1);
    if (ind != -1)
    {
        for (int i = 0; i < ind; i++) p1 = p1->next; //перехожу к элементу прямо перед вхождением top2 в top1
        q = p1->next;
        p1->next = top3->next; //склеиваю элемент перед top2 с началом top3
        while (top3->next != NULL) top3 = top3->next; //перехожу в конец top3
        top3->next = q; //склеиваю конец top3 с началом вхождения top2 в top1
    }
}

bool top2_contains_kratn_x(elem* top2, int x)
{
    bool res = false;
    while (top2->next != NULL && !res)
    {
        if (top2->next->info % x == 0) res = true; //нашелся эелемент, делящийся на x
        top2 = top2->next;
    }
    return res;
}

bool top3_contains_x(elem* top3, int x)
{
    bool res = false;
    while (top3->next != NULL && !res)
    {
        if (top3->next->info == x) res = true;
        top3 = top3->next;
    }
    return res;
}

elem* task7(elem* top1, elem* top2, elem* top3)
{
    elem* top4 = new elem;
    top4->info = -1;
    elem* p = top4;
    int x;
    while (top1->next != NULL)
    {
        x = top1->next->info;
        if (top2_contains_kratn_x(top2, x) && !top3_contains_x(top3, x))
        {
            p->next = new elem;
            p = p->next;
            p->info = x;
        }
        top1 = top1->next;
    }
    p->next = NULL;

    return top4;
}

int main()
{
    elem* top = create_list("nums.txt");
    show(top);

    elem* top1 = create_list("nums1.txt");
    show(top1);

    elem* top2 = create_list("nums2.txt");
    show(top2);

    //for (int i = 1; i <= 5; i++) del(top, i);
    //show(top);
    //cout << is_sublist(top1, top);

    //delete_top2_from_top1(top1, top);
    //show(top);

    //replace_once(top, top1, top2);
    //show(top);

    //add_top3_after_top2(top, top1, top2);
    //show(top);

    //add_top3_before_top2_once(top, top1, top2);
    //show(top);

    //elem* top3 = task7(top, top1, top2);
    //show(top3);
}


