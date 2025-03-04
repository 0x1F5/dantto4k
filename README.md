# dantto4k

## 使用方法

### dantto4k.exe
mmtsから復号化およびMPEG-2 TSへの変換を行います。
```
dantto4k.exe <input.mmts> <output.ts> [options]
        '-' can be used instead of a file path to enable piping via stdin or stdout.
options:
        --disableADTSConversion: Uses the raw LATM format without converting to ADTS.
        --listSmartCardReader: Lists the available smart card readers.
        --smartCardReaderName=<name>: Sets the smart card reader to use.
```

### BonDriver_dantto4k.dll
リアルタイムで復号化とMPEG-2 TSへの変換を行うBonDriverです。
BonDriver_dantto4k.iniで設定されたBonDriverをロードして、復号化とMPEG-2 TSへの変換を行います。
dantto4kは64bitで配布しており、BonRecTestおよびBonDriver_BDAは64bitである必要があります。

#### mirakurunでの動作
PT4Kで動作する場合、チャンネル再生まで15～20秒かかるため、mirakurunのtimeout(20秒)を超える場合があります。
mirakurunのソースコードを修正してtimeoutを30秒以上に変更する必要があります。

https://github.com/Chinachu/Mirakurun/blob/master/src/Mirakurun/Tuner.ts#L175C13-L175C55

## ビルド
### Windows
/thirdpartyフォルダにopenssl 3, tsduckを準備します。
下記のURLからbinaryをダウンロードすることができます。

- https://slproweb.com/products/Win32OpenSSL.html
- https://github.com/tsduck/tsduck/
### Linux
tsduck、tsduck-devのパッケージを下記のURLからダウンロードし、インストールします。
- https://github.com/tsduck/tsduck/releases

ubuntu24.04でのインストール例
```bash
wget https://github.com/tsduck/tsduck/releases/download/v3.40-4165/tsduck_3.40-4165.ubuntu24_amd64.deb --no-check-certificate
wget https://github.com/tsduck/tsduck/releases/download/v3.40-4165/tsduck-dev_3.40-4165.ubuntu24_amd64.deb --no-check-certificate
apt install -y ./tsduck_3.40-4165.ubuntu24_amd64.deb ./tsduck-dev_3.40-4165.ubuntu24_amd64.deb
```
下記の手順でビルドします。
```bash
sudo apt install -y --no-install-recommends make g++ libssl-dev libedit-dev libedit-dev zlib1g-dev libusb-1.0-0-dev libpcsclite-dev libnghttp2-dev libidn2-dev librtmp-dev libssh2-1-dev libpsl-dev libkrb5-dev libldap2-dev libzstd-dev libbrotli-dev libmbedtls-dev libudev-dev libssh-dev
git clone https://github.com/nekohkr/dantto4k.git
cd dantto4k
make
sudo make install
```

## References
- ARIB STD-B32
- ARIB STD-B60
- [superfashi/FFmpeg](https://github.com/superfashi/FFmpeg)
- b61decoder
