# Aircraft Tug
![Aircraft Tug](http://anthonydipilato.com/wp-content/uploads/2018/03/aircrafttug.jpg)

This project is for the small aircraft tug that I built.
You can see the build log and details at URL

The arduino acts as slave to the iOS app. The source for the app can be found at URL

### Inputs
- iOS commands via bluetooth HC-08

### Output
- 4 relays
- PWM pins for dual channel h-bridge to control motors

### Relays 
- Horn
- Flood Lights
- Warning Strobe
- Actuator for wheel hitch

### Commands
Commands received in format:
```
[Command],[Address],[Value][Newline]
```

| Code | Item |
| --- | --- |
| 0	| Status |
| 1	| Relay On |
| 2	| Relay Off |
| 3	| Set PWM |
| 4	| Motor Direction |


### Addresses

| Address | Item |
| --- | ---	|
| 0 | All (status only) |
| 1 | Horn |
| 2 | Flood Lights |
| 3 | Warning Strobe |
| 4 | Wheel Hitch |
| 5 | Left Motor |
| 6 | Right Motor |

Values for motor direction:
```
0 - Stop
1 - Forward
2 - Reverse
```

### Author
Anthony DiPilato, anthony@bumbol.com

### License
All code is available under the MIT license. See LICENSE file for info.
