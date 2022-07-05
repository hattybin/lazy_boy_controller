# lazy_boy_controller

Home Auotmation adaptor for a La-Z-boy™ chair. This first version only reclines. I didn't have enough of the right relays
onhand to contol every function. 


My model of La-Z-boy™'s Wire Legend*

1.  Yellow  ,GND
2.  Red     ,+30v
3.  Black   ,Lumbar   ,In
4.  White   ,Lumbar   ,Out
5.  Pink    ,Headrest ,In
6.  Grey    ,Headrest ,Out
7.  Brown   ,Legs     ,Out
8.  Green   ,Legs     ,In
9.  Blue    ,Back     ,Out
10. Orange  ,Back     ,In

*Colors and pin positions are for my chair only. I don't know if La-Z-boy™ kept connector connections standard for all models. Verify all the wires by checking for voltage on each when using the remote. 

![image](https://user-images.githubusercontent.com/1077409/177370569-12d916c7-b79c-4dd0-9d10-65e96f4f9efe.png)

![image](https://user-images.githubusercontent.com/1077409/177362685-c1e7bc7b-215f-44ad-945a-0333c1477421.png)

This is my prototype for testing. I ordered plugs so I can design an enclosure that can be installed by attaching it inline with the controller and base.

Hardware:
- 10 5v relays to control every function. I only have one for my prototype that I use to recline the back and open the legs together.
- 1 nodemcu v1
- 1 DC-DC adjustable voltage regulator for 5v to power the MCU and relay trigger
  - need to go from 30v to 3.3v to ~ 9v (i think nodemcu can take up to 9v on vin)


Notes:
- The chair electronics include limit switches that switch current away from the linear actuators when they hit 
- their limits. I take advantage of this and wrote a lazy implementation of the recline code. It sends the recline 
- signal for 15 seconds and does not check for feedback. The chair reclines in about 5 seconds so I can reduce the
- time to 5 seconds to reduce the time an open signal is sent to the relay when it doesn't move. I might eventually 
- try to use the limit switches for feedback but they aren't exposed at the controller port so each limit switch 
- would need to be wired directly to the pads in the main enclosure, making installing this device a lot more difficult. 
- My goal is to make something anyone can plug in and add to their home automation implementation.



