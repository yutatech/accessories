# README

## About
これは Apple HomeAppからシーリングライトとエアコンをコントロールするためのアプリケーションです。

## Setup
### Step1
`accessories/light/build.sh`
`accessories/thermostat/build.sh`
`accessories/remoconCrawler/build.sh`
を実行

### Step2
`chmod 777 hoge`を各ディレクトリの実行ファイルと`.conf`ファイルに対して実行

### Step3
`accessories/cmd4Config.json`をHomeBridgeのCMD4の設定ファイルにコピー&ペースト

### Step4
以下のファイルを `/etc/systemd/system` におく。ファイル名は`remoconCrawler.service`

```
[Unit]
Description = remoconCrawler

[Service]
ExecStart=/home/pi/accessories/remoconCrawler/build/remoconCrawler
Type=simple

[Install]
WantedBy=multi-user.target
```

次に
`sudo systemctl enable remoconCrawler`

## Homebridgeの管理画面へのアクセス方法
Raspberry PiにHomebridgeをインスールした時のデフォルトのままだと ` http://raspberrypi.local:8581/` にアクセス

## CMD4から実行されるコマンド例
`server.sh Set server On 1`
[コマンド] [Set or Get] [displayName] [characteristic] [Setの場合は目標値 Getの場合はない]