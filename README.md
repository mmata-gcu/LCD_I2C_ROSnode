# LCD_I2C_ROSnode
ROS node using a I2C LCD screen [Grove LCD color background 2x16](https://www.seeedstudio.com/Grove-LCD-RGB-Backlight.html). 

The node needs to run in the computer where the LCD screen is connected via I2C, and depends on the **i2c** linux library; for details on how to set this up, please check this related project [LCD_I2C](https://github.com/mmata-gcu/LCD_I2C).

This node subscribes a new topic "**/LCDstring**" using _std_msgs/String_ messages to display custom text (16 characters max).

The implemented behaviour is to keep printing the robot's IP address on the 1st line of the display, and any custom message received via the topic in the 2nd line (up to 16 characters, excess is discarded).

So far it handles just a "special" message (starting by '$') to change the display background colour, as _'$Color:R,G,B'_ (where R,G,B are colour components 0-255) i.e:

`rostopic pub /LCDstring std_msgs/String '$Color:0,255,255'`

then the next message will be displayed in the that colour (the colour won't be updated until a new message is output):

`rostopic pub /LCDstring std_msgs/String 'Hello'`

Download into your source catkin folder, for instance _~/catkin_ws/src_. To build the node, do `catkin_make` as usual, then `source ./devel/setup.sh`. 

To run the node and join it to the ROS network: `rosun lcd_node lcd_node`
