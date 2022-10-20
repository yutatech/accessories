# README

## About
これは Apple HomeAppからシーリング洗いとをコントロールするためのアプリケーションです。

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