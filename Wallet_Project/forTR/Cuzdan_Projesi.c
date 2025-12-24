#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define MAX_USERS 100
#define MAX_LEN 50
#define MAX_ISLEM 100

/*
    Düz Yazı Algoritması:
    A1: BAŞLA
    A2: Eğer kullanıcı hesabı yoksa devam et varsa A4'e GİT.
    A3: Ad, Soyad, Şifre ile yeni hesap oluştur.
    A4: Ad, Soyad, Şifre ile giriş yap.
    A5: Gelirler kısmına gelirleri GİR.
    A6: Giderler kısmına giderleri GİR.
    A7: Sistemde güncel bakiyeyi ve para girdi/çıktı işlemlerini görüntüle.
    A8: BİTİR
*/

//Konsol renkleri (ANSI Escape Codes)
void red()    { printf("\033[1;31m"); }
void green()  { printf("\033[1;32m"); }
void yellow() { printf("\033[1;33m"); }
void blue()   { printf("\033[1;34m"); }
void purple() { printf("\033[0;35m"); } 
void grey()   { printf("\033[1;90m"); } 
void orange() { printf("\033[38;5;180m"); } 
void reset()  { printf("\033[0m"); }

//GLOBAL Kullanıcı bilgileri (2 boyutlu karakter dizileri)
char adlar[MAX_USERS][MAX_LEN];
char soyadlar[MAX_USERS][MAX_LEN];
char sifreler[MAX_USERS][MAX_LEN];
int kullaniciSayisi = 0;

//Her kullanıcı için ayrı gelir-gider matrisleri ve sayaçlar
float gelirler[MAX_USERS][MAX_ISLEM];
float giderler[MAX_USERS][MAX_ISLEM];
int gelirSay[MAX_USERS];
int giderSay[MAX_USERS];
float bakiye[MAX_USERS];

//Yanlış Giriş
void yanlisGiris() {
    red();
    printf("\n Parola veya Kullanici Adi Yanlis!\n");
    reset();
}

void bufferTemizle() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); //EOF => End Of File
}

//Ana Menü
int anaMenu(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    orange();
    printf("\n ╔═════════════════════╗");
    printf("\n ║       Ana Menu      ║");
    printf("\n ╚═════════════════════╝ ");
    printf("\n ╔═════════════════════╗");
    printf("\n ║ 1) Giris Yap        ║ ");
    printf("\n ║ 2) Hesap Olustur    ║ ");
    printf("\n ║ 0) Cikis Yap        ║ ");
    printf("\n ╚═════════════════════╝");
    printf("\n Seciminiz: ");

    int secim;
    if (scanf("%d", &secim) != 1) {
        bufferTemizle();
        return -1;   // ana döngü devam etsin
    }

    bufferTemizle();
    reset();
    return secim;
}

//Cüzdan Menü
int cuzdanMenu(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    orange();
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║        CUZDAN MENU        ║");
    printf("\n ╚═══════════════════════════╝ ");
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║ 1) Gelir Ekle             ║");
    printf("\n ║ 2) Gider Ekle             ║");
    printf("\n ║ 3) Guncel Bakiye          ║");
    printf("\n ║ 4) Islem Gecmisi          ║");
    printf("\n ║ 0) Cikis                  ║");
    printf("\n ╚═══════════════════════════╝ ");
    printf("\n Seciminiz: ");
    reset();
    
    int secim;
    if (scanf("%d", &secim) != 1) {
        bufferTemizle();
        return -1;   // ana döngü devam etsin
    }

    bufferTemizle();
    reset();
    return secim;
}

//Bakiye Görüntüleme (kullanıcı indexine göre)
void bakiyeGoster(float bakiye){
    yellow();
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║          Bakiye           ║");
    printf("\n ╚═══════════════════════════╝ ");
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║                           ║");
    printf("\n          %.2f TL        ", bakiye);
    printf("\n ║                           ║");
    printf("\n ╚═══════════════════════════╝ ");
    reset();
}

