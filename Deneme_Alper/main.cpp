#include "mainwindow.h"
#include "Manager.h"
#include <QApplication>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>

using namespace std;


send_serial_test send_serial_cb;
recived_serial_test recived_serial_cb;
#define MTT 0x1021 /* crc-ccitt mask */
static unsigned short updateCRC(unsigned short crc, uint16_t karakter)
{
    karakter <<= 8;
    int i;
    for (i = 0; i < 8; i++)
    {
        if ((crc ^ karakter) & 0x8000)
            crc = (crc << 1) ^ MTT;
        else
            crc <<= 1;
        karakter <<= 1;
    }

    return crc;
}

static unsigned short computeCRC16(uint8_t* array, int length)
{
    unsigned short CRC16 = 0;
    int i;
    for (i = 0; i < length; i++)
    {
        CRC16 = updateCRC(CRC16, (uint16_t)array[i]);
    }

    return CRC16;
}
void data_read(const char * data);

const char* recived_serial_port(void)
{
    return recived_serial_cb();
}

int send_serial_port(const char* control_data,size_t size)
{
    return send_serial_cb(control_data, size);
}
void turnstile_vers(const uint8_t buf[])
{
    //ÜRETİCİ KODU
    if (buf[3] == 0x01){
        cout<<"Aktüel"<<endl;
    }
    //ÜRÜN MODELİ
    if (buf[4] == 0x01){
        cout<<"Kanatli"<<endl;
    }
    else if(buf[4] == 0x02){
        cout<<"Tripot"<<endl;
    }
}

void turnstile_serial_num(const uint8_t buf[])
{
    for(int i=3; i<11; i++){
    cout<<"Seri Numara: "<<buf[i];
    }
}

void turnstile_mod_sorgu(const uint8_t buf[])
{
    //İŞLETME MODU
    if (buf[3] == 0x01) {
        cout<<"Normalde Kapali"<<endl;
}
    else if(buf[3] == 0x02){
        cout<<"Normalde Acik"<<endl;
}
    //GIRIS CIKIS MODU
    if (buf[4] == 0x01) {
        cout<<"Serbest"<<endl;
    }
    else if(buf[4] == 0x02){
        cout<<"Blokeli"<<endl;
}
    else if(buf[4] == 0x03){
        cout<<"Kontrollü"<<endl;
}
    cout<<"Timeout süresi: "<<100*buf[6]<<" ms"<<endl; //Timeout Süresi
    cout<<"Fotosel Arıza timeout süresi:"<<100*buf[7]<<" ms"<<endl; //Fotosel arıza timeout süresi
    cout<<"Seri Geçiş Sayisi: "<<100*buf[8]<<endl; //Seri Geçiş sayısı
}

void turnstile_fotosel_stat(const uint8_t buf[])
{
    for(int i=3; i<11; i++)
        if(buf[i]&1){
            cout<<"Cisim Algilanmiyor"<<endl;
        }
        else
            cout<<"Cisim Algilaniyor"<<endl;

}

