#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>

namespace Ui {
class keyboard;
}

class keyboard : public QDialog
{
    Q_OBJECT

public:
    explicit keyboard(QWidget *parent = nullptr);

    ~keyboard();


signals:
    void setText(QString str);



public slots:
    void clearKeyK(int x);
    void setKeyString(QString str);
    void setOncePoint(int x);



private slots:
    void on_b_enter_clicked();
    void on_b_backspace_clicked();
    void on_b1_clicked();
    void on_b2_clicked();
    void on_b3_clicked();
    void on_b4_clicked();
    void on_b5_clicked();
    void on_b6_clicked();
    void on_b7_clicked();
    void on_b8_clicked();
    void on_b9_clicked();
    void on_b10_clicked();

    void on_b_shift_clicked();

    void on_b_q_clicked();
    void on_b_w_clicked();
    void on_b_e_clicked();
    void on_b_r_clicked();
    void on_b_t_clicked();
    void on_b_y_clicked();
    void on_b_u_clicked();
    void on_b_i_clicked();
    void on_b_o_clicked();
    void on_b_p_clicked();
    void on_b_pp_clicked();
    void on_b_ppp_clicked();
    void on_b_a_clicked();
    void on_b_s_clicked();
    void on_b_d_clicked();
    void on_b_f_clicked();
    void on_b_g_clicked();
    void on_b_h_clicked();
    void on_b_j_clicked();
    void on_b_k_clicked();
    void on_b_l_clicked();
    void on_b_ll_clicked();
    void on_b_lll_clicked();
    void on_b_z_clicked();
    void on_b_x_clicked();
    void on_b_c_clicked();
    void on_b_v_clicked();
    void on_b_b_clicked();
    void on_b_n_clicked();
    void on_b_m_clicked();
    void on_b_mm_clicked();
    void on_b_mmm_clicked();
    void on_b_mmm_2_clicked();


    void on_b_minus_clicked();

private:
    Ui::keyboard *ui;
    QString line="";
    int OncePoint;
};

#endif // KEYBOARD_H
