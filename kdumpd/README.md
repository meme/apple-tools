kdumpd
======

Ported to GNU/Linux.

Running:

```
sudo mkdir /PanicDumps
sudo chown root /PanicDumps
sudo chmod 1777 /PanicDumps
./kdumpd -w /PanicDumps
```

On the target iDevice:
```
sudo nvram boot-args="debug=0xc44 kdp_match_name=enX wdt=-1 _panicd_ip=169.254.XXX.XXX"
```