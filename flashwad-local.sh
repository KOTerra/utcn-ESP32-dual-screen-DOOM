#!/bin/sh -e

# Auto-detect PORT if not provided
PORT="${PORT:-$(ls /dev/tty.* /dev/cu.* 2>/dev/null | egrep -i 'usb|usbmodem|SLAB|usbserial' | head -n1)}"
if [ -z "$PORT" ]; then
  echo "No serial port found. Plug the board and run: ls /dev/tty.* /dev/cu.*"
  exit 1
fi

WAD="wads/doom1-cut.wad"
if [ ! -f "$WAD" ]; then
  echo "Missing WAD: $WAD"
  exit 1
fi

if ! command -v esptool.py >/dev/null 2>&1; then
  echo "esptool.py not found. Activate ESP-IDF environment first:"
  echo ". \$HOME/esp/v5.5.1/esp-idf/export.sh"
  exit 1
fi

echo "Flashing $WAD to IWAD partition (0x250000) on $PORT"
esptool.py --chip esp32s3 --port "$PORT" --baud 921600 --before default_reset --after hard_reset write_flash \
  --flash_mode dio --flash_freq 80m --flash_size 16MB 0x250000 "$WAD"