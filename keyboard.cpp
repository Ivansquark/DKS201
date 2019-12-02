#include "keyboard.h"
#include "ui_keyboard.h"
#include "mainwindow.h"

keyboard::keyboard(QWidget *parent) : QDialog(parent), ui(new Ui::keyboard)
{
    ui->setupUi(this);
    line="";
}

keyboard::~keyboard()
{
    delete ui;
}

void keyboard::setKeyString(QString str)
{

   ui->lineEdit->setText(str);
   line=str;
}
void keyboard::clearKeyK(int x) //-функция принимающая сигнал и управляющая клавиатурой;
{
    if (x==1)    //-очищает строку клавиатуры
    {
        line="";
        ui->lineEdit->clear();
    }
    else
    {
        if (x==2)      //- делает видимыми только цифры с точкой
        {
            ui->b_q->setEnabled(false);
            ui->b_w->setEnabled(false);
            ui->b_e->setEnabled(false);
            ui->b_r->setEnabled(false);
            ui->b_t->setEnabled(false);
            ui->b_y->setEnabled(false);
            ui->b_u->setEnabled(false);
            ui->b_i->setEnabled(false);
            ui->b_o->setEnabled(false);
            ui->b_p->setEnabled(false);
            ui->b_pp->setEnabled(false);
            ui->b_ppp->setEnabled(false);
            ui->b_a->setEnabled(false);
            ui->b_s->setEnabled(false);
            ui->b_d->setEnabled(false);
            ui->b_f->setEnabled(false);
            ui->b_g->setEnabled(false);
            ui->b_h->setEnabled(false);
            ui->b_j->setEnabled(false);
            ui->b_k->setEnabled(false);
            ui->b_l->setEnabled(false);
            ui->b_ll->setEnabled(false);
            ui->b_lll->setEnabled(false);
            ui->b_z->setEnabled(false);
            ui->b_x->setEnabled(false);
            ui->b_c->setEnabled(false);
            ui->b_v->setEnabled(false);
            ui->b_b->setEnabled(false);
            ui->b_n->setEnabled(false);
            ui->b_m->setEnabled(false);
            ui->b_mm->setEnabled(false);
            ui->b_mmm->setEnabled(false);
            ui->b_shift->setEnabled(false);
            ui->b_minus->setEnabled(false);
        }
        else
        {
            if(x==3)//- делает видимыми только цифры без точки
            {
                ui->b_q->setEnabled(false);
                ui->b_w->setEnabled(false);
                ui->b_e->setEnabled(false);
                ui->b_r->setEnabled(false);
                ui->b_t->setEnabled(false);
                ui->b_y->setEnabled(false);
                ui->b_u->setEnabled(false);
                ui->b_i->setEnabled(false);
                ui->b_o->setEnabled(false);
                ui->b_p->setEnabled(false);
                ui->b_pp->setEnabled(false);
                ui->b_ppp->setEnabled(false);
                ui->b_a->setEnabled(false);
                ui->b_s->setEnabled(false);
                ui->b_d->setEnabled(false);
                ui->b_f->setEnabled(false);
                ui->b_g->setEnabled(false);
                ui->b_h->setEnabled(false);
                ui->b_j->setEnabled(false);
                ui->b_k->setEnabled(false);
                ui->b_l->setEnabled(false);
                ui->b_ll->setEnabled(false);
                ui->b_lll->setEnabled(false);
                ui->b_z->setEnabled(false);
                ui->b_x->setEnabled(false);
                ui->b_c->setEnabled(false);
                ui->b_v->setEnabled(false);
                ui->b_b->setEnabled(false);
                ui->b_n->setEnabled(false);
                ui->b_m->setEnabled(false);
                ui->b_mm->setEnabled(false);
                ui->b_mmm->setEnabled(false);
                ui->b_shift->setEnabled(false);
                ui->b_mmm_2->setEnabled(false);
                ui->b_minus->setEnabled(false);

            }
            else if(x==4) // делает видимым цифры и минус (для напряжения)
            {
                ui->b_q->setEnabled(false);
                ui->b_w->setEnabled(false);
                ui->b_e->setEnabled(false);
                ui->b_r->setEnabled(false);
                ui->b_t->setEnabled(false);
                ui->b_y->setEnabled(false);
                ui->b_u->setEnabled(false);
                ui->b_i->setEnabled(false);
                ui->b_o->setEnabled(false);
                ui->b_p->setEnabled(false);
                ui->b_pp->setEnabled(false);
                ui->b_ppp->setEnabled(false);
                ui->b_a->setEnabled(false);
                ui->b_s->setEnabled(false);
                ui->b_d->setEnabled(false);
                ui->b_f->setEnabled(false);
                ui->b_g->setEnabled(false);
                ui->b_h->setEnabled(false);
                ui->b_j->setEnabled(false);
                ui->b_k->setEnabled(false);
                ui->b_l->setEnabled(false);
                ui->b_ll->setEnabled(false);
                ui->b_lll->setEnabled(false);
                ui->b_z->setEnabled(false);
                ui->b_x->setEnabled(false);
                ui->b_c->setEnabled(false);
                ui->b_v->setEnabled(false);
                ui->b_b->setEnabled(false);
                ui->b_n->setEnabled(false);
                ui->b_m->setEnabled(false);
                ui->b_mm->setEnabled(false);
                ui->b_mmm->setEnabled(false);
                ui->b_shift->setEnabled(false);
                ui->b_mmm_2->setEnabled(false);
            }
            else if(x==5) // делает видимым цифры с точкой и минус (для нижнего предела значений)
            {
                ui->b_q->setEnabled(false);
                ui->b_w->setEnabled(false);
                ui->b_e->setEnabled(false);
                ui->b_r->setEnabled(false);
                ui->b_t->setEnabled(false);
                ui->b_y->setEnabled(false);
                ui->b_u->setEnabled(false);
                ui->b_i->setEnabled(false);
                ui->b_o->setEnabled(false);
                ui->b_p->setEnabled(false);
                ui->b_pp->setEnabled(false);
                ui->b_ppp->setEnabled(false);
                ui->b_a->setEnabled(false);
                ui->b_s->setEnabled(false);
                ui->b_d->setEnabled(false);
                ui->b_f->setEnabled(false);
                ui->b_g->setEnabled(false);
                ui->b_h->setEnabled(false);
                ui->b_j->setEnabled(false);
                ui->b_k->setEnabled(false);
                ui->b_l->setEnabled(false);
                ui->b_ll->setEnabled(false);
                ui->b_lll->setEnabled(false);
                ui->b_z->setEnabled(false);
                ui->b_x->setEnabled(false);
                ui->b_c->setEnabled(false);
                ui->b_v->setEnabled(false);
                ui->b_b->setEnabled(false);
                ui->b_n->setEnabled(false);
                ui->b_m->setEnabled(false);
                ui->b_mm->setEnabled(false);
                ui->b_mmm->setEnabled(false);
                ui->b_shift->setEnabled(false);
                ui->b_mmm->setEnabled(false);
            }
        }
    }

}

