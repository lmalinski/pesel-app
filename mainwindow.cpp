#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_verifyBut_clicked()
{
    const int PESEL_LENGTH = 11;
    //Pobranie numeru pesel:
    QString pesel = ui->pesel->text();

    //Sprawdzenie długości:
    if(pesel.length() < PESEL_LENGTH)
    {
        ui->statusbar->showMessage("Za mało znaków!");
        return;
    }

    //dekompozycja numeru na cyfry:
    short digits [PESEL_LENGTH];
    for(size_t i=0;i < PESEL_LENGTH;i++)
    {
        char digit = pesel.at(i).toLatin1();
        //sprawdzenie czy cyfra:
        if(!isdigit(digit))
        {
            ui->statusbar->showMessage("Niedozwolone znaki!");
            return;
        }
        else digits[i] = digit - '0';
    }

    //obliczenie sumy kontrolnej:
    short const weights[] = {1,3,7,9,1,3,7,9,1,3,1};
    int suma = 0;
    for( size_t i =0;i<PESEL_LENGTH;i++)
        suma+=digits[i]*weights[i];

    //wyryfikacja sumy:
    if(suma%10==0)
       ui->processBut->setEnabled(true);
    else
        ui->statusbar->showMessage("Błąd sumy kontrolnej!");
}

void MainWindow::on_pesel_editingFinished()
{
    ui->processBut->setEnabled(false);
}

void MainWindow::on_processBut_clicked()
{
    const int PESEL_LENGTH = 11;
    //Pobranie numeru pesel:
    QString pesel = ui->pesel->text();

    //dekompozycja numeru na cyfry:
    short digits [PESEL_LENGTH];
    for(size_t i=0;i < PESEL_LENGTH;i++)
    {
        char digit = pesel.at(i).toLatin1();
        //sprawdzenie czy cyfra:
        if(!isdigit(digit))
        {
            ui->statusbar->showMessage("Niedozwolone znaki!");
            return;
        }
        else digits[i] = digit - '0';
    }

    //określenie płci:
    if(digits[9] % 2) ui->sexDisp->setText("Mezczyzna");
    else ui->sexDisp->setText("Kobieta");

    //określenie stulecia:
    int encodedMonth = 10*digits[2]+digits[3];
    int milenium,realMonth;
    if (80 < encodedMonth)
    {
        milenium = 1800;
        realMonth = encodedMonth-80;
    }
    else if(60 < encodedMonth)
    {
        milenium = 2200;
        realMonth = encodedMonth-60;
    }
    else if(40 < encodedMonth)
    {
        milenium = 2100;
        realMonth = encodedMonth-40;
    }
    else if(20 < encodedMonth)
    {
        milenium = 2000;
        realMonth = encodedMonth-20;
    }
    else
    {
        milenium = 1900;
        realMonth = encodedMonth;
    }

    //okreslenie roku:
    int year = milenium + 10*digits[0]+digits[1];

    //okreslenie dnia urodzenia:
    int day = 10*digits[4]+digits[5];
    QDate birthDate(year,realMonth,day);
    int age = QDate::currentDate().year() - birthDate.year();
    ui->dateDisp->setText(birthDate.toString());
    ui->ageDisp->setText(QString::number(age));

}
