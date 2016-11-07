
default: build
	pebble install --emulator basalt

test: default

push: build
	pebble install --phone 192.168.1.223

build:
	pebble build

screenshot:
	pebble screenshot --phone 192.168.1.223	

config:
	pebble emu-app-config

fake-bt: 
	pebble emu-bt-connection --connected no

fake-bat:
	pebble emu-battery --percent 10


.PHONY: build