void turnstile_pass_stat(const uint8_t buf[])
{
    //TURNİKE DURUMU
    if (buf[3] == 0x00) {
        cout<<"Alarm, Ariza Yok"<<endl;
    }
    else if(buf[3] == 0x01){
        cout<<"Ariza Var"<<endl;
    }
    else if(buf[3] == 0x02){
        cout<<"Alarm Var"<<endl;
    }
    else if(buf[3] == 0x03){
        cout<<"Alarm ve Alarm Var"<<endl;
    }

    cout<<"Gecebilecek kisi sayisi:"<<buf[4]<<endl; // GEÇEBİLECEK KISI SAYISI

    //ARIZA KODU (BIT OLARAK DIGER DURUMLAR BULUNDU)
    if (buf[5] == 0x00) {
        cout<<"Arıza Yok"<<endl;
    }
    else if(buf[5] == 0x01){
        cout<<"Fotosel"<<endl;
    }
    else if(buf[5] == 0x02){
        cout<<"Motor"<<endl;
    }
    else if(buf[5] == 0x03){
        cout<<"Fotosel, Motor"<<endl;
    }
    else if(buf[5] == 0x04){
        cout<<"Jeton kutusu dolu"<<endl;
    }
    else if(buf[5] == 0x05){
        cout<<"Fotosel, Jeton Kutusu Dolu"<<endl;
    }
    else if(buf[5] == 0x06){
        cout<<"Motor, Jeton Kutusu Dolu"<<endl;
    }
    else if(buf[5] == 0x07){
        cout<<"Fotosel, Motor, Jeton Kutusu Dolu"<<endl;
    }

    //ALARM DURUMU  (BIT OLARAK DIGER DURUMLAR BULUNDU)
    if (buf[6] == 0x00) {
        cout<<"Alarm yok"<<endl;
    }
    else if(buf[6] == 0x01){
        cout<<"Giriş zorlama "<<endl;
    }
    else if(buf[6] == 0x02){
        cout<<"Kapak kapanmıyor "<<endl;
    }
    else if(buf[6] == 0x03){
        cout<<"Giriş Zorlama, Kapan Kapanmiyor"<<endl;
    }
    else if(buf[6] == 0x04){
        cout<<"Yolcu cikmadi"<<endl;
    }
    else if(buf[6] == 0x05){
        cout<<"Giriş Zorlama,Yolcu cikmadi"<<endl;
    }
    else if(buf[6] == 0x06){
        cout<<"Kapan Kapanmiyor, Yolcu cikmadi"<<endl;
    }
    else if(buf[6] == 0x07){
        cout<<"Giriş Zorlama, Kapan Kapanmiyor, Yolcu cikmadi"<<endl;
    }
    else if(buf[6] == 0x08){
        cout<<"Acil durum"<<endl;
    }
    else if(buf[6] == 0x09){
        cout<<"Giriş Zorlama, Acil durum"<<endl;
    }
    else if(buf[6] == 0x0A){
        cout<<"Kapan Kapanmiyor, Acil durum"<<endl;
    }
    else if(buf[6] == 0x0B){
        cout<<"Giriş Zorlama, Kapan Kapanmiyor, Acil durum"<<endl;
    }
    else if(buf[6] == 0x0C){
        cout<<"Yolcu cikmadi, Acil durum"<<endl;
    }
    else if(buf[6] == 0x0D){
        cout<<"Giriş Zorlama,Yolcu cikmadi, Acil durum"<<endl;
    }
    else if(buf[6] == 0x0E){
        cout<<"Kapan Kapanmiyor, Yolcu cikmadi, Acil durum"<<endl;
    }
    else if(buf[6] == 0x0F){
        cout<<"Giriş Zorlama, Kapan Kapanmiyor, Yolcu cikmadi, Acil durum"<<endl;
    }
}
void time_token_read(const uint8_t buf[])
{
    // SOR
}

void time_out_read(const uint8_t buf[])
{
    // SOR
}

void free_pass(const uint8_t buf[])
{
    if(buf[3] == 0x00){
        cout<<"Buton ON"<<endl;
    }
    else if(buf[3] == 0x01){
        cout<<"Buton OFF"<<endl;

    }
}

void flat_stat(const uint8_t buf[])
{
    if(buf[3] == 0x00){
        cout<<"Kanatlar Acik"<<endl;
    }
    else if(buf[3] == 0x01){
        cout<<"Kanatlar kapali"<<endl;

    }
}

void request_fail(const uint8_t buf[])
{
   // KOMUT CEVABI
    if (buf[2] == 0x00) {
        cout<<"Komutu alamamışsa"<<endl;
    }
    else if(buf[2] == 0x01){
        cout<<"Turnike versiyon "<<endl;
    }
    else if(buf[2] == 0x02){
        cout<<"Turnike seri numarası "<<endl;
    }
    else if(buf[2] == 0x03){
        cout<<"Turnike mod"<<endl;
    }
    else if(buf[2] == 0x04){
        cout<<"Turnike fotosel durumları"<<endl;
    }
    else if(buf[2] == 0x05){
        cout<<"Turnike geçiş durumu"<<endl;
    }
    //HATA KODU
    if (buf[3] == 0x01) {
        cout<<"CRC hatası"<<endl;
    }
    else if(buf[3] == 0x02){
        cout<<"Komut desteklenmiyor"<<endl;
    }
    else if(buf[3] == 0x03){
        cout<<"Komut içeriğinde illegal parametre"<<endl;
    }
    else if(buf[3] == 0x04){
        cout<<"Veri uzunluğu hatalı"<<endl;
    }
}

