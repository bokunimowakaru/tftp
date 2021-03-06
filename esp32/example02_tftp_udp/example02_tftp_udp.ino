/*******************************************************************************
example02 TFTPクライアント・再送信
TFTPサーバ上からファイルをダウンロードし、UDPブロードキャスト送信します。
（UDPはTFTPでも使われているIP通信方式。TFTPを理解しやすくなる）

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
#include <WiFiUdp.h>                        // UDP通信を行うライブラリ
#define SSID "1234ABCD"                     // ★要変更★無線LAN APのSSID
#define PASS "password"                     // ★要変更★パスワード
#define TFTP_SERV "192.168.0.123"           // ★要変更★TFTPサーバのIPアドレス
#define FILENAME  "tftpc_1.ini"             // ★要変更★転送ファイル名
#define PORT 1024                           // 送信のポート番号
#define DEVICE "tftpc_1,"                   // デバイス名(5文字+"_"+番号+",")
IPAddress IP_BROAD;                         // ブロードキャストIPアドレス(CL C)

void setup(){                               // 起動時に一度だけ実行する関数
    Serial.begin(115200);                   // 動作確認のためのシリアル出力開始
    Serial.println("eg.02 TFTP UDP");       // シリアル出力表示
    delay(500);                             // 電源安定待ち時間処理0.5秒
    WiFi.mode(WIFI_STA);                    // 無線LANをSTAモードに設定
    WiFi.begin(SSID,PASS);                  // 無線LANアクセスポイントへ接続
    while(WiFi.status() != WL_CONNECTED){   // 接続に成功するまで待つ
        delay(500);                         // 待ち時間処理
        Serial.print(".");                  // シリアル出力表示
    }
    IP_BROAD = WiFi.localIP();              // IPアドレスを取得
    Serial.println(IP_BROAD);               // 本機のIPアドレスをシリアル出力
    IP_BROAD[3] = 255;                      // ブロードキャストIP(CL C)に変換
    Serial.print("udp:");                   // シリアル出力表示
    Serial.println(IP_BROAD);               // ブロードキャストIPをシリアル出力
}

void loop(){
    WiFiUDP udp;                            // UDP通信用のインスタンスを定義
    char data[513];                         // TFTPデータ用変数
    int len;                                // TFTPデータ長
    
    tftpStart(TFTP_SERV, FILENAME);         // TFTPの開始
    udp.beginPacket(IP_BROAD, PORT);        // UDP送信先を設定
    udp.print(DEVICE);                      // デバイス名を送信
    do{                                     // 繰り返し処理
        len = tftpGet(data);                // TFTP受信(data=受信データ)
        Serial.print(data);                 // 受信データを表示
        for(int i = 0;i < len; i++){        // 英数字以外を_に置き換える
            if(!isAlphaNumeric(data[i])) data[i]='_';
        }
        udp.println(data);                  // 変数dataの内容を送信
    }while(len == 512);                     // 512バイトのときに繰り返し
    udp.println();                          // 改行を送信
    udp.endPacket();                        // UDP送信の終了(実際に送信する)
    delay(200);                             // 送信待ち時間
    Serial.println();                       // 改行
    delay(60000);                           // 60秒の待ち時間処理
}
