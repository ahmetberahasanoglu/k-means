#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 9 // Veri sayýsýný global olarak tanýmladýk isteseydik veriler arttýkca artacak sekilde yapabilirdik
#define K 3 // Küme sayýsýný baþtan belirttim isteseydik veriler arttýkca artacak þekilde yapabilir veya kullanicidan alabilirdik
#define MAX_ITER 100 // Maksimum iterasyon sayýsý yani, Kararlý hale (stable state) gelinene kadar 2. ve 3. adýmlarýn tekrarlanmasý iþlemi maksimum 100 kere olacak


// Veri yapýsý
typedef struct {
    double x; // x koordinatý
    double y; // y koordinatý
    int etiket; // küme etiketi, yani her bir veri noktasýnýn hangi kümeyle iliþkilendirildiðini belirten deðerimiz
} Veri;

// Küme yapýsý
typedef struct {
    double x; // küme merkezinin x koordinatý
    double y; // küme merkezinin y koordinatý
    int boyut; // kümeye ait veri sayýsý
} Kume;

// Veri noktalarý arasýndaki mesafeyi ölçmek için öklidyen uzaklýðý hesaplayan fonksiyon kullandýk
double okliduzaklik(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Standart k means sistemine uygun olarak erileri rastgele kümelere atayan fonksiyon kullandýk
void rastgelEtiket(Veri data[], int n, int k) {
    int i = 0;
    for (i; i < n; i++) {
        data[i].etiket = rand() % k; // 0 ile k-1 arasýnda rastgele bir sayý üret ve verinin etiketi olarak ata
    }
}

// Küme merkezlerini Kararlý hale (stable state) gelinene kadar 2. ve 3. adýmlarýn tekrarlanmasýný saglayan fonksiyon
void kumeleriTekrarla(Veri data[], int n, Kume cluster[], int k) {
    // Küme merkezlerini sýfýrladýk
    int i = 0;
    for (i; i < k; i++) {
        cluster[i].x = 0;
        cluster[i].y = 0;
        cluster[i].boyut = 0;
    }
    
    // Her veri için kümesine göre merkez koordinatlarýný ve boyutunu güncelledik
    int j = 0;
    for (j; j < n; j++) {
        cluster[data[j].etiket].x += data[j].x;
        cluster[data[j].etiket].y += data[j].y;
        cluster[data[j].etiket].boyut++;
    }
    
    // Her küme için merkez koordinatlarýný ortalamaya bölerek yeni merkezi hesapladik
    int l = 0;
    for (l; l < k; l++) {
        if (cluster[l].boyut > 0) { // Eðer kümenin boyutu sýfýrdan büyükse aþaðýdaki islemleri yaptýk
            cluster[l].x /= cluster[l].boyut;
            cluster[l].y /= cluster[l].boyut;
        }
    }
}

// her veri noktasýný en yakýn küme merkezine atayarak verileri kümelere baðladýk
int verileriAta(Veri data[], int n, Kume cluster[], int k) {
    int degisendeger = 0; // Atamalarda deðiþim olup olmadýðýný tutan deðiþken
    
    // Her veri için en yakýn küme merkezini bul
    int i = 0;
    for (i; i < n; i++) {
        double min_uzaklik = okliduzaklik(data[i].x, data[i].y, cluster[0].x, cluster[0].y); // Ýlk küme merkezine olan uzaklýðý baþlangýç deðeri olarak al
        int en_yakin_kume = 0; // En yakýn kümenin indeksini tutan deðiþken
        
        // Diðer küme merkezleri ile karþýlaþtýr
        int j = 1;
        for (j; j < k; j++) {
            double uzaklik = okliduzaklik(data[i].x, data[i].y, cluster[j].x, cluster[j].y); // Veri ile küme merkezi arasýndaki uzaklýðý hesapla
            
            if (uzaklik < min_uzaklik) { // Eðer uzaklýk daha küçükse
                min_uzaklik = uzaklik; // Uzaklýðý güncelle
                en_yakin_kume = j; // En yakýn kümenin indeksini güncelle
            }
        }
        
        if (data[i].etiket != en_yakin_kume) { // Eðer verinin etiketi en yakýn kümeyle ayný deðilse
            data[i].etiket = en_yakin_kume; // Verinin etiketini güncelle
            degisendeger++; // Deðiþim sayýsýný arttýr
        }
    }
    
    return degisendeger; 
}

// K-means algoritmasýný uygulayan fonksiyon
void k_means(Veri data[], int n, Kume cluster[], int k) {
    
    rastgelEtiket(data, n, k); // Verilere rastgele etiketler atadýk
    
    int iterasyon = 0; 
    
    while (1) { // Sonsuz döngü
        
        kumeleriTekrarla(data, n, cluster, k); // Küme merkezlerini güncelle
        
        int degisendeger = verileriAta(data, n, cluster, k); // Verileri en yakýn kümeye atayýp ve deðiþim sayýsýný aldik
        
        iterasyon++; 
        
        if (degisendeger == 0 || iterasyon == MAX_ITER) { // Eðer hiçbir deðiþim olmadýysa veya maksimum iterasyona ulaþýldýysa çýk
            break;
        }
        
    }
}

// Verileri ekrana yazdýran fonk
void dataYazdir(Veri data[], int n) {
    printf("\n\nNesne\t    x\t    y\t Etiket\n");
    int i = 0;
    for (i; i < n; i++) {
        printf("%d\t %.2f\t %.2f\t %d\n", i+1, data[i].x, data[i].y, data[i].etiket);
    }
}

// Kümeleri ekrana yazdýran fonk
void kumeleriYazdir(Kume cluster[], int k) {
    
    printf("\nKume\t    x\t    y\t Kumenin icindeki veri miktari\n");
    int i = 0;
    for (i; i < k; i++) {
        printf("%d\t %.2f\t %.2f\t %d\n", i+1, cluster[i].x, cluster[i].y, cluster[i].boyut);
    }
}

int main() {
    printf("Hazirlayanlar	\nAhmet Bera Hasanoglu 1220505041\nFatih Turker Akpinar 1220505014 \nTaha Yasin Cicek     1220505012\n");
    srand(time(NULL)); // Rastgele etiket olusturmak icin bu yapýyý kullandýk daha once zar atma uygulamasýnda da kullanmistik
    
    Veri data[N] = {{515.4117036, 378.6268695}, {514.3296181, 335.6531661}, {429.8885106, 409.4434442}, {641.5721022, 230.0060802}, {579.9098991, 252.9621446}, {580.2409894, 221.7516489} , {624.9774151, 395.7559439} , {612.1353904, 400.5265048} , {664.4501542, 403.6348547} }; // Veri seti
    // Buradaki veriler, 2D_clustering_data dosyasýndaki [115-117] arasýndaki 0 etiketli [118-120] arasý 1 etiketli [227-229] arasý 2 etiketli verilerden alýndý
	// Bu verileri aldýðýmýzda veriler K adet küme etiketiyle gruplara ayýrýr ve her bir kümenin merkezini, kümeye ait veri sayýsýný ve verilerin küme etiketlerini ekrana yazdýrýr. 
    
    Kume cluster[K]; // Küme dizisi
    
    k_means(data, N, cluster, K); // K-means algoritmasýný uygula
    
    dataYazdir(data, N); // Verileri yazdýr
    
    kumeleriYazdir(cluster, K); // Kümeleri yazdýr
    
}
