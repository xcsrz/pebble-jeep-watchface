
default:
	pebble build && pebble install --emulator basalt

test: default

push:
	pebble build && pebble install --phone 192.168.1.223

screenshot:
	pebble screenshot --phone 192.168.1.223	