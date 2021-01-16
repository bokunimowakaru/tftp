/*******************************************************************************
example01 TFTPクライアントの基本動作
TFTPサーバ上からファイルをダウンロードし、シリアル出力します。

                                           Copyright (c) 2016-2021 Wataru KUNINO
********************************************************************************
TFTPとは
　TFTPはネットワーク機器などの設定ファイルやファームウェアを転送するときなどに
　使用されているデータ転送プロトコルです。  
　使い勝手が簡単で、プロトコルも簡単なので、機器のメンテナンスに向いています。
　認証や暗号化は行わないので、転送時のみ有効にする、もしくは侵入・ファイル転送
　されても問題の無い用途で利用します。

Raspberry PiへのTFTPサーバのインストール方法
    $ sudo apt-get install tftpd-hpa
    
    設定ファイル(/etc/default/tftpd-hpa) 例
    # /etc/default/tftpd-hpa
    TFTP_USERNAME="tftp"
    TFTP_DIRECTORY="/srv/tftp"
    TFTP_ADDRESS="0.0.0.0:69"
    TFTP_OPTIONS="--secure"

## TFTPサーバの起動と停止

    $ sudo chmod 755 /srv/tftp
    $ sudo /etc/init.d/tftpd-hpa start
    $ sudo /etc/init.d/tftpd-hpa stop

## 転送用のファイルを保存

    $ sudo echo "; Hello! This is from RasPi" | sudo tee /srv/tftp/tftpc_1.ini
    $ sudo chmod 644 /srv/tftp/tftpc_1.ini
    $ cat /srv/tftp/tftpc_1.ini
    ; Hello! This is from RasPi

【注意事項】
・TFTPクライアント(ESP側)やTFTPサーバ(PCやRaspberry Pi側)起動すると、各機器が
　セキュリティの脅威にさらされた状態となります。
・また、ウィルスやワームが侵入すると、同じネットワーク上の全ての機器へ感染する
　恐れが高まります。
・インターネットに接続すると外部からの侵入される場合があります。
・TFTPクライアントは少なくともローカルネット内のみで動作させるようにして下さい。
・TFTPが不必要なときは、停止させてください。
*******************************************************************************/

#include <WiFi.h>                           // ESP32用WiFiライブラリ
#define SSID "1234ABCD"                     // ★要変更★無線LAN APのSSID
#define PASS "password"                     // ★要変更★パスワード
#define TFTP_SERV "192.168.0.123"           // ★要変更★TFTPサーバのIPアドレス
#define FILENAME  "tftpc_1.ini"             // ★要変更★転送ファイル名

void setup(){                               // 起動時に一度だけ実行する関数
    Serial.begin(115200);                   // 動作確認のためのシリアル出力開始
    Serial.println("eg.01 TFTP");           // 「eg.01 TFTP」をシリアル出力表示
    delay(500);                             // 電源安定待ち時間処理0.5秒
    WiFi.mode(WIFI_STA);                    // 無線LANをSTAモードに設定
    WiFi.begin(SSID,PASS);                  // 無線LANアクセスポイントへ接続
    while(WiFi.status() != WL_CONNECTED){   // 接続に成功するまで待つ
        delay(500);                         // 待ち時間処理
        Serial.print(".");                  // シリアル出力表示
    }
    Serial.println(WiFi.localIP());         // 本機のIPアドレスをシリアル出力
}

void loop(){
    char data[513];                         // TFTPデータ用変数
    int len;                                // TFTPデータ長
    
    tftpStart(TFTP_SERV, FILENAME);         // TFTPの開始
    do{                                     // 繰り返し処理
        len = tftpGet(data);                // TFTP受信(data=受信データ)
        Serial.print(data);                 // 受信データを表示
    }while(len == 512);                     // 512バイトのときに繰り返し
    Serial.print(data);                     // 受信データを表示
    delay(60000);                           // 60秒の待ち時間処理
}