int turnstile_set_mode(uint8_t operate_mode, uint8_t input_mode,uint8_t output_mode, uint8_t timeout, uint8_t fotosel, uint8_t serial_pass_num)
{
    uint8_t buf[11] = {0x01,0x07,0x11,operate_mode,input_mode,output_mode,timeout,fotosel,serial_pass_num };
    uint16_t val16 = computeCRC16(buf, sizeof (buf)/sizeof(buf[0]));
    buf[9] = val16 & 0xFF; // LOW
    buf[10] = val16 >> 8;   // HIGH
    return send_serial_port((const char*)buf, sizeof(buf));

}

int turnstile_pass_allow(uint8_t  pass_type)
{
    uint8_t buf[6] = {0x01,0x07,0x12,pass_type};
    uint16_t val16 = computeCRC16(buf, sizeof (buf)/sizeof(buf[0]));
    buf[4] = val16 & 0xFF;
    buf[5] = val16 >> 8;
    return send_serial_port((const char*)buf, sizeof(buf));
}


int turnstile_enter_emergency_mode(void)
{
    uint8_t buf[5] = {0x01,0x07,0x13,};
    uint16_t val16 = computeCRC16(buf, sizeof (buf)/sizeof(buf[0]));
    buf[3] = val16 & 0xFF;
    buf[4] = val16 >> 8;
    return send_serial_port((const char*)buf, sizeof(buf));

}

int turnstile_enter_normal_mode(void)
{
    uint8_t buf[5] = {0x01,0x07,0x14,};
    uint16_t val16 = computeCRC16(buf, sizeof (buf)/sizeof(buf[0]));
    buf[3] = val16 & 0xFF;
    buf[4] = val16 >> 8;
    return send_serial_port((const char*)buf, sizeof(buf));
}

int delete_jeton(void)
{
    uint8_t buf[5] = {0x01,0x07,0x15,};
    uint16_t val16 = computeCRC16(buf, sizeof (buf)/sizeof(buf[0]));
    buf[3] = val16 & 0xFF;
    buf[4] = val16 >> 8;
    return send_serial_port((const char*)buf, sizeof(buf));
}

int edit_timeout(uint16_t time)
{
    uint8_t buf[7] = {0x01,0x03,0x16};
    buf[3]= time >> 8;
    buf[4]= time & 0xFF;
    uint16_t val16 = computeCRC16(buf, sizeof(buf)/sizeof(buf[0]));
    buf[5] = val16 & 0xFF;
    buf[6] = val16 >> 8;
    return send_serial_port((const char*)buf, sizeof (buf));
}

int date_time_settings(uint8_t day, uint8_t mo, uint8_t year, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint8_t buf[11] = {0x01,0x07,0x17,day,mo,year,hour,min,sec};
    uint16_t val16 = computeCRC16(buf, sizeof(buf)/sizeof(buf[0]));
    buf[9] =val16 & 0xFF;
    buf[10]= val16 >> 8;
    return send_serial_port((const char*)buf, sizeof (buf));
}

int transparent_mode(uint8_t flap_state, uint8_t input_led, uint8_t output_led)
{
    uint8_t buf[11] = {0x01,0x07,0x18,flap_state,input_led,output_led};
    uint16_t val16 = computeCRC16(buf, sizeof (buf)/sizeof(buf[0]));
    buf[9] = val16 & 0xFF;
    buf[10] = val16 >> 8;
    return send_serial_port((const char*)buf, sizeof(buf));
}
int send_packet(turnstile_event data)
{
     uint8_t data_t=(uint8_t)data;
        if(data_t==0x12){
            uint8_t buf[6] = {0x01,0x01,0x12,0x01, };
            uint16_t val16 = computeCRC16(buf, sizeof(buf)/sizeof(buf[0]));
            buf[4] = val16 & 0xFF;
            buf[5] = val16 >> 8;
            return send_serial_port((const char*)buf, sizeof (buf));
        }
        else {
            uint8_t buf[5] = {0x01,0x01,data_t,};
            uint16_t val16 = computeCRC16(buf, sizeof(buf)/sizeof(buf[0]));
            buf[3] = val16 & 0xFF; // LOW BYTE
            buf[4] = val16 >> 8; // HIGH BYTE
            return send_serial_port((const char*)buf, sizeof(buf));
        }
    }


int main(int argc, char *argv[])
{
    // SERİ PORTTAN DATALARI OKUYUP SORGU KOMUTLARINI MASSAGE BOX İLE ÇALIŞTIRICAM
}
