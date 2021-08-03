# fanctl (prototype)
###	Generic program for fan speed control.

Control the speed of a fan using avaiable interface(s). Fanctl has support for the following interfaces:

* asus
* amdgpu (development)

## Usage

Note: the executable must be run as root.

Create a file with an interface and their arguments (default path: /etc/fanctl.conf), using as syntax:

```
interface_name:
[temperature, fan speed (0 to 100, without percentage symbol '%')]
...
[temperature, fan speed]
```

Example:

> sudo fanctl --config-file="/etc/fanctl.conf"

/etc/fanctl.conf

```
asus_fan:
[20, 0]
[30, 25]
[40, 50]
[50, 75]
[60, 100]
```
## Building
> mkdir build
> 
> cd build
> 
> cmake ..
> 
> make
