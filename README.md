# tftp; TFTPクライアント TFTP client for esp32

TFTPサーバ上からファイルをダウンロードします。  

## TFTPとは

TFTPはネットワーク機器などの設定ファイルやファームウェアを転送するときなどに使用されているデータ転送プロトコルです。  
使い勝手が簡単で、プロトコルも簡単なので、機器のメンテナンスに向いています。  
認証や暗号化は行わないので、転送時のみ有効にする、もしくは侵入・ファイル転送されても問題の無い用途で利用します。

## サンプル集の内容

### example01_tftp.ino  

TFTPサーバ(Raspberry Pi)上のファイルをダウンロードし、シリアル出力するESP32マイコン用のプログラムです。  
512バイト以上のファイルのブロック転送にも対応しています。
プログラム中の define部 の SSID 、 PASS にWi-Fiアクセスポイントの接続情報を、
TFTP_SERV にTFTPサーバ(Raspberry Pi)のIPアドレスを、 
FILENAME にESP32がRaspberry Piから取得するファイル名を記入してください。  

    #define SSID "1234ABCD"                     // ★要変更★無線LAN APのSSID
    #define PASS "password"                     // ★要変更★パスワード
    #define TFTP_SERV "192.168.0.123"           // ★要変更★TFTPサーバのIPアドレス
    #define FILENAME  "tftpc_1.ini"             // ★要変更★転送ファイル名

本例ではファイル名 tftpc_1.ini （テキストファイル）を転送します。  
後述のRaspberry PiでTFTPサーバをインストールし、転送するファイルを作成しておく必要があります。  

### example02_tftp_udp.ino  

TFTPサーバ上からファイルをダウンロードし、UDPブロードキャスト送信するESP32マイコン用のプログラムです。  
UDPはTFTPでも使われているIP通信方式です。TFTPを理解する前にUDPの仕組みを理解しておくと良いでしょう。  

### example03_tftp_sd.ino  

TFTPサーバ上からファイルをダウンロードし、SDカードに保存するESP32マイコン用のプログラムです。  
FTPに比べ簡単な実装と手順で、ファイル転送が可能になります。  

### example58_tftp.ino  

ESP32マイコンのADコンバータの設定を TFTPサーバから変更するサンプル・プログラムです。  
TFTPサーバ上からADCの入力ピンとディープスリープ時間の設定ファイルをダウンロードし、ESP32マイコンの設定を変更します。  
本プログラムは、CQ出版社「超特急Web接続! ESPマイコン・プログラム全集」に付属のCD-ROMに収録していたものを改良しました（本プログラムの解説も CD-ROM 内に収録）。  
（TFTPのドライバ部を複数ブロック転送対応版に変更。プログラムそのものは511バイト以下の転送用。）  

## Raspberry PiへのTFTPサーバのインストール方法

    $ sudo apt-get install tftpd-hpa
    
## TFTPサーバ用の設定ファイル(/etc/default/tftpd-hpa) の例

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

## 注意事項

* TFTPクライアント(ESP側)やTFTPサーバ(PCやRaspberry Pi側)起動すると、各機器がセキュリティの脅威にさらされた状態となります。
* また、ウィルスやワームが侵入すると、同じネットワーク上の全ての機器へ感染する恐れが高まります。
* インターネットに接続すると外部からの侵入される場合があります。
* TFTPクライアントは少なくともローカルネット内のみで動作させるようにして下さい。
* TFTPが不必要なときは、停止させてください。

Copyright (c) 2016-2021 Wataru KUNINO  
<https://bokunimo.net/>
