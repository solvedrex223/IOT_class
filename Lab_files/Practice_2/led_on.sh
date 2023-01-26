raspi-gpio set 4 op
while true
do
    raspi-gpio set 4 dh
    sleep 1
    raspi-gpio set 4 dl
    sleep 1
done