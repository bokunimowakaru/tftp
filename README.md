# tftp; TFTPクライアント TFTP client for esp32

TFTPサーバ上からファイルをダウンロードします。  

## TFTPとは

TFTPはネットワーク機器などの設定ファイルやファームウェアを転送するときなどに使用されているデータ転送プロトコルです。  
使い勝手が簡単で、プロトコルも簡単なので、機器のメンテナンスに向いています。  
認証や暗号化は行わないので、転送時のみ有効にする、もしくは侵入・ファイル転送されても問題の無い用途で利用します。

## Raspberry PiへのTFTPサーバのインストール方法

    $ sudo apt-get install tftpd-hpa
    
## 設定ファイル(/etc/default/tftpd-hpa) の例

    # /etc/default/tftpd-hpa
    TFTP_USERNAME="tftp"
    TFTP_DIRECTORY="/srv/tftp"
    TFTP_ADDRESS="0.0.0.0:69"

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

Copyright (c) 2016-2019 Wataru KUNINO  
<https://bokunimo.net/>
