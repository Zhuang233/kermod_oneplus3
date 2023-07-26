make clean
make
adb push ./hello.ko /storage/emulated/0/Download/ko/
# adb shell /data/local/tmp/loadmod.sh