//İşlem Geçmişi (kullanıcı indexine göre)
void IslemGecmisi(int kullaniciIndex){
    purple();
    printf("\n ╔═══════════════════════════╗");
    printf("\n ║       Islem Gecmisi       ║");
    printf("\n ╚═══════════════════════════╝ ");

    printf("\n ╔═══════════════════════════╗");
    printf("\n ║                           ║");
    printf("\n ║  [Gelirler]               ║\n");

    for (int i = 0; i < gelirSay[kullaniciIndex]; i++)
    {
        printf("\t + %d) %.2f TL \n", i+1, gelirler[kullaniciIndex][i]);   
    }

    printf("\n ║                           ║");
    printf("\n ║  [Giderler]               ║\n");


    for (int i = 0; i < giderSay[kullaniciIndex]; i++)
    {
        printf("\t - %d) %.2f TL\n", i+1, giderler[kullaniciIndex][i]);
    }
    
    printf("\n ║  [Bakiye]                 ║");
    printf("\n \t    %.2f TL        ", bakiye[kullaniciIndex]);
    printf("\n ║                           ║");
    printf("\n ╚═══════════════════════════╝");
    reset();
}

//Hesap Oluşturma
void hesapOlustur(){
    if(kullaniciSayisi >= MAX_USERS){
        red();
        printf("\n Maksimum Kullanici Sayisina Ulasildi!");
        reset();
        return;
    }

    char ad[MAX_LEN], soyad[MAX_LEN], sifre[MAX_LEN];

    blue();
    printf("\n +----------------------+");
    printf("\n |     Hesap Olustur    |");
    printf("\n +----------------------+");
    printf("\n | Ad: "); scanf("%s", &ad); 
    printf("\n | Soyad: "); scanf("%s", &soyad);
    printf("\n | Sifre: "); scanf("%s", &sifre);
    printf("\n +----------------------+");
    reset();

    //Matriste yeni kullanıcı olarak kaydet
    strncpy(adlar[kullaniciSayisi], ad, MAX_LEN-1);
    adlar[kullaniciSayisi][MAX_LEN-1] = '\0';
    strncpy(soyadlar[kullaniciSayisi], soyad, MAX_LEN-1);
    soyadlar[kullaniciSayisi][MAX_LEN-1] = '\0';
    strncpy(sifreler[kullaniciSayisi], sifre, MAX_LEN-1);
    sifreler[kullaniciSayisi][MAX_LEN-1] = '\0';

    //Başlangıç Değerleri
    gelirSay[kullaniciSayisi] = 0;
    giderSay[kullaniciSayisi] = 0;
    bakiye[kullaniciSayisi] = 0.0f;

    kullaniciSayisi++;

    green();
    printf("\n Hesap Basariyla Olusturuldu! (Kullanici ID: %d)\n", kullaniciSayisi);
    reset();
}

//Giriş Yapma
int girisYap(){
    if (kullaniciSayisi == 0)
    {
        red();
        printf("\nOnce Hesap Olusturmaniz Gerekmektedir.");
        reset();
        return -1;
    }

    char ad[MAX_LEN], soyad[MAX_LEN], sifre[MAX_LEN];

    blue();
    printf("\n +----------------------+");
    printf("\n |       Giris Yap      |");
    printf("\n +----------------------+");
    printf("\n | Ad: "); scanf("%s", &ad);
    printf("\n | Soyad: "); scanf("%s", &soyad);
    printf("\n | Sifre:  "); scanf("%s", &sifre);
    printf("\n +----------------------+");
    reset();
    
    for (int i = 0; i < kullaniciSayisi; i++)
    {
        if (strcmp(ad, adlar[i]) == 0 &&
            strcmp(soyad, soyadlar[i]) == 0 &&
            strcmp(sifre, sifreler[i]) == 0)
        {
            green();
            printf("\n Giris basarili! (Kullanici ID: %d)\n", i+1);
            reset();
            return i; // aktif kullanıcı indexi
        }
    }

    yanlisGiris();
    return -1;    
}

//Dosyaya Kaydetme
void verileriKaydet() {
    FILE *fp;
    fp = fopen("veriler.txt", "w");

    if (fp == NULL) {
        red();
        printf("\nDOSYA OLUSTURULAMADI!");
        reset();
        return;
    }

    for (int i = 0; i < kullaniciSayisi; i++) {
        fprintf(fp, "%s;%s;%s;%d;%d;",
                adlar[i], soyadlar[i], sifreler[i],
                gelirSay[i], giderSay[i]);

        for (int j = 0; j < gelirSay[i]; j++) {
            fprintf(fp, "%.2f", gelirler[i][j]);
            if (j < gelirSay[i] - 1) fprintf(fp, ",");
        }
        fprintf(fp, ";");

        for (int j = 0; j < giderSay[i]; j++) {
            fprintf(fp, "%.2f", giderler[i][j]);
            if (j < giderSay[i] - 1) fprintf(fp, ",");
        }
        fprintf(fp, ";%.2f\n", bakiye[i]);
    }

    fclose(fp);
}

