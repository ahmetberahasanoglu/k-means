#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 9 // Veri say�s�n� global olarak tan�mlad�k isteseydik veriler artt�kca artacak sekilde yapabilirdik
#define K 3 // K�me say�s�n� ba�tan belirttim isteseydik veriler artt�kca artacak �ekilde yapabilir veya kullanicidan alabilirdik
#define MAX_ITER 100 // Maksimum iterasyon say�s� yani, Kararl� hale (stable state) gelinene kadar 2. ve 3. ad�mlar�n tekrarlanmas� i�lemi maksimum 100 kere olacak


// Veri yap�s�
typedef struct {
    double x; // x koordinat�
    double y; // y koordinat�
    int etiket; // k�me etiketi, yani her bir veri noktas�n�n hangi k�meyle ili�kilendirildi�ini belirten de�erimiz
} Veri;

// K�me yap�s�
typedef struct {
    double x; // k�me merkezinin x koordinat�
    double y; // k�me merkezinin y koordinat�
    int boyut; // k�meye ait veri say�s�
} Kume;

// Veri noktalar� aras�ndaki mesafeyi �l�mek i�in �klidyen uzakl��� hesaplayan fonksiyon kulland�k
double okliduzaklik(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Standart k means sistemine uygun olarak erileri rastgele k�melere atayan fonksiyon kulland�k
void rastgelEtiket(Veri data[], int n, int k) {
    int i = 0;
    for (i; i < n; i++) {
        data[i].etiket = rand() % k; // 0 ile k-1 aras�nda rastgele bir say� �ret ve verinin etiketi olarak ata
    }
}

// K�me merkezlerini Kararl� hale (stable state) gelinene kadar 2. ve 3. ad�mlar�n tekrarlanmas�n� saglayan fonksiyon
void kumeleriTekrarla(Veri data[], int n, Kume cluster[], int k) {
    // K�me merkezlerini s�f�rlad�k
    int i = 0;
    for (i; i < k; i++) {
        cluster[i].x = 0;
        cluster[i].y = 0;
        cluster[i].boyut = 0;
    }
    
    // Her veri i�in k�mesine g�re merkez koordinatlar�n� ve boyutunu g�ncelledik
    int j = 0;
    for (j; j < n; j++) {
        cluster[data[j].etiket].x += data[j].x;
        cluster[data[j].etiket].y += data[j].y;
        cluster[data[j].etiket].boyut++;
    }
    
    // Her k�me i�in merkez koordinatlar�n� ortalamaya b�lerek yeni merkezi hesapladik
    int l = 0;
    for (l; l < k; l++) {
        if (cluster[l].boyut > 0) { // E�er k�menin boyutu s�f�rdan b�y�kse a�a��daki islemleri yapt�k
            cluster[l].x /= cluster[l].boyut;
            cluster[l].y /= cluster[l].boyut;
        }
    }
}

// her veri noktas�n� en yak�n k�me merkezine atayarak verileri k�melere ba�lad�k
int verileriAta(Veri data[], int n, Kume cluster[], int k) {
    int degisendeger = 0; // Atamalarda de�i�im olup olmad���n� tutan de�i�ken
    
    // Her veri i�in en yak�n k�me merkezini bul
    int i = 0;
    for (i; i < n; i++) {
        double min_uzaklik = okliduzaklik(data[i].x, data[i].y, cluster[0].x, cluster[0].y); // �lk k�me merkezine olan uzakl��� ba�lang�� de�eri olarak al
        int en_yakin_kume = 0; // En yak�n k�menin indeksini tutan de�i�ken
        
        // Di�er k�me merkezleri ile kar��la�t�r
        int j = 1;
        for (j; j < k; j++) {
            double uzaklik = okliduzaklik(data[i].x, data[i].y, cluster[j].x, cluster[j].y); // Veri ile k�me merkezi aras�ndaki uzakl��� hesapla
            
            if (uzaklik < min_uzaklik) { // E�er uzakl�k daha k���kse
                min_uzaklik = uzaklik; // Uzakl��� g�ncelle
                en_yakin_kume = j; // En yak�n k�menin indeksini g�ncelle
            }
        }
        
        if (data[i].etiket != en_yakin_kume) { // E�er verinin etiketi en yak�n k�meyle ayn� de�ilse
            data[i].etiket = en_yakin_kume; // Verinin etiketini g�ncelle
            degisendeger++; // De�i�im say�s�n� artt�r
        }
    }
    
    return degisendeger; 
}

// K-means algoritmas�n� uygulayan fonksiyon
void k_means(Veri data[], int n, Kume cluster[], int k) {
    
    rastgelEtiket(data, n, k); // Verilere rastgele etiketler atad�k
    
    int iterasyon = 0; 
    
    while (1) { // Sonsuz d�ng�
        
        kumeleriTekrarla(data, n, cluster, k); // K�me merkezlerini g�ncelle
        
        int degisendeger = verileriAta(data, n, cluster, k); // Verileri en yak�n k�meye atay�p ve de�i�im say�s�n� aldik
        
        iterasyon++; 
        
        if (degisendeger == 0 || iterasyon == MAX_ITER) { // E�er hi�bir de�i�im olmad�ysa veya maksimum iterasyona ula��ld�ysa ��k
            break;
        }
        
    }
}

// Verileri ekrana yazd�ran fonk
void dataYazdir(Veri data[], int n) {
    printf("\n\nNesne\t    x\t    y\t Etiket\n");
    int i = 0;
    for (i; i < n; i++) {
        printf("%d\t %.2f\t %.2f\t %d\n", i+1, data[i].x, data[i].y, data[i].etiket);
    }
}

// K�meleri ekrana yazd�ran fonk
void kumeleriYazdir(Kume cluster[], int k) {
    
    printf("\nKume\t    x\t    y\t Kumenin icindeki veri miktari\n");
    int i = 0;
    for (i; i < k; i++) {
        printf("%d\t %.2f\t %.2f\t %d\n", i+1, cluster[i].x, cluster[i].y, cluster[i].boyut);
    }
}

int main() {
    printf("Hazirlayanlar	\nAhmet Bera Hasanoglu 1220505041\nFatih Turker Akpinar 1220505014 \nTaha Yasin Cicek     1220505012\n");
    srand(time(NULL)); // Rastgele etiket olusturmak icin bu yap�y� kulland�k daha once zar atma uygulamas�nda da kullanmistik
    
    Veri data[N] = {{515.4117036, 378.6268695}, {514.3296181, 335.6531661}, {429.8885106, 409.4434442}, {641.5721022, 230.0060802}, {579.9098991, 252.9621446}, {580.2409894, 221.7516489} , {624.9774151, 395.7559439} , {612.1353904, 400.5265048} , {664.4501542, 403.6348547} }; // Veri seti
    // Buradaki veriler, 2D_clustering_data dosyas�ndaki [115-117] aras�ndaki 0 etiketli [118-120] aras� 1 etiketli [227-229] aras� 2 etiketli verilerden al�nd�
	// Bu verileri ald���m�zda veriler K adet k�me etiketiyle gruplara ay�r�r ve her bir k�menin merkezini, k�meye ait veri say�s�n� ve verilerin k�me etiketlerini ekrana yazd�r�r. 
    
    Kume cluster[K]; // K�me dizisi
    
    k_means(data, N, cluster, K); // K-means algoritmas�n� uygula
    
    dataYazdir(data, N); // Verileri yazd�r
    
    kumeleriYazdir(cluster, K); // K�meleri yazd�r
    
}