void keyboard::setOncePoint(int x) // выставляем флаг о вводе точки или минуса только один раз
{
    OncePoint=x;
    if (OncePoint==1)
    {
        if(ui->lineEdit->text().contains("."))
        {
            ui->b_mmm_2->setEnabled(false);
        }
    }
    if (OncePoint==2)
    {
        if(ui->lineEdit->text().contains("-")||ui->lineEdit->text().size()>1)
        {
            ui->b_minus->setEnabled(false);
        }
        if(ui->lineEdit->text().contains("."))
        {
            ui->b_mmm_2->setEnabled(false);
        }
    }
    //else ui->b_mmm_2->setEnabled(true);
}

void keyboard::on_b_enter_clicked() // - функция передает строку из lineEdit как сигнал и закрывает клавиатуру;
{
    emit setText(ui->lineEdit->text());

    close();
}

void keyboard::on_b_backspace_clicked() // нажатие backspace удаляет последний символ
{
    line = ui->lineEdit->text();
    line.chop(1); // - удаляет последний символ
    ui->lineEdit->setText(line);

    if (OncePoint==1)
    {
        if (!ui->lineEdit->text().contains("."))
        {
            ui->b_mmm_2->setEnabled(true);
        }
    }
    else
    {
        if (OncePoint==2) // делаем видимым минус если строка очищается полностью
        {
            if(line.size()<1)
            {
                ui->b_minus->setEnabled(true);
            }
        }
    }
}
void keyboard::on_b1_clicked()
{
    line+="1";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b2_clicked()
{
    line+="2";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b3_clicked()
{
    line+="3";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b4_clicked()
{
    line+="4";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b5_clicked()
{
    line+="5";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b6_clicked()
{
    line+="6";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b7_clicked()
{
    line+="7";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b8_clicked()
{
    line+="8";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b9_clicked()
{
    line+="9";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}
void keyboard::on_b10_clicked()
{
    line+="0";
    ui->lineEdit->setText(line);
    if (OncePoint==2)                                        // делает невидимым минус при выставлении соответствующего сигнала
    {                                                        // делает невидимым минус при выставлении соответствующего сигнала
        if(line.size()>0) ui->b_minus->setEnabled(false);    // делает невидимым минус при выставлении соответствующего сигнала
    }                                                        // делает невидимым минус при выставлении соответствующего сигнала
}



void keyboard::on_b_shift_clicked()  //нажатие клавиши Shift - перевод букв в верхний регистр.
{
    QString letter="";
    letter=ui->b_q->text();
    if (letter=="й")
    {
        ui->b_q->setText("Й");
        ui->b_w->setText("Ц");
        ui->b_e->setText("У");
        ui->b_r->setText("К");
        ui->b_t->setText("Е");
        ui->b_y->setText("Н");
        ui->b_u->setText("Г");
        ui->b_i->setText("Ш");
        ui->b_o->setText("Щ");
        ui->b_p->setText("З");
        ui->b_pp->setText("Х");
        ui->b_ppp->setText("Ъ");
        ui->b_a->setText("Ф");
        ui->b_s->setText("Ы");
        ui->b_d->setText("В");
        ui->b_f->setText("А");
        ui->b_g->setText("П");
        ui->b_h->setText("Р");
        ui->b_j->setText("О");
        ui->b_k->setText("Л");
        ui->b_l->setText("Д");
        ui->b_ll->setText("Ж");
        ui->b_lll->setText("Э");
        ui->b_z->setText("Я");
        ui->b_x->setText("Ч");
        ui->b_c->setText("С");
        ui->b_v->setText("М");
        ui->b_b->setText("И");
        ui->b_n->setText("Т");
        ui->b_m->setText("Ь");
        ui->b_mm->setText("Б");
        ui->b_mmm->setText("Ю");
    }
    else
    {
        ui->b_q->setText("й");
        ui->b_w->setText("ц");
        ui->b_e->setText("у");
        ui->b_r->setText("к");
        ui->b_t->setText("е");
        ui->b_y->setText("н");
        ui->b_u->setText("г");
        ui->b_i->setText("ш");
        ui->b_o->setText("щ");
        ui->b_p->setText("з");
        ui->b_pp->setText("х");
        ui->b_ppp->setText("ъ");
        ui->b_a->setText("ф");
        ui->b_s->setText("ы");
        ui->b_d->setText("в");
        ui->b_f->setText("а");
        ui->b_g->setText("п");
        ui->b_h->setText("р");
        ui->b_j->setText("о");
        ui->b_k->setText("л");
        ui->b_l->setText("д");
        ui->b_ll->setText("ж");
        ui->b_lll->setText("э");
        ui->b_z->setText("я");
        ui->b_x->setText("ч");
        ui->b_c->setText("с");
        ui->b_v->setText("м");
        ui->b_b->setText("и");
        ui->b_n->setText("т");
        ui->b_m->setText("ь");
        ui->b_mm->setText("б");
        ui->b_mmm->setText("ю");
    }
}

void keyboard::on_b_q_clicked()
{
    line+=ui->b_q->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_w_clicked()
{
    line+=ui->b_w->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_e_clicked()
{
    line+=ui->b_e->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_r_clicked()
{
    line+=ui->b_r->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_t_clicked()
{
    line+=ui->b_t->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_y_clicked()
{
    line+=ui->b_y->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_u_clicked()
{
    line+=ui->b_u->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_i_clicked()
{
    line+=ui->b_i->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_o_clicked()
{
    line+=ui->b_o->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_p_clicked()
{
    line+=ui->b_p->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_pp_clicked()
{
    line+=ui->b_pp->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_ppp_clicked()
{
    line+=ui->b_ppp->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_a_clicked()
{
    line+=ui->b_a->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_s_clicked()
{
    line+=ui->b_s->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_d_clicked()
{
    line+=ui->b_d->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_f_clicked()
{
    line+=ui->b_f->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_g_clicked()
{
    line+=ui->b_g->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_h_clicked()
{
    line+=ui->b_h->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_j_clicked()
{
    line+=ui->b_j->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_k_clicked()
{
    line+=ui->b_k->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_l_clicked()
{
    line+=ui->b_l->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_ll_clicked()
{
    line+=ui->b_ll->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_lll_clicked()
{
    line+=ui->b_lll->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_z_clicked()
{
    line+=ui->b_z->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_x_clicked()
{
    line+=ui->b_x->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_c_clicked()
{
    line+=ui->b_c->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_v_clicked()
{
    line+=ui->b_v->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_b_clicked()
{
    line+=ui->b_b->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_n_clicked()
{
    line+=ui->b_n->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_m_clicked()
{
    line+=ui->b_m->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_mm_clicked()
{
    line+=ui->b_mm->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_mmm_clicked()
{
    line+=ui->b_mmm->text();
    ui->lineEdit->setText(line);
}
void keyboard::on_b_mmm_2_clicked()
{
    line+=ui->b_mmm_2->text();
    ui->lineEdit->setText(line);
    if(OncePoint==1)// если есть точка то должна пропасть
    {
        if (ui->lineEdit->text().contains("."))
        {
        ui->b_mmm_2->setEnabled(false);
        }
    }
}
void keyboard::on_b_minus_clicked()
{
    line+=ui->b_minus->text();
    ui->lineEdit->setText(line);
    if(OncePoint==2)// если есть минус то должен пропасть
    {
        if (line.size()>0)
        {
        ui->b_minus->setEnabled(false);
        }
    }
}