//Dosyadan Okuma
void verileriYukle() {
    FILE *fp;
    fp = fopen("veriler.txt", "r");
    if (!fp) return;

    char satir[1024];

    while (fgets(satir, sizeof(satir), fp)) {
        char *token; //parçalanmış kelimeleri tutar

        token = strtok(satir, ";");
        strcpy(adlar[kullaniciSayisi], token);

        token = strtok(NULL, ";");
        strcpy(soyadlar[kullaniciSayisi], token);

        token = strtok(NULL, ";");
        strcpy(sifreler[kullaniciSayisi], token);

        token = strtok(NULL, ";");
        gelirSay[kullaniciSayisi] = atoi(token); //atoi = ASCII TO INTEGER

        token = strtok(NULL, ";");
        giderSay[kullaniciSayisi] = atoi(token);

        // Gelirler
        token = strtok(NULL, ";");
        char *g = strtok(token, ",");
        for (int i = 0; i < gelirSay[kullaniciSayisi]; i++) {
            gelirler[kullaniciSayisi][i] = atof(g); //atof = ASCII TO FLOAT
            g = strtok(NULL, ",");
        }

        // Giderler
        token = strtok(NULL, ";");
        char *d = strtok(token, ",");
        for (int i = 0; i < giderSay[kullaniciSayisi]; i++) {
            giderler[kullaniciSayisi][i] = atof(d);
            d = strtok(NULL, ",");
        }

        token = strtok(NULL, ";");
        bakiye[kullaniciSayisi] = atof(token);

        kullaniciSayisi++;
    }

    fclose(fp);
}

//Main
int main(){
    verileriYukle();

    //Döngü kullanımı
    while (1)
    {
        int secim = anaMenu();

        if (secim == 0)
        {
            grey();
            printf("Programdan Cikiliyor...");
            reset();
            return 0;
        }
        else if(secim == 2){
            hesapOlustur();
            verileriKaydet();
        }
        else if(secim == 1){
            int aktif = girisYap();
            if (aktif == -1)
            {
                verileriKaydet();
                continue;
            }
            
            //Cüzdan menüsüne geçiş kısmı
            int ksecim = 0;
            do
            {
                ksecim = cuzdanMenu();

                if (ksecim == 1) //Gelir Ekle
                {
                    if (gelirSay[aktif] < MAX_ISLEM)
                    {
                        float g;
                        printf("\n Gelir Miktari: ");
                        scanf("%f", &g);
                        gelirler[aktif][gelirSay[aktif]++] = g;
                        bakiye[aktif] += g;
                        verileriKaydet();
                        green();
                        printf("\n Islem Basarili!");
                        reset();
                    }
                    else{
                        red();
                        printf("\nBu kullanici için maksimum gelir kaydi doldu!");
                        reset();
                    }
                }
                else if(ksecim == 2) //Gider Ekle
                {
                    if (giderSay[aktif] < MAX_ISLEM)
                    {
                        float g = 0;
                        printf("\n Gider Miktari: ");
                        scanf("%f", &g);
                        giderler[aktif][giderSay[aktif]++] = g;
                        bakiye[aktif] -= g;
                        verileriKaydet();
                        green();
                        printf("\n Islem Basarili!");
                        reset();
                    }
                    else{
                        red();
                        printf("\nBu kullanici için maksimum gider kaydi doldu.");
                        reset();
                    }
                }
                else if (ksecim == 3) //Güncel Bakiye 
                {
                    bakiyeGoster(bakiye[aktif]);
                }
                else if(ksecim == 4) //İşlem Geçmişi
                {
                    IslemGecmisi(aktif);
                }
                else if(ksecim == 0) //Çıkış
                {
                    verileriKaydet();
                    grey();
                    printf("Hesaptan Cikiliyor..");
                    reset();
                }
                else{
                    red();
                    printf("\n Lutfen Gecerli Bir Deger Tuslayiniz.");                    
                    reset();
                }                
            } while (ksecim != 0);  //Eğer kullanıcı 0'a basmadıysa menüyü tekrar göster
        }
        else{
            red();
            printf("\n Lutfen Gecerli Bir Deger Tuslayiniz.");                    
            reset();
        }
    }
    
    return 0;
}