
# 赤外線用の学習リモコンをBluetooth操作対応リモコンにしてみた
https://zenn.dev/toyotoyo/articles/dab070a42478a9


## 赤外線受信した内容でBluetooth送信する

AtomLite_IR_Recv_2_Ble_Gamepad.ino

## 赤外線送信

AtomLite_IR_Send.ino

## AtomLite+IR UNIT ケースの 3D プリンタ出力イメージ

AtomLite_IR_UNIT.stl
AtomLite_IR_UNIT_Cap.stl


# 普通の赤外線リモコンをマクロ対応学習リモコンにしてみた
https://zenn.dev/toyotoyo/articles/d4aa950e323469


## 赤外線リモコンを受信し信号を確認
AtomLite_IR_Recv.ino

## 赤外線受信した内容で他の赤外線信号を送信
AtomLite_IR_Recv_2_IR_Send.ino

## 赤外線受信した内容でBluetoothや他の赤外線信号を送信
AtomLite_IR_Recv_2_Ble_Gamepad_IR_Send.ino