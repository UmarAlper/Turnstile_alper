#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QDebug>
#include "Manager.h"
#include <stdio.h>
#include <iostream>

using namespace std;
QSerialPort *serial;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial=new QSerialPort();
    serial->setPortName("com14");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    send_serial_cb = send_over_serial;
    recived_serial_cb=recived_over_serial;

}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

QSerialPort* MainWindow::m_serial= 0;

QByteArray  output;
QByteArray Read_buf;

const char* MainWindow::recived_over_serial(void){
   Read_buf= m_serial->readAll();
   return Read_buf;
}


void MainWindow::on_pushButton_clicked()
{
send_packet(TURNSTILE_VERSION_CMD);
}


void MainWindow::on_pushButton_2_clicked()
{
send_packet(TURNSTILE_SERIAL_NUMBER_CMD);
}


void MainWindow::on_pushButton_3_clicked()
{
send_packet(TURNSTILE_MOD_CMD);
}


void MainWindow::on_pushButton_4_clicked()
{
send_packet(TURNSTILE_FOTOSEL_STATE_CMD);
}


void MainWindow::on_pushButton_5_clicked()
{
send_packet(TURNSTILE_PASS_STATE_CMD);
}


void MainWindow::on_pushButton_6_clicked()
{
send_packet(TURNSTILE_TIME_RESTRICTED_CMD);
}


void MainWindow::on_pushButton_7_clicked()
{
send_packet(TURNSTILE_TIMEOUT_READ_CMD);
}


void MainWindow::on_pushButton_8_clicked()
{
send_packet(TURNSTILE_FREE_PASS_REQ_CMD);
}


void MainWindow::on_pushButton_9_clicked()
{
send_packet(TURNSTILE_FLAP_STATE_CMD);
}

