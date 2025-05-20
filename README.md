# README

## About
これは Apple HomeAppからシーリングライトとエアコンをコントロールするためのアプリケーションです。

## Setup
### Step1
- `accessories/light/build.sh`
- `accessories/thermostat/build.sh`
- `accessories/remoconCrawler/build.sh`

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
- `server.sh Set server On 1`
- `./airPurifier Set airpurifier RotationSpeed 20`
- `./airPurifier Get airpurifier RotationSpeed`
- `/home/pi/accessories/thermostat/thermostat Get Thermostat TargetHeatingCoolingState`
- `/home/pi/accessories/thermostat/thermostat Set Thermostat TargetHeatingCoolingState 1`

[コマンド] [Set or Get] [displayName] [characteristic] [Setの場合は目標値 Getの場合はない]

## cmdBuffer
`accessory/cmdBuffer`に赤外線リモコンの波形パターンが記録されたcsvファイルのパスを書き込むとremoconEmulatorが赤外線信号を出力する

例:`/home/pi/accessories/light/cmd_zento.csv`

## remocon_analyzer
arduinoを用いてリモコンの赤外線信号を解析し、シリアルモニターにscvデータを出力するプログラム

## デバイス一覧
- 照明 `accesories/light`
- エアコン `accesories/thermostat`
- 空気清浄機 `accesories/airPurifier`
  - Arduino側の[README.md](./airPurifier/Arduino/README.md)